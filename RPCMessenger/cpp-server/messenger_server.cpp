#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <map>
#include <vector>

#include <grpc++/grpc++.h>
#include <sys/time.h>
#include <algorithm>

#include "messenger.grpc.pb.h"

#define ASYNC

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::CompletionQueue;
using grpc::ServerCompletionQueue;
using grpc::Status;

using messenger::Messenger;
using messenger::LoginRequest;
using messenger::LoginReply;
using messenger::PendingMessagesRequest;
using messenger::PendingMessagesReply;
using messenger::ResultReply;
using messenger::TextMessage;


std::multimap<uint64_t, std::string> text_messages;
std::map<uint32_t, std::string> users;
uint32_t nextUserId = 0;

uint64_t GetLinuxTimeStamp()
{
	struct timeval tp;
	::gettimeofday(&tp, NULL);
	return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

#ifndef ASYNC
class MessengerServiceImpl final : public Messenger::Service
{
	Status Login(ServerContext* context, const LoginRequest* request, LoginReply* response)
	{
		users[nextUserId] = request->user_name();

		response->set_time_stamp(GetLinuxTimeStamp());
		response->set_user_id(nextUserId);
		++nextUserId;

		text_messages.emplace(GetLinuxTimeStamp(), request->user_name() + " has connected.\n");
		response->mutable_result()->set_result(messenger::ResultReply_Result::ResultReply_Result_SUCCESS);
		return Status::OK;
	}

	Status SendTextMessage(ServerContext* context, const TextMessage* request, ResultReply* response)
	{
		response->set_result(messenger::ResultReply_Result::ResultReply_Result_SUCCESS);
		text_messages.emplace(request->time_stamp(), users[request->user_id()] + ": " + request->text_message() + "\n");
		return Status::OK;
	}

	Status RequestPendingMessages(ServerContext* context, const PendingMessagesRequest* request, PendingMessagesReply* response)
	{
		response->mutable_result()->set_result(messenger::ResultReply_Result::ResultReply_Result_SUCCESS);
		auto timestamp = request->time_stamp();
		for (auto it = text_messages.crbegin(); it != text_messages.crend(); ++it)
		{
			if (it->first < timestamp) break;
			response->add_text_messages(it->second);
		}
		
		return Status::OK;
	}

}; 

void RunSyncServer()
{
	std::string server_address("0.0.0.0:6604");
	MessengerServiceImpl service;
	ServerBuilder builder;
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	builder.RegisterService(&service);
	std::unique_ptr<Server> server(builder.BuildAndStart());
	std::cout << "Server listening on " << server_address << std::endl;
	server->Wait();
}
#else
enum class CallStatus 
{ 
	CREATE, 
	PROCESS, 
	FINISH 
};

enum class RpcType
{
	LOGIN,
	SEND_TEXT_MESSAGE,
	REQUEST_PENDING_MESSAGES,
	NONE
};

class CallDataWrapper
{
protected:
	RpcType rpcType = RpcType::NONE;
	void* callData = nullptr;

public:
	CallDataWrapper(RpcType _rpcType, void* _callData)
		: rpcType(_rpcType)
		, callData(_callData)
	{
	}
};

template <class TRequest, class TResponse, class TDerived>
class CallDataBase
{
protected:
	using RequestFunction = void(Messenger::AsyncService::*)(ServerContext*, TRequest*, ServerAsyncResponseWriter<TResponse>*, CompletionQueue*, ServerCompletionQueue*, void*);

	Messenger::AsyncService* m_pAsyncService;
	ServerCompletionQueue* m_pCompletionQueue;
	ServerContext m_context;

	TRequest m_request;
	TResponse m_response;
	ServerAsyncResponseWriter<TResponse> m_responseWriter;

	CallStatus m_currentStatus;
	CallDataWrapper* m_wrapper;

	RequestFunction m_requestFunction;
	
	~CallDataBase()
	{
		delete m_wrapper;
		m_wrapper = nullptr;
		std::cout << "Wrapper deleted" << std::endl;
	}

	CallDataBase(Messenger::AsyncService* pService, ServerCompletionQueue* pCompletionQueue, RpcType rpcType, RequestFunction requestFunction)
		:   m_pAsyncService(pService)
		,	m_pCompletionQueue(pCompletionQueue)
		,   m_responseWriter(&m_context)
		,	m_currentStatus(CallStatus::CREATE)
		,	m_wrapper(nullptr)
		,	m_requestFunction(requestFunction)
	{
		m_wrapper = new CallDataWrapper(rpcType, this);
		Proceed();
	}
public:
	void Proceed()
	{
		switch (m_currentStatus)
		{
			case CallStatus::CREATE:
			{
				std::cout << "Creating..." << std::endl;
				m_currentStatus = CallStatus::PROCESS;
				(m_pAsyncService->*m_requestFunction)(&m_context, &m_request, &m_responseWriter, m_pCompletionQueue, m_pCompletionQueue, m_wrapper);
				break;
			}
			case CallStatus::PROCESS:
			{
				std::cout << "Processing..." << std::endl;
				new TDerived(m_pAsyncService, m_pCompletionQueue);
				auto status = (static_cast<TDerived*>(this))->OnProcess();
				m_currentStatus = CallStatus::FINISH;
				m_responseWriter.Finish(m_response, status, m_wrapper);
				break;
			}
			case CallStatus::FINISH:
			{
				std::cout << "Finishing..." << std::endl;
				delete this;
				break;
			}
			default:
			{
				std::cout << "Error: Unknown tag" << std::endl;
				delete this;
				break;
			}
		}
	}
};



class LoginCallData : public CallDataBase<LoginRequest, LoginReply, LoginCallData> 
{
public:
	LoginCallData(Messenger::AsyncService* pService, ServerCompletionQueue* pCompletionQueue)
		: CallDataBase<LoginRequest, LoginReply, LoginCallData>(pService, pCompletionQueue, RpcType::LOGIN, &Messenger::AsyncService::RequestLogin)
	{}

	Status OnProcess()
	{
		users[nextUserId] = m_request.user_name();

		m_response.set_time_stamp(GetLinuxTimeStamp());
		m_response.set_user_id(nextUserId);
		++nextUserId;

		text_messages.emplace(GetLinuxTimeStamp(), m_request.user_name() + " has connected.\n");

		m_response.mutable_result()->set_result(messenger::ResultReply_Result::ResultReply_Result_SUCCESS);
		return Status::OK;
	}
};

class SendTextMessageCallData : public CallDataBase<TextMessage, ResultReply, SendTextMessageCallData>
{
public:
	SendTextMessageCallData(Messenger::AsyncService* pService, ServerCompletionQueue* pCompletionQueue)
		: CallDataBase<TextMessage, ResultReply, SendTextMessageCallData>(pService, pCompletionQueue, RpcType::SEND_TEXT_MESSAGE, &Messenger::AsyncService::RequestSendTextMessage)
	{}

	Status OnProcess()
	{
		m_response.set_result(messenger::ResultReply_Result::ResultReply_Result_SUCCESS);
		text_messages.emplace(m_request.time_stamp(), users[m_request.user_id()] + ": " + m_request.text_message() + "\n");
		return Status::OK;
	}
};

class RequestPendingMessagesCallData : public CallDataBase<PendingMessagesRequest, PendingMessagesReply, RequestPendingMessagesCallData>
{
public:
	RequestPendingMessagesCallData(Messenger::AsyncService* pService, ServerCompletionQueue* pCompletionQueue)
		: CallDataBase<PendingMessagesRequest, PendingMessagesReply, RequestPendingMessagesCallData>(pService, pCompletionQueue, RpcType::REQUEST_PENDING_MESSAGES, &Messenger::AsyncService::RequestRequestPendingMessages)
	{}

	Status OnProcess()
	{
		m_response.mutable_result()->set_result(messenger::ResultReply_Result::ResultReply_Result_SUCCESS);
		auto timestamp = m_request.time_stamp();
		for (auto it = text_messages.crbegin(); it != text_messages.crend(); ++it)
		{
			if (it->first < timestamp) break;
			m_response.add_text_messages(it->second);
		}
		return Status::OK;
	}
};

class CallDataProcessor : public CallDataWrapper
{
public:
	void Proceed()
	{
		switch (rpcType)
		{
		case RpcType::LOGIN:
			static_cast<LoginCallData*>(callData)->Proceed();
			break;
		case RpcType::SEND_TEXT_MESSAGE:
			static_cast<SendTextMessageCallData*>(callData)->Proceed();
			break;
		case RpcType::REQUEST_PENDING_MESSAGES:
			static_cast<RequestPendingMessagesCallData*>(callData)->Proceed();
			break;
		default:
			std::cout << "Error, rpc type code: " << (int)rpcType << std::endl;
			break;
		}
	}
};

class MessengerServer final 
{
private:
	std::unique_ptr<ServerCompletionQueue> m_completionQueue;
	Messenger::AsyncService m_asyncService;
	std::unique_ptr<Server> m_server;
public: 
	~MessengerServer()
	{
		m_server->Shutdown();
		m_completionQueue->Shutdown();
	}

	void RunAsync()
	{
		std::string server_address("0.0.0.0:6604");
		ServerBuilder builder;
		builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
		builder.RegisterService(&m_asyncService);
		m_completionQueue = builder.AddCompletionQueue();
		m_server = builder.BuildAndStart();
		std::cout << "Server listening on " << server_address << std::endl;
		HandleRpcs();
	}

	void HandleRpcs()
	{
		new LoginCallData(&m_asyncService, m_completionQueue.get());
		new SendTextMessageCallData(&m_asyncService, m_completionQueue.get());
		new RequestPendingMessagesCallData(&m_asyncService, m_completionQueue.get());
		void* tag;
		bool ok;

		for (;;)
		{
			auto result = m_completionQueue->Next(&tag, &ok);
			if (!ok || !result) break; //TODO: Check for undeleted items
			auto processor = static_cast<CallDataProcessor*>(tag);
			processor->Proceed();
		}
	}
};
#endif

int main(int argc, char** argv) 
{
#ifndef ASYNC
	RunSyncServer();
#else
	MessengerServer server;
	server.RunAsync();
#endif
	return 0;
}
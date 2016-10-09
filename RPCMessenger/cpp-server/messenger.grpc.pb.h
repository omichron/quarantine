// Generated by the gRPC protobuf plugin.
// If you make any local change, they will be lost.
// source: messenger.proto
#ifndef GRPC_messenger_2eproto__INCLUDED
#define GRPC_messenger_2eproto__INCLUDED

#include "messenger.pb.h"

#include <grpc++/impl/codegen/async_stream.h>
#include <grpc++/impl/codegen/async_unary_call.h>
#include <grpc++/impl/codegen/proto_utils.h>
#include <grpc++/impl/codegen/rpc_method.h>
#include <grpc++/impl/codegen/service_type.h>
#include <grpc++/impl/codegen/status.h>
#include <grpc++/impl/codegen/stub_options.h>
#include <grpc++/impl/codegen/sync_stream.h>

namespace grpc {
class CompletionQueue;
class Channel;
class RpcService;
class ServerCompletionQueue;
class ServerContext;
}  // namespace grpc

namespace messenger {

class Messenger GRPC_FINAL {
 public:
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status Login(::grpc::ClientContext* context, const ::messenger::LoginRequest& request, ::messenger::LoginReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::messenger::LoginReply>> AsyncLogin(::grpc::ClientContext* context, const ::messenger::LoginRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::messenger::LoginReply>>(AsyncLoginRaw(context, request, cq));
    }
    virtual ::grpc::Status SendTextMessage(::grpc::ClientContext* context, const ::messenger::TextMessage& request, ::messenger::ResultReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::messenger::ResultReply>> AsyncSendTextMessage(::grpc::ClientContext* context, const ::messenger::TextMessage& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::messenger::ResultReply>>(AsyncSendTextMessageRaw(context, request, cq));
    }
    virtual ::grpc::Status RequestPendingMessages(::grpc::ClientContext* context, const ::messenger::PendingMessagesRequest& request, ::messenger::PendingMessagesReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::messenger::PendingMessagesReply>> AsyncRequestPendingMessages(::grpc::ClientContext* context, const ::messenger::PendingMessagesRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::messenger::PendingMessagesReply>>(AsyncRequestPendingMessagesRaw(context, request, cq));
    }
  private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::messenger::LoginReply>* AsyncLoginRaw(::grpc::ClientContext* context, const ::messenger::LoginRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::messenger::ResultReply>* AsyncSendTextMessageRaw(::grpc::ClientContext* context, const ::messenger::TextMessage& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::messenger::PendingMessagesReply>* AsyncRequestPendingMessagesRaw(::grpc::ClientContext* context, const ::messenger::PendingMessagesRequest& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub GRPC_FINAL : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    ::grpc::Status Login(::grpc::ClientContext* context, const ::messenger::LoginRequest& request, ::messenger::LoginReply* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::messenger::LoginReply>> AsyncLogin(::grpc::ClientContext* context, const ::messenger::LoginRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::messenger::LoginReply>>(AsyncLoginRaw(context, request, cq));
    }
    ::grpc::Status SendTextMessage(::grpc::ClientContext* context, const ::messenger::TextMessage& request, ::messenger::ResultReply* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::messenger::ResultReply>> AsyncSendTextMessage(::grpc::ClientContext* context, const ::messenger::TextMessage& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::messenger::ResultReply>>(AsyncSendTextMessageRaw(context, request, cq));
    }
    ::grpc::Status RequestPendingMessages(::grpc::ClientContext* context, const ::messenger::PendingMessagesRequest& request, ::messenger::PendingMessagesReply* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::messenger::PendingMessagesReply>> AsyncRequestPendingMessages(::grpc::ClientContext* context, const ::messenger::PendingMessagesRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::messenger::PendingMessagesReply>>(AsyncRequestPendingMessagesRaw(context, request, cq));
    }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    ::grpc::ClientAsyncResponseReader< ::messenger::LoginReply>* AsyncLoginRaw(::grpc::ClientContext* context, const ::messenger::LoginRequest& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    ::grpc::ClientAsyncResponseReader< ::messenger::ResultReply>* AsyncSendTextMessageRaw(::grpc::ClientContext* context, const ::messenger::TextMessage& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    ::grpc::ClientAsyncResponseReader< ::messenger::PendingMessagesReply>* AsyncRequestPendingMessagesRaw(::grpc::ClientContext* context, const ::messenger::PendingMessagesRequest& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    const ::grpc::RpcMethod rpcmethod_Login_;
    const ::grpc::RpcMethod rpcmethod_SendTextMessage_;
    const ::grpc::RpcMethod rpcmethod_RequestPendingMessages_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status Login(::grpc::ServerContext* context, const ::messenger::LoginRequest* request, ::messenger::LoginReply* response);
    virtual ::grpc::Status SendTextMessage(::grpc::ServerContext* context, const ::messenger::TextMessage* request, ::messenger::ResultReply* response);
    virtual ::grpc::Status RequestPendingMessages(::grpc::ServerContext* context, const ::messenger::PendingMessagesRequest* request, ::messenger::PendingMessagesReply* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_Login : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_Login() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_Login() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Login(::grpc::ServerContext* context, const ::messenger::LoginRequest* request, ::messenger::LoginReply* response) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestLogin(::grpc::ServerContext* context, ::messenger::LoginRequest* request, ::grpc::ServerAsyncResponseWriter< ::messenger::LoginReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_SendTextMessage : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_SendTextMessage() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_SendTextMessage() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SendTextMessage(::grpc::ServerContext* context, const ::messenger::TextMessage* request, ::messenger::ResultReply* response) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestSendTextMessage(::grpc::ServerContext* context, ::messenger::TextMessage* request, ::grpc::ServerAsyncResponseWriter< ::messenger::ResultReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_RequestPendingMessages : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_RequestPendingMessages() {
      ::grpc::Service::MarkMethodAsync(2);
    }
    ~WithAsyncMethod_RequestPendingMessages() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status RequestPendingMessages(::grpc::ServerContext* context, const ::messenger::PendingMessagesRequest* request, ::messenger::PendingMessagesReply* response) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestRequestPendingMessages(::grpc::ServerContext* context, ::messenger::PendingMessagesRequest* request, ::grpc::ServerAsyncResponseWriter< ::messenger::PendingMessagesReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(2, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_Login<WithAsyncMethod_SendTextMessage<WithAsyncMethod_RequestPendingMessages<Service > > > AsyncService;
  template <class BaseClass>
  class WithGenericMethod_Login : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_Login() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_Login() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Login(::grpc::ServerContext* context, const ::messenger::LoginRequest* request, ::messenger::LoginReply* response) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_SendTextMessage : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_SendTextMessage() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_SendTextMessage() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SendTextMessage(::grpc::ServerContext* context, const ::messenger::TextMessage* request, ::messenger::ResultReply* response) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_RequestPendingMessages : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_RequestPendingMessages() {
      ::grpc::Service::MarkMethodGeneric(2);
    }
    ~WithGenericMethod_RequestPendingMessages() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status RequestPendingMessages(::grpc::ServerContext* context, const ::messenger::PendingMessagesRequest* request, ::messenger::PendingMessagesReply* response) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
};

}  // namespace messenger


#endif  // GRPC_messenger_2eproto__INCLUDED

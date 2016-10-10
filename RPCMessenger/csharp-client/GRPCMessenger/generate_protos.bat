@rem Generate the C# code for .proto files

setlocal

@rem enter this directory
cd /d %~dp0

set TOOLS_PATH=packages\Grpc.Tools.1.0.0\tools\windows_x86
set PROTO_PATH=K:\qvarantine\RPCMessenger\protos

%TOOLS_PATH%\protoc.exe -I %PROTO_PATH% --csharp_out Messenger %PROTO_PATH%\messenger.proto --grpc_out Messenger --plugin=protoc-gen-grpc=%TOOLS_PATH%\grpc_csharp_plugin.exe
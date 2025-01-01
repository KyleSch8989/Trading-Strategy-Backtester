// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: protos/service.proto
#ifndef GRPC_protos_2fservice_2eproto__INCLUDED
#define GRPC_protos_2fservice_2eproto__INCLUDED

#include "protos/service.pb.h"

#include <functional>
#include <grpcpp/generic/async_generic_service.h>
#include <grpcpp/support/async_stream.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/support/client_callback.h>
#include <grpcpp/client_context.h>
#include <grpcpp/completion_queue.h>
#include <grpcpp/support/message_allocator.h>
#include <grpcpp/support/method_handler.h>
#include <grpcpp/impl/proto_utils.h>
#include <grpcpp/impl/rpc_method.h>
#include <grpcpp/support/server_callback.h>
#include <grpcpp/impl/server_callback_handlers.h>
#include <grpcpp/server_context.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/status.h>
#include <grpcpp/support/stub_options.h>
#include <grpcpp/support/sync_stream.h>

class BacktesterGui final {
 public:
  static constexpr char const* service_full_name() {
    return "BacktesterGui";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status SMA_Call(::grpc::ClientContext* context, const ::SMARequest& request, ::SMAReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::SMAReply>> AsyncSMA_Call(::grpc::ClientContext* context, const ::SMARequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::SMAReply>>(AsyncSMA_CallRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::SMAReply>> PrepareAsyncSMA_Call(::grpc::ClientContext* context, const ::SMARequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::SMAReply>>(PrepareAsyncSMA_CallRaw(context, request, cq));
    }
    class async_interface {
     public:
      virtual ~async_interface() {}
      virtual void SMA_Call(::grpc::ClientContext* context, const ::SMARequest* request, ::SMAReply* response, std::function<void(::grpc::Status)>) = 0;
      virtual void SMA_Call(::grpc::ClientContext* context, const ::SMARequest* request, ::SMAReply* response, ::grpc::ClientUnaryReactor* reactor) = 0;
    };
    typedef class async_interface experimental_async_interface;
    virtual class async_interface* async() { return nullptr; }
    class async_interface* experimental_async() { return async(); }
   private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::SMAReply>* AsyncSMA_CallRaw(::grpc::ClientContext* context, const ::SMARequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::SMAReply>* PrepareAsyncSMA_CallRaw(::grpc::ClientContext* context, const ::SMARequest& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());
    ::grpc::Status SMA_Call(::grpc::ClientContext* context, const ::SMARequest& request, ::SMAReply* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::SMAReply>> AsyncSMA_Call(::grpc::ClientContext* context, const ::SMARequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::SMAReply>>(AsyncSMA_CallRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::SMAReply>> PrepareAsyncSMA_Call(::grpc::ClientContext* context, const ::SMARequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::SMAReply>>(PrepareAsyncSMA_CallRaw(context, request, cq));
    }
    class async final :
      public StubInterface::async_interface {
     public:
      void SMA_Call(::grpc::ClientContext* context, const ::SMARequest* request, ::SMAReply* response, std::function<void(::grpc::Status)>) override;
      void SMA_Call(::grpc::ClientContext* context, const ::SMARequest* request, ::SMAReply* response, ::grpc::ClientUnaryReactor* reactor) override;
     private:
      friend class Stub;
      explicit async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class async* async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class async async_stub_{this};
    ::grpc::ClientAsyncResponseReader< ::SMAReply>* AsyncSMA_CallRaw(::grpc::ClientContext* context, const ::SMARequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::SMAReply>* PrepareAsyncSMA_CallRaw(::grpc::ClientContext* context, const ::SMARequest& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_SMA_Call_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status SMA_Call(::grpc::ServerContext* context, const ::SMARequest* request, ::SMAReply* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_SMA_Call : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_SMA_Call() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_SMA_Call() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SMA_Call(::grpc::ServerContext* /*context*/, const ::SMARequest* /*request*/, ::SMAReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestSMA_Call(::grpc::ServerContext* context, ::SMARequest* request, ::grpc::ServerAsyncResponseWriter< ::SMAReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_SMA_Call<Service > AsyncService;
  template <class BaseClass>
  class WithCallbackMethod_SMA_Call : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_SMA_Call() {
      ::grpc::Service::MarkMethodCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::SMARequest, ::SMAReply>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::SMARequest* request, ::SMAReply* response) { return this->SMA_Call(context, request, response); }));}
    void SetMessageAllocatorFor_SMA_Call(
        ::grpc::MessageAllocator< ::SMARequest, ::SMAReply>* allocator) {
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(0);
      static_cast<::grpc::internal::CallbackUnaryHandler< ::SMARequest, ::SMAReply>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~WithCallbackMethod_SMA_Call() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SMA_Call(::grpc::ServerContext* /*context*/, const ::SMARequest* /*request*/, ::SMAReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* SMA_Call(
      ::grpc::CallbackServerContext* /*context*/, const ::SMARequest* /*request*/, ::SMAReply* /*response*/)  { return nullptr; }
  };
  typedef WithCallbackMethod_SMA_Call<Service > CallbackService;
  typedef CallbackService ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_SMA_Call : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_SMA_Call() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_SMA_Call() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SMA_Call(::grpc::ServerContext* /*context*/, const ::SMARequest* /*request*/, ::SMAReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_SMA_Call : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_SMA_Call() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_SMA_Call() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SMA_Call(::grpc::ServerContext* /*context*/, const ::SMARequest* /*request*/, ::SMAReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestSMA_Call(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_SMA_Call : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_SMA_Call() {
      ::grpc::Service::MarkMethodRawCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->SMA_Call(context, request, response); }));
    }
    ~WithRawCallbackMethod_SMA_Call() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SMA_Call(::grpc::ServerContext* /*context*/, const ::SMARequest* /*request*/, ::SMAReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* SMA_Call(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_SMA_Call : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_SMA_Call() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler<
          ::SMARequest, ::SMAReply>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::SMARequest, ::SMAReply>* streamer) {
                       return this->StreamedSMA_Call(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_SMA_Call() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status SMA_Call(::grpc::ServerContext* /*context*/, const ::SMARequest* /*request*/, ::SMAReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedSMA_Call(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::SMARequest,::SMAReply>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_SMA_Call<Service > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_SMA_Call<Service > StreamedService;
};


#endif  // GRPC_protos_2fservice_2eproto__INCLUDED

#ifndef CLUE_STREAM_H
#define CLUE_STREAM_H
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <vector>
#include <grpcpp/grpcpp.h>
#include "clue.grpc.pb.h"

using grpc::Status;
using grpc::ClientContext;
using grpc::ClientReaderWriter;

namespace clue {

template <class RequestCreator, class Request, class Response>
class Stream : public grpc::ClientBidiReactor<Request, Response> {
  public:
    Stream(CLUE::Stub* stub, RequestCreator creator)
        : request_creator_(creator),
          stop_(false) {}

    void Start() {
      this->AddHold();
      this->StartCall();
    }

    Response FetchOne() {
      AddFetchNum(1);
      return ReadNext();
    }

    std::vector<Response> FetchMany(int num) {
      AddFetchNum(num);
      std::vector<Response> result;
      for (int i = 0; i < num; ++i) {
        result.push_back(ReadNext());
      }
      return result;
    }

    void OnDone(const Status& s) override {
      std::unique_lock<std::mutex> lock(mutex_);
      stop_ = true;
      response_condition_.notify_one();
    }
    void OnReadDone(bool ok) override {
      std::unique_lock<std::mutex> lock(mutex_);
      response_queue_.push(temp_response_);
      response_condition_.notify_one();
    }
    void Close() {
      this->RemoveHold();
      this->StartWritesDone();
    }

  protected:
    Response PopResponse() {
      Response response = response_queue_.front();
      response_queue_.pop();
      return response;
    }
    Response ReadNext() {
      std::unique_lock<std::mutex> lock(mutex_);

      if (!response_queue_.empty()) {
        return PopResponse();
      } else {
        this->StartRead(&temp_response_);
        response_condition_.wait(lock, [this] { return !response_queue_.empty() || stop_; });
        return PopResponse();
      }      
    }

    void AddFetchNum(int fetch_num) {
      std::shared_ptr<Request> request = request_creator_.get(fetch_num);
      this->StartWrite(request.get());
    }

    RequestCreator request_creator_;

    std::mutex mutex_;
    std::condition_variable response_condition_;

    std::queue<Response> response_queue_;
    Response temp_response_;

    bool stop_;
};

}
#endif

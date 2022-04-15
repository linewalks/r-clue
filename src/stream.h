#ifndef STREAM_H
#define STREAM_H
#include <stream.h>
#include "utils.h"

using namespace Rcpp;

template <class RequestCreator, class Request, class Response>
class Stream {
  public:
    Stream(std::shared_ptr<clue::Stream<RequestCreator, Request, Response>> stream) 
      : stream_(stream) {}

    DataFrame Fetch(int fetch_num) {
      std::vector<std::shared_ptr<Response>> response_vector;
      stream_->FetchMany(fetch_num, &response_vector);
      return convert_ptr_to_dataframe<Response>(response_vector);
    }

    DataFrame FetchAll() {
      std::vector<std::shared_ptr<Response>> response_vector;
      stream_->FetchAll(&response_vector);
      return convert_ptr_to_dataframe<Response>(response_vector);
    }

    void Close() {
      stream_->Close();
    }

  protected:
    std::shared_ptr<clue::Stream<RequestCreator, Request, Response>> stream_;
};
#endif

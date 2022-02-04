#ifndef STREAM_H
#define STREAM_H
#include <stream.h>
#include "utils.h"

using namespace Rcpp;

template <class Request, class Response>
class Stream {
  public:
    Stream(std::shared_ptr<clue::Stream<Request, Response>> stream) {
      stream_ = stream;
    }

    DataFrame Fetch(int fetch_num) {
      std::vector<Response> response_vector = stream_->FetchMany(fetch_num);
      return convert_to_dataframe<Response>(response_vector);
    }

  protected:
    std::shared_ptr<clue::Stream<Request, Response>> stream_;
};
#endif

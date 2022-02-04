#ifndef CONNECTION_H
#define CONNECTION_H
#include <connection.h>
#include "stream.h"
#include "utils.h"

using namespace Rcpp;

class Connection {
  public:
    Connection(std::shared_ptr<clue::Connection> connection) {
      connection_ = connection;
    }

    DataFrame GetCohortList(int page, int length, string term) {
      ResponseCohortList response = connection_->GetCohortList(page, length, term);
      int cohort_size = response.cohort_list_size();

      std::vector<CohortInfo> cohort_vector;
      for (int i = 0; i < cohort_size; ++i) {
        cohort_vector.push_back(response.cohort_list()[i]);
      }
      return convert_to_dataframe<CohortInfo>(cohort_vector);
    }

    Stream<RequestCohortStream, PersonInfo> GetCohortPersonTable(int cohort_id) {
      return Stream<RequestCohortStream, PersonInfo>(connection_->GetCohortPersonTable(cohort_id));
    }

  protected:
    std::shared_ptr<clue::Connection> connection_;
};
#endif

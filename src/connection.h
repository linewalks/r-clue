#ifndef CONNECTION_H
#define CONNECTION_H
#include <connection.h>
#include "stream.h"
#include "utils.h"

using namespace Rcpp;

#define GET_COHORT_TABLE_R(t) Stream<RequestCohortStream, INFO(t)> FUNC(t)(int cohort_id) { \
  return Stream<RequestCohortStream, INFO(t)>(connection_->FUNC(t)(cohort_id)); \
}

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

    GET_COHORT_TABLE_R(Person)
    GET_COHORT_TABLE_R(ConditionOccurrence)
    GET_COHORT_TABLE_R(Death)
    GET_COHORT_TABLE_R(DeviceExposure)
    GET_COHORT_TABLE_R(DrugExposure)
    GET_COHORT_TABLE_R(Measurement)
    GET_COHORT_TABLE_R(Observation)
    GET_COHORT_TABLE_R(ObservationPeriod)
    GET_COHORT_TABLE_R(ProcedureOccurrence)
    GET_COHORT_TABLE_R(VisitOccurrence)

  protected:
    std::shared_ptr<clue::Connection> connection_;
};
#endif

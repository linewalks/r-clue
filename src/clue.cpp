
#include <Rcpp.h>
#include "clue.h"
#include "utils.h"

using namespace Rcpp;

#define COHORT_FUNC_STRING(t) "GetCohort" #t "Table"
#define COHORT_TABLE_METHOD(t) .method(COHORT_FUNC_STRING(t), &Connection::FUNC(t))

#define EXPOSE_COHORT_STREAM(t) typedef Stream<clue::RequestCohortCreator, RequestCohortStream, t##Info> StreamCohort##t##Info; \
  RCPP_EXPOSED_CLASS_NODECL(StreamCohort##t##Info)

#define STREAM_CLASS(t) class_<t>(""#t"") \
  .method("Fetch", &t::Fetch) \
  .method("FetchAll", &t::FetchAll) \
  .method("Close", &t::Close); \

#define STREAM_COHORT_CLASS(t) STREAM_CLASS(StreamCohort##t##Info)

EXPOSE_COHORT_STREAM(Person)
EXPOSE_COHORT_STREAM(ConditionOccurrence)
EXPOSE_COHORT_STREAM(Death)
EXPOSE_COHORT_STREAM(DeviceExposure)
EXPOSE_COHORT_STREAM(DrugExposure)
EXPOSE_COHORT_STREAM(Measurement)
EXPOSE_COHORT_STREAM(Observation)
EXPOSE_COHORT_STREAM(ObservationPeriod)
EXPOSE_COHORT_STREAM(ProcedureOccurrence)
EXPOSE_COHORT_STREAM(VisitOccurrence)

typedef Stream<clue::RequestIncidenceRateCreator, RequestIncidenceRateStream, IncidenceRateRawInfo> StreamIncidenceRateRawInfo;
RCPP_EXPOSED_CLASS_NODECL(StreamIncidenceRateRawInfo)

RCPP_EXPOSED_CLASS(Connection)
RCPP_EXPOSED_CLASS(CLUEWrapper)

RCPP_MODULE(CLUE) {

  STREAM_COHORT_CLASS(Person);
  STREAM_COHORT_CLASS(ConditionOccurrence)
  STREAM_COHORT_CLASS(Death)
  STREAM_COHORT_CLASS(DeviceExposure)
  STREAM_COHORT_CLASS(DrugExposure)
  STREAM_COHORT_CLASS(Measurement)
  STREAM_COHORT_CLASS(Observation)
  STREAM_COHORT_CLASS(ObservationPeriod)
  STREAM_COHORT_CLASS(ProcedureOccurrence)
  STREAM_COHORT_CLASS(VisitOccurrence)
  STREAM_CLASS(StreamIncidenceRateRawInfo)

  class_<Connection>("Connection")
    .method("GetCohortList", &Connection::GetCohortList)
    COHORT_TABLE_METHOD(Person)
    COHORT_TABLE_METHOD(ConditionOccurrence)
    COHORT_TABLE_METHOD(Death)
    COHORT_TABLE_METHOD(DeviceExposure)
    COHORT_TABLE_METHOD(DrugExposure)
    COHORT_TABLE_METHOD(Measurement)
    COHORT_TABLE_METHOD(Observation)
    COHORT_TABLE_METHOD(ObservationPeriod)
    COHORT_TABLE_METHOD(ProcedureOccurrence)
    COHORT_TABLE_METHOD(VisitOccurrence)

    .method("GetCohortComparison", &Connection::GetCohortComparison)
    .method("GetIncidenceRateResult", &Connection::GetIncidenceRateResult)
    .method("GetIncidenceRateRaw", &Connection::GetIncidenceRateRaw)
    ;

  class_<CLUEWrapper>("CLUEWrapper")
    .constructor<std::string, int, std::string, std::string>()
    .method("Connect", &CLUEWrapper::Connect)
    ;
}

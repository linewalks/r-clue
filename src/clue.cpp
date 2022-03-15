
#include <Rcpp.h>
#include "clue.h"
#include "utils.h"

using namespace Rcpp;

#define FUNC_STRING(t) "GetCohort" #t "Table"
#define COHORT_TABLE_METHOD(t) .method(FUNC_STRING(t), &Connection::FUNC(t))

#define STREAM_TYPE_NAME Stream##t##Info
#define EXPOSE_STREAM(t) typedef Stream<RequestCohortStream, t##Info> Stream##t##Info; RCPP_EXPOSED_CLASS_NODECL(Stream##t##Info)

#define STREAM_CLASS_STRING(t) "Stream" #t "Info"
#define STREAM_CLASS(t) class_<Stream##t##Info>(STREAM_CLASS_STRING(t)) \
    .method("Fetch", &Stream##t##Info::Fetch) \
    .method("Close", &Stream##t##Info::Close) \
    ;

EXPOSE_STREAM(Person)
EXPOSE_STREAM(ConditionOccurrence)
EXPOSE_STREAM(Death)
EXPOSE_STREAM(DeviceExposure)
EXPOSE_STREAM(DrugExposure)
EXPOSE_STREAM(Measurement)
EXPOSE_STREAM(Observation)
EXPOSE_STREAM(ObservationPeriod)
EXPOSE_STREAM(ProcedureOccurrence)
EXPOSE_STREAM(VisitOccurrence)

RCPP_EXPOSED_CLASS(Connection)
RCPP_EXPOSED_CLASS(CLUEWrapper)

RCPP_MODULE(CLUE) {

  STREAM_CLASS(Person);
  STREAM_CLASS(ConditionOccurrence)
  STREAM_CLASS(Death)
  STREAM_CLASS(DeviceExposure)
  STREAM_CLASS(DrugExposure)
  STREAM_CLASS(Measurement)
  STREAM_CLASS(Observation)
  STREAM_CLASS(ObservationPeriod)
  STREAM_CLASS(ProcedureOccurrence)
  STREAM_CLASS(VisitOccurrence)

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
    ;

  class_<CLUEWrapper>("CLUEWrapper")
    .constructor<std::string, int, std::string, std::string>()
    .method("Connect", &CLUEWrapper::Connect)
    ;
}

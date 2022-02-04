
#include <Rcpp.h>
#include "clue.h"
#include "utils.h"

using namespace Rcpp;

typedef Stream<RequestCohortStream, PersonInfo> StreamPersonInfo;

RCPP_EXPOSED_CLASS_NODECL(StreamPersonInfo)
RCPP_EXPOSED_CLASS(Connection)
RCPP_EXPOSED_CLASS(CLUEWrapper)

RCPP_MODULE(CLUE) {

  class_<StreamPersonInfo>("StreamPersonInfo")
    .method("Fetch", &StreamPersonInfo::Fetch)
    ;

  class_<Connection>("Connection")
    .method("GetCohortList", &Connection::GetCohortList)
    .method("GetCohortPersonTable", &Connection::GetCohortPersonTable)
    ;

  class_<CLUEWrapper>("CLUEWrapper")
    .constructor<std::string, int, std::string, std::string>()
    .method("Connect", &CLUEWrapper::Connect)
    ;
}

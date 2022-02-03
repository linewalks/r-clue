
#include <Rcpp.h>
#include <clue.h>
#include <clue.grpc.pb.h>
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

  protected:
    std::shared_ptr<clue::Connection> connection_;
};

class CLUEWrapper {
  public:
    CLUEWrapper(std::string host, int port, std::string username, std::string password) {
      clue_ = std::unique_ptr<clue::CLUE>(new clue::CLUE(host, port, username, password));
    }

    Connection Connect() {
      std::shared_ptr<clue::Connection> connect_ptr = clue_->Connect();
      return Connection(connect_ptr);
    }

  protected:
    std::unique_ptr<clue::CLUE> clue_;
};

RCPP_EXPOSED_CLASS(Connection)
RCPP_EXPOSED_CLASS(CLUEWrapper)

RCPP_MODULE(CLUE) {

  class_<Connection>("Connection")
    .method("GetCohortList", &Connection::GetCohortList)
    ;

  class_<CLUEWrapper>("CLUEWrapper")
    .constructor<std::string, int, std::string, std::string>()
    .method("Connect", &CLUEWrapper::Connect)
    ;
}

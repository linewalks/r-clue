#ifndef CLUE_H
#define CLUE_H
#include <clue.h>
#include "connection.h"

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
#endif

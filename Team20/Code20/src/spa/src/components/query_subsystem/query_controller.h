#ifndef QUERY_CONTROLLER_H
#define QUERY_CONTROLLER_H

#include <list>
#include <string>

class QueryController {
 public:
  QueryController();
  void ProcessQuery(std::string query, std::list<std::string> &results);
};

#endif

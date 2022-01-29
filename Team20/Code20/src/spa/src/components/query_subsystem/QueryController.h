#ifndef SPA_QUERYCONTROLLER_H
#define SPA_QUERYCONTROLLER_H

#include <list>
#include <string>

class QueryController {
public:
    QueryController();
    void ProcessQuery(std::string query, std::list<std::string>& results);
};


#endif //SPA_QUERYCONTROLLER_H

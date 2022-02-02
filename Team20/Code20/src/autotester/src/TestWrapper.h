#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <string>
#include <iostream>
#include <list>

// include your other headers here
#include "AbstractWrapper.h"
#include "components/query_subsystem/QueryController.h"
#include "components/source_subsystem/SourceController.h"

class TestWrapper : public AbstractWrapper {
private:
    QueryController *queryController;
    SourceController *sourceController;

public:
    // default constructor
    TestWrapper();

    // destructor
    ~TestWrapper();
  
    // method for parsing the SIMPLE source
    virtual void parse(std::string filename);
  
    // method for evaluating a query
    virtual void evaluate(std::string query, std::list<std::string>& results);
};

#endif

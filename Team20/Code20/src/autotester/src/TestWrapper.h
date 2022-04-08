#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <string>
#include <iostream>
#include <list>

// include your other headers here
#include "AbstractWrapper.h"
#include "components/pkb/common.h"
#include "components/query_subsystem/query_controller.h"
#include "components/source_subsystem/source_controller.h"
#include "components/source_subsystem/source_declarations.h"

class TestWrapper : public AbstractWrapper {
 private:
  PkbPtr pkb;
  QueryController *query_controller;

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

#include "TestWrapper.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
    std::cout << "TestWrapper::TestWrapper() Start" << std::endl;
    queryController = new QueryController();
    sourceController = new SourceController();
    std::cout << "TestWrapper::TestWrapper() End" << std::endl;
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
    std::cout << "TestWrapper::parse() Start" << std::endl;
    std::string simple_source = sourceController->RetrieveFileContent(filename);
    std::cout << simple_source << std::endl;
    std::cout << "TestWrapper::parse() End" << std::endl;
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
// call your evaluator to evaluate the query here
  // ...code to evaluate query...

  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
}

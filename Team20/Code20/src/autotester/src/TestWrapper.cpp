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
    std::vector<SourceToken> tokens = sourceController->Tokenize(simple_source);
    std::cout << "TestWrapper::parse() End" << std::endl;
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
    std::cout << "TestWrapper::evaluate() Start" << std::endl;
    queryController->ProcessQuery(query, results);
    std::cout << "TestWrapper::evaluate() End" << std::endl;
}

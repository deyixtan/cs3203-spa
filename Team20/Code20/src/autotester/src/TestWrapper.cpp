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
  pkb = new PKB();
  source_controller = new source::SourceController();
  query_controller = new QueryController(pkb);
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
  source::TokenStream token_stream;
  source::String source = source_controller->RetrieveFileContent(filename);
  source_controller->Tokenize(source, token_stream);
  source::ProgramNodePtr ast = source_controller->ParseTokenStream(token_stream);
  source_controller->PopulatePKB(pkb, ast);
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results) {
  query_controller->ProcessQuery(query, results);
}

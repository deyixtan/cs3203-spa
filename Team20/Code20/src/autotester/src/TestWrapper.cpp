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
  pkb = PKB::get_instance();
  source_controller = new SourceController();
  query_controller = new QueryController(pkb);
  std::cout << "TestWrapper::TestWrapper() End" << std::endl;
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
  std::cout << "TestWrapper::parse() Start" << std::endl;
  std::string simple_source = source_controller->RetrieveFileContent(filename);
  std::vector<SourceToken *> tokens_ptr;
  source_controller->Tokenize(simple_source, tokens_ptr);

  // // Print tokens
  // std::vector<SourceToken *>::iterator it;
  // for (it = tokens_ptr.begin(); it != tokens_ptr.end(); ++it) {
  //  std::cout << (*it)->GetTypeStr() << " " << (*it)->GetValue() << std::endl;
  // }

  std::shared_ptr<ProgramNode> ast = source_controller->ParseTokenStream(tokens_ptr);
  source_controller->PopulatePKB(pkb, ast);

  std::cout << "RESULT: " << pkb->get_stmt_by_num(STMT).size() << std::endl;

  std::cout << "TestWrapper::parse() End" << std::endl;
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results) {
  std::cout << "TestWrapper::evaluate() Start" << std::endl;
  query_controller->ProcessQuery(query, results);
  std::cout << "TestWrapper::evaluate() End" << std::endl;
}

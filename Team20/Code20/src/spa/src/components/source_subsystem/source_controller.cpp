#include "source_controller.h"

namespace source {

SourceController::SourceController() {}

void SourceController::Tokenize(std::string program_source, std::vector<std::shared_ptr<SourceToken>> &tokens_ptr) {
  SourceLexer lexer = SourceLexer(program_source);
  try {
    lexer.Tokenize(tokens_ptr);
  } catch (const UnexpectedTokenException &unexpected_token) {
    std::cout << unexpected_token.what() << std::endl;
    tokens_ptr.clear();
  }
}

std::shared_ptr<ProgramNode> SourceController::ParseTokenStream(std::vector<std::shared_ptr<SourceToken>> &tokens_ptr) {
  SourceParser parser = SourceParser(tokens_ptr);
  std::shared_ptr<ProgramNode> program_node;
  try {
    program_node = parser.ParseProgram();
  } catch (const ProcedureExistException &procedure_exist) {
    program_node = std::make_shared<ProgramNode>();
    std::cout << "ERROR: " << procedure_exist.what() << std::endl;
  } catch (const CyclicCallException &cycle_call) {
    program_node = std::make_shared<ProgramNode>();
    std::cout << "ERROR: " << cycle_call.what() << std::endl;
  } catch (const EmptyStatementListException &empty_statement_list) {
    program_node = std::make_shared<ProgramNode>();
    std::cout << "ERROR: " << empty_statement_list.what() << std::endl;
  } catch (const EndOfStreamException &end_of_stream) {
    program_node = std::make_shared<ProgramNode>();
    std::cout << "ERROR: " << end_of_stream.what() << std::endl;
  } catch (const InvalidCallException &invalid_call) {
    program_node = std::make_shared<ProgramNode>();
    std::cout << "ERROR: " << invalid_call.what() << std::endl;
  } catch (const InvalidParseException &invalid_parse) {
    program_node = std::make_shared<ProgramNode>();
    std::cout << "ERROR: " << invalid_parse.what() << std::endl;
  } catch (const MismatchedTokenException &mismatched_token) {
    program_node = std::make_shared<ProgramNode>();
    std::cout << "ERROR: " << mismatched_token.what() << std::endl;
  } catch (const UnexpectedTokenException &unexpected_token) {
    program_node = std::make_shared<ProgramNode>();
    std::cout << "ERROR: " << unexpected_token.what() << std::endl;
  }
  return program_node;
}

std::string SourceController::RetrieveFileContent(std::string file_path) {
  return FileUtil::ReadFileContent(file_path);
}

void SourceController::PopulatePKB(PKB *pkb, std::shared_ptr<ProgramNode> ast) {
  Populator* populator = new Populator(pkb);
  DesignExtractor design_extractor = DesignExtractor(populator);
  design_extractor.Visit(ast);
}

}

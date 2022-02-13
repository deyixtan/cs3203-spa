#include "catch.hpp"
#include "components/source_subsystem/source_lexer.h"
#include "components/source_subsystem/source_parser.h"

using namespace source;

TEST_CASE("Test single procedure with one assign statement") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("procedure main { a = 1; }");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);
  program_node = parser.ParseProgram();

  // set up expected
  std::shared_ptr<VariableNode> variable_node = std::make_shared<VariableNode>("a");
  std::shared_ptr<ConstantNode> constant_node = std::make_shared<ConstantNode>("1");
  std::shared_ptr<AssignStatementNode>
      assign_stmt = std::make_shared<AssignStatementNode>(1, variable_node, constant_node);

  std::vector<std::shared_ptr<StatementNode>> statements;
  statements.emplace_back(assign_stmt);
  std::shared_ptr<StatementListNode> stmt_list = std::make_shared<StatementListNode>(statements);

  std::shared_ptr<ProcedureNode> procedure = std::make_shared<ProcedureNode>("main", stmt_list);

  std::vector<std::shared_ptr<ProcedureNode>> procedures;
  procedures.emplace_back(procedure);
  std::shared_ptr<ProgramNode> expected_program_node = std::make_shared<ProgramNode>(procedures);

  // test
  REQUIRE(*program_node == *expected_program_node);
}

TEST_CASE("Test single procedure with one read statement") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("procedure main { read a; }");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);
  program_node = parser.ParseProgram();

  // set up expected
  std::shared_ptr<VariableNode> variable_node = std::make_shared<VariableNode>("a");
  std::shared_ptr<ReadStatementNode>
      assign_stmt = std::make_shared<ReadStatementNode>(1, variable_node);

  std::vector<std::shared_ptr<StatementNode>> statements;
  statements.emplace_back(assign_stmt);
  std::shared_ptr<StatementListNode> stmt_list = std::make_shared<StatementListNode>(statements);

  std::shared_ptr<ProcedureNode> procedure = std::make_shared<ProcedureNode>("main", stmt_list);

  std::vector<std::shared_ptr<ProcedureNode>> procedures;
  procedures.emplace_back(procedure);
  std::shared_ptr<ProgramNode> expected_program_node = std::make_shared<ProgramNode>(procedures);

  // test
  REQUIRE(*program_node == *expected_program_node);
}

TEST_CASE("Test single procedure with one print statement") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("procedure main { print a; }");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);
  program_node = parser.ParseProgram();

  // set up expected
  std::shared_ptr<VariableNode> variable_node = std::make_shared<VariableNode>("a");
  std::shared_ptr<PrintStatementNode>
      assign_stmt = std::make_shared<PrintStatementNode>(1, variable_node);

  std::vector<std::shared_ptr<StatementNode>> statements;
  statements.emplace_back(assign_stmt);
  std::shared_ptr<StatementListNode> stmt_list = std::make_shared<StatementListNode>(statements);

  std::shared_ptr<ProcedureNode> procedure = std::make_shared<ProcedureNode>("main", stmt_list);

  std::vector<std::shared_ptr<ProcedureNode>> procedures;
  procedures.emplace_back(procedure);
  std::shared_ptr<ProgramNode> expected_program_node = std::make_shared<ProgramNode>(procedures);

  // test
  REQUIRE(*program_node == *expected_program_node);
}

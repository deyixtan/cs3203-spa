#include "catch.hpp"
#include "components/source_subsystem/source_lexer.h"
#include "components/source_subsystem/source_parser.h"
#include "components/source_subsystem/exceptions/mismatch_token.h"

using namespace source;

TEST_CASE("Test one read statement") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("read x;");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);

  // test
  REQUIRE_THROWS_WITH(parser.ParseProgram(), MismatchedTokenException().what());
}

TEST_CASE("Test one print statement") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("print x;");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);

  // test
  REQUIRE_THROWS_WITH(parser.ParseProgram(), MismatchedTokenException().what());
}

TEST_CASE("Test one if-statement") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("if (a == 1) then { a = 2; } else { a = 3; }");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);

  // test
  REQUIRE_THROWS_WITH(parser.ParseProgram(), MismatchedTokenException().what());
}

TEST_CASE("Test one while-statement") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("while (a == 1) { a = 2; }");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);

  // test
  REQUIRE_THROWS_WITH(parser.ParseProgram(), MismatchedTokenException().what());
}

TEST_CASE("Test one assign statement") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("a = 1;");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);

  // test
  REQUIRE_THROWS_WITH(parser.ParseProgram(), MismatchedTokenException().what());
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

TEST_CASE("Test single procedure with one if statement (simple equal condition)") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("procedure main { if (a == 1) then { a = 2; } else { a = 3; } }");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);
  program_node = parser.ParseProgram();

  // set up expected
  // if's stmt_list
  std::shared_ptr<VariableNode> if_variable_node = std::make_shared<VariableNode>("a");
  std::shared_ptr<ConstantNode> if_constant_node = std::make_shared<ConstantNode>("2");
  std::shared_ptr<AssignStatementNode>
      if_assign_stmt = std::make_shared<AssignStatementNode>(2, if_variable_node, if_constant_node);

  std::vector<std::shared_ptr<StatementNode>> if_statements;
  if_statements.emplace_back(if_assign_stmt);
  std::shared_ptr<StatementListNode> if_stmt_list = std::make_shared<StatementListNode>(if_statements);

  // else's stmt_list
  std::shared_ptr<VariableNode> else_variable_node = std::make_shared<VariableNode>("a");
  std::shared_ptr<ConstantNode> else_constant_node = std::make_shared<ConstantNode>("3");
  std::shared_ptr<AssignStatementNode>
      else_assign_stmt = std::make_shared<AssignStatementNode>(3, else_variable_node, else_constant_node);

  std::vector<std::shared_ptr<StatementNode>> else_statements;
  else_statements.emplace_back(else_assign_stmt);
  std::shared_ptr<StatementListNode> else_stmt_list = std::make_shared<StatementListNode>(else_statements);

  // condition
  std::shared_ptr<VariableNode> condition_variable_node = std::make_shared<VariableNode>("a");
  std::shared_ptr<ConstantNode> condition_constant_node = std::make_shared<ConstantNode>("1");
  std::shared_ptr<RelationalExpressionNode> condition_node =
      std::make_shared<RelationalExpressionNode>(RelationOperator::EQUALS,
                                                 condition_variable_node,
                                                 condition_constant_node);

  std::shared_ptr<IfStatementNode>
      if_stmt = std::make_shared<IfStatementNode>(1, condition_node, if_stmt_list, else_stmt_list);

  // if_stmt to procedure's stmt_list
  std::vector<std::shared_ptr<StatementNode>> statements;
  statements.emplace_back(if_stmt);
  std::shared_ptr<StatementListNode> stmt_list = std::make_shared<StatementListNode>(statements);

  std::shared_ptr<ProcedureNode> procedure = std::make_shared<ProcedureNode>("main", stmt_list);

  std::vector<std::shared_ptr<ProcedureNode>> procedures;
  procedures.emplace_back(procedure);
  std::shared_ptr<ProgramNode> expected_program_node = std::make_shared<ProgramNode>(procedures);

  // test
  REQUIRE(*program_node == *expected_program_node);
}

TEST_CASE("Test single procedure with one while statement (simple equal condition)") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("procedure main { while (a == 1) { a = 2; } }");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);
  program_node = parser.ParseProgram();

  // set up expected
  // while's stmt_list
  std::shared_ptr<VariableNode> variable_node = std::make_shared<VariableNode>("a");
  std::shared_ptr<ConstantNode> constant_node = std::make_shared<ConstantNode>("2");
  std::shared_ptr<AssignStatementNode>
      assign_stmt = std::make_shared<AssignStatementNode>(2, variable_node, constant_node);

  std::vector<std::shared_ptr<StatementNode>> while_statements;
  while_statements.emplace_back(assign_stmt);
  std::shared_ptr<StatementListNode> while_stmt_list = std::make_shared<StatementListNode>(while_statements);

  // condition
  std::shared_ptr<VariableNode> condition_variable_node = std::make_shared<VariableNode>("a");
  std::shared_ptr<ConstantNode> condition_constant_node = std::make_shared<ConstantNode>("1");
  std::shared_ptr<RelationalExpressionNode> condition_node =
      std::make_shared<RelationalExpressionNode>(RelationOperator::EQUALS,
                                                 condition_variable_node,
                                                 condition_constant_node);

  std::shared_ptr<WhileStatementNode>
      while_stmt = std::make_shared<WhileStatementNode>(1, condition_node, while_stmt_list);

  // while_stmt to procedure's stmt_list
  std::vector<std::shared_ptr<StatementNode>> statements;
  statements.emplace_back(while_stmt);
  std::shared_ptr<StatementListNode> stmt_list = std::make_shared<StatementListNode>(statements);

  std::shared_ptr<ProcedureNode> procedure = std::make_shared<ProcedureNode>("main", stmt_list);

  std::vector<std::shared_ptr<ProcedureNode>> procedures;
  procedures.emplace_back(procedure);
  std::shared_ptr<ProgramNode> expected_program_node = std::make_shared<ProgramNode>(procedures);

  // test
  REQUIRE(*program_node == *expected_program_node);
}

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

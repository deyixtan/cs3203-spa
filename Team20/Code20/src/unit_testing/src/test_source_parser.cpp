#include "catch.hpp"
#include "components/source_subsystem/lexer/source_lexer.h"
#include "components/source_subsystem/parser/source_parser.h"
#include "components/source_subsystem/exceptions/empty_statement_list.h"
#include "components/source_subsystem/exceptions/invalid_parse_condition.h"
#include "components/source_subsystem/exceptions/invalid_parse_factor.h"
#include "components/source_subsystem/exceptions/invalid_parse_relation.h"
#include "components/source_subsystem/exceptions/invalid_parse_statement.h"
#include "components/source_subsystem/exceptions/mismatch_token.h"
#include "components/source_subsystem/exceptions/unexpected_token.h"

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

TEST_CASE("Test single program with no procedure") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);

  // test
  REQUIRE_THROWS_WITH(parser.ParseProgram(), EmptyStatementListException().what());
}

TEST_CASE("Test single procedure with no statement") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("procedure main { }");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);

  // test
  REQUIRE_THROWS_WITH(parser.ParseProgram(), EmptyStatementListException().what());
}

TEST_CASE("Test invalid single procedure with one assign statement") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("procedure main {{ a = 1; }");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);

  // test
  REQUIRE_THROWS_WITH(parser.ParseProgram(), InvalidParseStatementException().what());
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
  std::shared_ptr<VariableNode> variable_node = std::make_shared<VariableNode>("a", "1", "main");
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
  std::shared_ptr<VariableNode> variable_node = std::make_shared<VariableNode>("a", "1", "main");
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
  std::shared_ptr<VariableNode> if_variable_node = std::make_shared<VariableNode>("a", "2", "main");
  std::shared_ptr<ConstantNode> if_constant_node = std::make_shared<ConstantNode>("2");
  std::shared_ptr<AssignStatementNode>
      if_assign_stmt = std::make_shared<AssignStatementNode>(2, if_variable_node, if_constant_node);

  std::vector<std::shared_ptr<StatementNode>> if_statements;
  if_statements.emplace_back(if_assign_stmt);
  std::shared_ptr<StatementListNode> if_stmt_list = std::make_shared<StatementListNode>(if_statements);

  // else's stmt_list
  std::shared_ptr<VariableNode> else_variable_node = std::make_shared<VariableNode>("a", "3", "main");
  std::shared_ptr<ConstantNode> else_constant_node = std::make_shared<ConstantNode>("3");
  std::shared_ptr<AssignStatementNode>
      else_assign_stmt = std::make_shared<AssignStatementNode>(3, else_variable_node, else_constant_node);

  std::vector<std::shared_ptr<StatementNode>> else_statements;
  else_statements.emplace_back(else_assign_stmt);
  std::shared_ptr<StatementListNode> else_stmt_list = std::make_shared<StatementListNode>(else_statements);

  // condition
  std::shared_ptr<VariableNode> condition_variable_node = std::make_shared<VariableNode>("a", "1", "main");
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

TEST_CASE("Test single procedure with one if statement (simple not equal condition)") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("procedure main { if (a != 1) then { a = 2; } else { a = 3; } }");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);
  program_node = parser.ParseProgram();

  // set up expected
  // if's stmt_list
  std::shared_ptr<VariableNode> if_variable_node = std::make_shared<VariableNode>("a", "2", "main");
  std::shared_ptr<ConstantNode> if_constant_node = std::make_shared<ConstantNode>("2");
  std::shared_ptr<AssignStatementNode>
      if_assign_stmt = std::make_shared<AssignStatementNode>(2, if_variable_node, if_constant_node);

  std::vector<std::shared_ptr<StatementNode>> if_statements;
  if_statements.emplace_back(if_assign_stmt);
  std::shared_ptr<StatementListNode> if_stmt_list = std::make_shared<StatementListNode>(if_statements);

  // else's stmt_list
  std::shared_ptr<VariableNode> else_variable_node = std::make_shared<VariableNode>("a", "3", "main");
  std::shared_ptr<ConstantNode> else_constant_node = std::make_shared<ConstantNode>("3");
  std::shared_ptr<AssignStatementNode>
      else_assign_stmt = std::make_shared<AssignStatementNode>(3, else_variable_node, else_constant_node);

  std::vector<std::shared_ptr<StatementNode>> else_statements;
  else_statements.emplace_back(else_assign_stmt);
  std::shared_ptr<StatementListNode> else_stmt_list = std::make_shared<StatementListNode>(else_statements);

  // condition
  std::shared_ptr<VariableNode> condition_variable_node = std::make_shared<VariableNode>("a", "1", "main");
  std::shared_ptr<ConstantNode> condition_constant_node = std::make_shared<ConstantNode>("1");
  std::shared_ptr<RelationalExpressionNode> condition_node =
      std::make_shared<RelationalExpressionNode>(RelationOperator::NOT_EQUALS,
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

TEST_CASE("Test single procedure with one if statement (simple greater than condition)") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("procedure main { if (a > 1) then { a = 2; } else { a = 3; } }");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);
  program_node = parser.ParseProgram();

  // set up expected
  // if's stmt_list
  std::shared_ptr<VariableNode> if_variable_node = std::make_shared<VariableNode>("a", "2", "main");
  std::shared_ptr<ConstantNode> if_constant_node = std::make_shared<ConstantNode>("2");
  std::shared_ptr<AssignStatementNode>
      if_assign_stmt = std::make_shared<AssignStatementNode>(2, if_variable_node, if_constant_node);

  std::vector<std::shared_ptr<StatementNode>> if_statements;
  if_statements.emplace_back(if_assign_stmt);
  std::shared_ptr<StatementListNode> if_stmt_list = std::make_shared<StatementListNode>(if_statements);

  // else's stmt_list
  std::shared_ptr<VariableNode> else_variable_node = std::make_shared<VariableNode>("a", "3", "main");
  std::shared_ptr<ConstantNode> else_constant_node = std::make_shared<ConstantNode>("3");
  std::shared_ptr<AssignStatementNode>
      else_assign_stmt = std::make_shared<AssignStatementNode>(3, else_variable_node, else_constant_node);

  std::vector<std::shared_ptr<StatementNode>> else_statements;
  else_statements.emplace_back(else_assign_stmt);
  std::shared_ptr<StatementListNode> else_stmt_list = std::make_shared<StatementListNode>(else_statements);

  // condition
  std::shared_ptr<VariableNode> condition_variable_node = std::make_shared<VariableNode>("a", "1", "main");
  std::shared_ptr<ConstantNode> condition_constant_node = std::make_shared<ConstantNode>("1");
  std::shared_ptr<RelationalExpressionNode> condition_node =
      std::make_shared<RelationalExpressionNode>(RelationOperator::GREATER_THAN,
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

TEST_CASE("Test single procedure with one if statement (simple greater than equal condition)") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("procedure main { if (a >= 1) then { a = 2; } else { a = 3; } }");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);
  program_node = parser.ParseProgram();

  // set up expected
  // if's stmt_list
  std::shared_ptr<VariableNode> if_variable_node = std::make_shared<VariableNode>("a", "2", "main");
  std::shared_ptr<ConstantNode> if_constant_node = std::make_shared<ConstantNode>("2");
  std::shared_ptr<AssignStatementNode>
      if_assign_stmt = std::make_shared<AssignStatementNode>(2, if_variable_node, if_constant_node);

  std::vector<std::shared_ptr<StatementNode>> if_statements;
  if_statements.emplace_back(if_assign_stmt);
  std::shared_ptr<StatementListNode> if_stmt_list = std::make_shared<StatementListNode>(if_statements);

  // else's stmt_list
  std::shared_ptr<VariableNode> else_variable_node = std::make_shared<VariableNode>("a", "3", "main");
  std::shared_ptr<ConstantNode> else_constant_node = std::make_shared<ConstantNode>("3");
  std::shared_ptr<AssignStatementNode>
      else_assign_stmt = std::make_shared<AssignStatementNode>(3, else_variable_node, else_constant_node);

  std::vector<std::shared_ptr<StatementNode>> else_statements;
  else_statements.emplace_back(else_assign_stmt);
  std::shared_ptr<StatementListNode> else_stmt_list = std::make_shared<StatementListNode>(else_statements);

  // condition
  std::shared_ptr<VariableNode> condition_variable_node = std::make_shared<VariableNode>("a", "1", "main");
  std::shared_ptr<ConstantNode> condition_constant_node = std::make_shared<ConstantNode>("1");
  std::shared_ptr<RelationalExpressionNode> condition_node =
      std::make_shared<RelationalExpressionNode>(RelationOperator::GREATER_THAN_EQUALS,
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

TEST_CASE("Test single procedure with one if statement (simple lesser than condition)") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("procedure main { if (a < 1) then { a = 2; } else { a = 3; } }");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);
  program_node = parser.ParseProgram();

  // set up expected
  // if's stmt_list
  std::shared_ptr<VariableNode> if_variable_node = std::make_shared<VariableNode>("a", "2", "main");
  std::shared_ptr<ConstantNode> if_constant_node = std::make_shared<ConstantNode>("2");
  std::shared_ptr<AssignStatementNode>
      if_assign_stmt = std::make_shared<AssignStatementNode>(2, if_variable_node, if_constant_node);

  std::vector<std::shared_ptr<StatementNode>> if_statements;
  if_statements.emplace_back(if_assign_stmt);
  std::shared_ptr<StatementListNode> if_stmt_list = std::make_shared<StatementListNode>(if_statements);

  // else's stmt_list
  std::shared_ptr<VariableNode> else_variable_node = std::make_shared<VariableNode>("a", "3", "main");
  std::shared_ptr<ConstantNode> else_constant_node = std::make_shared<ConstantNode>("3");
  std::shared_ptr<AssignStatementNode>
      else_assign_stmt = std::make_shared<AssignStatementNode>(3, else_variable_node, else_constant_node);

  std::vector<std::shared_ptr<StatementNode>> else_statements;
  else_statements.emplace_back(else_assign_stmt);
  std::shared_ptr<StatementListNode> else_stmt_list = std::make_shared<StatementListNode>(else_statements);

  // condition
  std::shared_ptr<VariableNode> condition_variable_node = std::make_shared<VariableNode>("a", "1", "main");
  std::shared_ptr<ConstantNode> condition_constant_node = std::make_shared<ConstantNode>("1");
  std::shared_ptr<RelationalExpressionNode> condition_node =
      std::make_shared<RelationalExpressionNode>(RelationOperator::LESS_THAN,
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

TEST_CASE("Test single procedure with one if statement (simple lesser than equal condition)") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("procedure main { if (a <= 1) then { a = 2; } else { a = 3; } }");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);
  program_node = parser.ParseProgram();

  // set up expected
  // if's stmt_list
  std::shared_ptr<VariableNode> if_variable_node = std::make_shared<VariableNode>("a", "2", "main");
  std::shared_ptr<ConstantNode> if_constant_node = std::make_shared<ConstantNode>("2");
  std::shared_ptr<AssignStatementNode>
      if_assign_stmt = std::make_shared<AssignStatementNode>(2, if_variable_node, if_constant_node);

  std::vector<std::shared_ptr<StatementNode>> if_statements;
  if_statements.emplace_back(if_assign_stmt);
  std::shared_ptr<StatementListNode> if_stmt_list = std::make_shared<StatementListNode>(if_statements);

  // else's stmt_list
  std::shared_ptr<VariableNode> else_variable_node = std::make_shared<VariableNode>("a", "3", "main");
  std::shared_ptr<ConstantNode> else_constant_node = std::make_shared<ConstantNode>("3");
  std::shared_ptr<AssignStatementNode>
      else_assign_stmt = std::make_shared<AssignStatementNode>(3, else_variable_node, else_constant_node);

  std::vector<std::shared_ptr<StatementNode>> else_statements;
  else_statements.emplace_back(else_assign_stmt);
  std::shared_ptr<StatementListNode> else_stmt_list = std::make_shared<StatementListNode>(else_statements);

  // condition
  std::shared_ptr<VariableNode> condition_variable_node = std::make_shared<VariableNode>("a", "1", "main");
  std::shared_ptr<ConstantNode> condition_constant_node = std::make_shared<ConstantNode>("1");
  std::shared_ptr<RelationalExpressionNode> condition_node =
      std::make_shared<RelationalExpressionNode>(RelationOperator::LESS_THAN_EQUALS,
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

TEST_CASE("Test single procedure with one if statement (invalid condition)") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("procedure main { if (a a a) then { a = 2; } else { a = 3; } }");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);

  // test
  REQUIRE_THROWS_WITH(parser.ParseProgram(), InvalidParseRelationException().what());
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
  std::shared_ptr<VariableNode> variable_node = std::make_shared<VariableNode>("a", "2", "main");
  std::shared_ptr<ConstantNode> constant_node = std::make_shared<ConstantNode>("2");
  std::shared_ptr<AssignStatementNode>
      assign_stmt = std::make_shared<AssignStatementNode>(2, variable_node, constant_node);

  std::vector<std::shared_ptr<StatementNode>> while_statements;
  while_statements.emplace_back(assign_stmt);
  std::shared_ptr<StatementListNode> while_stmt_list = std::make_shared<StatementListNode>(while_statements);

  // condition
  std::shared_ptr<VariableNode> condition_variable_node = std::make_shared<VariableNode>("a", "1", "main");
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

TEST_CASE("Test single procedure with one while statement (logical AND 2 relation expressions)") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("procedure main { while ((a == 1) && (a == 2)) { a = 3; } }");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);
  program_node = parser.ParseProgram();

  // set up expected
  // while's stmt_list
  std::shared_ptr<VariableNode> variable_node = std::make_shared<VariableNode>("a", "3", "main");
  std::shared_ptr<ConstantNode> constant_node = std::make_shared<ConstantNode>("3");
  std::shared_ptr<AssignStatementNode>
      assign_stmt = std::make_shared<AssignStatementNode>(2, variable_node, constant_node);

  std::vector<std::shared_ptr<StatementNode>> while_statements;
  while_statements.emplace_back(assign_stmt);
  std::shared_ptr<StatementListNode> while_stmt_list = std::make_shared<StatementListNode>(while_statements);

  // condition
  std::shared_ptr<VariableNode> condition_variable_node1 = std::make_shared<VariableNode>("a", "1", "main");
  std::shared_ptr<ConstantNode> condition_constant_node1 = std::make_shared<ConstantNode>("1");
  std::shared_ptr<RelationalExpressionNode> condition_node1 =
      std::make_shared<RelationalExpressionNode>(RelationOperator::EQUALS,
                                                 condition_variable_node1,
                                                 condition_constant_node1);

  std::shared_ptr<VariableNode> condition_variable_node2 = std::make_shared<VariableNode>("a", "2", "main");
  std::shared_ptr<ConstantNode> condition_constant_node2 = std::make_shared<ConstantNode>("2");
  std::shared_ptr<RelationalExpressionNode> condition_node2 =
      std::make_shared<RelationalExpressionNode>(RelationOperator::EQUALS,
                                                 condition_variable_node2,
                                                 condition_constant_node2);

  std::shared_ptr<BooleanExpressionNode> condition_variable_node3 =
      std::make_shared<BooleanExpressionNode>(BooleanOperator::AND, condition_node1, condition_node2);

  std::shared_ptr<WhileStatementNode>
      while_stmt = std::make_shared<WhileStatementNode>(1, condition_variable_node3, while_stmt_list);

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

TEST_CASE("Test single procedure with one while statement (logical OR 2 relation expressions)") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("procedure main { while ((a == 1) || (a == 2)) { a = 3; } }");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);
  program_node = parser.ParseProgram();

  // set up expected
  // while's stmt_list
  std::shared_ptr<VariableNode> variable_node = std::make_shared<VariableNode>("a", "3", "main");
  std::shared_ptr<ConstantNode> constant_node = std::make_shared<ConstantNode>("3");
  std::shared_ptr<AssignStatementNode>
      assign_stmt = std::make_shared<AssignStatementNode>(2, variable_node, constant_node);

  std::vector<std::shared_ptr<StatementNode>> while_statements;
  while_statements.emplace_back(assign_stmt);
  std::shared_ptr<StatementListNode> while_stmt_list = std::make_shared<StatementListNode>(while_statements);

  // condition
  std::shared_ptr<VariableNode> condition_variable_node1 = std::make_shared<VariableNode>("a", "1", "main");
  std::shared_ptr<ConstantNode> condition_constant_node1 = std::make_shared<ConstantNode>("1");
  std::shared_ptr<RelationalExpressionNode> condition_node1 =
      std::make_shared<RelationalExpressionNode>(RelationOperator::EQUALS,
                                                 condition_variable_node1,
                                                 condition_constant_node1);

  std::shared_ptr<VariableNode> condition_variable_node2 = std::make_shared<VariableNode>("a", "2", "main");
  std::shared_ptr<ConstantNode> condition_constant_node2 = std::make_shared<ConstantNode>("2");
  std::shared_ptr<RelationalExpressionNode> condition_node2 =
      std::make_shared<RelationalExpressionNode>(RelationOperator::EQUALS,
                                                 condition_variable_node2,
                                                 condition_constant_node2);

  std::shared_ptr<BooleanExpressionNode> condition_variable_node3 =
      std::make_shared<BooleanExpressionNode>(BooleanOperator::OR, condition_node1, condition_node2);

  std::shared_ptr<WhileStatementNode>
      while_stmt = std::make_shared<WhileStatementNode>(1, condition_variable_node3, while_stmt_list);

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

TEST_CASE("Test single procedure with while statement (negate boolean expression)") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("procedure main { while (!(a == 1)) { a = 2; } }");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);
  program_node = parser.ParseProgram();

  // set up expected
  // while's stmt_list
  std::shared_ptr<VariableNode> variable_node = std::make_shared<VariableNode>("a", "2", "main");
  std::shared_ptr<ConstantNode> constant_node = std::make_shared<ConstantNode>("2");
  std::shared_ptr<AssignStatementNode>
      assign_stmt = std::make_shared<AssignStatementNode>(2, variable_node, constant_node);

  std::vector<std::shared_ptr<StatementNode>> while_statements;
  while_statements.emplace_back(assign_stmt);
  std::shared_ptr<StatementListNode> while_stmt_list = std::make_shared<StatementListNode>(while_statements);

  // condition
  std::shared_ptr<VariableNode> condition_variable_node1 = std::make_shared<VariableNode>("a", "1", "main");
  std::shared_ptr<ConstantNode> condition_constant_node1 = std::make_shared<ConstantNode>("1");
  std::shared_ptr<RelationalExpressionNode> condition_node1 =
      std::make_shared<RelationalExpressionNode>(RelationOperator::EQUALS,
                                                 condition_variable_node1,
                                                 condition_constant_node1);
  std::shared_ptr<NotExpressionNode> condition_node2 = std::make_shared<NotExpressionNode>(condition_node1);

  std::shared_ptr<WhileStatementNode>
      while_stmt = std::make_shared<WhileStatementNode>(1, condition_node2, while_stmt_list);

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
  std::shared_ptr<VariableNode> variable_node = std::make_shared<VariableNode>("a", "1", "main");
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

TEST_CASE("Test single procedure with one assign statement (addition expression)") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("procedure main { a = a + 1; }");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);
  program_node = parser.ParseProgram();

  // set up expected
  std::shared_ptr<VariableNode> variable1_node = std::make_shared<VariableNode>("a", "1", "main");
  std::shared_ptr<ConstantNode> constant_node = std::make_shared<ConstantNode>("1");
  std::shared_ptr<CombinationExpressionNode> combination_node =
      std::make_shared<CombinationExpressionNode>(ArithmeticOperator::PLUS, variable1_node, constant_node);

  std::shared_ptr<VariableNode> variable2_node = std::make_shared<VariableNode>("a", "1", "main");
  std::shared_ptr<AssignStatementNode>
      assign_stmt = std::make_shared<AssignStatementNode>(1, variable2_node, combination_node);

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


TEST_CASE("Test single procedure with one assign statement (subtraction expression)") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("procedure main { a = a - 1; }");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);
  program_node = parser.ParseProgram();

  // set up expected
  std::shared_ptr<VariableNode> variable1_node = std::make_shared<VariableNode>("a", "1", "main");
  std::shared_ptr<ConstantNode> constant_node = std::make_shared<ConstantNode>("1");
  std::shared_ptr<CombinationExpressionNode> combination_node =
      std::make_shared<CombinationExpressionNode>(ArithmeticOperator::MINUS, variable1_node, constant_node);

  std::shared_ptr<VariableNode> variable2_node = std::make_shared<VariableNode>("a", "1", "main");
  std::shared_ptr<AssignStatementNode>
      assign_stmt = std::make_shared<AssignStatementNode>(1, variable2_node, combination_node);

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

TEST_CASE("Test single procedure with one assign statement (multiply expression)") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("procedure main { a = a * 1; }");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);
  program_node = parser.ParseProgram();

  // set up expected
  std::shared_ptr<VariableNode> variable1_node = std::make_shared<VariableNode>("a", "1", "main");
  std::shared_ptr<ConstantNode> constant_node = std::make_shared<ConstantNode>("1");
  std::shared_ptr<CombinationExpressionNode> combination_node =
      std::make_shared<CombinationExpressionNode>(ArithmeticOperator::MULTIPLY, variable1_node, constant_node);

  std::shared_ptr<VariableNode> variable2_node = std::make_shared<VariableNode>("a", "1", "main");
  std::shared_ptr<AssignStatementNode>
      assign_stmt = std::make_shared<AssignStatementNode>(1, variable2_node, combination_node);

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

TEST_CASE("Test single procedure with one assign statement (divide expression)") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("procedure main { a = a / 1; }");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);
  program_node = parser.ParseProgram();

  // set up expected
  std::shared_ptr<VariableNode> variable1_node = std::make_shared<VariableNode>("a", "1", "main");
  std::shared_ptr<ConstantNode> constant_node = std::make_shared<ConstantNode>("1");
  std::shared_ptr<CombinationExpressionNode> combination_node =
      std::make_shared<CombinationExpressionNode>(ArithmeticOperator::DIVIDE, variable1_node, constant_node);

  std::shared_ptr<VariableNode> variable2_node = std::make_shared<VariableNode>("a", "1", "main");
  std::shared_ptr<AssignStatementNode>
      assign_stmt = std::make_shared<AssignStatementNode>(1, variable2_node, combination_node);

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

TEST_CASE("Test single procedure with one assign statement (mod expression)") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("procedure main { a = a % 1; }");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);
  program_node = parser.ParseProgram();

  // set up expected
  std::shared_ptr<VariableNode> variable1_node = std::make_shared<VariableNode>("a", "1", "main");
  std::shared_ptr<ConstantNode> constant_node = std::make_shared<ConstantNode>("1");
  std::shared_ptr<CombinationExpressionNode> combination_node =
      std::make_shared<CombinationExpressionNode>(ArithmeticOperator::MOD, variable1_node, constant_node);

  std::shared_ptr<VariableNode> variable2_node = std::make_shared<VariableNode>("a", "1", "main");
  std::shared_ptr<AssignStatementNode>
      assign_stmt = std::make_shared<AssignStatementNode>(1, variable2_node, combination_node);

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

TEST_CASE("Test single procedure with one invalid assign statement") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("procedure main { a =");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);

  // test
  REQUIRE_THROWS_WITH(parser.ParseProgram(), EndOfStreamException().what());
}

TEST_CASE("Test single procedure with one invalid assign statement 2") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("procedure main { a = }");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);

  // test
  REQUIRE_THROWS_WITH(parser.ParseProgram(), InvalidParseFactorException().what());
}

TEST_CASE("Test single procedure with one invalid if-statement") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("procedure main { if ( }");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);

  // test
  REQUIRE_THROWS_WITH(parser.ParseProgram(), InvalidParseConditionException().what());
}

TEST_CASE("Test single procedure with one assign statement (with combination expression)") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("procedure main { a = b / 5; }");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);
  program_node = parser.ParseProgram();

  // set up expected
  std::shared_ptr<VariableNode> variable_a_node = std::make_shared<VariableNode>("a", "5", "main");
  std::shared_ptr<VariableNode> variable_b_node = std::make_shared<VariableNode>("b", "5", "main");
  std::shared_ptr<ConstantNode> constant_node = std::make_shared<ConstantNode>("5");
  std::shared_ptr<CombinationExpressionNode> combination_node =
      std::make_shared<CombinationExpressionNode>(ArithmeticOperator::DIVIDE, variable_b_node, constant_node);
  std::shared_ptr<AssignStatementNode>
      assign_stmt = std::make_shared<AssignStatementNode>(1, variable_a_node, combination_node);

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


TEST_CASE("Test single procedure with one assign statement (with combination expression 2)") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("procedure main { a = (b / 5) + 2; }");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);
  program_node = parser.ParseProgram();

  // set up expected
  std::shared_ptr<VariableNode> variable_node1 = std::make_shared<VariableNode>("b", "5", "main");
  std::shared_ptr<ConstantNode> constant_node1 = std::make_shared<ConstantNode>("5");
  std::shared_ptr<CombinationExpressionNode> combination_node1 =
      std::make_shared<CombinationExpressionNode>(ArithmeticOperator::DIVIDE, variable_node1, constant_node1);

  std::shared_ptr<ConstantNode> constant_node2 = std::make_shared<ConstantNode>("2");
  std::shared_ptr<CombinationExpressionNode> combination_node2 =
      std::make_shared<CombinationExpressionNode>(ArithmeticOperator::PLUS, combination_node1, constant_node2);

  std::shared_ptr<VariableNode> variable_node2 = std::make_shared<VariableNode>("a", "1", "main");
  std::shared_ptr<AssignStatementNode>
      assign_stmt = std::make_shared<AssignStatementNode>(1, variable_node2, combination_node2);

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

TEST_CASE("Test single procedure with one call statement") {
  // set up actual
  std::vector<std::shared_ptr<SourceToken>> token_list;
  std::shared_ptr<ProgramNode> program_node;

  SourceLexer lexer = SourceLexer("procedure foo { a = b / 5; } procedure main { call foo; }");
  lexer.Tokenize(token_list);
  SourceParser parser = SourceParser(token_list);
  program_node = parser.ParseProgram();

  // set up expected
  std::shared_ptr<VariableNode> variable_a_node = std::make_shared<VariableNode>("a", "5", "foo");
  std::shared_ptr<VariableNode> variable_b_node = std::make_shared<VariableNode>("b", "5", "foo");
  std::shared_ptr<ConstantNode> constant_node = std::make_shared<ConstantNode>("5");
  std::shared_ptr<CombinationExpressionNode> combination_node =
      std::make_shared<CombinationExpressionNode>(ArithmeticOperator::DIVIDE, variable_b_node, constant_node);
  std::shared_ptr<AssignStatementNode>
      assign_stmt = std::make_shared<AssignStatementNode>(1, variable_a_node, combination_node);

  std::vector<std::shared_ptr<StatementNode>> statements;
  statements.emplace_back(assign_stmt);
  std::shared_ptr<StatementListNode> stmt_list = std::make_shared<StatementListNode>(statements);

  std::shared_ptr<ProcedureNode> procedure = std::make_shared<ProcedureNode>("foo", stmt_list);
  std::shared_ptr<CallStatementNode> call_stmt = std::make_shared<CallStatementNode>(2, "main", "foo");
  std::vector<std::shared_ptr<StatementNode>> statements_main;
  statements_main.emplace_back(call_stmt);
  std::shared_ptr<StatementListNode> stmt_list_main = std::make_shared<StatementListNode>(statements_main);

  std::shared_ptr<ProcedureNode> procedure_main = std::make_shared<ProcedureNode>("main", stmt_list_main);

  std::vector<std::shared_ptr<ProcedureNode>> procedures;
  procedures.emplace_back(procedure);
  procedures.emplace_back(procedure_main);
  std::shared_ptr<ProgramNode> expected_program_node = std::make_shared<ProgramNode>(procedures);

  // test
  REQUIRE(*program_node == *expected_program_node);
}

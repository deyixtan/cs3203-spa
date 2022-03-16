#include "catch.hpp"
#include "components/source_subsystem/lexer/source_lexer.h"
#include "components/source_subsystem/design_extractor.h"
#include "components/source_subsystem/populator.h"
#include "components/source_subsystem/exceptions/unexpected_token.h"
#include "components/source_subsystem/ast/node_variable.h"
#include "components/source_subsystem/ast/node_read_statement.h"
#include "components/source_subsystem/ast/node_print_statement.h"
#include "components/source_subsystem/ast/node_combination_expression.h"
#include "components/source_subsystem/ast/node_constant.h"
#include "components/source_subsystem/ast/node_assign_statement.h"
#include "components/source_subsystem/ast/node_if_statement.h"
#include "components/source_subsystem/ast/node_relational_expression.h"
#include "components/source_subsystem/ast/node_boolean_expression.h"
#include "components/source_subsystem/ast/node_while_statement.h"

using namespace source;

TEST_CASE("Test DE Modify population for single procedure with one read statement") {
  // set up AST for: procedure main { read a; }
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

  // set up expected PKB
  PKB *pkb = new PKB();
  pkb->AddStmt("1", STMT);
  pkb->AddStmt("1", READ);
  pkb->AddStmt("main", PROC);
  pkb->GetModifyStore()->AddStmtVar("1", "a");
  pkb->GetModifyStore()->AddProcVar("main", "a");

  // set up actual traverse of DE
  PKB *test_pkb = new PKB();
  Populator *populator = new Populator(test_pkb);
  DesignExtractor *design_extractor = new DesignExtractor(*expected_program_node, *populator);
  design_extractor->TraverseAst();

  // test
  REQUIRE(test_pkb->GetStmt(STMT) == pkb->GetStmt(STMT));
  REQUIRE(test_pkb->GetStmt(READ) == pkb->GetStmt(READ));
  REQUIRE(test_pkb->GetStmt(PROC) == pkb->GetStmt(PROC));
  REQUIRE(test_pkb->GetModifyStore()->GetAllModStmt(STMT) == pkb->GetModifyStore()->GetAllModStmt(STMT));
  REQUIRE(test_pkb->GetModifyStore()->GetVarModByStmt("1") == pkb->GetModifyStore()->GetVarModByStmt("1"));
  REQUIRE(test_pkb->GetModifyStore()->GetStmtModByVar("a") == pkb->GetModifyStore()->GetStmtModByVar("a"));
  //REQUIRE(test_pkb->GetModifyStore()->GetProcModByVar("a") == pkb->GetModifyStore()->GetProcModByVar("a"));
  //REQUIRE(test_pkb->GetModifyStore()->GetVarModByProc("main") == pkb->GetModifyStore()->GetVarModByProc("main"));
  REQUIRE(test_pkb->GetModifyStore()->GetAllStmtModify() == pkb->GetModifyStore()->GetAllStmtModify());
  //REQUIRE(test_pkb->GetModifyStore()->GetAllProcModify() == pkb->GetModifyStore()->GetAllProcModify());
}

TEST_CASE("Test DE population for single procedure with multiple statements") {
  // set up AST for: procedure main { read a; print x; }
  std::shared_ptr<VariableNode> variable_node_a = std::make_shared<VariableNode>("a", "1", "main");
  std::shared_ptr<ReadStatementNode>
      read_stmt = std::make_shared<ReadStatementNode>(1, variable_node_a);

  std::shared_ptr<VariableNode> variable_node_x = std::make_shared<VariableNode>("x", "2", "main");
  std::shared_ptr<PrintStatementNode>
      print_stmt = std::make_shared<PrintStatementNode>(2, variable_node_x);

  std::vector<std::shared_ptr<StatementNode>> statements;
  statements.emplace_back(read_stmt);
  statements.emplace_back(print_stmt);
  std::shared_ptr<StatementListNode> stmt_list = std::make_shared<StatementListNode>(statements);

  std::shared_ptr<ProcedureNode> procedure = std::make_shared<ProcedureNode>("main", stmt_list);

  std::vector<std::shared_ptr<ProcedureNode>> procedures;
  procedures.emplace_back(procedure);
  std::shared_ptr<ProgramNode> expected_program_node = std::make_shared<ProgramNode>(procedures);

  // set up expected PKB
  PKB *pkb = new PKB();
  pkb->AddStmt("1", STMT);
  pkb->AddStmt("2", STMT);
  pkb->AddStmt("1", READ);
  pkb->AddStmt("2", PRINT);
  pkb->AddStmt("main", PROC);
  pkb->GetModifyStore()->AddStmtVar("1", "a");
  pkb->GetModifyStore()->AddProcVar("main", "a");
  pkb->GetUsageStore()->AddProcVar("main", "x");
  pkb->GetUsageStore()->AddStmtVar("2", "x");
  pkb->GetFollowStore()->AddFollow("1", "2");
  pkb->GetFollowStore()->AddFollowStar("1", "2");

  // set up actual traverse of DE
  PKB *test_pkb = new PKB();
  Populator *populator = new Populator(test_pkb);
  DesignExtractor *design_extractor = new DesignExtractor(*expected_program_node, *populator);
  design_extractor->TraverseAst();

  // test
  REQUIRE(test_pkb->GetStmt(STMT) == pkb->GetStmt(STMT));
  REQUIRE(test_pkb->GetStmt(READ) == pkb->GetStmt(READ));
  REQUIRE(test_pkb->GetStmt(PRINT) == pkb->GetStmt(PRINT));
  REQUIRE(test_pkb->GetStmt(PROC) == pkb->GetStmt(PROC));
  REQUIRE(test_pkb->GetModifyStore()->GetAllModStmt(STMT) == pkb->GetModifyStore()->GetAllModStmt(STMT));
  REQUIRE(test_pkb->GetModifyStore()->GetVarModByStmt("1") == pkb->GetModifyStore()->GetVarModByStmt("1"));
  REQUIRE(test_pkb->GetModifyStore()->GetStmtModByVar("a") == pkb->GetModifyStore()->GetStmtModByVar("a"));
  REQUIRE(test_pkb->GetModifyStore()->GetProcModByVar("a") == pkb->GetModifyStore()->GetProcModByVar("a"));
  REQUIRE(test_pkb->GetModifyStore()->GetVarModByProc("main") == pkb->GetModifyStore()->GetVarModByProc("main"));
  REQUIRE(test_pkb->GetUsageStore()->GetVarUsedByStmt("2") == pkb->GetUsageStore()->GetVarUsedByStmt("2"));
  REQUIRE(test_pkb->GetUsageStore()->GetStmtUsedByVar("x") == pkb->GetUsageStore()->GetStmtUsedByVar("x"));
  REQUIRE(test_pkb->GetUsageStore()->GetVarUsedByProc("main") == pkb->GetUsageStore()->GetVarUsedByProc("main"));
  REQUIRE(test_pkb->GetUsageStore()->GetProcUsedByVar("x") == pkb->GetUsageStore()->GetProcUsedByVar("x"));
  REQUIRE(test_pkb->GetUsageStore()->GetAllStmtUsing() == pkb->GetUsageStore()->GetAllStmtUsing());
  REQUIRE(test_pkb->GetUsageStore()->GetAllProcUsing() == pkb->GetUsageStore()->GetAllProcUsing());
  REQUIRE(test_pkb->GetUsageStore()->GetAllUsesStmt(STMT) == pkb->GetUsageStore()->GetAllUsesStmt(STMT));
  REQUIRE(test_pkb->GetFollowStore()->GetAllFollowStmt(STMT, STMT) == pkb->GetFollowStore()->GetAllFollowStmt(STMT, STMT));
  REQUIRE(test_pkb->GetFollowStore()->GetAllFollowStarStmt(STMT, STMT) == pkb->GetFollowStore()->GetAllFollowStarStmt(STMT, STMT));
}

TEST_CASE("Test DE population for single procedure with pattern statements") {
  // set up AST for: procedure main { x = x + 1 }
  std::shared_ptr<VariableNode> variable_node_x = std::make_shared<VariableNode>("x", "1", "main");
  std::shared_ptr<ConstantNode> constant_node = std::make_shared<ConstantNode>("1");
  std::shared_ptr<CombinationExpressionNode> combination_node =
      std::make_shared<CombinationExpressionNode>(ArithmeticOperator::PLUS, variable_node_x, constant_node);
  std::shared_ptr<VariableNode> variable_node_x_2 = std::make_shared<VariableNode>("x", "1", "main");

  std::shared_ptr<AssignStatementNode>
      assign_stmt = std::make_shared<AssignStatementNode>(1, variable_node_x_2, combination_node);

  std::vector<std::shared_ptr<StatementNode>> statements;
  statements.emplace_back(assign_stmt);
  std::shared_ptr<StatementListNode> stmt_list = std::make_shared<StatementListNode>(statements);

  std::shared_ptr<ProcedureNode> procedure = std::make_shared<ProcedureNode>("main", stmt_list);

  std::vector<std::shared_ptr<ProcedureNode>> procedures;
  procedures.emplace_back(procedure);
  std::shared_ptr<ProgramNode> expected_program_node = std::make_shared<ProgramNode>(procedures);

  // set up expected PKB
  PKB *pkb = new PKB();
  pkb->AddStmt("1", STMT);
  pkb->AddStmt("1", ASSIGN);
  pkb->AddStmt("main", PROC);
  pkb->GetModifyStore()->AddStmtVar("1", "x");
  pkb->GetModifyStore()->AddProcVar("main", "x");
  pkb->GetUsageStore()->AddProcVar("main", "x");
  pkb->GetUsageStore()->AddStmtVar("1", "x");
  pkb->GetPatternStore()->AddStmtWithPattern("1", "x", "x + 1");

  // set up actual traverse of DE
  PKB *test_pkb = new PKB();
  Populator *populator = new Populator(test_pkb);
  DesignExtractor *design_extractor = new DesignExtractor(*expected_program_node, *populator);
  design_extractor->TraverseAst();

  // test
  REQUIRE(test_pkb->GetStmt(STMT) == pkb->GetStmt(STMT));
  REQUIRE(test_pkb->GetStmt(ASSIGN) == pkb->GetStmt(ASSIGN));
  REQUIRE(test_pkb->GetStmt(PROC) == pkb->GetStmt(PROC));
  REQUIRE(test_pkb->GetModifyStore()->GetAllModStmt(STMT) == pkb->GetModifyStore()->GetAllModStmt(STMT));
  REQUIRE(test_pkb->GetModifyStore()->GetVarModByStmt("1") == pkb->GetModifyStore()->GetVarModByStmt("1"));
  REQUIRE(test_pkb->GetModifyStore()->GetStmtModByVar("x") == pkb->GetModifyStore()->GetStmtModByVar("x"));
  REQUIRE(test_pkb->GetModifyStore()->GetProcModByVar("x") == pkb->GetModifyStore()->GetProcModByVar("x"));
  REQUIRE(test_pkb->GetModifyStore()->GetVarModByProc("main") == pkb->GetModifyStore()->GetVarModByProc("main"));
  REQUIRE(test_pkb->GetUsageStore()->GetVarUsedByStmt("1") == pkb->GetUsageStore()->GetVarUsedByStmt("1"));
  REQUIRE(test_pkb->GetUsageStore()->GetStmtUsedByVar("x") == pkb->GetUsageStore()->GetStmtUsedByVar("x"));
  REQUIRE(test_pkb->GetUsageStore()->GetVarUsedByProc("main") == pkb->GetUsageStore()->GetVarUsedByProc("main"));
  REQUIRE(test_pkb->GetUsageStore()->GetProcUsedByVar("x") == pkb->GetUsageStore()->GetProcUsedByVar("x"));
  REQUIRE(test_pkb->GetPatternStore()->GetStmtWithPattern("x", "x + 1") == pkb->GetPatternStore()->GetStmtWithPattern("x", "x + 1"));
}

TEST_CASE("Test DE population for single procedure with one if statement (simple equal condition)") {
  // set up AST for: procedure main { if (a == 1) then { a = 2; } else { a = 3; } }

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

  // set up expected PKB
  PKB *pkb = new PKB();
  pkb->AddStmt("1", STMT);
  pkb->AddStmt("1", IF);
  pkb->AddStmt("2", STMT);
  pkb->AddStmt("2", ASSIGN);
  pkb->AddStmt("3", STMT);
  pkb->AddStmt("3", ASSIGN);
  pkb->AddStmt("main", PROC);
  pkb->GetParentStore()->AddParent("1", "2");
  pkb->GetParentStore()->AddParent("1", "3");
  pkb->GetUsageStore()->AddStmtVar("1", "a");
  pkb->GetModifyStore()->AddStmtVar("1", "a");
  pkb->GetPatternStore()->AddStmtWithPattern("2", "a", "(2)");
  pkb->GetPatternStore()->AddStmtWithPattern("3", "a", "(3)");

  // set up actual traverse of DE
  PKB *test_pkb = new PKB();
  Populator *populator = new Populator(test_pkb);
  DesignExtractor *design_extractor = new DesignExtractor(*expected_program_node, *populator);
  design_extractor->TraverseAst();

  // test
  REQUIRE(test_pkb->GetStmt(STMT) == pkb->GetStmt(STMT));
  REQUIRE(test_pkb->GetStmt(IF) == pkb->GetStmt(IF));
  REQUIRE(test_pkb->GetStmt(ASSIGN) == pkb->GetStmt(ASSIGN));
  REQUIRE(test_pkb->GetStmt(PROC) == pkb->GetStmt(PROC));
  REQUIRE(test_pkb->GetParentStore()->GetParentOf("2") == pkb->GetParentStore()->GetParentOf("2"));
  REQUIRE(test_pkb->GetParentStore()->GetParentOf("3") == pkb->GetParentStore()->GetParentOf("3"));
  REQUIRE(test_pkb->GetPatternStore()->GetStmtWithPattern("a", "2") == pkb->GetPatternStore()->GetStmtWithPattern("a", "2"));
  REQUIRE(test_pkb->GetPatternStore()->GetStmtWithPattern("a", "3") == pkb->GetPatternStore()->GetStmtWithPattern("a", "3"));
  REQUIRE(test_pkb->GetPatternStore()->GetStmtWithPattern("a", "_") == pkb->GetPatternStore()->GetStmtWithPattern("a", "_"));
  REQUIRE(test_pkb->GetUsageStore()->GetVarUsedByStmt("1") == pkb->GetUsageStore()->GetVarUsedByStmt("1"));
  REQUIRE(test_pkb->GetModifyStore()->GetVarModByStmt("1") == pkb->GetModifyStore()->GetVarModByStmt("1"));
}

TEST_CASE("Test DE population for single procedure with one while statement") {
  // set up AST for: procedure main { while ((a == 1) && (a == 2)) { a = 3; } }

  // while's stmt_list
  std::shared_ptr<VariableNode> variable_node = std::make_shared<VariableNode>("a", "2", "main");
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

  std::shared_ptr<VariableNode> condition_variable_node2 = std::make_shared<VariableNode>("a", "1", "main");
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

  // set up expected PKB
  PKB *pkb = new PKB();
  pkb->AddStmt("1", STMT);
  pkb->AddStmt("1", WHILE);
  pkb->AddStmt("2", STMT);
  pkb->AddStmt("2", ASSIGN);
  pkb->AddStmt("main", PROC);
  pkb->GetParentStore()->AddParent("1", "2");
  pkb->GetUsageStore()->AddStmtVar("1", "a");
  pkb->GetModifyStore()->AddStmtVar("1", "a");

  // set up actual traverse of DE
  PKB *test_pkb = new PKB();
  Populator *populator = new Populator(test_pkb);
  DesignExtractor *design_extractor = new DesignExtractor(*expected_program_node, *populator);
  design_extractor->TraverseAst();

  // test
  REQUIRE(test_pkb->GetStmt(STMT) == pkb->GetStmt(STMT));
  REQUIRE(test_pkb->GetStmt(WHILE) == pkb->GetStmt(WHILE));
  REQUIRE(test_pkb->GetStmt(ASSIGN) == pkb->GetStmt(ASSIGN));
  REQUIRE(test_pkb->GetStmt(PROC) == pkb->GetStmt(PROC));
  REQUIRE(test_pkb->GetParentStore()->GetParentOf("2") == pkb->GetParentStore()->GetParentOf("2"));
  REQUIRE(test_pkb->GetUsageStore()->GetVarUsedByStmt("1") == pkb->GetUsageStore()->GetVarUsedByStmt("1"));
  REQUIRE(test_pkb->GetModifyStore()->GetVarModByStmt("1") == pkb->GetModifyStore()->GetVarModByStmt("1"));
}

TEST_CASE("Test DE parent population for single procedure with nested while and if statements") {
  // set up AST for:

  // procedure main {
  //     while ((a == 1) && (a == 2)) {       1
  //         if (x == 1) then {               2
  //             a = 3;                       3
  //         } else {
  //             a = 4;                       4
  //         }
  //     }
  // }

  // if's stmt_list
  std::shared_ptr<VariableNode> if_variable_node = std::make_shared<VariableNode>("a", "3", "main");
  std::shared_ptr<ConstantNode> if_constant_node = std::make_shared<ConstantNode>("3");
  std::shared_ptr<AssignStatementNode>
      if_assign_stmt = std::make_shared<AssignStatementNode>(3, if_variable_node, if_constant_node);

  std::vector<std::shared_ptr<StatementNode>> if_statements;
  if_statements.emplace_back(if_assign_stmt);
  std::shared_ptr<StatementListNode> if_stmt_list = std::make_shared<StatementListNode>(if_statements);

  // else's stmt_list
  std::shared_ptr<VariableNode> else_variable_node = std::make_shared<VariableNode>("a", "4", "main");
  std::shared_ptr<ConstantNode> else_constant_node = std::make_shared<ConstantNode>("4");
  std::shared_ptr<AssignStatementNode>
      else_assign_stmt = std::make_shared<AssignStatementNode>(4, else_variable_node, else_constant_node);

  std::vector<std::shared_ptr<StatementNode>> else_statements;
  else_statements.emplace_back(else_assign_stmt);
  std::shared_ptr<StatementListNode> else_stmt_list = std::make_shared<StatementListNode>(else_statements);

  // condition
  std::shared_ptr<VariableNode> condition_variable_node = std::make_shared<VariableNode>("x", "2", "main");
  std::shared_ptr<ConstantNode> condition_constant_node = std::make_shared<ConstantNode>("1");
  std::shared_ptr<RelationalExpressionNode> condition_node =
      std::make_shared<RelationalExpressionNode>(RelationOperator::EQUALS,
                                                 condition_variable_node,
                                                 condition_constant_node);

  std::shared_ptr<IfStatementNode>
      if_stmt = std::make_shared<IfStatementNode>(2, condition_node, if_stmt_list, else_stmt_list);

  // while's stmt_list
  std::shared_ptr<VariableNode> variable_node = std::make_shared<VariableNode>("a", "2", "main");
  std::shared_ptr<ConstantNode> constant_node = std::make_shared<ConstantNode>("1");
  std::shared_ptr<AssignStatementNode>
      assign_stmt = std::make_shared<AssignStatementNode>(2, variable_node, constant_node);

  std::vector<std::shared_ptr<StatementNode>> while_statements;
  while_statements.emplace_back(if_stmt);
  std::shared_ptr<StatementListNode> while_stmt_list = std::make_shared<StatementListNode>(while_statements);

  // condition
  std::shared_ptr<VariableNode> condition_variable_node1 = std::make_shared<VariableNode>("a", "1", "main");
  std::shared_ptr<ConstantNode> condition_constant_node1 = std::make_shared<ConstantNode>("1");
  std::shared_ptr<RelationalExpressionNode> condition_node1 =
      std::make_shared<RelationalExpressionNode>(RelationOperator::EQUALS,
                                                 condition_variable_node1,
                                                 condition_constant_node1);

  std::shared_ptr<VariableNode> condition_variable_node2 = std::make_shared<VariableNode>("a", "1", "main");
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

  // set up expected PKB
  PKB *pkb = new PKB();
  pkb->AddStmt("1", STMT);
  pkb->AddStmt("1", WHILE);
  pkb->AddStmt("2", STMT);
  pkb->AddStmt("2", IF);
  pkb->AddStmt("3", STMT);
  pkb->AddStmt("3", ASSIGN);
  pkb->AddStmt("4", STMT);
  pkb->AddStmt("4", ASSIGN);
  pkb->AddStmt("main", PROC);
  pkb->GetParentStore()->AddParent("2", "3");
  pkb->GetParentStore()->AddParent("2", "4");
  pkb->GetParentStore()->AddParent("1", "2");
  std::vector<std::string> parent_star_1 = {"1", "2"};
  pkb->GetParentStore()->AddParentStar("3", parent_star_1);
  pkb->GetParentStore()->AddParentStar("4", parent_star_1);
  std::vector<std::string> parent_star_2 = {"1"};
  pkb->GetParentStore()->AddParentStar("2", parent_star_2);

  // set up actual traverse of DE
  PKB *test_pkb = new PKB();
  Populator *populator = new Populator(test_pkb);
  DesignExtractor *design_extractor = new DesignExtractor(*expected_program_node, *populator);
  design_extractor->TraverseAst();

  // test
  REQUIRE(test_pkb->GetStmt(STMT) == pkb->GetStmt(STMT));
  REQUIRE(test_pkb->GetStmt(WHILE) == pkb->GetStmt(WHILE));
  REQUIRE(test_pkb->GetStmt(IF) == pkb->GetStmt(IF));
  REQUIRE(test_pkb->GetStmt(ASSIGN) == pkb->GetStmt(ASSIGN));
  REQUIRE(test_pkb->GetStmt(PROC) == pkb->GetStmt(PROC));
  REQUIRE(test_pkb->GetParentStore()->GetAllDescOf("1") == pkb->GetParentStore()->GetAllDescOf("1"));
  REQUIRE(test_pkb->GetParentStore()->GetAllDescOf("2") == pkb->GetParentStore()->GetAllDescOf("2"));
  REQUIRE(test_pkb->GetParentStore()->GetParentOf("2") == pkb->GetParentStore()->GetParentOf("2"));
  REQUIRE(test_pkb->GetParentStore()->GetParentOf("3") == pkb->GetParentStore()->GetParentOf("3"));
  REQUIRE(test_pkb->GetParentStore()->GetParentOf("4") == pkb->GetParentStore()->GetParentOf("4"));
  REQUIRE(test_pkb->GetParentStore()->GetChildOf("1") == pkb->GetParentStore()->GetChildOf("1"));
  REQUIRE(test_pkb->GetParentStore()->GetChildOf("2") == pkb->GetParentStore()->GetChildOf("2"));
  REQUIRE(test_pkb->GetParentStore()->GetAllAnceOf("3") == pkb->GetParentStore()->GetAllAnceOf("3"));
  REQUIRE(test_pkb->GetParentStore()->GetAllAnceOf("4") == pkb->GetParentStore()->GetAllAnceOf("4"));
  REQUIRE(test_pkb->GetParentStore()->GetAllParentStmt(IF) == pkb->GetParentStore()->GetAllParentStmt(IF));
  REQUIRE(test_pkb->GetParentStore()->GetAllParentStmt(WHILE) == pkb->GetParentStore()->GetAllParentStmt(WHILE));
  REQUIRE(test_pkb->GetParentStore()->GetAllParentStmt(IF, ASSIGN) == pkb->GetParentStore()->GetAllParentStmt(IF, ASSIGN));
  REQUIRE(test_pkb->GetParentStore()->GetAllParentStmt(WHILE, ASSIGN) == pkb->GetParentStore()->GetAllParentStmt(WHILE, ASSIGN));
  REQUIRE(test_pkb->GetParentStore()->GetAllParentStarStmt(IF) == pkb->GetParentStore()->GetAllParentStarStmt(IF));
  REQUIRE(test_pkb->GetParentStore()->GetAllParentStarStmt(WHILE) == pkb->GetParentStore()->GetAllParentStarStmt(WHILE));
  REQUIRE(test_pkb->GetParentStore()->GetAllParentStarStmt(IF, ASSIGN) == pkb->GetParentStore()->GetAllParentStarStmt(IF, ASSIGN));
  REQUIRE(test_pkb->GetParentStore()->GetAllParentStarStmt(WHILE, ASSIGN) == pkb->GetParentStore()->GetAllParentStarStmt(WHILE, ASSIGN));
  REQUIRE(test_pkb->GetParentStore()->GetParentChildPairs() == pkb->GetParentStore()->GetParentChildPairs());
  REQUIRE(test_pkb->GetParentStore()->GetAnceDescPairs() == pkb->GetParentStore()->GetAnceDescPairs());
}

TEST_CASE("Test DE follows population for single procedure with multiple assign statements") {
  // set up AST for:

  // procedure main {
  //     a = 1;       1
  //     b = 2;       2
  //     c = 3;       3
  //     d = 4;       4
  //     e = 5;       5
  // }

  std::shared_ptr<VariableNode> variable_node_a = std::make_shared<VariableNode>("a", "1", "main");
  std::shared_ptr<ConstantNode> constant_node_1 = std::make_shared<ConstantNode>("1");
  std::shared_ptr<AssignStatementNode>
      assign_stmt_1 = std::make_shared<AssignStatementNode>(1, variable_node_a, constant_node_1);

  std::shared_ptr<VariableNode> variable_node_b = std::make_shared<VariableNode>("b", "2", "main");
  std::shared_ptr<ConstantNode> constant_node_2 = std::make_shared<ConstantNode>("2");
  std::shared_ptr<AssignStatementNode>
      assign_stmt_2 = std::make_shared<AssignStatementNode>(2, variable_node_b, constant_node_2);

  std::shared_ptr<VariableNode> variable_node_c = std::make_shared<VariableNode>("c", "3", "main");
  std::shared_ptr<ConstantNode> constant_node_3 = std::make_shared<ConstantNode>("3");
  std::shared_ptr<AssignStatementNode>
      assign_stmt_3 = std::make_shared<AssignStatementNode>(3, variable_node_c, constant_node_3);

  std::shared_ptr<VariableNode> variable_node_d = std::make_shared<VariableNode>("d", "4", "main");
  std::shared_ptr<ConstantNode> constant_node_4 = std::make_shared<ConstantNode>("4");
  std::shared_ptr<AssignStatementNode>
      assign_stmt_4 = std::make_shared<AssignStatementNode>(4, variable_node_d, constant_node_4);

  std::shared_ptr<VariableNode> variable_node_e = std::make_shared<VariableNode>("e", "5", "main");
  std::shared_ptr<ConstantNode> constant_node_5 = std::make_shared<ConstantNode>("5");
  std::shared_ptr<AssignStatementNode>
      assign_stmt_5 = std::make_shared<AssignStatementNode>(5, variable_node_e, constant_node_5);

  std::vector<std::shared_ptr<StatementNode>> statements;
  statements.emplace_back(assign_stmt_1);
  statements.emplace_back(assign_stmt_2);
  statements.emplace_back(assign_stmt_3);
  statements.emplace_back(assign_stmt_4);
  statements.emplace_back(assign_stmt_5);
  std::shared_ptr<StatementListNode> stmt_list = std::make_shared<StatementListNode>(statements);

  std::shared_ptr<ProcedureNode> procedure = std::make_shared<ProcedureNode>("main", stmt_list);

  std::vector<std::shared_ptr<ProcedureNode>> procedures;
  procedures.emplace_back(procedure);
  std::shared_ptr<ProgramNode> expected_program_node = std::make_shared<ProgramNode>(procedures);

  // set up expected PKB
  PKB *pkb = new PKB();
  pkb->AddStmt("1", STMT);
  pkb->AddStmt("1", ASSIGN);
  pkb->AddStmt("2", STMT);
  pkb->AddStmt("2", ASSIGN);
  pkb->AddStmt("3", STMT);
  pkb->AddStmt("3", ASSIGN);
  pkb->AddStmt("4", STMT);
  pkb->AddStmt("4", ASSIGN);
  pkb->AddStmt("5", STMT);
  pkb->AddStmt("5", ASSIGN);
  pkb->AddStmt("main", PROC);
  pkb->GetFollowStore()->AddFollow("1", "2");
  pkb->GetFollowStore()->AddFollow("2", "3");
  pkb->GetFollowStore()->AddFollow("3", "4");
  pkb->GetFollowStore()->AddFollow("4", "5");
  pkb->GetFollowStore()->AddFollowStar("1", "2");
  pkb->GetFollowStore()->AddFollowStar("1", "3");
  pkb->GetFollowStore()->AddFollowStar("1", "4");
  pkb->GetFollowStore()->AddFollowStar("1", "5");
  pkb->GetFollowStore()->AddFollowStar("2", "3");
  pkb->GetFollowStore()->AddFollowStar("2", "4");
  pkb->GetFollowStore()->AddFollowStar("2", "5");
  pkb->GetFollowStore()->AddFollowStar("3", "4");
  pkb->GetFollowStore()->AddFollowStar("3", "5");
  pkb->GetFollowStore()->AddFollowStar("4", "5");

  // set up actual traverse of DE
  PKB *test_pkb = new PKB();
  Populator *populator = new Populator(test_pkb);
  DesignExtractor *design_extractor = new DesignExtractor(*expected_program_node, *populator);
  design_extractor->TraverseAst();

  // test
  REQUIRE(test_pkb->GetStmt(STMT) == pkb->GetStmt(STMT));
  REQUIRE(test_pkb->GetStmt(ASSIGN) == pkb->GetStmt(ASSIGN));
  REQUIRE(test_pkb->GetStmt(PROC) == pkb->GetStmt(PROC));

  REQUIRE(test_pkb->GetFollowStore()->GetFollowerOf("1") == pkb->GetFollowStore()->GetFollowerOf("1"));
  REQUIRE(test_pkb->GetFollowStore()->GetFollowerOf("2") == pkb->GetFollowStore()->GetFollowerOf("2"));
  REQUIRE(test_pkb->GetFollowStore()->GetFollowerOf("3") == pkb->GetFollowStore()->GetFollowerOf("3"));
  REQUIRE(test_pkb->GetFollowStore()->GetFollowerOf("4") == pkb->GetFollowStore()->GetFollowerOf("4"));
  REQUIRE(test_pkb->GetFollowStore()->GetFollowerOf("5") == pkb->GetFollowStore()->GetFollowerOf("5"));
  REQUIRE(test_pkb->GetFollowStore()->GetFollowingOf("1") == pkb->GetFollowStore()->GetFollowingOf("1"));
  REQUIRE(test_pkb->GetFollowStore()->GetFollowingOf("2") == pkb->GetFollowStore()->GetFollowingOf("2"));
  REQUIRE(test_pkb->GetFollowStore()->GetFollowingOf("3") == pkb->GetFollowStore()->GetFollowingOf("3"));
  REQUIRE(test_pkb->GetFollowStore()->GetFollowingOf("4") == pkb->GetFollowStore()->GetFollowingOf("4"));
  REQUIRE(test_pkb->GetFollowStore()->GetFollowingOf("5") == pkb->GetFollowStore()->GetFollowingOf("5"));

  REQUIRE(test_pkb->GetFollowStore()->GetFollowerStarOf("1") == pkb->GetFollowStore()->GetFollowerStarOf("1"));
  REQUIRE(test_pkb->GetFollowStore()->GetFollowerStarOf("2") == pkb->GetFollowStore()->GetFollowerStarOf("2"));
  REQUIRE(test_pkb->GetFollowStore()->GetFollowerStarOf("3") == pkb->GetFollowStore()->GetFollowerStarOf("3"));
  REQUIRE(test_pkb->GetFollowStore()->GetFollowerStarOf("4") == pkb->GetFollowStore()->GetFollowerStarOf("4"));
  REQUIRE(test_pkb->GetFollowStore()->GetFollowerStarOf("5") == pkb->GetFollowStore()->GetFollowerStarOf("5"));
  REQUIRE(test_pkb->GetFollowStore()->GetFollowingStarOf("1") == pkb->GetFollowStore()->GetFollowingStarOf("1"));
  REQUIRE(test_pkb->GetFollowStore()->GetFollowingStarOf("2") == pkb->GetFollowStore()->GetFollowingStarOf("2"));
  REQUIRE(test_pkb->GetFollowStore()->GetFollowingStarOf("3") == pkb->GetFollowStore()->GetFollowingStarOf("3"));
  REQUIRE(test_pkb->GetFollowStore()->GetFollowingStarOf("4") == pkb->GetFollowStore()->GetFollowingStarOf("4"));
  REQUIRE(test_pkb->GetFollowStore()->GetFollowingStarOf("5") == pkb->GetFollowStore()->GetFollowingStarOf("5"));

  REQUIRE(test_pkb->GetFollowStore()->GetFollowPairs() == pkb->GetFollowStore()->GetFollowPairs());
  REQUIRE(test_pkb->GetFollowStore()->GetFollowStarPairs() == pkb->GetFollowStore()->GetFollowStarPairs());

  REQUIRE(test_pkb->GetFollowStore()->GetAllFollowStmt(STMT) == pkb->GetFollowStore()->GetAllFollowStmt(STMT));
  REQUIRE(test_pkb->GetFollowStore()->GetAllFollowStmt(STMT, STMT) == pkb->GetFollowStore()->GetAllFollowStmt(STMT, STMT));
  REQUIRE(test_pkb->GetFollowStore()->GetAllFollowStarStmt(STMT) == pkb->GetFollowStore()->GetAllFollowStarStmt(STMT));
  REQUIRE(test_pkb->GetFollowStore()->GetAllFollowStarStmt(STMT, STMT) == pkb->GetFollowStore()->GetAllFollowStarStmt(STMT, STMT));
}


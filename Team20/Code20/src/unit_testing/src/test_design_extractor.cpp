#include "catch.hpp"
#include "components/pkb/pkb_declarations.h"
#include "components/pkb/pkb.h"
#include "components/source_subsystem/iterator/design_extractor.h"
#include "components/pkb/pkb_client/pkb_client.h"
#include "components/source_subsystem/exceptions/unexpected_token.h"
#include "components/source_subsystem/types/ast/node_program.h"
#include "components/source_subsystem/types/ast/node_procedure.h"
#include "components/source_subsystem/types/ast/node_statement_list.h"
#include "components/source_subsystem/types/ast/node_variable.h"
#include "components/source_subsystem/types/ast/node_read_statement.h"
#include "components/source_subsystem/types/ast/node_print_statement.h"
#include "components/source_subsystem/types/ast/node_combination_expression.h"
#include "components/source_subsystem/types/ast/node_constant.h"
#include "components/source_subsystem/types/ast/node_assign_statement.h"
#include "components/source_subsystem/types/ast/node_if_statement.h"
#include "components/source_subsystem/types/ast/node_relational_expression.h"
#include "components/source_subsystem/types/ast/node_boolean_expression.h"
#include "components/source_subsystem/types/ast/node_while_statement.h"
#include "components/source_subsystem/iterator/cfg_builder.h"
#include "components/pkb/stores/uses_store/uses_store.h"
#include "components/pkb/stores/modifies_store/modifies_store.h"
#include "components/pkb/stores/follows_store/follows_store.h"
#include "components/pkb/stores/parent_store/parent_store.h"
#include "components/pkb/stores/pattern_store/pattern_store.h"

using namespace source;
using namespace pkb;

TEST_CASE("Test DE Modify population for single procedure with one read statement") {
  // set up AST for: procedure main { read a; }
  std::string stmt_no = "1";
  std::shared_ptr<VariableNode> variable_node = std::make_shared<VariableNode>("a");
  std::shared_ptr<ReadStatementNode>
      assign_stmt = std::make_shared<ReadStatementNode>(stmt_no, variable_node);

  std::vector<std::shared_ptr<StatementNode>> statements;
  statements.emplace_back(assign_stmt);
  std::shared_ptr<StatementListNode> stmt_list = std::make_shared<StatementListNode>(statements);

  std::shared_ptr<ProcedureNode> procedure = std::make_shared<ProcedureNode>("main", stmt_list);

  std::vector<std::shared_ptr<ProcedureNode>> procedures;
  procedures.emplace_back(procedure);
  std::shared_ptr<ProgramNode> expected_program_node = std::make_shared<ProgramNode>(procedures);

  // set up expected PKB
  pkb::PkbPtr pkb = std::make_shared<PKB>();
  pkb->AddStmt("1", StmtType::STMT);
  pkb->AddStmt("1", StmtType::READ);
  pkb->AddStmt("main", StmtType::PROC);
  pkb->AddTypeOfStmt("1", StmtType::ASSIGN);
  pkb->GetModifiesStore()->AddStmtVar("1", "a");

  // set up actual traverse of DE
  pkb::PkbPtr test_pkb = std::make_shared<PKB>();
  std::shared_ptr<PkbClient> pkb_client = std::make_shared<PkbClient>(test_pkb);
  DesignExtractorPtr design_extractor = std::make_shared<DesignExtractor>(pkb_client);
  design_extractor->IterateAst(expected_program_node);

  // test
  REQUIRE(test_pkb->GetStmt(StmtType::STMT) == pkb->GetStmt(StmtType::STMT));
  REQUIRE(test_pkb->GetStmt(StmtType::READ) == pkb->GetStmt(StmtType::READ));
  REQUIRE(test_pkb->GetStmt(StmtType::PROC) == pkb->GetStmt(StmtType::PROC));
  REQUIRE(test_pkb->GetModifiesStore()->GetAllModStmt(StmtType::STMT) == pkb->GetModifiesStore()->GetAllModStmt(StmtType::STMT));
  REQUIRE(test_pkb->GetModifiesStore()->GetVarModByStmt("1") == pkb->GetModifiesStore()->GetVarModByStmt("1"));
  REQUIRE(test_pkb->GetModifiesStore()->GetStmtModByVar(StmtType::STMT, "a") == pkb->GetModifiesStore()->GetStmtModByVar(STMT, "a"));
  REQUIRE(test_pkb->GetModifiesStore()->GetAllStmtModify() == pkb->GetModifiesStore()->GetAllStmtModify());
}

TEST_CASE("Test DE population for single procedure with multiple statements") {
  // set up AST for: procedure main { read a; print x; }
  std::string stmt_no_a = "1";
  std::shared_ptr<VariableNode> variable_node_a = std::make_shared<VariableNode>("a");
  std::shared_ptr<ReadStatementNode>
      read_stmt = std::make_shared<ReadStatementNode>(stmt_no_a, variable_node_a);

  std::string stmt_no_b = "2";
  std::shared_ptr<VariableNode> variable_node_x = std::make_shared<VariableNode>("x");
  std::shared_ptr<PrintStatementNode>
      print_stmt = std::make_shared<PrintStatementNode>(stmt_no_b, variable_node_x);

  std::vector<std::shared_ptr<StatementNode>> statements;
  statements.emplace_back(read_stmt);
  statements.emplace_back(print_stmt);
  std::shared_ptr<StatementListNode> stmt_list = std::make_shared<StatementListNode>(statements);

  std::shared_ptr<ProcedureNode> procedure = std::make_shared<ProcedureNode>("main", stmt_list);

  std::vector<std::shared_ptr<ProcedureNode>> procedures;
  procedures.emplace_back(procedure);
  std::shared_ptr<ProgramNode> expected_program_node = std::make_shared<ProgramNode>(procedures);

  // set up expected PKB
  pkb::PkbPtr pkb = std::make_shared<PKB>();
  pkb->AddStmt("1", StmtType::STMT);
  pkb->AddStmt("2", StmtType::STMT);
  pkb->AddStmt("1", StmtType::READ);
  pkb->AddStmt("2", StmtType::PRINT);
  pkb->AddStmt("main", StmtType::PROC);
  pkb->AddTypeOfStmt("1", StmtType::READ);
  pkb->AddTypeOfStmt("2", StmtType::ASSIGN);
  pkb->GetModifiesStore()->AddStmtVar("1", "a");
  pkb->GetUsesStore()->AddStmtVar("2", "x");
  pkb->GetFollowsStore()->AddFollow("1", "2");
  pkb->GetFollowsStore()->AddFollowStar("1", "2");

  // set up actual traverse of DE
  pkb::PkbPtr test_pkb = std::make_shared<PKB>();
  std::shared_ptr<PkbClient> pkb_client = std::make_shared<PkbClient>(test_pkb);
  DesignExtractorPtr design_extractor = std::make_shared<DesignExtractor>(pkb_client);
  design_extractor->IterateAst(expected_program_node);

  // test
  REQUIRE(test_pkb->GetStmt(StmtType::STMT) == pkb->GetStmt(STMT));
  REQUIRE(test_pkb->GetStmt(StmtType::READ) == pkb->GetStmt(READ));
  REQUIRE(test_pkb->GetStmt(StmtType::PRINT) == pkb->GetStmt(PRINT));
  REQUIRE(test_pkb->GetStmt(StmtType::PROC) == pkb->GetStmt(PROC));
  REQUIRE(test_pkb->GetModifiesStore()->GetAllModStmt(STMT) == pkb->GetModifiesStore()->GetAllModStmt(STMT));
  REQUIRE(test_pkb->GetModifiesStore()->GetVarModByStmt("1") == pkb->GetModifiesStore()->GetVarModByStmt("1"));
  REQUIRE(test_pkb->GetModifiesStore()->GetStmtModByVar(StmtType::STMT,"a") == pkb->GetModifiesStore()->GetStmtModByVar(STMT, "a"));
  REQUIRE(test_pkb->GetModifiesStore()->GetStmtModByVar(StmtType::STMT, "a") == pkb->GetModifiesStore()->GetStmtModByVar(STMT,"a"));
  REQUIRE(test_pkb->GetUsesStore()->GetVarUsedByStmt("2") == pkb->GetUsesStore()->GetVarUsedByStmt("2"));
  REQUIRE(test_pkb->GetUsesStore()->GetStmtUsedByVar(StmtType::STMT, "x") == pkb->GetUsesStore()->GetStmtUsedByVar(STMT, "x"));
  REQUIRE(test_pkb->GetUsesStore()->GetStmtUsedByVar(StmtType::STMT, "x") == pkb->GetUsesStore()->GetStmtUsedByVar(STMT, "x"));
  REQUIRE(test_pkb->GetUsesStore()->GetAllStmtUsing() == pkb->GetUsesStore()->GetAllStmtUsing());
  REQUIRE(test_pkb->GetUsesStore()->GetAllUsesStmt(StmtType::STMT) == pkb->GetUsesStore()->GetAllUsesStmt(STMT));
  REQUIRE(test_pkb->GetFollowsStore()->GetAllFollowStmt(StmtType::STMT, StmtType::STMT) == pkb->GetFollowsStore()->GetAllFollowStmt(STMT, STMT));
  REQUIRE(test_pkb->GetFollowsStore()->GetAllFollowStarStmt(StmtType::STMT, StmtType::STMT) == pkb->GetFollowsStore()->GetAllFollowStarStmt(STMT, STMT));
}

TEST_CASE("Test DE population for single procedure with pattern statements") {
  // set up AST for: procedure main { x = x + 1 }
  std::shared_ptr<VariableNode> variable_node_x = std::make_shared<VariableNode>("x");
  std::shared_ptr<ConstantNode> constant_node = std::make_shared<ConstantNode>("1");
  std::shared_ptr<CombinationExpressionNode> combination_node =
      std::make_shared<CombinationExpressionNode>(ArithmeticOperator::PLUS, variable_node_x, constant_node);
  std::shared_ptr<VariableNode> variable_node_x_2 = std::make_shared<VariableNode>("x");

  std::string stmt_no = "1";
  std::shared_ptr<AssignStatementNode>
      assign_stmt = std::make_shared<AssignStatementNode>(stmt_no, variable_node_x_2, combination_node);

  std::vector<std::shared_ptr<StatementNode>> statements;
  statements.emplace_back(assign_stmt);
  std::shared_ptr<StatementListNode> stmt_list = std::make_shared<StatementListNode>(statements);

  std::shared_ptr<ProcedureNode> procedure = std::make_shared<ProcedureNode>("main", stmt_list);

  std::vector<std::shared_ptr<ProcedureNode>> procedures;
  procedures.emplace_back(procedure);
  std::shared_ptr<ProgramNode> expected_program_node = std::make_shared<ProgramNode>(procedures);

  // set up expected PKB
  pkb::PkbPtr pkb = std::make_shared<PKB>();
  pkb->AddStmt("1", STMT);
  pkb->AddStmt("1", ASSIGN);
  pkb->AddStmt("main", PROC);
  pkb->AddTypeOfStmt("1", ASSIGN);
  pkb->GetModifiesStore()->AddStmtVar("1", "x");
  pkb->GetUsesStore()->AddStmtVar("1", "x");
  pkb->GetPatternStore()->AddStmtWithPattern("1", "x", "((x)+(1))");

  // set up actual traverse of DE
  pkb::PkbPtr test_pkb = std::make_shared<PKB>();
  std::shared_ptr<PkbClient> pkb_client = std::make_shared<PkbClient>(test_pkb);
  DesignExtractorPtr design_extractor = std::make_shared<DesignExtractor>(pkb_client);
  design_extractor->IterateAst(expected_program_node);

  // test
  REQUIRE(test_pkb->GetStmt(STMT) == pkb->GetStmt(STMT));
  REQUIRE(test_pkb->GetStmt(ASSIGN) == pkb->GetStmt(ASSIGN));
  REQUIRE(test_pkb->GetStmt(PROC) == pkb->GetStmt(PROC));
  REQUIRE(test_pkb->GetModifiesStore()->GetAllModStmt(STMT) == pkb->GetModifiesStore()->GetAllModStmt(STMT));
  REQUIRE(test_pkb->GetModifiesStore()->GetVarModByStmt("1") == pkb->GetModifiesStore()->GetVarModByStmt("1"));
  REQUIRE(test_pkb->GetModifiesStore()->GetStmtModByVar(STMT, "x") == pkb->GetModifiesStore()->GetStmtModByVar(STMT, "x"));
  REQUIRE(test_pkb->GetModifiesStore()->GetStmtModByVar(STMT, "x") == pkb->GetModifiesStore()->GetStmtModByVar(STMT, "x"));
  REQUIRE(test_pkb->GetUsesStore()->GetVarUsedByStmt("1") == pkb->GetUsesStore()->GetVarUsedByStmt("1"));
  REQUIRE(test_pkb->GetUsesStore()->GetStmtUsedByVar(STMT, "x") == pkb->GetUsesStore()->GetStmtUsedByVar(STMT, "x"));
  REQUIRE(test_pkb->GetUsesStore()->GetStmtUsedByVar(STMT, "x") == pkb->GetUsesStore()->GetStmtUsedByVar(STMT, "x"));
  REQUIRE(test_pkb->GetPatternStore()->GetStmtWithPatternExact("x", "x + 1") == pkb->GetPatternStore()->GetStmtWithPatternExact("x", "x + 1"));
}

TEST_CASE("Test DE population for single procedure with one if statement (simple equal condition)") {
  // set up AST for: procedure main { if (a == 1) then { a = 2; } else { a = 3; } }

  // if's stmt_list
  std::string if_stmt_no = "2";
  std::shared_ptr<VariableNode> if_variable_node = std::make_shared<VariableNode>("a");
  std::shared_ptr<ConstantNode> if_constant_node = std::make_shared<ConstantNode>("2");
  std::shared_ptr<AssignStatementNode>
      if_assign_stmt = std::make_shared<AssignStatementNode>(if_stmt_no, if_variable_node, if_constant_node);

  std::vector<std::shared_ptr<StatementNode>> if_statements;
  if_statements.emplace_back(if_assign_stmt);
  std::shared_ptr<StatementListNode> if_stmt_list = std::make_shared<StatementListNode>(if_statements);

  // else's stmt_list
  std::string ese_stmt_no = "3";
  std::shared_ptr<VariableNode> else_variable_node = std::make_shared<VariableNode>("a");
  std::shared_ptr<ConstantNode> else_constant_node = std::make_shared<ConstantNode>("3");
  std::shared_ptr<AssignStatementNode>
      else_assign_stmt = std::make_shared<AssignStatementNode>(ese_stmt_no, else_variable_node, else_constant_node);

  std::vector<std::shared_ptr<StatementNode>> else_statements;
  else_statements.emplace_back(else_assign_stmt);
  std::shared_ptr<StatementListNode> else_stmt_list = std::make_shared<StatementListNode>(else_statements);

  // condition
  std::string stmt_no = "1";
  std::shared_ptr<VariableNode> condition_variable_node = std::make_shared<VariableNode>("a");
  std::shared_ptr<ConstantNode> condition_constant_node = std::make_shared<ConstantNode>("1");
  std::shared_ptr<RelationalExpressionNode> condition_node =
      std::make_shared<RelationalExpressionNode>(RelationOperator::EQUALS,
                                                 condition_variable_node,
                                                 condition_constant_node);

  std::shared_ptr<IfStatementNode>
      if_stmt = std::make_shared<IfStatementNode>(stmt_no, condition_node, if_stmt_list, else_stmt_list);

  // if_stmt to procedure's stmt_list
  std::vector<std::shared_ptr<StatementNode>> statements;
  statements.emplace_back(if_stmt);
  std::shared_ptr<StatementListNode> stmt_list = std::make_shared<StatementListNode>(statements);

  std::shared_ptr<ProcedureNode> procedure = std::make_shared<ProcedureNode>("main", stmt_list);

  std::vector<std::shared_ptr<ProcedureNode>> procedures;
  procedures.emplace_back(procedure);
  std::shared_ptr<ProgramNode> expected_program_node = std::make_shared<ProgramNode>(procedures);

  // set up expected PKB
  pkb::PkbPtr pkb = std::make_shared<PKB>();
  pkb->AddStmt("1", STMT);
  pkb->AddStmt("1", IF);
  pkb->AddStmt("2", STMT);
  pkb->AddStmt("2", ASSIGN);
  pkb->AddStmt("3", STMT);
  pkb->AddStmt("3", ASSIGN);
  pkb->AddStmt("main", PROC);
  pkb->AddTypeOfStmt("1", IF);
  pkb->AddTypeOfStmt("2", ASSIGN);
  pkb->AddTypeOfStmt("3", ASSIGN);
  pkb->GetParentStore()->AddParent("1", "2");
  pkb->GetParentStore()->AddParent("1", "3");
  pkb->GetUsesStore()->AddStmtVar("1", "a");
  pkb->GetModifiesStore()->AddStmtVar("1", "a");
  pkb->GetPatternStore()->AddStmtWithPattern("2", "a", "(2)");
  pkb->GetPatternStore()->AddStmtWithPattern("3", "a", "(3)");

  // set up actual traverse of DE
  pkb::PkbPtr test_pkb = std::make_shared<PKB>();
  std::shared_ptr<PkbClient> pkb_client = std::make_shared<PkbClient>(test_pkb);
  DesignExtractorPtr design_extractor = std::make_shared<DesignExtractor>(pkb_client);
  CfgBuilderPtr cfg_builder = std::make_shared<CfgBuilder>(pkb_client);
  cfg_builder->IterateAst(expected_program_node);
  design_extractor->IterateAst(expected_program_node);
  cfg_builder->IterateCfg();

  // test
  REQUIRE(test_pkb->GetStmt(STMT) == pkb->GetStmt(STMT));
  REQUIRE(test_pkb->GetStmt(IF) == pkb->GetStmt(IF));
  REQUIRE(test_pkb->GetStmt(ASSIGN) == pkb->GetStmt(ASSIGN));
  REQUIRE(test_pkb->GetStmt(PROC) == pkb->GetStmt(PROC));
  REQUIRE(test_pkb->GetParentStore()->GetParentOf(STMT, "2") == pkb->GetParentStore()->GetParentOf(STMT, "2"));
  REQUIRE(test_pkb->GetParentStore()->GetParentOf(STMT, "3") == pkb->GetParentStore()->GetParentOf(STMT, "3"));
  REQUIRE(test_pkb->GetPatternStore()->GetStmtWithPatternExact("a", "2") == pkb->GetPatternStore()->GetStmtWithPatternExact("a", "2"));
  REQUIRE(test_pkb->GetPatternStore()->GetStmtWithPatternExact("a", "3") == pkb->GetPatternStore()->GetStmtWithPatternExact("a", "3"));
  REQUIRE(test_pkb->GetPatternStore()->GetStmtWithPatternWildcard("a") == pkb->GetPatternStore()->GetStmtWithPatternWildcard("a"));
  REQUIRE(test_pkb->GetUsesStore()->GetVarUsedByStmt("1") == pkb->GetUsesStore()->GetVarUsedByStmt("1"));
  REQUIRE(test_pkb->GetModifiesStore()->GetVarModByStmt("1") == pkb->GetModifiesStore()->GetVarModByStmt("1"));
}

TEST_CASE("Test DE population for single procedure with one while statement") {
  // set up AST for: procedure main { while ((a == 1) && (a == 2)) { a = 3; } }

  // while's stmt_list
  std::string condition_stmt_no = "2";
  std::shared_ptr<VariableNode> variable_node = std::make_shared<VariableNode>("a");
  std::shared_ptr<ConstantNode> constant_node = std::make_shared<ConstantNode>("3");
  std::shared_ptr<AssignStatementNode>
      assign_stmt = std::make_shared<AssignStatementNode>(condition_stmt_no, variable_node, constant_node);

  std::vector<std::shared_ptr<StatementNode>> while_statements;
  while_statements.emplace_back(assign_stmt);
  std::shared_ptr<StatementListNode> while_stmt_list = std::make_shared<StatementListNode>(while_statements);

  // condition
  std::shared_ptr<VariableNode> condition_variable_node1 = std::make_shared<VariableNode>("a");
  std::shared_ptr<ConstantNode> condition_constant_node1 = std::make_shared<ConstantNode>("1");
  std::shared_ptr<RelationalExpressionNode> condition_node1 =
      std::make_shared<RelationalExpressionNode>(RelationOperator::EQUALS,
                                                 condition_variable_node1,
                                                 condition_constant_node1);

  std::shared_ptr<VariableNode> condition_variable_node2 = std::make_shared<VariableNode>("a");
  std::shared_ptr<ConstantNode> condition_constant_node2 = std::make_shared<ConstantNode>("2");
  std::shared_ptr<RelationalExpressionNode> condition_node2 =
      std::make_shared<RelationalExpressionNode>(RelationOperator::EQUALS,
                                                 condition_variable_node2,
                                                 condition_constant_node2);

  std::shared_ptr<BooleanExpressionNode> condition_variable_node3 =
      std::make_shared<BooleanExpressionNode>(BooleanOperator::AND, condition_node1, condition_node2);

  std::string stmt_no = "1";
  std::shared_ptr<WhileStatementNode>
      while_stmt = std::make_shared<WhileStatementNode>(stmt_no, condition_variable_node3, while_stmt_list);

  // while_stmt to procedure's stmt_list
  std::vector<std::shared_ptr<StatementNode>> statements;
  statements.emplace_back(while_stmt);
  std::shared_ptr<StatementListNode> stmt_list = std::make_shared<StatementListNode>(statements);

  std::shared_ptr<ProcedureNode> procedure = std::make_shared<ProcedureNode>("main", stmt_list);

  std::vector<std::shared_ptr<ProcedureNode>> procedures;
  procedures.emplace_back(procedure);
  std::shared_ptr<ProgramNode> expected_program_node = std::make_shared<ProgramNode>(procedures);

  // set up expected PKB
  pkb::PkbPtr pkb = std::make_shared<PKB>();
  pkb->AddStmt("1", STMT);
  pkb->AddStmt("1", WHILE);
  pkb->AddStmt("2", STMT);
  pkb->AddStmt("2", ASSIGN);
  pkb->AddStmt("main", PROC);
  pkb->AddTypeOfStmt("1", WHILE);
  pkb->AddTypeOfStmt("2", ASSIGN);
  pkb->GetParentStore()->AddParent("1", "2");
  pkb->GetUsesStore()->AddStmtVar("1", "a");
  pkb->GetModifiesStore()->AddStmtVar("1", "a");

  // set up actual traverse of DE
  pkb::PkbPtr test_pkb = std::make_shared<PKB>();
  std::shared_ptr<PkbClient> pkb_client = std::make_shared<PkbClient>(test_pkb);
  DesignExtractorPtr design_extractor = std::make_shared<DesignExtractor>(pkb_client);
  design_extractor->IterateAst(expected_program_node);

  // test
  REQUIRE(test_pkb->GetStmt(STMT) == pkb->GetStmt(STMT));
  REQUIRE(test_pkb->GetStmt(WHILE) == pkb->GetStmt(WHILE));
  REQUIRE(test_pkb->GetStmt(ASSIGN) == pkb->GetStmt(ASSIGN));
  REQUIRE(test_pkb->GetStmt(PROC) == pkb->GetStmt(PROC));
  REQUIRE(test_pkb->GetParentStore()->GetParentOf(STMT, "2") == pkb->GetParentStore()->GetParentOf(STMT, "2"));
  REQUIRE(test_pkb->GetUsesStore()->GetVarUsedByStmt("1") == pkb->GetUsesStore()->GetVarUsedByStmt("1"));
  REQUIRE(test_pkb->GetModifiesStore()->GetVarModByStmt("1") == pkb->GetModifiesStore()->GetVarModByStmt("1"));
}

TEST_CASE("Test DE parent population for single procedure with nested while and if statements") {
  // set up AST for:

//   procedure main {
//       while ((a == 1) && (a == 2)) {       1
//           if (x == 1) then {               2
//               a = 3;                       3
//           } else {
//               a = 4;                       4
//           }
//       }
//   }

  // if's stmt_list
  std::string if_stmt_no = "3";
  std::shared_ptr<VariableNode> if_variable_node = std::make_shared<VariableNode>("a");
  std::shared_ptr<ConstantNode> if_constant_node = std::make_shared<ConstantNode>("3");
  std::shared_ptr<AssignStatementNode>
      if_assign_stmt = std::make_shared<AssignStatementNode>(if_stmt_no, if_variable_node, if_constant_node);

  std::vector<std::shared_ptr<StatementNode>> if_statements;
  if_statements.emplace_back(if_assign_stmt);
  std::shared_ptr<StatementListNode> if_stmt_list = std::make_shared<StatementListNode>(if_statements);

  // else's stmt_list
  std::string else_stmt_no = "4";
  std::shared_ptr<VariableNode> else_variable_node = std::make_shared<VariableNode>("a");
  std::shared_ptr<ConstantNode> else_constant_node = std::make_shared<ConstantNode>("4");
  std::shared_ptr<AssignStatementNode>
      else_assign_stmt = std::make_shared<AssignStatementNode>(else_stmt_no, else_variable_node, else_constant_node);

  std::vector<std::shared_ptr<StatementNode>> else_statements;
  else_statements.emplace_back(else_assign_stmt);
  std::shared_ptr<StatementListNode> else_stmt_list = std::make_shared<StatementListNode>(else_statements);

  // condition
  std::string condition_stmt_no = "2";
  std::shared_ptr<VariableNode> condition_variable_node = std::make_shared<VariableNode>("x");
  std::shared_ptr<ConstantNode> condition_constant_node = std::make_shared<ConstantNode>("1");
  std::shared_ptr<RelationalExpressionNode> condition_node =
      std::make_shared<RelationalExpressionNode>(RelationOperator::EQUALS,
                                                 condition_variable_node,
                                                 condition_constant_node);

  std::shared_ptr<IfStatementNode>
      if_stmt = std::make_shared<IfStatementNode>(condition_stmt_no, condition_node, if_stmt_list, else_stmt_list);

  // while's stmt_list
  std::string stmt_no = "2";
  std::shared_ptr<VariableNode> variable_node = std::make_shared<VariableNode>("a");
  std::shared_ptr<ConstantNode> constant_node = std::make_shared<ConstantNode>("1");
  std::shared_ptr<AssignStatementNode>
      assign_stmt = std::make_shared<AssignStatementNode>(stmt_no, variable_node, constant_node);

  std::vector<std::shared_ptr<StatementNode>> while_statements;
  while_statements.emplace_back(if_stmt);
  std::shared_ptr<StatementListNode> while_stmt_list = std::make_shared<StatementListNode>(while_statements);

  // condition
  std::shared_ptr<VariableNode> condition_variable_node1 = std::make_shared<VariableNode>("a");
  std::shared_ptr<ConstantNode> condition_constant_node1 = std::make_shared<ConstantNode>("1");
  std::shared_ptr<RelationalExpressionNode> condition_node1 =
      std::make_shared<RelationalExpressionNode>(RelationOperator::EQUALS,
                                                 condition_variable_node1,
                                                 condition_constant_node1);

  std::shared_ptr<VariableNode> condition_variable_node2 = std::make_shared<VariableNode>("a");
  std::shared_ptr<ConstantNode> condition_constant_node2 = std::make_shared<ConstantNode>("2");
  std::shared_ptr<RelationalExpressionNode> condition_node2 =
      std::make_shared<RelationalExpressionNode>(RelationOperator::EQUALS,
                                                 condition_variable_node2,
                                                 condition_constant_node2);

  std::shared_ptr<BooleanExpressionNode> condition_variable_node3 =
      std::make_shared<BooleanExpressionNode>(BooleanOperator::AND, condition_node1, condition_node2);

  std::string stmt_no3 = "1";
  std::shared_ptr<WhileStatementNode>
      while_stmt = std::make_shared<WhileStatementNode>(stmt_no3, condition_variable_node3, while_stmt_list);

  // while_stmt to procedure's stmt_list
  std::vector<std::shared_ptr<StatementNode>> statements;
  statements.emplace_back(while_stmt);
  std::shared_ptr<StatementListNode> stmt_list = std::make_shared<StatementListNode>(statements);

  std::shared_ptr<ProcedureNode> procedure = std::make_shared<ProcedureNode>("main", stmt_list);

  std::vector<std::shared_ptr<ProcedureNode>> procedures;
  procedures.emplace_back(procedure);
  std::shared_ptr<ProgramNode> expected_program_node = std::make_shared<ProgramNode>(procedures);

  // set up expected PKB
  pkb::PkbPtr pkb = std::make_shared<PKB>();
  pkb->AddStmt("1", STMT);
  pkb->AddStmt("1", WHILE);
  pkb->AddStmt("2", STMT);
  pkb->AddStmt("2", IF);
  pkb->AddStmt("3", STMT);
  pkb->AddStmt("3", ASSIGN);
  pkb->AddStmt("4", STMT);
  pkb->AddStmt("4", ASSIGN);
  pkb->AddStmt("main", PROC);
  pkb->AddTypeOfStmt("1", WHILE);
  pkb->AddTypeOfStmt("2", IF);
  pkb->AddTypeOfStmt("3", ASSIGN);
  pkb->AddTypeOfStmt("4", ASSIGN);
  pkb->GetParentStore()->AddParent("2", "3");
  pkb->GetParentStore()->AddParent("2", "4");
  pkb->GetParentStore()->AddParent("1", "2");
  std::vector<std::string> parent_star_1 = {"1", "2"};
  pkb->GetParentStore()->AddParentStar("3", parent_star_1);
  pkb->GetParentStore()->AddParentStar("4", parent_star_1);
  std::vector<std::string> parent_star_2 = {"1"};
  pkb->GetParentStore()->AddParentStar("2", parent_star_2);

  // set up actual traverse of DE
  pkb::PkbPtr test_pkb = std::make_shared<PKB>();
  std::shared_ptr<PkbClient> pkb_client = std::make_shared<PkbClient>(test_pkb);
  DesignExtractorPtr design_extractor = std::make_shared<DesignExtractor>(pkb_client);
  CfgBuilderPtr cfg_builder = std::make_shared<CfgBuilder>(pkb_client);
  cfg_builder->IterateAst(expected_program_node);
  design_extractor->IterateAst(expected_program_node);
  cfg_builder->IterateCfg();

  // test
  REQUIRE(test_pkb->GetStmt(STMT) == pkb->GetStmt(STMT));
  REQUIRE(test_pkb->GetStmt(WHILE) == pkb->GetStmt(WHILE));
  REQUIRE(test_pkb->GetStmt(IF) == pkb->GetStmt(IF));
  REQUIRE(test_pkb->GetStmt(ASSIGN) == pkb->GetStmt(ASSIGN));
  REQUIRE(test_pkb->GetStmt(PROC) == pkb->GetStmt(PROC));
  REQUIRE(test_pkb->GetParentStore()->GetAllDescOf(STMT, "1") == pkb->GetParentStore()->GetAllDescOf(STMT, "1"));
  REQUIRE(test_pkb->GetParentStore()->GetAllDescOf(STMT, "2") == pkb->GetParentStore()->GetAllDescOf(STMT, "2"));
  REQUIRE(test_pkb->GetParentStore()->GetParentOf(STMT, "2") == pkb->GetParentStore()->GetParentOf(STMT, "2"));
  REQUIRE(test_pkb->GetParentStore()->GetParentOf(STMT, "3") == pkb->GetParentStore()->GetParentOf(STMT, "3"));
  REQUIRE(test_pkb->GetParentStore()->GetParentOf(STMT, "4") == pkb->GetParentStore()->GetParentOf(STMT, "4"));
  REQUIRE(test_pkb->GetParentStore()->GetChildOf(STMT, "1") == pkb->GetParentStore()->GetChildOf(STMT, "1"));
  REQUIRE(test_pkb->GetParentStore()->GetChildOf(STMT, "2") == pkb->GetParentStore()->GetChildOf(STMT, "2"));
  REQUIRE(test_pkb->GetParentStore()->GetAllAnceOf(STMT, "3") == pkb->GetParentStore()->GetAllAnceOf(STMT, "3"));
  REQUIRE(test_pkb->GetParentStore()->GetAllAnceOf(STMT, "4") == pkb->GetParentStore()->GetAllAnceOf(STMT, "4"));
  REQUIRE(test_pkb->GetParentStore()->GetAllParentStmt(IF, STMT) == pkb->GetParentStore()->GetAllParentStmt(IF, STMT));
  REQUIRE(test_pkb->GetParentStore()->GetAllParentStmt(WHILE, STMT) == pkb->GetParentStore()->GetAllParentStmt(WHILE, STMT));
  REQUIRE(test_pkb->GetParentStore()->GetAllParentStmt(IF, ASSIGN) == pkb->GetParentStore()->GetAllParentStmt(IF, ASSIGN));
  REQUIRE(test_pkb->GetParentStore()->GetAllParentStmt(WHILE, ASSIGN) == pkb->GetParentStore()->GetAllParentStmt(WHILE, ASSIGN));
  REQUIRE(test_pkb->GetParentStore()->GetAllParentStarStmt(IF, STMT) == pkb->GetParentStore()->GetAllParentStarStmt(IF, STMT));
  REQUIRE(test_pkb->GetParentStore()->GetAllParentStarStmt(WHILE, STMT) == pkb->GetParentStore()->GetAllParentStarStmt(WHILE, STMT));
  REQUIRE(test_pkb->GetParentStore()->GetAllParentStarStmt(IF, ASSIGN) == pkb->GetParentStore()->GetAllParentStarStmt(IF, ASSIGN));
  REQUIRE(test_pkb->GetParentStore()->GetAllParentStarStmt(WHILE, ASSIGN) == pkb->GetParentStore()->GetAllParentStarStmt(WHILE, ASSIGN));
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

  std::string stmt_no_a = "1";
  std::shared_ptr<VariableNode> variable_node_a = std::make_shared<VariableNode>("a");
  std::shared_ptr<ConstantNode> constant_node_1 = std::make_shared<ConstantNode>("1");
  std::shared_ptr<AssignStatementNode>
      assign_stmt_1 = std::make_shared<AssignStatementNode>(stmt_no_a, variable_node_a, constant_node_1);

  std::string stmt_no_b = "2";
  std::shared_ptr<VariableNode> variable_node_b = std::make_shared<VariableNode>("b");
  std::shared_ptr<ConstantNode> constant_node_2 = std::make_shared<ConstantNode>("2");
  std::shared_ptr<AssignStatementNode>
      assign_stmt_2 = std::make_shared<AssignStatementNode>(stmt_no_b, variable_node_b, constant_node_2);

  std::string stmt_no_c = "3";
  std::shared_ptr<VariableNode> variable_node_c = std::make_shared<VariableNode>("c");
  std::shared_ptr<ConstantNode> constant_node_3 = std::make_shared<ConstantNode>("3");
  std::shared_ptr<AssignStatementNode>
      assign_stmt_3 = std::make_shared<AssignStatementNode>(stmt_no_c, variable_node_c, constant_node_3);

  std::string stmt_no_d = "4";
  std::shared_ptr<VariableNode> variable_node_d = std::make_shared<VariableNode>("d");
  std::shared_ptr<ConstantNode> constant_node_4 = std::make_shared<ConstantNode>("4");
  std::shared_ptr<AssignStatementNode>
      assign_stmt_4 = std::make_shared<AssignStatementNode>(stmt_no_d, variable_node_d, constant_node_4);

  std::string stmt_no_e = "5";
  std::shared_ptr<VariableNode> variable_node_e = std::make_shared<VariableNode>("e");
  std::shared_ptr<ConstantNode> constant_node_5 = std::make_shared<ConstantNode>("5");
  std::shared_ptr<AssignStatementNode>
      assign_stmt_5 = std::make_shared<AssignStatementNode>(stmt_no_e, variable_node_e, constant_node_5);

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
  pkb::PkbPtr pkb = std::make_shared<PKB>();
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
  pkb->AddTypeOfStmt("1", ASSIGN);
  pkb->AddTypeOfStmt("2", ASSIGN);
  pkb->AddTypeOfStmt("3", ASSIGN);
  pkb->AddTypeOfStmt("4", ASSIGN);
  pkb->AddTypeOfStmt("5", ASSIGN);
  pkb->GetFollowsStore()->AddFollow("1", "2");
  pkb->GetFollowsStore()->AddFollow("2", "3");
  pkb->GetFollowsStore()->AddFollow("3", "4");
  pkb->GetFollowsStore()->AddFollow("4", "5");
  pkb->GetFollowsStore()->AddFollowStar("1", "2");
  pkb->GetFollowsStore()->AddFollowStar("2", "3");
  pkb->GetFollowsStore()->AddFollowStar("1", "3");
  pkb->GetFollowsStore()->AddFollowStar("3", "4");
  pkb->GetFollowsStore()->AddFollowStar("2", "4");
  pkb->GetFollowsStore()->AddFollowStar("1", "4");
  pkb->GetFollowsStore()->AddFollowStar("4", "5");
  pkb->GetFollowsStore()->AddFollowStar("3", "5");
  pkb->GetFollowsStore()->AddFollowStar("2", "5");
  pkb->GetFollowsStore()->AddFollowStar("1", "5");

  // set up actual traverse of DE
  pkb::PkbPtr test_pkb = std::make_shared<PKB>();
  std::shared_ptr<PkbClient> pkb_client = std::make_shared<PkbClient>(test_pkb);
  DesignExtractorPtr design_extractor = std::make_shared<DesignExtractor>(pkb_client);
  CfgBuilderPtr cfg_builder = std::make_shared<CfgBuilder>(pkb_client);
  cfg_builder->IterateAst(expected_program_node);
  design_extractor->IterateAst(expected_program_node);
  cfg_builder->IterateCfg();

  // test
  REQUIRE(test_pkb->GetStmt(STMT) == pkb->GetStmt(STMT));
  REQUIRE(test_pkb->GetStmt(ASSIGN) == pkb->GetStmt(ASSIGN));
  REQUIRE(test_pkb->GetStmt(PROC) == pkb->GetStmt(PROC));

  REQUIRE(test_pkb->GetFollowsStore()->GetFollowerOf(STMT, "1") == pkb->GetFollowsStore()->GetFollowerOf(STMT, "1"));
  REQUIRE(test_pkb->GetFollowsStore()->GetFollowerOf(STMT, "2") == pkb->GetFollowsStore()->GetFollowerOf(STMT, "2"));
  REQUIRE(test_pkb->GetFollowsStore()->GetFollowerOf(STMT, "3") == pkb->GetFollowsStore()->GetFollowerOf(STMT, "3"));
  REQUIRE(test_pkb->GetFollowsStore()->GetFollowerOf(STMT, "4") == pkb->GetFollowsStore()->GetFollowerOf(STMT, "4"));
  REQUIRE(test_pkb->GetFollowsStore()->GetFollowerOf(STMT, "5") == pkb->GetFollowsStore()->GetFollowerOf(STMT, "5"));
  REQUIRE(test_pkb->GetFollowsStore()->GetFollowingOf(STMT, "1") == pkb->GetFollowsStore()->GetFollowingOf(STMT, "1"));
  REQUIRE(test_pkb->GetFollowsStore()->GetFollowingOf(STMT, "2") == pkb->GetFollowsStore()->GetFollowingOf(STMT, "2"));
  REQUIRE(test_pkb->GetFollowsStore()->GetFollowingOf(STMT, "3") == pkb->GetFollowsStore()->GetFollowingOf(STMT, "3"));
  REQUIRE(test_pkb->GetFollowsStore()->GetFollowingOf(STMT, "4") == pkb->GetFollowsStore()->GetFollowingOf(STMT, "4"));
  REQUIRE(test_pkb->GetFollowsStore()->GetFollowingOf(STMT, "5") == pkb->GetFollowsStore()->GetFollowingOf(STMT, "5"));

  REQUIRE(test_pkb->GetFollowsStore()->GetFollowerStarOf(STMT, "1") == pkb->GetFollowsStore()->GetFollowerStarOf(STMT, "1"));
  REQUIRE(test_pkb->GetFollowsStore()->GetFollowerStarOf(STMT, "2") == pkb->GetFollowsStore()->GetFollowerStarOf(STMT, "2"));
  REQUIRE(test_pkb->GetFollowsStore()->GetFollowerStarOf(STMT, "3") == pkb->GetFollowsStore()->GetFollowerStarOf(STMT, "3"));
  REQUIRE(test_pkb->GetFollowsStore()->GetFollowerStarOf(STMT, "4") == pkb->GetFollowsStore()->GetFollowerStarOf(STMT, "4"));
  REQUIRE(test_pkb->GetFollowsStore()->GetFollowerStarOf(STMT, "5") == pkb->GetFollowsStore()->GetFollowerStarOf(STMT, "5"));
  REQUIRE(test_pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "1") == pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "1"));
  REQUIRE(test_pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "2") == pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "2"));
  REQUIRE(test_pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "3") == pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "3"));
  REQUIRE(test_pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "4") == pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "4"));
  REQUIRE(test_pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "5") == pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "5"));

  REQUIRE(test_pkb->GetFollowsStore()->GetAllFollowStmt(STMT, ASSIGN) == pkb->GetFollowsStore()->GetAllFollowStmt(STMT, ASSIGN));
  REQUIRE(test_pkb->GetFollowsStore()->GetAllFollowStmt(STMT, STMT) == pkb->GetFollowsStore()->GetAllFollowStmt(STMT, STMT));
  REQUIRE(test_pkb->GetFollowsStore()->GetAllFollowStarStmt(STMT, ASSIGN) == pkb->GetFollowsStore()->GetAllFollowStarStmt(STMT, ASSIGN));
  REQUIRE(test_pkb->GetFollowsStore()->GetAllFollowStarStmt(STMT, STMT) == pkb->GetFollowsStore()->GetAllFollowStarStmt(STMT, STMT));
}

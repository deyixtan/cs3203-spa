#include "catch.hpp"
#include "components/source_subsystem/source_lexer.h"
#include "components/source_subsystem/design_extractor.h"
#include "components/source_subsystem/exceptions/unexpected_token.h"

using namespace source;

TEST_CASE("Test DE population for single procedure with one read statement") {
  // set up AST for: procedure main { read a; }
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

  // set up expected PKB
  PKB *pkb = new PKB();
  pkb->AddStmt("1", STMT);
  pkb->AddStmt("1", READ);
  pkb->AddStmt("main", PROC);
  pkb->GetModifyStore()->AddStmtVar("1", "a");
  pkb->GetModifyStore()->AddProcVar("main", "a");

  // set up actual traverse of DE
  PKB *test_pkb = new PKB();
  DesignExtractor *design_extractor = new DesignExtractor(*expected_program_node, test_pkb);
  design_extractor->TraverseAst();

  // test
  REQUIRE(test_pkb->GetStmt(STMT) == pkb->GetStmt(STMT));
  REQUIRE(test_pkb->GetStmt(READ) == pkb->GetStmt(READ));
  REQUIRE(test_pkb->GetStmt(PROC) == pkb->GetStmt(PROC));
  REQUIRE(test_pkb->GetModifyStore()->GetAllModStmt(STMT) == pkb->GetModifyStore()->GetAllModStmt(STMT));
  REQUIRE(test_pkb->GetModifyStore()->GetVarModByStmt("1") == pkb->GetModifyStore()->GetVarModByStmt("1"));
  REQUIRE(test_pkb->GetModifyStore()->GetStmtModByVar("a") == pkb->GetModifyStore()->GetStmtModByVar("a"));
  REQUIRE(test_pkb->GetModifyStore()->GetProcModByVar("a") == pkb->GetModifyStore()->GetProcModByVar("a"));
  REQUIRE(test_pkb->GetModifyStore()->GetVarModByProc("main") == pkb->GetModifyStore()->GetVarModByProc("main"));
}

TEST_CASE("Test DE population for single procedure with multiple statements") {
  // set up AST for: procedure main { read a; print x; }
  std::shared_ptr<VariableNode> variable_node_a = std::make_shared<VariableNode>("a");
  std::shared_ptr<ReadStatementNode>
      read_stmt = std::make_shared<ReadStatementNode>(1, variable_node_a);

  std::shared_ptr<VariableNode> variable_node_x = std::make_shared<VariableNode>("x");
  std::shared_ptr<PrintStatementNode>
      print_stmt = std::make_shared<PrintStatementNode>(1, variable_node_x);

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

  // set up actual traverse of DE
  PKB *test_pkb = new PKB();
  DesignExtractor *design_extractor = new DesignExtractor(*expected_program_node, test_pkb);
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
}


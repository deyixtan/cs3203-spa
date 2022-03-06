#include "read_node_handler.h"
#include "../../pkb/design_extractor/design_extractor.h"

ReadNodeHandler::ReadNodeHandler(const ReadStatementNode node) :
    read_node(node) {}

void ReadNodeHandler::process(PKB pkb) {
  pkb.AddStmt(stmt, STMT);
  design_extractor.PopulateStmt(read_node.GetStatementNumber());
  std::shared_ptr<ReadStatementNode> read_stmt = static_pointer_cast<ReadStatementNode>(stmt);
  var_name = read_stmt->GetIdentifier()->GetIdentifier();
  PopulateVars(var_name);
  PopulateRead(stmt_num);
  for (std::string s : visited) {
    PopulateModifies(s, var_name);
  }
  PopulateModifies(stmt_num, var_name);
}
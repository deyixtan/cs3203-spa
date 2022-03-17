#include "node_call_statement.h"
#include "../../iterator/design_extractor.h"
#include "../../iterator/cfg_builder.h"
#include "../cfg/cfg_node.h"

CallStatementNode::CallStatementNode(int stmt_no, std::string proc, std::string identifier)
    : StatementNode(stmt_no), proc_name(proc), m_identifier(identifier) {}

std::string CallStatementNode::GetIdentifier() {
  return m_identifier;
}

StmtType CallStatementNode::GetStatementType() {
  return StmtType::CALL;
}

std::string CallStatementNode::ToString() {
  return "call " + m_identifier + ";\n";
}

bool CallStatementNode::operator==(const StatementNode &other) const {
  const auto casted_other = dynamic_cast<const CallStatementNode*>(&other);
  return m_stmt_no == casted_other->m_stmt_no && m_identifier == casted_other->m_identifier;
}

void CallStatementNode::Accept(DesignExtractor *de) {
  std::string stmt_num = std::to_string(GetStatementNumber());
  de->GetPkbClient()->PopulateStmt(stmt_num);
  std::string callee_name = m_identifier;
  de->GetPkbClient()->PopulateCall(stmt_num);
  de->GetPkbClient()->PopulateCalls(proc_name, callee_name);
}

std::shared_ptr<CfgNode> CallStatementNode::Accept(CfgBuilder *cb, std::shared_ptr<CfgNode> cfg_node) {
  cfg_node->AddStatement(std::to_string(GetStatementNumber()));
  return cfg_node;
}

#include "node_call_statement.h"

CallStatementNode::CallStatementNode(int stmt_no, std::string identifier)
    : StatementNode(stmt_no), m_identifier(identifier) {}

std::string CallStatementNode::GetIdentifier() {
  return m_identifier;
}

StmtType CallStatementNode::GetStatementType() {
  return StmtType::CALL;
}

std::string CallStatementNode::ToString() {
  return "call " + m_identifier + ";\n";
}

std::string CallStatementNode::Process(Populator populator, std::vector<std::string> *visited, std::shared_ptr<source::CfgProcedureNode> cfg_proc_node, std::shared_ptr<source::CfgGroupNode> cfg_node) {
  if (cfg_node == nullptr) {
    cfg_proc_node->GetLastNode()->GetNodes().emplace_back(GetStatementNumber());
  } else {
    cfg_node->GetNodes().emplace_back(GetStatementNumber());
  }
  return "";
}

bool CallStatementNode::operator==(const StatementNode &other) const {
  const auto casted_other = dynamic_cast<const CallStatementNode*>(&other);
  return m_stmt_no == casted_other->m_stmt_no && m_identifier == casted_other->m_identifier;
}

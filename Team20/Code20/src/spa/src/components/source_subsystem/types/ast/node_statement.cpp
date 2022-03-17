#include "node_statement.h"
#include "../../iterator/design_extractor.h"

StatementNode::StatementNode(int stmt_no) : m_stmt_no(stmt_no) {}

int StatementNode::GetStatementNumber() {
  return m_stmt_no;
}

std::string StatementNode::GetStatementLabel() {
  if (m_stmt_no == 0) {
    return "   ";
  }

  std::string num = std::to_string(m_stmt_no);
  return std::string(3 - num.length(), ' ') + num;
}

std::string StatementNode::ToString() {
  return GetStatementLabel();
}

std::string StatementNode::GetPatternFormat() {
  return "";
}

//StmtType GetStatementType() {
//  return STMT;
//}
//
//std::string StatementNode::Process(Populator populator, std::vector<std::string> *visited, bool is_uses, std::shared_ptr<source::CfgProcedureNode> cfg_proc_node, std::shared_ptr<source::CfgGroupNode> cfg_node) {
//  return "";
//}
//
//bool StatementNode::operator==(const StatementNode &other) const {
//  return false;
//}
//
//void StatementNode::Accept(DesignExtractor *de) {
//
//}
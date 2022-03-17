#include "node_read_statement.h"
#include "../../iterator/design_extractor.h"
#include "../../iterator/cfg_builder.h"
#include "../cfg/cfg_node.h"

ReadStatementNode::ReadStatementNode(int stmt_no, std::shared_ptr<VariableNode> identifier)
    : StatementNode(stmt_no),
    m_identifier(identifier){}

std::shared_ptr<VariableNode> ReadStatementNode::GetIdentifier() {
  return m_identifier;
}

StmtType ReadStatementNode::GetStatementType() {
  return StmtType::READ;
}

std::string ReadStatementNode::ToString() {
  return StatementNode::ToString() + "read " + m_identifier->ToString() + ";\n";
}

std::string ReadStatementNode::GetPatternFormat() {
  return "";
}

bool ReadStatementNode::operator==(const StatementNode &other) const {
  const auto casted_other = dynamic_cast<const ReadStatementNode*>(&other);
  return m_stmt_no == casted_other->m_stmt_no && *m_identifier == *(casted_other->m_identifier);
}

void ReadStatementNode::Accept(DesignExtractor *de) {
  std::string stmt_num = std::to_string(GetStatementNumber());
  de->GetPkbClient()->PopulateStmt(stmt_num);
  std::string var_name = m_identifier->GetIdentifier();
  de->GetPkbClient()->PopulateRead(stmt_num);
  de->Visit(m_identifier, false);
  de->GetPkbClient()->PopulateParentStar(stmt_num, de->GetVisited());
}

std::shared_ptr<CfgNode> ReadStatementNode::Accept(CfgBuilder *cb, std::shared_ptr<CfgNode> cfg_node) {
  cfg_node->AddStatement(std::to_string(GetStatementNumber()));
  return cfg_node;
}

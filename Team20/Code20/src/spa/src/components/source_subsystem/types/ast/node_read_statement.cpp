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
  de->GetPopulator()->PopulateStmt(stmt_num);
  std::string var_name = m_identifier->GetIdentifier();
  de->GetPopulator()->PopulateRead(stmt_num);
//  m_identifier->Process(populator, visited, false, cfg_proc_node, cfg_node);
  de->Visit(m_identifier, false);
  de->GetPopulator()->PopulateParentStar(stmt_num, de->GetVisited());
//  if (cfg_node == nullptr) {
//    cfg_proc_node->GetLastNode()->GetNodes().emplace_back(GetStatementNumber());
//  } else {
//    cfg_node->GetNodes().emplace_back(GetStatementNumber());
//  }
}

std::shared_ptr<CfgNode> ReadStatementNode::Accept(CfgBuilder *cb, std::shared_ptr<CfgNode> cfg_node) {
  cfg_node->AddStatement(std::to_string(GetStatementNumber()));
  return cfg_node;
}

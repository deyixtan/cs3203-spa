#include "node_read_statement.h"
#include "../../iterator/design_extractor.h"
#include "../../iterator/cfg_builder.h"
#include "../cfg/cfg_node.h"

namespace source {

ReadStatementNode::ReadStatementNode(int stmt_no, std::shared_ptr<VariableNode> identifier)
    : StatementNode(stmt_no),
      m_identifier(identifier) {}

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
  const auto casted_other = dynamic_cast<const ReadStatementNode *>(&other);
  return m_stmt_no == casted_other->m_stmt_no && *m_identifier == *(casted_other->m_identifier);
}

void ReadStatementNode::Accept(DesignExtractor *de, std::string proc_name) {
  std::string stmt_num = std::to_string(GetStatementNumber());
  std::string var_name = m_identifier->GetIdentifier();
  de->GetPkbClient()->PopulateTypeOfStmt(stmt_num, READ);

  de->GetPkbClient()->PopulateRead(de->GetVisited(), stmt_num, var_name);
  de->Visit(m_identifier, proc_name, false);
}

std::shared_ptr<CfgNode> ReadStatementNode::Accept(CfgBuilder *cb, std::shared_ptr<CfgNode> cfg_node) {
  cfg_node->AddStatement(StmtType::READ, std::to_string(GetStatementNumber()));
  return cfg_node;
}

}

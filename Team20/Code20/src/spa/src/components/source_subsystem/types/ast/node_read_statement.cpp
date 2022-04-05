#include "node_read_statement.h"
#include "../../iterator/design_extractor.h"
#include "../../iterator/cfg_builder.h"

namespace source {

ReadStatementNode::ReadStatementNode(String stmt_no, VariableNodePtr identifier)
    : StatementNode(stmt_no), m_identifier(identifier) {}

VariableNodePtr ReadStatementNode::GetIdentifier() {
  return m_identifier;
}

StmtType ReadStatementNode::GetStatementType() {
  return StmtType::READ;
}

bool ReadStatementNode::operator==(const StatementNode &other) const {
  const auto casted_other = dynamic_cast<const ReadStatementNode *>(&other);
  return m_stmt_no == casted_other->m_stmt_no && *m_identifier == *(casted_other->m_identifier);
}

void ReadStatementNode::Accept(DesignExtractor *de, String proc_name) {
  String stmt_num = GetStatementNumber();
  String var_name = m_identifier->GetIdentifier();
  de->GetPkbClient()->PopulateTypeOfStmt(stmt_num, READ);

  de->GetPkbClient()->PopulateRead(de->GetVisited(), stmt_num, var_name);
  de->Visit(m_identifier, proc_name, false);
}

CfgNodePtr ReadStatementNode::Accept(CfgBuilder *cb, CfgNodePtr cfg_node) {
  cfg_node->AddStatement(StmtType::READ, GetStatementNumber());
  return cfg_node;
}

}

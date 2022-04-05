#include "node_read_statement.h"
#include "components/source_subsystem/pkb_client.h"
#include "components/source_subsystem/iterator/design_extractor.h"
#include "components/source_subsystem/types/ast/node_variable.h"
#include "components/source_subsystem/types/cfg/cfg_node.h"

namespace source {

ReadStatementNode::ReadStatementNode(String &stmt_no, VariableNodePtr variable)
    : StatementNode(stmt_no), m_variable(std::move(variable)) {}

VariableNodePtr ReadStatementNode::GetVariable() {
  return m_variable;
}

void ReadStatementNode::Accept(DesignExtractor *de, String proc_name) {
  String stmt_num = GetStatementNumber();
  String var_name = m_variable->GetName();
  de->GetPkbClient()->PopulateTypeOfStmt(stmt_num, READ);

  de->GetPkbClient()->PopulateRead(de->GetVisited(), stmt_num, var_name);
  de->Visit(m_variable, proc_name, false);
}

CfgNodePtr ReadStatementNode::Accept(CfgBuilder *cfg_builder, CfgNodePtr cfg_node) {
  cfg_node->AddStatement(StmtType::READ, GetStatementNumber());
  return cfg_node;
}

bool ReadStatementNode::operator==(const StatementNode &other) const {
  const auto casted_other = dynamic_cast<const ReadStatementNode *>(&other);
  return m_stmt_no == casted_other->m_stmt_no && *m_variable == *(casted_other->m_variable);
}

}

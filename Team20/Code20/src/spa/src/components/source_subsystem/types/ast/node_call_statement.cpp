#include "node_call_statement.h"
#include "../../pkb_client.h"
#include "../../iterator/design_extractor.h"
#include "../call_graph/call_graph.h"
#include "../../iterator/cfg_builder.h"

namespace source {

CallStatementNode::CallStatementNode(String stmt_no, String proc, String identifier)
    : StatementNode(stmt_no), proc_name(proc), m_identifier(identifier) {}

String CallStatementNode::GetIdentifier() {
  return m_identifier;
}

StmtType CallStatementNode::GetStatementType() {
  return StmtType::CALL;
}

void CallStatementNode::Accept(DesignExtractor *de, String proc_name) {
  String stmt_num = GetStatementNumber();
  String callee_name = m_identifier;
  de->GetPkbClient()->PopulateTypeOfStmt(stmt_num, CALL);
  de->GetPkbClient()->PopulateCall(de->GetVisited(), stmt_num, proc_name, callee_name);
  de->GetCallGraph()->AddEdge(proc_name, callee_name);
}

CfgNodePtr CallStatementNode::Accept(CfgBuilder *cb, CfgNodePtr cfg_node) {
  cfg_node->AddStatement(StmtType::CALL, GetStatementNumber());
  return cfg_node;
}

bool CallStatementNode::operator==(const StatementNode &other) const {
  const auto casted_other = dynamic_cast<const CallStatementNode *>(&other);
  return m_stmt_no == casted_other->m_stmt_no && m_identifier == casted_other->m_identifier;
}

}

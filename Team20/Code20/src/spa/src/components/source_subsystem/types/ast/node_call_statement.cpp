#include "node_call_statement.h"
#include "components/source_subsystem/pkb_client.h"
#include "components/source_subsystem/iterator/design_extractor.h"
#include "components/source_subsystem/types/call_graph/call_graph.h"
#include "components/source_subsystem/types/cfg/cfg_node.h"

namespace source {

CallStatementNode::CallStatementNode(String &stmt_no, String caller_name, String callee_name)
    : StatementNode(stmt_no), m_caller_name(std::move(caller_name)), m_callee_name(std::move(callee_name)) {}

String CallStatementNode::GetCallerName() {
  return m_caller_name;
}

String CallStatementNode::GetCalleeName() {
  return m_callee_name;
}

void CallStatementNode::Accept(DesignExtractorPtr design_extractor) {
  design_extractor->Visit(this);
}

//void CallStatementNode::Accept(DesignExtractor *design_extractor, String proc_name) {
//  String stmt_num = GetStatementNumber();
//  String callee_name = m_callee_name;
//  design_extractor->GetPkbClient()->PopulateTypeOfStmt(stmt_num, CALL);
//  design_extractor->GetPkbClient()->PopulateCall(design_extractor->GetVisited(), stmt_num, proc_name, callee_name);
//  design_extractor->GetCallGraph()->AddEdge(proc_name, callee_name);
//}

CfgNodePtr CallStatementNode::Accept(CfgBuilder *cfg_builder, CfgNodePtr cfg_node) {
  cfg_node->AddStatement(StmtType::CALL, GetStatementNumber());
  return cfg_node;
}

bool CallStatementNode::operator==(const StatementNode &other) const {
  const auto casted_other = dynamic_cast<const CallStatementNode *>(&other);
  return m_stmt_no == casted_other->m_stmt_no && m_caller_name == casted_other->m_caller_name && m_callee_name == casted_other->m_callee_name;
}

StmtType CallStatementNode::GetStatementType() {
  return StmtType::CALL;
}

}

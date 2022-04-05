#include "node_statement.h"
#include "../../iterator/design_extractor.h"

namespace source {

StatementNode::StatementNode(String stmt_no) : m_stmt_no(stmt_no) {}

String StatementNode::GetStatementNumber() {
  return m_stmt_no;
}

}

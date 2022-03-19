#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_CALL_STATEMENT_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_CALL_STATEMENT_H_

#include "node_statement.h"

class CallStatementNode : public StatementNode {
 private:
  std::string m_identifier;
  std::string proc_name;

 public:
  CallStatementNode(int stmt_no, std::string proc_name, std::string m_identifier);
  [[nodiscard]] std::string GetIdentifier();
  [[nodiscard]] StmtType GetStatementType() override;
  [[nodiscard]] std::string ToString() override;
  [[nodiscard]] bool operator==(const StatementNode &other) const override;
  void Accept(DesignExtractor *de, std::string proc_name);
  std::shared_ptr<CfgNode> Accept(CfgBuilder *cb, std::shared_ptr<CfgNode> cfg_node);
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_CALL_STATEMENT_H_

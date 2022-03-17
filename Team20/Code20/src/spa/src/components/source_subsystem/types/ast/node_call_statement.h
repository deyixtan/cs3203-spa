#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_CALL_STATEMENT_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_CALL_STATEMENT_H_

#include "node_statement.h"

class CallStatementNode : public StatementNode {
 private:
  std::string m_identifier;

 public:
  CallStatementNode(int stmt_no, std::string m_identifier);
  [[nodiscard]] std::string GetIdentifier();
  [[nodiscard]] StmtType GetStatementType() override;
  [[nodiscard]] std::string Process(Populator populator, std::vector<std::string> *visited, bool is_uses, std::shared_ptr<source::CfgProcedureNode> cfg_proc_node, std::shared_ptr<source::CfgGroupNode> cfg_node) override;
  [[nodiscard]] std::string ToString() override;
  [[nodiscard]] bool operator==(const StatementNode &other) const override;
  void Accept(DesignExtractor *de);
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_CALL_STATEMENT_H_

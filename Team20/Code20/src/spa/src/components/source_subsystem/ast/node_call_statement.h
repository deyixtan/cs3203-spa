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
  void Process(Populator populator, std::vector<std::string> *visited) override;
  void Process(Populator populator, std::vector<std::string> *visited, std::string stmt) override;
  [[nodiscard]] std::string Process(Populator populator, std::vector<std::string> *visited, std::string stmt_num, int direction, std::string pattern) override;
  [[nodiscard]] std::string ToString() override;
  [[nodiscard]] bool operator==(const StatementNode &other) const override;
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_CALL_STATEMENT_H_

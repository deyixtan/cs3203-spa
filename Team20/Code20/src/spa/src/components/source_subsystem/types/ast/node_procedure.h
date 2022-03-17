#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_PROCEDURE_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_PROCEDURE_H_

#include "node.h"

class DesignExtractor;
class StatementListNode;

class ProcedureNode : public Node {
 private:
  std::string m_identifier;
  std::shared_ptr<StatementListNode> m_stmt_list;

 public:
  ProcedureNode(std::string identifier, std::shared_ptr<StatementListNode> stmt_list);
  [[nodiscard]] std::string GetIdentifier();
  [[nodiscard]] std::shared_ptr<StatementListNode> GetStatementList();
  [[nodiscard]] std::string Process(Populator populator, std::vector<std::string> *visited, bool is_uses, std::shared_ptr<source::CfgProcedureNode> cfg_proc_node, std::shared_ptr<source::CfgGroupNode> cfg_node) override;
  [[nodiscard]] std::string ToString() override;
  [[nodiscard]] std::string GetPatternFormat() override;
  [[nodiscard]] bool operator==(const ProcedureNode &other) const;
  void Accept(DesignExtractor *de);
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_PROCEDURE_H_

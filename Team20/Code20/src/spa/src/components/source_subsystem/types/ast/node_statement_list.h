#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_STATEMENT_LIST_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_STATEMENT_LIST_H_

#include "node.h"
#include "node_statement.h"
#include "components/source_subsystem/types/cfg/cfg_group_node.h"
#include <iostream>

class StatementListNode : public Node {
 private:
  std::vector<std::shared_ptr<StatementNode>> m_statements;

 public:
  StatementListNode();
  StatementListNode(std::vector<std::shared_ptr<StatementNode>> statements);
  [[nodiscard]] std::vector<std::shared_ptr<StatementNode>> GetStatements();
  [[nodiscard]] std::string Process(Populator populator, std::vector<std::string> *visited, bool is_uses, std::shared_ptr<source::CfgProcedureNode> cfg_proc_node, std::shared_ptr<source::CfgGroupNode> cfg_node) override;
  [[nodiscard]] std::string ToString() override;
  [[nodiscard]] std::string GetPatternFormat() override;
  [[nodiscard]] bool operator==(const StatementListNode &other) const;
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_STATEMENT_LIST_H_

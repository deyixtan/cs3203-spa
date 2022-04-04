#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_STATEMENT_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_STATEMENT_H_

#include "node.h"
#include "../../../pkb/stores/store.h"

namespace source {

class StatementNode : public Node {
 protected:
  int m_stmt_no;

 protected:
  [[nodiscard]] std::string GetStatementLabel();

 public:
  StatementNode(int stmt_no);
  [[nodiscard]] int GetStatementNumber();
  [[nodiscard]] virtual StmtType GetStatementType() = 0;
  [[nodiscard]] std::string ToString() override;
  [[nodiscard]] std::string GetPatternFormat() override;
  virtual bool operator==(const StatementNode &other) const = 0;
  virtual void Accept(DesignExtractor *de, std::string proc_name) = 0;
  virtual std::shared_ptr<CfgNode> Accept(CfgBuilder *cb, std::shared_ptr<CfgNode> cfg_node) = 0;
};

}

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_STATEMENT_H_

#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_ASSIGN_STATEMENT_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_ASSIGN_STATEMENT_H_

#include "node_statement.h"
#include "node_variable.h"
#include "node_constant.h"
#include "node_combination_expression.h"

class DesignExtractor;

class AssignStatementNode : public StatementNode {
 private:
  std::shared_ptr<VariableNode> m_identifier;
  std::shared_ptr<ExpressionNode> m_expression;

 public:
  AssignStatementNode(int stmt_no,
                      std::shared_ptr<VariableNode> identifier,
                      std::shared_ptr<ExpressionNode> expression);
  [[nodiscard]] std::shared_ptr<VariableNode> GetIdentifier();
  [[nodiscard]] std::shared_ptr<ExpressionNode> GetExpression();
  [[nodiscard]] StmtType GetStatementType() override;
  [[nodiscard]] std::string Process(Populator populator, std::vector<std::string> *visited, bool is_uses, std::shared_ptr<source::CfgProcedureNode> cfg_proc_node, std::shared_ptr<source::CfgGroupNode> cfg_node) override;
  [[nodiscard]] std::string ToString() override;
  [[nodiscard]] std::string GetPatternFormat() override;
  [[nodiscard]] bool operator==(const StatementNode &other) const override;
  void Accept(DesignExtractor *de);
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_ASSIGN_STATEMENT_H_

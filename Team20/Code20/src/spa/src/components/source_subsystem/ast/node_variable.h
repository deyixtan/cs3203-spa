#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_VARIABLE_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_VARIABLE_H_

#include "node_expression.h"
#include "components/pkb/design_extractor/populator.h"

class VariableNode : public ExpressionNode {
 private:
  std::string m_identifier;

 public:
  VariableNode();
  VariableNode(std::string identifier);
  [[nodiscard]] std::string GetIdentifier();
  [[nodiscard]] ExpressionType GetExpressionType() override;
  [[nodiscard]] std::string ToString(int level) override;
  [[nodiscard]] void Process(Populator populator, std::vector<std::string>* visited, std::string stmt);
  [[nodiscard]] bool operator==(const ExpressionNode &other) const override;
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_VARIABLE_H_

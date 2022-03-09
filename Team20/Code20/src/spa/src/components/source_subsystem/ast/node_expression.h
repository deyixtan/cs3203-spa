#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_EXPRESSION_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_EXPRESSION_H_

#include "node.h"
#include "components/pkb/design_extractor/populator.h"

enum class ExpressionType {
  CONSTANT,
  COMBINATION,
  VARIABLE
};

class ExpressionNode : public Node {
 public:
  [[nodiscard]] virtual ExpressionType GetExpressionType() = 0;
  virtual void Process(Populator populator, std::vector<std::string>* visited) = 0;
  //virtual void Process(std::vector<std::string> *visited, std::string stmt, std::shared_ptr<ExpressionNode> expr) = 0;
  //virtual std::string Process(std::vector<std::string> *visited, std::string stmt_num, std::shared_ptr<ExpressionNode> expr, int direction, std::string pattern) = 0;
  [[nodiscard]] virtual bool operator==(const ExpressionNode &other) const = 0;
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_EXPRESSION_H_

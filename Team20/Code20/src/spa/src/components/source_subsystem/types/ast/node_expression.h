#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_EXPRESSION_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_EXPRESSION_H_

#include "node.h"
#include "components/source_subsystem/pkb_client.h"

class DesignExtractor;

enum class ExpressionType {
  CONSTANT,
  COMBINATION,
  VARIABLE
};

class ExpressionNode : public Node {
 public:
  [[nodiscard]] virtual ExpressionType GetExpressionType() = 0;
  [[nodiscard]] virtual bool operator==(const ExpressionNode &other) const = 0;
  virtual std::string Accept(DesignExtractor *de, std::string proc_name, bool is_uses) = 0;
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_EXPRESSION_H_

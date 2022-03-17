#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_ITERATOR_DESIGN_EXTRACTOR_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_ITERATOR_DESIGN_EXTRACTOR_H_

#include <string>
#include <vector>

class Populator;
class ProgramNode;
class ProcedureNode;
class StatementListNode;
class StatementNode;
class ExpressionNode;
class ConditionalExpressionNode;
class VariableNode;

class DesignExtractor {
 private:
  Populator *m_populator;
  std::vector<std::string> m_visited;

 public:
  DesignExtractor(Populator *populator);
  [[nodiscard]] Populator *GetPopulator();
  [[nodiscard]] std::vector<std::string> &GetVisited();
  void Visit(std::shared_ptr<ProgramNode> node);
  void Visit(std::shared_ptr<ProcedureNode> node);
  void Visit(std::shared_ptr<StatementListNode> node);
  void Visit(std::shared_ptr<StatementNode> node);
  std::string Visit(std::shared_ptr<ExpressionNode> node, bool is_uses);
  std::string Visit(std::shared_ptr<ConditionalExpressionNode> node, bool is_uses);
  void Visit(std::shared_ptr<VariableNode> node, bool is_uses);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_ITERATOR_DESIGN_EXTRACTOR_H_

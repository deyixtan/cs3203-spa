#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_ITERATOR_DESIGN_EXTRACTOR_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_ITERATOR_DESIGN_EXTRACTOR_H_

#include <string>
#include <vector>

class PkbClient;
class ProgramNode;
class ProcedureNode;
class StatementListNode;
class StatementNode;
class ExpressionNode;
class ConditionalExpressionNode;
class VariableNode;

class DesignExtractor {
 private:
  PkbClient *m_pkb_client;
  std::vector<std::string> m_visited;

 public:
  DesignExtractor(PkbClient *pkb_client);
  [[nodiscard]] PkbClient *GetPkbClient();
  [[nodiscard]] std::vector<std::string> &GetVisited();
  void Visit(std::shared_ptr<ProgramNode> node);
  void Visit(std::shared_ptr<ProcedureNode> node);
  void Visit(std::shared_ptr<StatementListNode> node);
  void Visit(std::shared_ptr<StatementNode> node);
  [[nodiscard]] std::string Visit(std::shared_ptr<ExpressionNode> node, bool is_uses);
  [[nodiscard]] std::string Visit(std::shared_ptr<ConditionalExpressionNode> node, bool is_uses);
  void Visit(std::shared_ptr<VariableNode> node, bool is_uses);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_ITERATOR_DESIGN_EXTRACTOR_H_

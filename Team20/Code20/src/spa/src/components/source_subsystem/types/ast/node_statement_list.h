#ifndef NODE_STATEMENT_LIST_H
#define NODE_STATEMENT_LIST_H

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/types/ast/node.h"

namespace source {

class StatementListNode : public Node, std::enable_shared_from_this<StatementListNode> {
 private:
  StatementNodeStream m_statements;

 public:
  StatementListNode();
  explicit StatementListNode(StatementNodeStream statements);
  [[nodiscard]] StatementNodeStream GetStatements();
  void Accept(DesignExtractorPtr design_extractor) override;
  CfgNodePtr Accept(CfgBuilder *cfg_builder, CfgNodePtr cfg_node);
  [[nodiscard]] bool operator==(const StatementListNode &other) const;
};

}

#endif //NODE_STATEMENT_LIST_H

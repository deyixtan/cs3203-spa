#ifndef NODE_STATEMENT_LIST_H
#define NODE_STATEMENT_LIST_H

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/types/ast/node.h"

namespace source {

class StatementListNode : public Node {
 private:
  StatementNodeStream m_statements;

 public:
  StatementListNode();
  explicit StatementListNode(StatementNodeStream statements);
  [[nodiscard]] StatementNodeStream GetStatements();
  void Accept(DesignExtractorPtr design_extractor) override;
  //void Accept(DesignExtractor *design_extractor, String &proc_name);
  CfgNodePtr Accept(CfgBuilder *cfg_builder, CfgNodePtr cfg_node);
  [[nodiscard]] bool operator==(const StatementListNode &other) const;
};

}

#endif //NODE_STATEMENT_LIST_H

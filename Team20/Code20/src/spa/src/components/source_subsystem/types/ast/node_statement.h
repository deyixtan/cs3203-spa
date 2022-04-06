#ifndef NODE_STATEMENT_H
#define NODE_STATEMENT_H

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/types/ast/node.h"
#include "../../../pkb/stores/store.h"

namespace source {

class StatementNode : public Node {
 protected:
  String m_stmt_no;

 public:
  explicit StatementNode(String stmt_no);
  [[nodiscard]] String GetStatementNumber();
  void Accept(DesignExtractorPtr design_extractor) = 0;
  CfgNodePtr Accept(CfgBuilderPtr cfg_builder, CfgNodePtr cfg_node) = 0;
  virtual bool operator==(const StatementNode &other) const = 0;
};

}

#endif //NODE_STATEMENT_H

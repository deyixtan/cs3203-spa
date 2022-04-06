#ifndef NODE_WHILE_STATEMENT_H
#define NODE_WHILE_STATEMENT_H

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/types/ast/node_statement.h"

namespace source {

class WhileStatementNode : public StatementNode {
 private:
  ConditionalExpressionNodePtr m_condition;
  StatementListNodePtr m_stmt_list;

 public:
  WhileStatementNode(String &stmt_no, ConditionalExpressionNodePtr condition, StatementListNodePtr stmt_list);
  [[nodiscard]] ConditionalExpressionNodePtr GetCondition();
  [[nodiscard]] StatementListNodePtr GetStatementList();
  void Accept(DesignExtractorPtr design_extractor) override;
  //void Accept(DesignExtractor *design_extractor, String proc_name);
  CfgNodePtr Accept(CfgBuilder *cfg_builder, CfgNodePtr cfg_node);
  [[nodiscard]] bool operator==(const StatementNode &other) const;
  StmtType GetStatementType();
};

}

#endif //NODE_WHILE_STATEMENT_H

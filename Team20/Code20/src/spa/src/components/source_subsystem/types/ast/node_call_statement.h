#ifndef NODE_CALL_STATEMENT_H
#define NODE_CALL_STATEMENT_H

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/types/ast/node_statement.h"

namespace source {

class CallStatementNode : public StatementNode {
 private:
  String m_caller_name;
  String m_callee_name;

 public:
  CallStatementNode(String &stmt_no, String caller_name, String callee_name);
  [[nodiscard]] String GetCallerName();
  [[nodiscard]] String GetCalleeName();
  void Accept(DesignExtractorPtr design_extractor) override;
  void Accept(CfgBuilderPtr cfg_builder) override;
  [[nodiscard]] bool operator==(const StatementNode &other) const override;
  [[nodiscard]] StmtType GetStatementType() override;
};

}

#endif //NODE_CALL_STATEMENT_H

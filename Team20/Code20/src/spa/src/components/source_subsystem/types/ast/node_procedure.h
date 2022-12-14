#ifndef NODE_PROCEDURE_H
#define NODE_PROCEDURE_H

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/types/ast/node.h"

namespace source {

class ProcedureNode : public Node {
 private:
  String m_name;
  StatementListNodePtr m_stmt_list;

 public:
  ProcedureNode(String name, StatementListNodePtr stmt_list);
  [[nodiscard]] String GetName();
  [[nodiscard]] StatementListNodePtr GetStatementList();
  void Accept(DesignExtractorPtr design_extractor) override;
  void Accept(CfgBuilderPtr cfg_builder) override;
  [[nodiscard]] bool operator==(const ProcedureNode &other) const;
};

}

#endif //NODE_PROCEDURE_H

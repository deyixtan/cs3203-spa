#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_READ_NODE_HANDLER_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_READ_NODE_HANDLER_H_

#include "node_handler.h"
#include "node_read_statement.h"
#include "../../pkb/design_extractor/design_extractor.h"

class ReadNodeHandler : public NodeHandler {
 private:
  ReadStatementNode read_node;
  ReadNodeHandler(const ReadStatementNode node);
  void process(DesignExtractor de);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_READ_NODE_HANDLER_H_

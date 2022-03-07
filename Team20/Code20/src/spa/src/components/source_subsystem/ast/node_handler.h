#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_HANDLER_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_HANDLER_H_

#include "components/pkb/design_extractor/populator.h"
#include "node_statement.h"

class NodeHandler {
 public:
  virtual void process(Populator populator, std::vector<std::string>* visited);

};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_HANDLER_H_

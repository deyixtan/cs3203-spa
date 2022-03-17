#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_H_

// TODO: remove dependence on PKB's StmtType

#include "components/source_subsystem/pkb_client.h"

class PkbClient;

class Node {
 public:
  [[nodiscard]] virtual std::string ToString() = 0;
  [[nodiscard]] virtual std::string GetPatternFormat() = 0;
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_H_

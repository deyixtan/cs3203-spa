#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_H_

// TODO: remove dependence on PKB's StmtType
#include "../../pkb/pkb.h"

class Node {
 public:
  [[nodiscard]] virtual std::string ToString(int level) = 0;

};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_H_

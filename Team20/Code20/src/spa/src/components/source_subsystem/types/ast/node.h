#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_H_

#include "components/source_subsystem/source_declarations.h"

namespace source {

class Node {
 public:
  [[nodiscard]] virtual std::string ToString() = 0;
  [[nodiscard]] virtual std::string GetPatternFormat() = 0;
};

}

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_H_

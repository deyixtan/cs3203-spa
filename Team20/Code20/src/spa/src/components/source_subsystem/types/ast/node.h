#ifndef NODE_H
#define NODE_H

#include "components/source_subsystem/source_declarations.h"

namespace source {

class Node {
 public:
  [[nodiscard]] virtual String GetPatternFormat() = 0;
};

}

#endif //NODE_H

#ifndef NODE_H
#define NODE_H

#include "components/source_subsystem/source_declarations.h"

namespace source {

class Node {
 public:
  virtual void Accept(DesignExtractor *de) = 0;
  virtual StringToCfgNodePtrMap Accept(CfgBuilder *cb) = 0;
};

}

#endif //NODE_H

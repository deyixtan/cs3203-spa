#ifndef NODE_H
#define NODE_H

#include "components/source_subsystem/source_declarations.h"

namespace source {

class Node : public std::enable_shared_from_this<Node> {
 public:
  virtual void Accept(DesignExtractorPtr design_extractor) = 0;

  // not needed for pure virtual
  // not all AST nodes needed to be iterated to build Cfg
  virtual void Accept(CfgBuilderPtr cfg_builder) {};
};

}

#endif //NODE_H

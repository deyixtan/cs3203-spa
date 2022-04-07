#ifndef NODE_H
#define NODE_H

#include "components/source_subsystem/source_declarations.h"

namespace source {

class Node : public std::enable_shared_from_this<Node> {
 public:
  virtual void Accept(DesignExtractorPtr design_extractor) = 0;
  virtual CfgNodePtr Accept(CfgBuilderPtr cfg_builder, CfgNodePtr cfg_node) = 0;
};

}

#endif //NODE_H

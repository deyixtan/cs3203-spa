#ifndef SOURCE_DECLARATIONS_H
#define SOURCE_DECLARATIONS_H

#include <memory>
#include <string>
#include <vector>

namespace source {
  class SourceToken;

  typedef std::shared_ptr<SourceToken> TokenPtr;
  typedef std::vector<TokenPtr> TokenStream;
}

#endif //SOURCE_DECLARATIONS_H

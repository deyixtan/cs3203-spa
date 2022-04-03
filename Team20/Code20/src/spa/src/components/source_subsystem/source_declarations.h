#ifndef SOURCE_DECLARATIONS_H
#define SOURCE_DECLARATIONS_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace source {
  class SourceToken;

  class ProgramNode;
  class ProgramNode;
  class ProcedureNode;
  class StatementListNode;
  class StatementNode;

  class Cfg;
  class CfgNode;

  class PkbClient;

  class DesignExtractor;
  class CfgBuilder;

  typedef std::shared_ptr<SourceToken> TokenPtr;
  typedef std::vector<TokenPtr> TokenStream;
}

#endif //SOURCE_DECLARATIONS_H

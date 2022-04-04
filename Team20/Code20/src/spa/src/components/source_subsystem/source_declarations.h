#ifndef SOURCE_DECLARATIONS_H
#define SOURCE_DECLARATIONS_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace source {
  // types
  enum class TokenType;
  class SourceToken;

  class ProgramNode;
  class ProgramNode;
  class ProcedureNode;
  class StatementListNode;
  class StatementNode;
  class ReadStatementNode;
  class PrintStatementNode;
  class AssignStatementNode;
  class CallStatementNode;
  class WhileStatementNode;
  class IfStatementNode;
  class ExpressionNode;
  class ConditionalExpressionNode;
  class RelationalExpressionNode;
  class VariableNode;
  class ConstantNode;

  class Cfg;
  class CfgNode;

  // components
  class SourceParserSession;
  class PkbClient;

  class DesignExtractor;
  class CfgBuilder;

  typedef std::shared_ptr<SourceToken> TokenPtr;
  typedef std::vector<TokenPtr> TokenStream;
}

#endif //SOURCE_DECLARATIONS_H

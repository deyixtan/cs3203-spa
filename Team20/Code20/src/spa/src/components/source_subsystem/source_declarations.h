#ifndef SOURCE_DECLARATIONS_H
#define SOURCE_DECLARATIONS_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace source {
  // token-related types
  enum class TokenType;
  class SourceToken;

  // ast-related types
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
  class BooleanExpressionNode;
  class RelationalExpressionNode;
  class NotExpressionNode;
  class CombinationExpressionNode;
  class VariableNode;
  class ConstantNode;

  // cfg-related types
  class Cfg;
  class CfgNode;

  // components
  class SourceLexer;
  class SourceParser;
  class SourceParserSession;
  class DesignExtractor;
  class CfgBuilder;
  class PkbClient;

  // type definitions
  typedef std::shared_ptr<SourceToken> TokenPtr;
  typedef std::vector<TokenPtr> TokenStream;

}

#endif //SOURCE_DECLARATIONS_H

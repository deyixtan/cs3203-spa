#ifndef SOURCE_DECLARATIONS_H
#define SOURCE_DECLARATIONS_H

#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace source {
  // forward-declarations
  // enum classes
  enum class TokenType;
  enum class BooleanOperator;
  enum class RelationOperator;
  enum class ArithmeticOperator;

  // token-related types
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
  // token-related types
  typedef std::shared_ptr<SourceToken> TokenPtr;
  typedef std::vector<TokenPtr> TokenStream;

  // ast-related types
  typedef std::shared_ptr<ProgramNode> ProgramNodePtr;
  typedef std::shared_ptr<ProcedureNode> ProcedureNodePtr;
  typedef std::shared_ptr<StatementListNode> StatementListNodePtr;
  typedef std::shared_ptr<StatementNode> StatementNodePtr;
  typedef std::shared_ptr<ReadStatementNode> ReadStatementNodePtr;
  typedef std::shared_ptr<PrintStatementNode> PrintStatementNodePtr;
  typedef std::shared_ptr<AssignStatementNode> AssignStatementNodePtr;
  typedef std::shared_ptr<CallStatementNode> CallStatementNodePtr;
  typedef std::shared_ptr<WhileStatementNode> WhileStatementNodePtr;
  typedef std::shared_ptr<IfStatementNode> IfStatementNodePtr;
  typedef std::shared_ptr<ExpressionNode> ExpressionNodePtr;
  typedef std::shared_ptr<ConditionalExpressionNode> ConditionalExpressionNodePtr;
  typedef std::shared_ptr<BooleanExpressionNode> BooleanExpressionNodePtr;
  typedef std::shared_ptr<RelationalExpressionNode> RelationalExpressionNodePtr;
  typedef std::shared_ptr<NotExpressionNode> NotExpressionNodePtr;
  typedef std::shared_ptr<CombinationExpressionNode> CombinationExpressionNodePtr;
  typedef std::shared_ptr<VariableNode> VariableNodePtr;
  typedef std::shared_ptr<ConstantNode> ConstantNodePtr;

  // ast-related types (stream)
  typedef std::vector<ProcedureNodePtr> ProcedureNodeStream;
  typedef std::vector<StatementNodePtr> StatementNodeStream;

}

#endif //SOURCE_DECLARATIONS_H

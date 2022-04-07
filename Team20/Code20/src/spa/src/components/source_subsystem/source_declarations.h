#ifndef SOURCE_DECLARATIONS_H
#define SOURCE_DECLARATIONS_H

#include <algorithm>
#include <iostream>
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

  // ast-related types
  class SourceToken;
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
  class CallGraph;
  struct CfgNodeStatement;
  class CfgNode;
  class Cfg;

  // components
  class SourceLexer;
  class SourceParser;
  class SourceParserSession;
  class DesignExtractor;
  class CfgBuilder;
  class PkbClient;

  // type definitions
  // general
  typedef std::exception Exception;
  typedef std::string String;
  typedef std::vector<String> StringStream;
  typedef std::queue<String> StringQueue;
  typedef std::unordered_set<String> StringSet;
  typedef std::unordered_map<std::string, int> StringToIntMap;
  typedef std::unordered_map<String, StringSet> StringToStringSetMap;

  // ast-related types
  typedef std::shared_ptr<SourceToken> TokenPtr;
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

  // ast-related types (others)
  typedef std::vector<TokenPtr> TokenStream;
  typedef std::vector<ProcedureNodePtr> ProcedureNodeStream;
  typedef std::vector<StatementNodePtr> StatementNodeStream;

  // cfg-related types
  typedef std::shared_ptr<CallGraph> CallGraphPtr;
  typedef std::shared_ptr<CfgNode> CfgNodePtr;
  typedef std::shared_ptr<Cfg> CfgPtr;

  // cfg-related types (others)
  typedef std::vector<CfgNodeStatement> CfgNodeStatementStream;
  typedef std::vector<CfgNodePtr> CfgNodeStream;
  typedef std::unordered_map<String , CfgNodePtr> StringToCfgNodePtrMap;

  // components
  typedef std::shared_ptr<CfgBuilder> CfgBuilderPtr;
  typedef std::shared_ptr<DesignExtractor> DesignExtractorPtr;
  typedef std::shared_ptr<PkbClient> PkbClientPtr;
}

#endif //SOURCE_DECLARATIONS_H

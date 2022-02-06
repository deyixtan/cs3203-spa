#ifndef SOURCE_PARSER_H
#define SOURCE_PARSER_H

#include <vector>

#include "components/source_subsystem/tnode.h"
#include "components/source_subsystem/source_token.h"

class SourceParser {
 private:
  std::vector<SourceToken *> tokens_ptr;
  int cursor;
  int line_number;

 public:
  SourceParser(std::vector<SourceToken *> tokens_ptr);
  SourceToken *FetchCurrentToken();
  SourceToken *FetchNextToken();
  void IncrementCursor();
  bool AreTokensProcessed();
  SourceToken *ConsumeToken(TokenType type);
  ProgramNode *ParseProgram();
  ProcedureNode *ParseProcedure();
  StatementListNode *ParseStatementList();
  StatementNode *ParseStatement();
  ReadStatementNode *ParseReadStatement();
  PrintStatementNode *ParsePrintStatement();
  WhileStatementNode *ParseWhileStatement();
  IfStatementNode *ParseIfStatement();
  AssignStatementNode *ParseAssignStatement();
  ConditionalExpressionNode *ParseConditionalExpression();
  BooleanExpressionNode *ParseConditionalExpression2();
  RelationalExpressionNode *ParseRelationalExpression();
  Expression *ParseRelationalFactor();
  Expression *ParseExpression();
  ExpressionNode *ParseExpression2(ExpressionNode *left);
  ExpressionNode *ParseTerm();
  ExpressionNode *ParseTerm2(ExpressionNode *left);
  ExpressionNode *ParseFactor();
};

#endif //SOURCE_PARSER_H
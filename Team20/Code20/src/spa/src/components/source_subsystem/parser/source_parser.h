#ifndef SOURCE_PARSER_H
#define SOURCE_PARSER_H

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/parser/source_parser_session.h"

namespace source {

class SourceParser {
 private:
  int m_cursor;
  int m_curr_stmt_no;
  TokenStream m_token_stream;
  SourceParserSession m_session;

 private:
  [[nodiscard]] bool HasMoreTokens();
  [[nodiscard]] TokenPtr FetchToken(int index);
  [[nodiscard]] TokenPtr FetchCurrentToken();
  TokenPtr ProcessToken(TokenType type);
  [[nodiscard]] bool IsConditionalOperand(int &cursor);
  [[nodiscard]] bool IsConditionalExpression();
  [[nodiscard]] BooleanOperator GetBooleanOperator();
  [[nodiscard]] RelationOperator GetRelationOperator();
  [[nodiscard]] ArithmeticOperator GetExpressionOperator();
  [[nodiscard]] ArithmeticOperator GetTermOperator();
  [[nodiscard]] ProcedureNodePtr ParseProcedure();
  [[nodiscard]] StatementListNodePtr ParseStatementList();
  [[nodiscard]] StatementNodePtr ParseStatement();
  [[nodiscard]] ReadStatementNodePtr ParseReadStatement();
  [[nodiscard]] PrintStatementNodePtr ParsePrintStatement();
  [[nodiscard]] AssignStatementNodePtr ParseAssignStatement();
  [[nodiscard]] CallStatementNodePtr ParseCallStatement();
  [[nodiscard]] WhileStatementNodePtr ParseWhileStatement();
  [[nodiscard]] IfStatementNodePtr ParseIfStatement();
  [[nodiscard]] ConditionalExpressionNodePtr ParseConditionalExpression();
  [[nodiscard]] BooleanExpressionNodePtr ParseBooleanExpressionNode();
  [[nodiscard]] RelationalExpressionNodePtr ParseRelationalExpression();
  [[nodiscard]] NotExpressionNodePtr ParseNotExpressionNode();
  [[nodiscard]] ExpressionNodePtr ParseRelationalFactor();
  [[nodiscard]] ExpressionNodePtr ParseExpression();
  [[nodiscard]] ExpressionNodePtr ParseExpression(ExpressionNodePtr left_term);
  [[nodiscard]] ExpressionNodePtr ParseTerm();
  [[nodiscard]] ExpressionNodePtr ParseTerm(ExpressionNodePtr left_factor);
  [[nodiscard]] ExpressionNodePtr ParseFactor();

 public:
  explicit SourceParser(TokenStream token_stream);
  [[nodiscard]] ProgramNodePtr ParseProgram();
};

}

#endif //SOURCE_PARSER_H

#ifndef SOURCE_PARSER_H
#define SOURCE_PARSER_H

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/parser/source_parser_session.h"

namespace source {

class SourceParser {
 private:
  int m_cursor;
  int m_curr_stmt_no;
  SourceParserSession m_session;
  TokenStream m_tokens_ptr;

 private:
  [[nodiscard]] bool AreTokensProcessed();
  [[nodiscard]] TokenPtr FetchToken(int tokens_ahead);
  [[nodiscard]] TokenPtr FetchCurrentToken();
  TokenPtr ProcessToken(TokenType type);
  [[nodiscard]] bool IsConditionalOperand(int &cursor);
  [[nodiscard]] bool IsConditionalExpression();
  [[nodiscard]] ProcedureNodePtr ParseProcedure();
  [[nodiscard]] StatementListNodePtr ParseStatementList();
  [[nodiscard]] StatementNodePtr ParseStatement();
  [[nodiscard]] ReadStatementNodePtr ParseReadStatement();
  [[nodiscard]] PrintStatementNodePtr ParsePrintStatement();
  [[nodiscard]] AssignStatementNodePtr ParseAssignStatement();
  [[nodiscard]] std::shared_ptr<WhileStatementNode> ParseWhileStatement();
  [[nodiscard]] std::shared_ptr<IfStatementNode> ParseIfStatement();
  [[nodiscard]] std::shared_ptr<CallStatementNode> ParseCallStatement();
  [[nodiscard]] std::shared_ptr<ConditionalExpressionNode> ParseConditionalExpression();
  [[nodiscard]] std::shared_ptr<RelationalExpressionNode> ParseRelationalExpression();
  [[nodiscard]] std::shared_ptr<ExpressionNode> ParseRelationalFactor();
  [[nodiscard]] std::shared_ptr<ExpressionNode> ParseExpression();
  [[nodiscard]] std::shared_ptr<ExpressionNode> ParseExpression(std::shared_ptr<ExpressionNode> left_term);
  [[nodiscard]] std::shared_ptr<ExpressionNode> ParseTerm();
  [[nodiscard]] std::shared_ptr<ExpressionNode> ParseTerm(std::shared_ptr<ExpressionNode> left_factor);
  [[nodiscard]] std::shared_ptr<ExpressionNode> ParseFactor();

 public:
  SourceParser(TokenStream tokens_ptr);
  [[nodiscard]] ProgramNodePtr ParseProgram();
};

}

#endif //SOURCE_PARSER_H

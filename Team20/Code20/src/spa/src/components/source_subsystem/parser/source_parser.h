#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_SOURCE_PARSER_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_SOURCE_PARSER_H_

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/parser/source_parser_session.h"

namespace source {

class SourceParser {
 private:
  SourceParserSession m_session;
  int m_cursor;
  int m_curr_stmt_no;
  std::vector<std::shared_ptr<SourceToken>> m_tokens_ptr;

 private:
  [[nodiscard]] bool AreTokensProcessed();
  [[nodiscard]] std::shared_ptr<SourceToken> FetchToken(int tokens_ahead);
  [[nodiscard]] std::shared_ptr<SourceToken> FetchCurrentToken();
  std::shared_ptr<SourceToken> ProcessToken(TokenType type);
  [[nodiscard]] bool IsConditionalOperand(int &cursor);
  [[nodiscard]] bool IsConditionalExpression();
  [[nodiscard]] std::shared_ptr<ProcedureNode> ParseProcedure();
  [[nodiscard]] std::shared_ptr<StatementListNode> ParseStatementList();
  [[nodiscard]] std::shared_ptr<StatementNode> ParseStatement();
  [[nodiscard]] std::shared_ptr<ReadStatementNode> ParseReadStatement();
  [[nodiscard]] std::shared_ptr<PrintStatementNode> ParsePrintStatement();
  [[nodiscard]] std::shared_ptr<WhileStatementNode> ParseWhileStatement();
  [[nodiscard]] std::shared_ptr<IfStatementNode> ParseIfStatement();
  [[nodiscard]] std::shared_ptr<AssignStatementNode> ParseAssignStatement();
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
  SourceParser(std::vector<std::shared_ptr<SourceToken>> tokens_ptr);
  [[nodiscard]] std::shared_ptr<ProgramNode> ParseProgram();
};

}

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_SOURCE_PARSER_H_

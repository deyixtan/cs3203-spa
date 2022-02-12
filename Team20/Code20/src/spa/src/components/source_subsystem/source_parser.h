#ifndef SOURCE_PARSER_H
#define SOURCE_PARSER_H

#include <vector>

#include "ast/node_program.h"
#include "ast/node_read_statement.h"
#include "ast/node_print_statement.h"
#include "ast/node_while_statement.h"
#include "ast/node_if_statement.h"
#include "ast/node_assign_statement.h"
#include "ast/node_boolean_expression.h"
#include "ast/node_relational_expression.h"
#include "ast/node_not_expression.h"
#include "ast/node_constant.h"
#include "components/source_subsystem/source_token.h"
#include "exceptions/end_of_stream.h"
#include "exceptions/mismatch_token.h"

namespace source {

class SourceParser {
 private:
  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;
  int cursor;
  int line_number;
  int max_tokens_index;

 public:
  SourceParser(std::vector<std::shared_ptr<SourceToken>> tokens_ptr);
  std::shared_ptr<SourceToken> FetchCurrentToken();
  std::shared_ptr<SourceToken> FetchNextToken();
  void IncrementCursor();
  bool AreTokensProcessed();
  std::shared_ptr<SourceToken> ConsumeToken(TokenType type);
  std::shared_ptr<ProgramNode> ParseProgram();
  std::shared_ptr<ProcedureNode> ParseProcedure();
  std::shared_ptr<StatementListNode> ParseStatementList();
  std::shared_ptr<StatementNode> ParseStatement();
  std::shared_ptr<ReadStatementNode> ParseReadStatement();
  std::shared_ptr<PrintStatementNode> ParsePrintStatement();
  std::shared_ptr<WhileStatementNode> ParseWhileStatement();
  std::shared_ptr<IfStatementNode> ParseIfStatement();
  std::shared_ptr<AssignStatementNode> ParseAssignStatement();
  std::shared_ptr<ConditionalExpressionNode> ParseConditionalExpression();
  std::shared_ptr<BooleanExpressionNode> ParseConditionalExpression2();
  std::shared_ptr<RelationalExpressionNode> ParseRelationalExpression();
  std::shared_ptr<ExpressionNode> ParseRelationalFactor();
  std::shared_ptr<ExpressionNode> ParseExpression();
  std::shared_ptr<ExpressionNode> ParseExpression2(std::shared_ptr<ExpressionNode> left);
  std::shared_ptr<ExpressionNode> ParseTerm();
  std::shared_ptr<ExpressionNode> ParseTerm2(std::shared_ptr<ExpressionNode> left);
  std::shared_ptr<ExpressionNode> ParseFactor();
};

}

#endif //SOURCE_PARSER_H

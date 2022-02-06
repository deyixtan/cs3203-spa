#include "components/source_subsystem/source_parser.h"

SourceParser::SourceParser(std::vector<SourceToken *> tokens_ptr) {
  this->tokens_ptr = tokens_ptr;
  this->cursor = 0;
  this->line_number = 0;
}

SourceToken *SourceParser::FetchCurrentToken() {
  if (cursor >= tokens_ptr.size()) {
    return nullptr;
  }
  return tokens_ptr[cursor];
}

void SourceParser::IncrementCursor() {
  if (cursor >= tokens_ptr.size()) {
    throw std::runtime_error("Cursor reached end of stream.");
  }
  cursor++;
}

bool SourceParser::AreTokensProcessed() {
  return cursor == tokens_ptr.size();
}

SourceToken *SourceParser::ConsumeToken(TokenType type) {
  SourceToken *token_ptr = FetchCurrentToken();
  if (token_ptr->GetType() != type || token_ptr->GetType() != TokenType::NEW_LINE) {
    throw std::runtime_error("Unable to consume token, mismatched type.");
  }
  IncrementCursor();

  if (token_ptr->GetType() == TokenType::NEW_LINE) {
    line_number++;
    return ConsumeToken(type);
  }
  return token_ptr;
}

ProgramNode *SourceParser::ParseProgram() {
  vector<ProcedureNode> procedures;
  while (!AreTokensProcessed()) {
    procedures.push_back(ParseProcedure());
  }
  return new ProgramNode(procedures);
}

ProcedureNode *SourceParser::ParseProcedure() {
  ConsumeToken(TokenType::PROCEDURE);
  SourceToken *identifier = ConsumeToken(TokenType::NAME);
  ConsumeToken(TokenType::OPENED_BRACES);
  StatementListNode stmt_list = ParseStatementList();
  ConsumeToken(TokenType::CLOSED_BRACES);
  return new ProcedureNode(identifier, stmt_list);
}

StatementListNode *SourceParser::ParseStatementList() {
  vector<StatementNode> statements;
  while (FetchCurrentToken()->GetType() != TokenType::CLOSED_BRACES) {
    statements.push_back(ParseStatement());
  }
  return new StatementListNode(statements)
}

StatementNode *SourceParser::ParseStatement() {
  SourceToken *token_ptr = FetchCurrentToken();
  switch (token_ptr->GetType()) {
    case TokenType::READ:return ParseReadStatement();
    case TokenType::PRINT:return ParsePrintStatement();
    case TokenType::WHILE:return ParseWhileStatement();
    case TokenType::IF:return ParseIfStatement();
    case TokenType::EQUAL:return ParseAssignStatement();
  }
  throw std::runtime_error("Parsing invalid statement.");
}

ReadStatementNode *SourceParser::ParseReadStatement() {
  ConsumeToken(TokenType::READ);
  SourceToken *identifier = ConsumeToken(TokenType::NAME);
  return new PrintStatementNode(identifier);
}

PrintStatementNode *SourceParser::ParsePrintStatement() {
  ConsumeToken(TokenType::PRINT);
  SourceToken *identifier = ConsumeToken(TokenType::NAME);
  return new PrintStatementNode(identifier);
}

WhileStatementNode *SourceParser::ParseWhileStatement() {
  ConsumeToken(TokenType::WHILE);
  ConsumeToken(TokenType::OPENED_PARENTHESIS);
  ConditionalExpressionNode *cond_expr = ParseConditionalExpression();
  ConsumeToken(TokenType::CLOSED_PARENTHESIS);
  ConsumeToken(TokenType::OPENED_BRACES);
  StatementListNode *stmt_list = ParseStatementList();
  ConsumeToken(TokenType::CLOSED_BRACES);
  return new WhileStatementNode(cond_expr, stmt_list);
}

IfStatementNode *SourceParser::ParseIfStatement() {
  ConsumeToken(TokenType::IF);
  ConsumeToken(TokenType::OPENED_PARENTHESIS);
  ConditionalExpressionNode *cond_expr = ParseConditionalExpression();
  ConsumeToken(TokenType::CLOSED_PARENTHESIS);
  ConsumeToken(TokenType::THEN);
  ConsumeToken(TokenType::OPENED_BRACES);
  StatementListNode *if_stmt_list = ParseStatementList();
  ConsumeToken(TokenType::CLOSED_BRACES);
  ConsumeToken(TokenType::ELSE);
  ConsumeToken(TokenType::OPENED_BRACES);
  StatementListNode *else_stmt_list = ParseStatementList();
  ConsumeToken(TokenType::CLOSED_BRACES);
  return new IfStatementNode(cond_expr, if_stmt_list, else_stmt_list);
}

AssignStatementNode *SourceParser::ParseAssignStatement() {
  SourceToken *identifier = ConsumeToken(TokenType::NAME);
  ConsumeToken(TokenType::EQUAL);;
  ExpressionNode *expr = ParseExpression();
  ConsumeToken(TokenType::SEMI_COLON);
  return new AssignStatementNode(identifier, expr);
}

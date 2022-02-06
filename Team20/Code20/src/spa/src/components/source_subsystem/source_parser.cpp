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

SourceToken *SourceParser::FetchNextToken() {
  if (cursor + 1 >= tokens_ptr.size()) {
    return nullptr;
  }
  return tokens_ptr[cursor + 1];
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

ConditionalExpressionNode *SourceParser::ParseConditionalExpression() {
  TokenType type = FetchCurrentToken()->GetType();
  if (type == TokenType::NOT) {
    ConsumeToken(TokenType::NOT);
    ConsumeToken(TokenType::OPENED_PARENTHESIS);
    ConditionalExpressionNode *expr = ParseConditionalExpression();
    ConsumeToken(TokenType::CLOSED_PARENTHESIS);
    return new NotExpressionNode(expr);
  } else if (type == TokenType::OPENED_PARENTHESIS) {
    ConsumeToken(TokenType::OPENED_PARENTHESIS);
    ConditionalExpressionNode *left = ParseConditionalExpression();
    ConsumeToken(TokenType::CLOSED_PARENTHESIS);
    BooleanExpressionNode *right = ParseConditionalExpression2();
    right->setLeft(left);
    return right;
  } else if (type == TokenType::NAME || type == TokenType::DIGIT || type == TokenType::OPENED_PARENTHESIS) {
    return ParseRelationalExpression();
  }
  throw std::runtime_error("Unable to parse conditional expression");
}

BooleanExpressionNode *SourceParser::ParseConditionalExpression2() {
  std::string value = "";
  switch (FetchCurrentToken()->GetType()) {
    case TokenType::AND:ConsumeToken(TokenType::AND);
      value = "&&";
      break;
    case TokenType::OR:ConsumeToken(TokenType::OR);
      value = "||";
      break;
    default:throw std::runtime_error("Parsing invalid conditional expression operator");
  }
  ConsumeToken(TokenType::OPENED_PARENTHESIS);
  ConditionalExpressionNode *right = ParseConditionalExpression();
  ConsumeToken(TokenType::CLOSED_PARENTHESIS);
  return new BooleanExpressionNode(value, right);
}

RelationalExpressionNode *SourceParser::ParseRelationalExpression() {
  ExpressionNode *left = ParseRelationalFactor();
  std::string value = "";
  switch (FetchCurrentToken()->GetType()) {
    case TokenType::IS_GREATER:ConsumeToken(TokenType::IS_GREATER);
      value = ">";
      break;
    case TokenType::IS_GREATER_EQUAL:ConsumeToken(TokenType::IS_GREATER_EQUAL);
      value = ">=";
      break;
    case TokenType::IS_LESSER_EQUAL:ConsumeToken(TokenType::IS_LESSER_EQUAL);
      value = "<=";
      break;
    case TokenType::IS_LESSER:ConsumeToken(TokenType::IS_LESSER);
      value = "<";
      break;
    case TokenType::IS_EQUAL:ConsumeToken(TokenType::IS_EQUAL);
      value = "==";
      break;
    case TokenType::IS_NOT_EQUAL:ConsumeToken(TokenType::IS_NOT_EQUAL);
      value = "!=";
      break;
    default:throw std::runtime_error("Parsing invalid relational expression");
  }
  ExpressionNode *right = ParseRelationalFactor();
  return new RelationalExpressionNode(value, left, right);
}

Expression *SourceParser::ParseRelationalFactor() {
  return ParseExpression();
}

Expression *SourceParser::ParseExpression() {
  Expression *expr = ParseTerm();
  return ParseExpression2(expr);
}

ExpressionNode *SourceParser::ParseExpression2(ExpressionNode *left) {
  std::string value = "";
  switch (FetchCurrentToken()->GetType()) {
    case TokenType::ADDITION:ConsumeToken(TokenType::ADDITION);
      value = "+";
      break;
    case TokenType::SUBTRACTION:ConsumeToken(TokenType::SUBTRACTION);
      value = "-";
      break;
    default:return left;
  }
  ExpressionNode *right = ParseTerm();
  return ParseExpression2(new CombinationExpressionNode(value, left, right));
}

ExpressionNode *SourceParser::ParseTerm() {
  ExpressionNode *factor = ParseFactor();
  return ParseTerm2(factor);
}

ExpressionNode *SourceParser::ParseTerm2(ExpressionNode *left) {
  std::string value = "";
  switch (FetchCurrentToken()->GetType()) {
    case TokenType::MULTIPLICATION:ConsumeToken(TokenType::MULTIPLICATION);
      value = "*";
      break;
    case TokenType::DIVISION:ConsumeToken(TokenType::DIVISION);
      value = "/";
      break;
    case TokenType::MODULUS:ConsumeToken(TokenType::MODULUS);
      value = "%";
      break;
    default:return left;
  }
  ExpressionNode *right = ParseFactor();
  return ParseTerm2(new CombinationExpressionNode(value, left, right));
}

ExpressionNode *SourceParser::ParseFactor() {
  switch (FetchCurrentToken()->GetType()) {
    case TokenType::NAME:return new IdentifierNode(ConsumeToken(TokenType::NAME)->GetValue());
    case TokenType::DIGIT:return new ConstantNode(ConsumeToken(TokenType::DIGIT)->GetValue());
    case TokenType::OPENED_PARENTHESIS:ConsumeToken(TokenType::OPENED_PARENTHESIS);
      ExpressionNode *expr = ParseExpression();
      ConsumeToken(TokenType::CLOSED_PARENTHESIS);
      return expr;
  }
  throw std::runtime_error("Unable to parse factor.");
}

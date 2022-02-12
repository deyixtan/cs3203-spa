#include "components/source_subsystem/source_parser.h"

namespace source {

SourceParser::SourceParser(std::vector<std::shared_ptr<SourceToken>> tokens_ptr) {
  this->tokens_ptr = tokens_ptr;
  this->cursor = 0;
  this->line_number = 0;
  this->max_tokens_index = tokens_ptr.size() - 1;
}

std::shared_ptr<SourceToken> SourceParser::FetchCurrentToken() {
  if (cursor >= tokens_ptr.size()) {
    return nullptr;
  }
  return tokens_ptr[cursor];
}

std::shared_ptr<SourceToken> SourceParser::FetchNextToken() {
  if (cursor + 1 >= tokens_ptr.size()) {
    return nullptr;
  }
  return tokens_ptr[cursor + 1];
}

void SourceParser::IncrementCursor() {
  if (cursor >= tokens_ptr.size()) {
    throw EndOfStreamException();
  }
  cursor++;
}

bool SourceParser::AreTokensProcessed() {
  return cursor >= max_tokens_index;
}

std::shared_ptr<SourceToken> SourceParser::ConsumeToken(TokenType type) {
  std::shared_ptr<SourceToken> token_ptr = FetchCurrentToken();
  if (token_ptr->GetType() != type) {
    throw MismatchedTokenException();
  }
  IncrementCursor();

  return token_ptr;
}

std::shared_ptr<ProgramNode> SourceParser::ParseProgram() {
  std::vector<std::shared_ptr<ProcedureNode>> procedures;
  while (!AreTokensProcessed()) {
    procedures.push_back(ParseProcedure());
  }
  return std::make_shared<ProgramNode>(procedures);
}

std::shared_ptr<ProcedureNode> SourceParser::ParseProcedure() {
  ConsumeToken(TokenType::PROCEDURE);
  std::shared_ptr<SourceToken> identifier = ConsumeToken(TokenType::NAME);
  ConsumeToken(TokenType::OPENED_BRACES);
  std::shared_ptr<StatementListNode> stmt_list = ParseStatementList();
  ConsumeToken(TokenType::CLOSED_BRACES);
  return std::make_shared<ProcedureNode>(identifier->GetValue(), stmt_list);
}

std::shared_ptr<StatementListNode> SourceParser::ParseStatementList() {
  std::vector<std::shared_ptr<StatementNode>> statements;
  while (FetchCurrentToken()->GetType() != TokenType::CLOSED_BRACES) {
    std::shared_ptr<StatementNode> statement = ParseStatement();
    if (statement == nullptr) {
      continue;
    }
    statements.push_back(statement);
  }
  return std::make_shared<StatementListNode>(statements);
}

std::shared_ptr<StatementNode> SourceParser::ParseStatement() {
  // handle assignment by predicting next token
  if (FetchNextToken()->GetType() == TokenType::EQUAL) {
    return ParseAssignStatement();
  }

  // handle other type of statements
  std::shared_ptr<SourceToken> token_ptr = FetchCurrentToken();
  switch (token_ptr->GetType()) {
    case TokenType::READ:return ParseReadStatement();
    case TokenType::PRINT:return ParsePrintStatement();
    case TokenType::WHILE:return ParseWhileStatement();
    case TokenType::IF:return ParseIfStatement();
    default:throw std::runtime_error("Parsing invalid statement.");
  }
}

std::shared_ptr<ReadStatementNode> SourceParser::ParseReadStatement() {
  ConsumeToken(TokenType::READ);
  std::shared_ptr<SourceToken> identifier = ConsumeToken(TokenType::NAME);
  ConsumeToken(TokenType::SEMI_COLON);
  return std::make_shared<ReadStatementNode>(line_number++, std::make_shared<VariableNode>(identifier->GetValue()));
}

std::shared_ptr<PrintStatementNode> SourceParser::ParsePrintStatement() {
  ConsumeToken(TokenType::PRINT);
  std::shared_ptr<SourceToken> identifier = ConsumeToken(TokenType::NAME);
  ConsumeToken(TokenType::SEMI_COLON);
  return std::make_shared<PrintStatementNode>(line_number++, std::make_shared<VariableNode>(identifier->GetValue()));
}

std::shared_ptr<WhileStatementNode> SourceParser::ParseWhileStatement() {
  ConsumeToken(TokenType::WHILE);
  ConsumeToken(TokenType::OPENED_PARENTHESIS);
  std::shared_ptr<ConditionalExpressionNode> cond_expr = ParseConditionalExpression();
  ConsumeToken(TokenType::CLOSED_PARENTHESIS);
  ConsumeToken(TokenType::OPENED_BRACES);
  std::shared_ptr<StatementListNode> stmt_list = ParseStatementList();
  ConsumeToken(TokenType::CLOSED_BRACES);
  return std::make_shared<WhileStatementNode>(line_number++, cond_expr, stmt_list);
}

std::shared_ptr<IfStatementNode> SourceParser::ParseIfStatement() {
  ConsumeToken(TokenType::IF);
  ConsumeToken(TokenType::OPENED_PARENTHESIS);
  std::shared_ptr<ConditionalExpressionNode> cond_expr = ParseConditionalExpression();
  ConsumeToken(TokenType::CLOSED_PARENTHESIS);
  ConsumeToken(TokenType::THEN);
  ConsumeToken(TokenType::OPENED_BRACES);
  std::shared_ptr<StatementListNode> if_stmt_list = ParseStatementList();
  ConsumeToken(TokenType::CLOSED_BRACES);
  ConsumeToken(TokenType::ELSE);
  ConsumeToken(TokenType::OPENED_BRACES);
  std::shared_ptr<StatementListNode> else_stmt_list = ParseStatementList();
  ConsumeToken(TokenType::CLOSED_BRACES);
  return std::make_shared<IfStatementNode>(line_number++, cond_expr, if_stmt_list, else_stmt_list);
}

std::shared_ptr<AssignStatementNode> SourceParser::ParseAssignStatement() {
  std::shared_ptr<SourceToken> identifier = ConsumeToken(TokenType::NAME);
  ConsumeToken(TokenType::EQUAL);;
  std::shared_ptr<ExpressionNode> expr = ParseExpression();
  ConsumeToken(TokenType::SEMI_COLON);
  return std::make_shared<AssignStatementNode>(line_number++,
                                               std::make_shared<VariableNode>(identifier->GetValue()),
                                               expr);
}

std::shared_ptr<ConditionalExpressionNode> SourceParser::ParseConditionalExpression() {
  TokenType type = FetchCurrentToken()->GetType();
  if (type == TokenType::NOT) {
    ConsumeToken(TokenType::NOT);
    ConsumeToken(TokenType::OPENED_PARENTHESIS);
    std::shared_ptr<ConditionalExpressionNode> expr = ParseConditionalExpression();
    ConsumeToken(TokenType::CLOSED_PARENTHESIS);
    return std::make_shared<NotExpressionNode>(expr);
  } else if (type == TokenType::OPENED_PARENTHESIS) {
    ConsumeToken(TokenType::OPENED_PARENTHESIS);
    std::shared_ptr<ConditionalExpressionNode> left = ParseConditionalExpression();
    ConsumeToken(TokenType::CLOSED_PARENTHESIS);
    std::shared_ptr<BooleanExpressionNode> right = ParseConditionalExpression2();
    right->SetLeftExpression(left);
    return right;
  } else if (type == TokenType::NAME || type == TokenType::DIGIT
      || type == TokenType::OPENED_PARENTHESIS) {
    return ParseRelationalExpression();
  }
  throw std::runtime_error("Unable to parse conditional expression");
}

std::shared_ptr<BooleanExpressionNode> SourceParser::ParseConditionalExpression2() {
  BooleanOperator value;
  switch (FetchCurrentToken()->GetType()) {
    case TokenType::AND:ConsumeToken(TokenType::AND);
      value = BooleanOperator::AND;
      break;
    case TokenType::OR:ConsumeToken(TokenType::OR);
      value = BooleanOperator::OR;
      break;
    default:throw std::runtime_error("Parsing invalid conditional expression operator");
  }
  ConsumeToken(TokenType::OPENED_PARENTHESIS);
  std::shared_ptr<ConditionalExpressionNode> right = ParseConditionalExpression();
  ConsumeToken(TokenType::CLOSED_PARENTHESIS);
  return std::make_shared<BooleanExpressionNode>(value, right);
}

std::shared_ptr<RelationalExpressionNode> SourceParser::ParseRelationalExpression() {
  std::shared_ptr<ExpressionNode> left = ParseRelationalFactor();
  RelationOperator value;
  switch (FetchCurrentToken()->GetType()) {
    case TokenType::IS_GREATER:ConsumeToken(TokenType::IS_GREATER);
      value = RelationOperator::GREATER_THAN;
      break;
    case TokenType::IS_GREATER_EQUAL:ConsumeToken(TokenType::IS_GREATER_EQUAL);
      value = RelationOperator::GREATER_THAN_EQUALS;
      break;
    case TokenType::IS_LESSER_EQUAL:ConsumeToken(TokenType::IS_LESSER_EQUAL);
      value = RelationOperator::LESS_THAN_EQUALS;
      break;
    case TokenType::IS_LESSER:ConsumeToken(TokenType::IS_LESSER);
      value = RelationOperator::LESS_THAN;
      break;
    case TokenType::IS_EQUAL:ConsumeToken(TokenType::IS_EQUAL);
      value = RelationOperator::EQUALS;
      break;
    case TokenType::IS_NOT_EQUAL:ConsumeToken(TokenType::IS_NOT_EQUAL);
      value = RelationOperator::NOT_EQUALS;
      break;
    default:throw std::runtime_error("Parsing invalid relational expression");
  }
  std::shared_ptr<ExpressionNode> right = ParseRelationalFactor();
  return std::make_shared<RelationalExpressionNode>(value, left, right);
}

std::shared_ptr<ExpressionNode> SourceParser::ParseRelationalFactor() {
  return ParseExpression();
}

std::shared_ptr<ExpressionNode> SourceParser::ParseExpression() {
  std::shared_ptr<ExpressionNode> expr = ParseTerm();
  return ParseExpression2(expr);
}

std::shared_ptr<ExpressionNode> SourceParser::ParseExpression2(std::shared_ptr<ExpressionNode> left) {
  ArithmeticOperator value;
  switch (FetchCurrentToken()->GetType()) {
    case TokenType::ADDITION:ConsumeToken(TokenType::ADDITION);
      value = ArithmeticOperator::PLUS;
      break;
    case TokenType::SUBTRACTION:ConsumeToken(TokenType::SUBTRACTION);
      value = ArithmeticOperator::MINUS;
      break;
    default:return left;
  }
  std::shared_ptr<ExpressionNode> right = ParseTerm();
  return ParseExpression2(std::make_shared<CombinationExpressionNode>(value, left, right));
}

std::shared_ptr<ExpressionNode> SourceParser::ParseTerm() {
  std::shared_ptr<ExpressionNode> factor = ParseFactor();
  return ParseTerm2(factor);
}

std::shared_ptr<ExpressionNode> SourceParser::ParseTerm2(std::shared_ptr<ExpressionNode> left) {
  ArithmeticOperator value;
  switch (FetchCurrentToken()->GetType()) {
    case TokenType::MULTIPLICATION:ConsumeToken(TokenType::MULTIPLICATION);
      value = ArithmeticOperator::MULTIPLY;
      break;
    case TokenType::DIVISION:ConsumeToken(TokenType::DIVISION);
      value = ArithmeticOperator::DIVIDE;
      break;
    case TokenType::MODULUS:ConsumeToken(TokenType::MODULUS);
      value = ArithmeticOperator::MOD;
      break;
    default:return left;
  }
  std::shared_ptr<ExpressionNode> right = ParseFactor();
  return ParseTerm2(std::make_shared<CombinationExpressionNode>(value, left, right));
}

std::shared_ptr<ExpressionNode> SourceParser::ParseFactor() {
  switch (FetchCurrentToken()->GetType()) {
    case TokenType::NAME:return std::make_shared<VariableNode>(ConsumeToken(TokenType::NAME)->GetValue());
    case TokenType::DIGIT:return std::make_shared<ConstantNode>(ConsumeToken(TokenType::DIGIT)->GetValue());
    case TokenType::OPENED_PARENTHESIS: {
      ConsumeToken(TokenType::OPENED_PARENTHESIS);
      std::shared_ptr<ExpressionNode> expr = ParseExpression();
      ConsumeToken(TokenType::CLOSED_PARENTHESIS);
      return expr;
    }
    default:throw std::runtime_error("Unable to parse factor.");
  }
}

}

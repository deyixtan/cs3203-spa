#include "components/source_subsystem/source_parser.h"

SourceParser::SourceParser(std::vector<SourceToken *> tokens_ptr) {
  this->tokens_ptr = tokens_ptr;
  this->cursor = 0;
  this->line_number = 0;
  this->max_tokens_index = tokens_ptr.size() - 1;
}

std::shared_ptr<SourceToken> SourceParser::FetchCurrentToken() {
  if (cursor >= tokens_ptr.size()) {
    return nullptr;
  }
  SourceToken token = *(tokens_ptr[cursor]);
  return std::make_shared<SourceToken>(token);
}

std::shared_ptr<SourceToken> SourceParser::FetchNextToken() {
  if (cursor + 1 >= tokens_ptr.size()) {
    return nullptr;
  }
  SourceToken token = *(tokens_ptr[cursor + 1]);
  return std::make_shared<SourceToken>(token);
}

void SourceParser::IncrementCursor() {
  if (cursor >= tokens_ptr.size()) {
    throw std::runtime_error("Cursor reached end of stream.");
  }
  cursor++;
}

bool SourceParser::AreTokensProcessed() {
  return cursor >= max_tokens_index;
}

std::shared_ptr<SourceToken> SourceParser::ConsumeToken(SourceTokenType type) {
  std::shared_ptr<SourceToken> token_ptr = FetchCurrentToken();
  if (token_ptr->GetType() != type) {
    throw std::runtime_error("Unable to consume token, mismatched type.");
  }
  IncrementCursor();

  if (token_ptr->GetType() == SourceTokenType::NEW_LINE) {
    line_number++;
  }
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
  ConsumeToken(SourceTokenType::PROCEDURE);
  std::shared_ptr<SourceToken> identifier = ConsumeToken(SourceTokenType::NAME);
  ConsumeToken(SourceTokenType::OPENED_BRACES);
  std::shared_ptr<StatementListNode> stmt_list = ParseStatementList();
  ConsumeToken(SourceTokenType::CLOSED_BRACES);
  return std::make_shared<ProcedureNode>(identifier->GetValue(), stmt_list);
}

std::shared_ptr<StatementListNode> SourceParser::ParseStatementList() {
  std::vector<std::shared_ptr<StatementNode>> statements;
  while (FetchCurrentToken()->GetType() != SourceTokenType::CLOSED_BRACES) {
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
  if (FetchNextToken()->GetType() == SourceTokenType::EQUAL) {
    return ParseAssignStatement();
  }

  // handle other type of statements
  std::shared_ptr<SourceToken> token_ptr = FetchCurrentToken();
  switch (token_ptr->GetType()) {
    case SourceTokenType::READ:return ParseReadStatement();
    case SourceTokenType::PRINT:return ParsePrintStatement();
    case SourceTokenType::WHILE:return ParseWhileStatement();
    case SourceTokenType::IF:return ParseIfStatement();
    case SourceTokenType::NEW_LINE: {
      ConsumeToken(SourceTokenType::NEW_LINE);
      return nullptr;
    }
    default:throw std::runtime_error("Parsing invalid statement.");
  }
}

std::shared_ptr<ReadStatementNode> SourceParser::ParseReadStatement() {
  ConsumeToken(SourceTokenType::READ);
  std::shared_ptr<SourceToken> identifier = ConsumeToken(SourceTokenType::NAME);
  ConsumeToken(SourceTokenType::SEMI_COLON);
  return std::make_shared<ReadStatementNode>(line_number, std::make_shared<VariableNode>(identifier->GetValue()));
}

std::shared_ptr<PrintStatementNode> SourceParser::ParsePrintStatement() {
  ConsumeToken(SourceTokenType::PRINT);
  std::shared_ptr<SourceToken> identifier = ConsumeToken(SourceTokenType::NAME);
  ConsumeToken(SourceTokenType::SEMI_COLON);
  return std::make_shared<PrintStatementNode>(line_number, std::make_shared<VariableNode>(identifier->GetValue()));
}

std::shared_ptr<WhileStatementNode> SourceParser::ParseWhileStatement() {
  ConsumeToken(SourceTokenType::WHILE);
  ConsumeToken(SourceTokenType::OPENED_PARENTHESIS);
  std::shared_ptr<ConditionalExpressionNode> cond_expr = ParseConditionalExpression();
  ConsumeToken(SourceTokenType::CLOSED_PARENTHESIS);
  ConsumeToken(SourceTokenType::OPENED_BRACES);
  std::shared_ptr<StatementListNode> stmt_list = ParseStatementList();
  ConsumeToken(SourceTokenType::CLOSED_BRACES);
  return std::make_shared<WhileStatementNode>(line_number, cond_expr, stmt_list);
}

std::shared_ptr<IfStatementNode> SourceParser::ParseIfStatement() {
  ConsumeToken(SourceTokenType::IF);
  ConsumeToken(SourceTokenType::OPENED_PARENTHESIS);
  std::shared_ptr<ConditionalExpressionNode> cond_expr = ParseConditionalExpression();
  ConsumeToken(SourceTokenType::CLOSED_PARENTHESIS);
  ConsumeToken(SourceTokenType::THEN);
  ConsumeToken(SourceTokenType::OPENED_BRACES);
  std::shared_ptr<StatementListNode> if_stmt_list = ParseStatementList();
  ConsumeToken(SourceTokenType::CLOSED_BRACES);
  ConsumeToken(SourceTokenType::ELSE);
  ConsumeToken(SourceTokenType::OPENED_BRACES);
  std::shared_ptr<StatementListNode> else_stmt_list = ParseStatementList();
  ConsumeToken(SourceTokenType::CLOSED_BRACES);
  return std::make_shared<IfStatementNode>(line_number, cond_expr, if_stmt_list, else_stmt_list);
}

std::shared_ptr<AssignStatementNode> SourceParser::ParseAssignStatement() {
  std::shared_ptr<SourceToken> identifier = ConsumeToken(SourceTokenType::NAME);
  ConsumeToken(SourceTokenType::EQUAL);;
  std::shared_ptr<ExpressionNode> expr = ParseExpression();
  ConsumeToken(SourceTokenType::SEMI_COLON);
  return std::make_shared<AssignStatementNode>(line_number,
                                               std::make_shared<VariableNode>(identifier->GetValue()),
                                               expr);
}

std::shared_ptr<ConditionalExpressionNode> SourceParser::ParseConditionalExpression() {
  SourceTokenType type = FetchCurrentToken()->GetType();
  if (type == SourceTokenType::NOT) {
    ConsumeToken(SourceTokenType::NOT);
    ConsumeToken(SourceTokenType::OPENED_PARENTHESIS);
    std::shared_ptr<ConditionalExpressionNode> expr = ParseConditionalExpression();
    ConsumeToken(SourceTokenType::CLOSED_PARENTHESIS);
    return std::make_shared<NotExpressionNode>(expr);
  } else if (type == SourceTokenType::OPENED_PARENTHESIS) {
    ConsumeToken(SourceTokenType::OPENED_PARENTHESIS);
    std::shared_ptr<ConditionalExpressionNode> left = ParseConditionalExpression();
    ConsumeToken(SourceTokenType::CLOSED_PARENTHESIS);
    std::shared_ptr<BooleanExpressionNode> right = ParseConditionalExpression2();
    right->SetLeftExpression(left);
    return right;
  } else if (type == SourceTokenType::NAME || type == SourceTokenType::DIGIT || type == SourceTokenType::OPENED_PARENTHESIS) {
    return ParseRelationalExpression();
  }
  throw std::runtime_error("Unable to parse conditional expression");
}

std::shared_ptr<BooleanExpressionNode> SourceParser::ParseConditionalExpression2() {
  BooleanOperator value;
  switch (FetchCurrentToken()->GetType()) {
    case SourceTokenType::AND:ConsumeToken(SourceTokenType::AND);
      value = BooleanOperator::AND;
      break;
    case SourceTokenType::OR:ConsumeToken(SourceTokenType::OR);
      value = BooleanOperator::OR;
      break;
    default:throw std::runtime_error("Parsing invalid conditional expression operator");
  }
  ConsumeToken(SourceTokenType::OPENED_PARENTHESIS);
  std::shared_ptr<ConditionalExpressionNode> right = ParseConditionalExpression();
  ConsumeToken(SourceTokenType::CLOSED_PARENTHESIS);
  return std::make_shared<BooleanExpressionNode>(value, right);
}

std::shared_ptr<RelationalExpressionNode> SourceParser::ParseRelationalExpression() {
  std::shared_ptr<ExpressionNode> left = ParseRelationalFactor();
  RelationOperator value;
  switch (FetchCurrentToken()->GetType()) {
    case SourceTokenType::IS_GREATER:ConsumeToken(SourceTokenType::IS_GREATER);
      value = RelationOperator::GREATER_THAN;
      break;
    case SourceTokenType::IS_GREATER_EQUAL:ConsumeToken(SourceTokenType::IS_GREATER_EQUAL);
      value = RelationOperator::GREATER_THAN_EQUALS;
      break;
    case SourceTokenType::IS_LESSER_EQUAL:ConsumeToken(SourceTokenType::IS_LESSER_EQUAL);
      value = RelationOperator::LESS_THAN_EQUALS;
      break;
    case SourceTokenType::IS_LESSER:ConsumeToken(SourceTokenType::IS_LESSER);
      value = RelationOperator::LESS_THAN;
      break;
    case SourceTokenType::IS_EQUAL:ConsumeToken(SourceTokenType::IS_EQUAL);
      value = RelationOperator::EQUALS;
      break;
    case SourceTokenType::IS_NOT_EQUAL:ConsumeToken(SourceTokenType::IS_NOT_EQUAL);
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
    case SourceTokenType::ADDITION:ConsumeToken(SourceTokenType::ADDITION);
      value = ArithmeticOperator::PLUS;
      break;
    case SourceTokenType::SUBTRACTION:ConsumeToken(SourceTokenType::SUBTRACTION);
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
    case SourceTokenType::MULTIPLICATION:ConsumeToken(SourceTokenType::MULTIPLICATION);
      value = ArithmeticOperator::MULTIPLY;
      break;
    case SourceTokenType::DIVISION:ConsumeToken(SourceTokenType::DIVISION);
      value = ArithmeticOperator::DIVIDE;
      break;
    case SourceTokenType::MODULUS:ConsumeToken(SourceTokenType::MODULUS);
      value = ArithmeticOperator::MOD;
      break;
    default:return left;
  }
  std::shared_ptr<ExpressionNode> right = ParseFactor();
  return ParseTerm2(std::make_shared<CombinationExpressionNode>(value, left, right));
}

std::shared_ptr<ExpressionNode> SourceParser::ParseFactor() {
  switch (FetchCurrentToken()->GetType()) {
    case SourceTokenType::NAME:return std::make_shared<VariableNode>(ConsumeToken(SourceTokenType::NAME)->GetValue());
    case SourceTokenType::DIGIT:return std::make_shared<ConstantNode>(ConsumeToken(SourceTokenType::DIGIT)->GetValue());
    case SourceTokenType::OPENED_PARENTHESIS: {
      ConsumeToken(SourceTokenType::OPENED_PARENTHESIS);
      std::shared_ptr<ExpressionNode> expr = ParseExpression();
      ConsumeToken(SourceTokenType::CLOSED_PARENTHESIS);
      return expr;
    }
    default:throw std::runtime_error("Unable to parse factor.");
  }
}

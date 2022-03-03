#include "components/source_subsystem/source_parser.h"

namespace source {

SourceParser::SourceParser(std::vector<std::shared_ptr<SourceToken>> tokens_ptr)
    : m_cursor(0), m_curr_stmt_no(0), m_tokens_ptr(std::move(tokens_ptr)) {}

bool SourceParser::AreTokensProcessed() {
  return m_cursor >= m_tokens_ptr.size();
}

std::shared_ptr<SourceToken> SourceParser::FetchToken(int tokens_ahead) {
  if (m_cursor + tokens_ahead >= m_tokens_ptr.size()) {
    throw EndOfStreamException();
  }
  return m_tokens_ptr[m_cursor + tokens_ahead];
}

std::shared_ptr<SourceToken> SourceParser::FetchCurrentToken() {
  return FetchToken(0);
}

std::shared_ptr<SourceToken> SourceParser::ProcessToken(TokenType type) {
  std::shared_ptr<SourceToken> token_ptr = FetchCurrentToken();

  if (token_ptr->GetType() != type) {
    throw MismatchedTokenException();
  }

  m_cursor++;
  return token_ptr;
}

bool SourceParser::IsConditionalOperand(int &cursor) {
  int nest = 0;

  while (cursor < m_tokens_ptr.size()) {
    TokenType type = m_tokens_ptr[cursor]->GetType();

    if (type == TokenType::OPENED_PARENTHESIS) {
      nest++;
    } else if (type == TokenType::CLOSED_PARENTHESIS) {
      if (nest > 0) {
        nest--;
      } else if (nest == 0) {
        cursor++;
        break;
      } else {
        return false;
      }
    }

    cursor++;
  }

  return cursor < m_tokens_ptr.size();
}

bool SourceParser::IsConditionalExpression() {
  int tmp_cursor = m_cursor;

  // check for valid start to conditional expression
  if (m_tokens_ptr[tmp_cursor++]->GetType() != TokenType::OPENED_PARENTHESIS) {
    return false;
  }

  // check left operand is valid
  if (!IsConditionalOperand(tmp_cursor)) {
    return false;
  }

  // check if boolean operator is valid
  if (m_tokens_ptr[tmp_cursor]->GetType() != TokenType::AND && m_tokens_ptr[tmp_cursor]->GetType() != TokenType::OR) {
    return false;
  }

  // check right operand is valid
  return IsConditionalOperand(tmp_cursor);
}

std::shared_ptr<ProgramNode> SourceParser::ParseProgram() {
  std::vector<std::shared_ptr<ProcedureNode>> procedures;
  while (!AreTokensProcessed()) {
    procedures.push_back(ParseProcedure());
  }
  if (procedures.size() == 0) {
    throw EmptyStatementListException();
  }
  return std::make_shared<ProgramNode>(procedures);
}

std::shared_ptr<ProcedureNode> SourceParser::ParseProcedure() {
  ProcessToken(TokenType::PROCEDURE);
  std::shared_ptr<SourceToken> identifier = ProcessToken(TokenType::NAME);
  ProcessToken(TokenType::OPENED_BRACES);
  std::shared_ptr<StatementListNode> stmt_list = ParseStatementList();
  ProcessToken(TokenType::CLOSED_BRACES);
  std::string procedure_name = identifier->GetValue();
  return std::make_shared<ProcedureNode>(procedure_name, stmt_list);
}

std::shared_ptr<StatementListNode> SourceParser::ParseStatementList() {
  std::vector<std::shared_ptr<StatementNode>> stmt_list;
  while (FetchCurrentToken()->GetType() != TokenType::CLOSED_BRACES) {
    stmt_list.push_back(ParseStatement());
  }
  if (stmt_list.size() == 0) {
    throw EmptyStatementListException();
  }
  return std::make_shared<StatementListNode>(stmt_list);
}

std::shared_ptr<StatementNode> SourceParser::ParseStatement() {
  // case 1: assignment by predicting if next token is '='
  if (FetchToken(1)->GetType() == TokenType::EQUAL) {
    return ParseAssignStatement();
  }
  // case 2: handle other type of supported statements
  std::shared_ptr<SourceToken> token_ptr = FetchCurrentToken();
  switch (token_ptr->GetType()) {
    case TokenType::READ:return ParseReadStatement();
    case TokenType::PRINT:return ParsePrintStatement();
    case TokenType::WHILE:return ParseWhileStatement();
    case TokenType::IF:return ParseIfStatement();
    default:throw InvalidParseStatementException();
  }
}

std::shared_ptr<ReadStatementNode> SourceParser::ParseReadStatement() {
  int stmt_no = ++m_curr_stmt_no;
  ProcessToken(TokenType::READ);
  std::shared_ptr<SourceToken> identifier = ProcessToken(TokenType::NAME);
  ProcessToken(TokenType::SEMI_COLON);
  std::shared_ptr<VariableNode> variable = std::make_shared<VariableNode>(identifier->GetValue());
  return std::make_shared<ReadStatementNode>(stmt_no, variable);
}

std::shared_ptr<PrintStatementNode> SourceParser::ParsePrintStatement() {
  int stmt_no = ++m_curr_stmt_no;
  ProcessToken(TokenType::PRINT);
  std::shared_ptr<SourceToken> identifier = ProcessToken(TokenType::NAME);
  ProcessToken(TokenType::SEMI_COLON);
  std::shared_ptr<VariableNode> variable = std::make_shared<VariableNode>(identifier->GetValue());
  return std::make_shared<PrintStatementNode>(stmt_no, variable);
}

std::shared_ptr<WhileStatementNode> SourceParser::ParseWhileStatement() {
  int stmt_no = ++m_curr_stmt_no;
  ProcessToken(TokenType::WHILE);
  ProcessToken(TokenType::OPENED_PARENTHESIS);
  std::shared_ptr<ConditionalExpressionNode> condition = ParseConditionalExpression();
  ProcessToken(TokenType::CLOSED_PARENTHESIS);
  ProcessToken(TokenType::OPENED_BRACES);
  std::shared_ptr<StatementListNode> stmt_list = ParseStatementList();
  ProcessToken(TokenType::CLOSED_BRACES);
  return std::make_shared<WhileStatementNode>(stmt_no, condition, stmt_list);
}

std::shared_ptr<IfStatementNode> SourceParser::ParseIfStatement() {
  int stmt_no = ++m_curr_stmt_no;
  ProcessToken(TokenType::IF);
  ProcessToken(TokenType::OPENED_PARENTHESIS);
  std::shared_ptr<ConditionalExpressionNode> condition = ParseConditionalExpression();
  ProcessToken(TokenType::CLOSED_PARENTHESIS);
  ProcessToken(TokenType::THEN);
  ProcessToken(TokenType::OPENED_BRACES);
  std::shared_ptr<StatementListNode> if_stmt_list = ParseStatementList();
  ProcessToken(TokenType::CLOSED_BRACES);
  ProcessToken(TokenType::ELSE);
  ProcessToken(TokenType::OPENED_BRACES);
  std::shared_ptr<StatementListNode> else_stmt_list = ParseStatementList();
  ProcessToken(TokenType::CLOSED_BRACES);
  return std::make_shared<IfStatementNode>(stmt_no, condition, if_stmt_list, else_stmt_list);
}

std::shared_ptr<AssignStatementNode> SourceParser::ParseAssignStatement() {
  int stmt_no = ++m_curr_stmt_no;
  std::shared_ptr<SourceToken> identifier = ProcessToken(TokenType::NAME);
  ProcessToken(TokenType::EQUAL);
  std::shared_ptr<ExpressionNode> expression = ParseExpression();
  ProcessToken(TokenType::SEMI_COLON);
  std::shared_ptr<VariableNode> variable = std::make_shared<VariableNode>(identifier->GetValue());
  return std::make_shared<AssignStatementNode>(stmt_no, variable, expression);
}

std::shared_ptr<ConditionalExpressionNode> SourceParser::ParseConditionalExpression() {
  TokenType type = FetchCurrentToken()->GetType();
  if (type == TokenType::NOT) {
    ProcessToken(TokenType::NOT);
    ProcessToken(TokenType::OPENED_PARENTHESIS);
    std::shared_ptr<ConditionalExpressionNode> expression = ParseConditionalExpression();
    ProcessToken(TokenType::CLOSED_PARENTHESIS);
    return std::make_shared<NotExpressionNode>(expression);
  } else if (IsConditionalExpression()) {
    ProcessToken(TokenType::OPENED_PARENTHESIS);
    std::shared_ptr<ConditionalExpressionNode> left_expression = ParseConditionalExpression();
    ProcessToken(TokenType::CLOSED_PARENTHESIS);
    BooleanOperator boolean_operator;
    switch (FetchCurrentToken()->GetType()) {
      case TokenType::AND:ProcessToken(TokenType::AND);
        boolean_operator = BooleanOperator::AND;
        break;
      case TokenType::OR:ProcessToken(TokenType::OR);
        boolean_operator = BooleanOperator::OR;
        break;
      default:throw InvalidParseConditionException();
    }
    ProcessToken(TokenType::OPENED_PARENTHESIS);
    std::shared_ptr<ConditionalExpressionNode> right_expression = ParseConditionalExpression();
    ProcessToken(TokenType::CLOSED_PARENTHESIS);
    return std::make_shared<BooleanExpressionNode>(boolean_operator, left_expression, right_expression);
  } else if (type == TokenType::OPENED_PARENTHESIS || type == TokenType::NAME || type == TokenType::INTEGER) {
    // 'rel_expr' grammar can be reduced to 'factor'
    return ParseRelationalExpression();
  }
  throw InvalidParseConditionException();
}

std::shared_ptr<RelationalExpressionNode> SourceParser::ParseRelationalExpression() {
  std::shared_ptr<ExpressionNode> left_relation_factor = ParseRelationalFactor();
  RelationOperator relation_operator;
  switch (FetchCurrentToken()->GetType()) {
    case TokenType::IS_GREATER:ProcessToken(TokenType::IS_GREATER);
      relation_operator = RelationOperator::GREATER_THAN;
      break;
    case TokenType::IS_GREATER_EQUAL:ProcessToken(TokenType::IS_GREATER_EQUAL);
      relation_operator = RelationOperator::GREATER_THAN_EQUALS;
      break;
    case TokenType::IS_LESSER:ProcessToken(TokenType::IS_LESSER);
      relation_operator = RelationOperator::LESS_THAN;
      break;
    case TokenType::IS_LESSER_EQUAL:ProcessToken(TokenType::IS_LESSER_EQUAL);
      relation_operator = RelationOperator::LESS_THAN_EQUALS;
      break;
    case TokenType::IS_EQUAL:ProcessToken(TokenType::IS_EQUAL);
      relation_operator = RelationOperator::EQUALS;
      break;
    case TokenType::IS_NOT_EQUAL:ProcessToken(TokenType::IS_NOT_EQUAL);
      relation_operator = RelationOperator::NOT_EQUALS;
      break;
    default:throw InvalidParseRelationException();
  }
  std::shared_ptr<ExpressionNode> right_relation_factor = ParseRelationalFactor();
  return std::make_shared<RelationalExpressionNode>(relation_operator, left_relation_factor, right_relation_factor);
}

std::shared_ptr<ExpressionNode> SourceParser::ParseRelationalFactor() {
  // rel_factor -> expr -> term -> factor -> 'var_name'/'const_value'
  // thus not required to check for 'var_name' and 'const_value'
  // i.e. just evaluate expr
  return ParseExpression();
}

std::shared_ptr<ExpressionNode> SourceParser::ParseExpression() {
  // expr is essentially 'term (operator) term'
  std::shared_ptr<ExpressionNode> left_term = ParseTerm();
  return ParseExpression(left_term);
}

std::shared_ptr<ExpressionNode> SourceParser::ParseExpression(std::shared_ptr<ExpressionNode> left_term) {
  // recursively call and append '(add/minus) term' part of the expression
  ArithmeticOperator arithmetic_operator;
  switch (FetchCurrentToken()->GetType()) {
    case TokenType::ADDITION:ProcessToken(TokenType::ADDITION);
      arithmetic_operator = ArithmeticOperator::PLUS;
      break;
    case TokenType::SUBTRACTION:ProcessToken(TokenType::SUBTRACTION);
      arithmetic_operator = ArithmeticOperator::MINUS;
      break;
    default:return left_term;
  }
  std::shared_ptr<ExpressionNode> right_term = ParseTerm();
  return ParseExpression(std::make_shared<CombinationExpressionNode>(arithmetic_operator, left_term, right_term));
}

std::shared_ptr<ExpressionNode> SourceParser::ParseTerm() {
  // expr is essentially 'factor (multiply/divide/mod) factor'
  std::shared_ptr<ExpressionNode> left_factor = ParseFactor();
  return ParseTerm(left_factor);
}

std::shared_ptr<ExpressionNode> SourceParser::ParseTerm(std::shared_ptr<ExpressionNode> left_factor) {
  // recursively call and append '(operator) factor' part of the expression
  ArithmeticOperator arithmetic_operator;
  switch (FetchCurrentToken()->GetType()) {
    case TokenType::MULTIPLICATION:ProcessToken(TokenType::MULTIPLICATION);
      arithmetic_operator = ArithmeticOperator::MULTIPLY;
      break;
    case TokenType::DIVISION:ProcessToken(TokenType::DIVISION);
      arithmetic_operator = ArithmeticOperator::DIVIDE;
      break;
    case TokenType::MODULUS:ProcessToken(TokenType::MODULUS);
      arithmetic_operator = ArithmeticOperator::MOD;
      break;
    default:return left_factor;
  }
  std::shared_ptr<ExpressionNode> right_factor = ParseFactor();
  return ParseTerm(std::make_shared<CombinationExpressionNode>(arithmetic_operator, left_factor, right_factor));
}

std::shared_ptr<ExpressionNode> SourceParser::ParseFactor() {
  TokenType type = FetchCurrentToken()->GetType();
  switch (type) {
    case TokenType::NAME:return std::make_shared<VariableNode>(ProcessToken(TokenType::NAME)->GetValue());
    case TokenType::INTEGER:return std::make_shared<ConstantNode>(ProcessToken(TokenType::INTEGER)->GetValue());
    case TokenType::OPENED_PARENTHESIS: {
      ProcessToken(TokenType::OPENED_PARENTHESIS);
      std::shared_ptr<ExpressionNode> expression = ParseExpression();
      ProcessToken(TokenType::CLOSED_PARENTHESIS);
      return expression;
    }
    default:throw InvalidParseFactorException();
  }
}

}

#include "source_parser.h"
#include "components/source_subsystem/exceptions/cyclic_call.h"
#include "components/source_subsystem/exceptions/empty_statement_list.h"
#include "components/source_subsystem/exceptions/end_of_stream.h"
#include "components/source_subsystem/exceptions/invalid_call.h"
#include "components/source_subsystem/exceptions/invalid_expression_operator.h"
#include "components/source_subsystem/exceptions/invalid_parse_condition.h"
#include "components/source_subsystem/exceptions/invalid_parse_factor.h"
#include "components/source_subsystem/exceptions/invalid_parse_relation.h"
#include "components/source_subsystem/exceptions/invalid_parse_statement.h"
#include "components/source_subsystem/exceptions/invalid_term_operator.h"
#include "components/source_subsystem/exceptions/mismatch_token.h"
#include "components/source_subsystem/types/ast/node_assign_statement.h"
#include "components/source_subsystem/types/ast/node_boolean_expression.h"
#include "components/source_subsystem/types/ast/node_call_statement.h"
#include "components/source_subsystem/types/ast/node_combination_expression.h"
#include "components/source_subsystem/types/ast/node_constant.h"
#include "components/source_subsystem/types/ast/node_if_statement.h"
#include "components/source_subsystem/types/ast/node_not_expression.h"
#include "components/source_subsystem/types/ast/node_print_statement.h"
#include "components/source_subsystem/types/ast/node_procedure.h"
#include "components/source_subsystem/types/ast/node_program.h"
#include "components/source_subsystem/types/ast/node_read_statement.h"
#include "components/source_subsystem/types/ast/node_relational_expression.h"
#include "components/source_subsystem/types/ast/node_statement_list.h"
#include "components/source_subsystem/types/ast/node_variable.h"
#include "components/source_subsystem/types/ast/node_while_statement.h"
#include "components/source_subsystem/types/source_token/source_token.h"

namespace source {

SourceParser::SourceParser(TokenStream token_stream)
    : m_cursor(0), m_curr_stmt_no(0), m_token_stream(std::move(token_stream)), m_session(SourceParserSession()) {}

bool SourceParser::HasMoreTokens() {
  return m_cursor < m_token_stream.size();
}

TokenPtr SourceParser::FetchToken(int index) {
  // index relative from cursor
  int new_cursor_idx = m_cursor + index;
  if (new_cursor_idx >= m_token_stream.size()) {
    throw EndOfStreamException();
  }
  return m_token_stream[new_cursor_idx];
}

TokenPtr SourceParser::FetchCurrentToken() {
  return FetchToken(0);
}

TokenPtr SourceParser::ProcessToken(TokenType type) {
  TokenPtr token_ptr = FetchCurrentToken();
  if (token_ptr->GetType() != type) {
    throw MismatchedTokenException();
  }

  m_cursor += 1;
  return token_ptr;
}

bool SourceParser::IsConditionalOperand(int &cursor) {
  // check for valid conditional operand
  // by analysing its parenthesis
  int parenthesis_count = 0;
  while (cursor < m_token_stream.size()) {
    TokenType type = m_token_stream[cursor]->GetType();
    if (type == TokenType::OPENED_PARENTHESIS) {
      parenthesis_count++;
    } else if (type == TokenType::CLOSED_PARENTHESIS) {
      if (parenthesis_count > 0) {
        parenthesis_count--;
      } else if (parenthesis_count == 0) {
        cursor++;
        break;
      } else {
        return false;
      }
    }
    cursor++;
  }

  return cursor < m_token_stream.size();
}

bool SourceParser::IsConditionalExpression() {
  int tmp_cursor = m_cursor;

  // check for valid start of conditional expression
  if (m_token_stream[tmp_cursor++]->GetType() != TokenType::OPENED_PARENTHESIS) {
    return false;
  }

  // check left operand is valid
  if (!IsConditionalOperand(tmp_cursor)) {
    return false;
  }

  // check if boolean operator is valid
  if (m_token_stream[tmp_cursor]->GetType() != TokenType::AND
      && m_token_stream[tmp_cursor]->GetType() != TokenType::OR) {
    return false;
  }

  // check right operand is valid
  return IsConditionalOperand(tmp_cursor);
}

BooleanOperator SourceParser::GetBooleanOperator() {
  TokenType curr_type = FetchCurrentToken()->GetType();
  switch (curr_type) {
    case TokenType::AND: {
      ProcessToken(TokenType::AND);
      return BooleanOperator::AND;
    }
    case TokenType::OR: {
      ProcessToken(TokenType::OR);
      return BooleanOperator::OR;
    }
    default: {
      throw InvalidParseConditionException();
    }
  }
}

RelationOperator SourceParser::GetRelationOperator() {
  TokenType curr_type = FetchCurrentToken()->GetType();
  switch (curr_type) {
    case TokenType::IS_GREATER: {
      ProcessToken(TokenType::IS_GREATER);
      return RelationOperator::GREATER_THAN;
    }
    case TokenType::IS_GREATER_EQUAL: {
      ProcessToken(TokenType::IS_GREATER_EQUAL);
      return RelationOperator::GREATER_THAN_EQUALS;
    }
    case TokenType::IS_LESSER: {
      ProcessToken(TokenType::IS_LESSER);
      return RelationOperator::LESS_THAN;
    }
    case TokenType::IS_LESSER_EQUAL: {
      ProcessToken(TokenType::IS_LESSER_EQUAL);
      return RelationOperator::LESS_THAN_EQUALS;
    }
    case TokenType::IS_EQUAL: {
      ProcessToken(TokenType::IS_EQUAL);
      return RelationOperator::EQUALS;
    }
    case TokenType::IS_NOT_EQUAL: {
      ProcessToken(TokenType::IS_NOT_EQUAL);
      return RelationOperator::NOT_EQUALS;
    }
    default: {
      throw InvalidParseRelationException();
    }
  }
}

ArithmeticOperator SourceParser::GetExpressionOperator() {
  // subset of entire arithmetic operator values
  // check for only addition and subtraction
  TokenType curr_type = FetchCurrentToken()->GetType();
  switch (curr_type) {
    case TokenType::ADDITION: {
      ProcessToken(TokenType::ADDITION);
      return ArithmeticOperator::PLUS;
    }
    case TokenType::SUBTRACTION: {
      ProcessToken(TokenType::SUBTRACTION);
      return ArithmeticOperator::MINUS;
    }
    default: {
      throw InvalidExpressionOperatorException();
    }
  }
}

ArithmeticOperator SourceParser::GetTermOperator() {
  // subset of entire arithmetic operator values
  // check for only multiplication, division and modulus
  TokenType curr_type = FetchCurrentToken()->GetType();
  switch (curr_type) {
    case TokenType::MULTIPLICATION: {
      ProcessToken(TokenType::MULTIPLICATION);
      return ArithmeticOperator::MULTIPLY;
    }
    case TokenType::DIVISION: {
      ProcessToken(TokenType::DIVISION);
      return ArithmeticOperator::DIVIDE;
    }
    case TokenType::MODULUS: {
      ProcessToken(TokenType::MODULUS);
      return ArithmeticOperator::MOD;
    }
    default: {
      throw InvalidTermOperatorException();
    }
  }
}

ProgramNodePtr SourceParser::ParseProgram() {
  ProcedureNodeStream procedure_node_stream;

  // recursive decent parsing
  while (HasMoreTokens()) {
    procedure_node_stream.push_back(ParseProcedure());
  }

  // validate parsed program
  if (procedure_node_stream.empty()) {
    throw EmptyStatementListException();
  }
  if (m_session.DoesInvalidCallExist()) {
    throw InvalidCallException();
  }
  if (m_session.DoesCyclicCallExist()) {
    throw CyclicCallException();
  }

  return std::make_shared<ProgramNode>(procedure_node_stream);
}

ProcedureNodePtr SourceParser::ParseProcedure() {
  ProcessToken(TokenType::PROCEDURE);
  String proc_name = ProcessToken(TokenType::NAME)->GetValue();

  // add procedure name into parser session
  // to check for any procedure name duplication
  m_session.AddProcedure(proc_name);

  ProcessToken(TokenType::OPENED_BRACES);
  StatementListNodePtr stmt_list = ParseStatementList();
  ProcessToken(TokenType::CLOSED_BRACES);

  return std::make_shared<ProcedureNode>(proc_name, stmt_list);
}

StatementListNodePtr SourceParser::ParseStatementList() {
  StatementNodeStream stmt_node_stream;

  // add all statements into a statement stream
  while (FetchCurrentToken()->GetType() != TokenType::CLOSED_BRACES) {
    stmt_node_stream.push_back(ParseStatement());
  }

  // validate statement stream
  if (stmt_node_stream.empty()) {
    throw EmptyStatementListException();
  }

  return std::make_shared<StatementListNode>(stmt_node_stream);
}

StatementNodePtr SourceParser::ParseStatement() {
  TokenPtr curr_token = FetchCurrentToken();
  TokenPtr next_token = FetchToken(1);
  TokenType curr_token_type = curr_token->GetType();
  TokenType next_token_type = next_token->GetType();

  // case 1: check if it is an assign statement
  //         by predicting if next token is '='
  if (next_token_type == TokenType::EQUAL) {
    return ParseAssignStatement();
  }

  // case 2: handle any other type of supported statements
  switch (curr_token_type) {
    case TokenType::READ: {
      return ParseReadStatement();
    }
    case TokenType::PRINT: {
      return ParsePrintStatement();
    }
    case TokenType::WHILE: {
      return ParseWhileStatement();
    }
    case TokenType::IF: {
      return ParseIfStatement();
    }
    case TokenType::CALL: {
      return ParseCallStatement();
    }
    default: {
      throw InvalidParseStatementException();
    }
  }
}

ReadStatementNodePtr SourceParser::ParseReadStatement() {
  String stmt_no = std::to_string(++m_curr_stmt_no);
  ProcessToken(TokenType::READ);
  String var_name = ProcessToken(TokenType::NAME)->GetValue();
  ProcessToken(TokenType::SEMI_COLON);

  VariableNodePtr variable_node = std::make_shared<VariableNode>(var_name);
  return std::make_shared<ReadStatementNode>(stmt_no, variable_node);
}

PrintStatementNodePtr SourceParser::ParsePrintStatement() {
  String stmt_no = std::to_string(++m_curr_stmt_no);
  ProcessToken(TokenType::PRINT);
  String var_name = ProcessToken(TokenType::NAME)->GetValue();
  ProcessToken(TokenType::SEMI_COLON);

  VariableNodePtr variable_node = std::make_shared<VariableNode>(var_name);
  return std::make_shared<PrintStatementNode>(stmt_no, variable_node);
}

AssignStatementNodePtr SourceParser::ParseAssignStatement() {
  String stmt_no = std::to_string(++m_curr_stmt_no);
  String lhs_var_name = ProcessToken(TokenType::NAME)->GetValue();
  ProcessToken(TokenType::EQUAL);
  ExpressionNodePtr rhs_expression_node = ParseExpression();
  ProcessToken(TokenType::SEMI_COLON);

  VariableNodePtr lhs_variable_node = std::make_shared<VariableNode>(lhs_var_name);
  return std::make_shared<AssignStatementNode>(stmt_no, lhs_variable_node, rhs_expression_node);
}

CallStatementNodePtr SourceParser::ParseCallStatement() {
  String stmt_no = std::to_string(++m_curr_stmt_no);
  ProcessToken(TokenType::CALL);
  String callee_name = ProcessToken(TokenType::NAME)->GetValue();

  // add callee name into parser session
  // to check for cyclic calls
  m_session.AddMethodCall(callee_name);

  ProcessToken(TokenType::SEMI_COLON);

  String proc_name = m_session.GetCurrProcedure();
  return std::make_shared<CallStatementNode>(stmt_no, proc_name, callee_name);
}

WhileStatementNodePtr SourceParser::ParseWhileStatement() {
  String stmt_no = std::to_string(++m_curr_stmt_no);
  ProcessToken(TokenType::WHILE);
  ProcessToken(TokenType::OPENED_PARENTHESIS);
  ConditionalExpressionNodePtr condition_node = ParseConditionalExpression();
  ProcessToken(TokenType::CLOSED_PARENTHESIS);
  ProcessToken(TokenType::OPENED_BRACES);
  StatementListNodePtr stmt_list_node = ParseStatementList();
  ProcessToken(TokenType::CLOSED_BRACES);

  return std::make_shared<WhileStatementNode>(stmt_no, condition_node, stmt_list_node);
}

IfStatementNodePtr SourceParser::ParseIfStatement() {
  String stmt_no = std::to_string(++m_curr_stmt_no);
  ProcessToken(TokenType::IF);
  ProcessToken(TokenType::OPENED_PARENTHESIS);
  ConditionalExpressionNodePtr condition_node = ParseConditionalExpression();
  ProcessToken(TokenType::CLOSED_PARENTHESIS);
  ProcessToken(TokenType::THEN);
  ProcessToken(TokenType::OPENED_BRACES);
  StatementListNodePtr if_stmt_list_node = ParseStatementList();
  ProcessToken(TokenType::CLOSED_BRACES);
  ProcessToken(TokenType::ELSE);
  ProcessToken(TokenType::OPENED_BRACES);
  StatementListNodePtr else_stmt_list_node = ParseStatementList();
  ProcessToken(TokenType::CLOSED_BRACES);

  return std::make_shared<IfStatementNode>(stmt_no, condition_node, if_stmt_list_node, else_stmt_list_node);
}

ConditionalExpressionNodePtr SourceParser::ParseConditionalExpression() {
  TokenType curr_type = FetchCurrentToken()->GetType();
  if (curr_type == TokenType::NOT) {
    return ParseNotExpressionNode();
  } else if (IsConditionalExpression()) {
    return ParseBooleanExpressionNode();
  } else if (curr_type == TokenType::NAME || curr_type == TokenType::INTEGER
      || curr_type == TokenType::OPENED_PARENTHESIS) {
    // 'rel_expr' grammar can be reduced to 'rel_factor'
    // therefore just need to check if the starting token matches
    // the starting token of 'var_name', 'const_value' or 'expr'
    return ParseRelationalExpression();
  }

  throw InvalidParseConditionException();
}

BooleanExpressionNodePtr SourceParser::ParseBooleanExpressionNode() {
  ProcessToken(TokenType::OPENED_PARENTHESIS);
  ConditionalExpressionNodePtr lhs_expression_node = ParseConditionalExpression();
  ProcessToken(TokenType::CLOSED_PARENTHESIS);
  BooleanOperator boolean_operator = GetBooleanOperator();
  ProcessToken(TokenType::OPENED_PARENTHESIS);
  ConditionalExpressionNodePtr rhs_expression = ParseConditionalExpression();
  ProcessToken(TokenType::CLOSED_PARENTHESIS);

  return std::make_shared<BooleanExpressionNode>(boolean_operator, lhs_expression_node, rhs_expression);
}

RelationalExpressionNodePtr SourceParser::ParseRelationalExpression() {
  ExpressionNodePtr lhs_rel_factor_node = ParseRelationalFactor();
  RelationOperator relation_operator = GetRelationOperator();
  ExpressionNodePtr rhs_rel_factor_node = ParseRelationalFactor();

  return std::make_shared<RelationalExpressionNode>(relation_operator, lhs_rel_factor_node, rhs_rel_factor_node);
}

NotExpressionNodePtr SourceParser::ParseNotExpressionNode() {
  ProcessToken(TokenType::NOT);
  ProcessToken(TokenType::OPENED_PARENTHESIS);
  ConditionalExpressionNodePtr conditional_expression_node = ParseConditionalExpression();
  ProcessToken(TokenType::CLOSED_PARENTHESIS);

  return std::make_shared<NotExpressionNode>(conditional_expression_node);
}

ExpressionNodePtr SourceParser::ParseRelationalFactor() {
  // rel_factor -> expr -> term -> factor -> 'var_name'/'const_value'
  // thus not required to check for 'var_name' and 'const_value'
  // i.e. just evaluate expression right away
  return ParseExpression();
}

ExpressionNodePtr SourceParser::ParseExpression() {
  // expr is essentially 'term (operator) term'
  ExpressionNodePtr left_term_node = ParseTerm();
  return ParseExpression(left_term_node);
}

ExpressionNodePtr SourceParser::ParseExpression(ExpressionNodePtr left_term_node) {
  // recursively call and append '(add/minus) term' part of the expression
  ArithmeticOperator arithmetic_operator;
  try {
    arithmetic_operator = GetExpressionOperator();
  } catch (const InvalidExpressionOperatorException &invalid_expr_operator_exception) {
    // when an exception is caught, it means no
    // operator was found and also means no right term
    return left_term_node;
  }
  ExpressionNodePtr right_term_node = ParseTerm();

  return ParseExpression(std::make_shared<CombinationExpressionNode>(arithmetic_operator,
                                                                     left_term_node,
                                                                     right_term_node));
}

ExpressionNodePtr SourceParser::ParseTerm() {
  // term is essentially 'factor (multiply/divide/mod) factor'
  ExpressionNodePtr left_factor_node = ParseFactor();
  return ParseTerm(left_factor_node);
}

ExpressionNodePtr SourceParser::ParseTerm(ExpressionNodePtr left_factor_node) {
  // recursively call and append '(operator) factor' part of the expression
  ArithmeticOperator arithmetic_operator;
  try {
    arithmetic_operator = GetTermOperator();
  } catch (const InvalidTermOperatorException &invalid_term_operator_exception) {
    // when an exception is caught, it means no
    // operator was found and also means no right factor
    return left_factor_node;
  }
  ExpressionNodePtr right_factor_node = ParseFactor();

  return ParseTerm(std::make_shared<CombinationExpressionNode>(arithmetic_operator,
                                                               left_factor_node,
                                                               right_factor_node));
}

ExpressionNodePtr SourceParser::ParseFactor() {
  TokenType type = FetchCurrentToken()->GetType();
  switch (type) {
    case TokenType::NAME: {
      String var_name = ProcessToken(TokenType::NAME)->GetValue();
      return std::make_shared<VariableNode>(var_name);
    }
    case TokenType::INTEGER: {
      String constant_value = ProcessToken(TokenType::INTEGER)->GetValue();
      return std::make_shared<ConstantNode>(constant_value);
    }
    case TokenType::OPENED_PARENTHESIS: {
      ProcessToken(TokenType::OPENED_PARENTHESIS);
      ExpressionNodePtr expression = ParseExpression();
      ProcessToken(TokenType::CLOSED_PARENTHESIS);
      return expression;
    }
    default: {
      throw InvalidParseFactorException();
    }
  }
}

}

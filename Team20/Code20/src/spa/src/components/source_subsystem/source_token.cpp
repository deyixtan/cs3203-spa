#include "source_token.h"

namespace source {

SourceToken::SourceToken(TokenType type, std::string value) : m_type(type), m_value(std::move(value)) {}

TokenType SourceToken::GetType() {
  return m_type;
}

std::string SourceToken::GetValue() {
  return m_value;
}

std::string SourceToken::ToString() {
  std::string type_str;
  switch (m_type) {
    case TokenType::WHITE_SPACE:type_str = "WHITE_SPACE";
      break;
    case TokenType::INTEGER:type_str = "INTEGER";
      break;
    case TokenType::NAME:type_str = "NAME";
      break;
    case TokenType::OPENED_BRACES:type_str = "OPENED_BRACES";
      break;
    case TokenType::CLOSED_BRACES:type_str = "CLOSED_BRACES";
      break;
    case TokenType::OPENED_PARENTHESIS:type_str = "OPENED_PARENTHESIS";
      break;
    case TokenType::CLOSED_PARENTHESIS:type_str = "CLOSED_PARENTHESIS";
      break;
    case TokenType::AND:type_str = "AND";
      break;
    case TokenType::OR:type_str = "OR";
      break;
    case TokenType::NOT:type_str = "NOT";
      break;
    case TokenType::IS_GREATER:type_str = "IS_GREATER";
      break;
    case TokenType::IS_GREATER_EQUAL:type_str = "IS_GREATER_EQUAL";
      break;
    case TokenType::IS_LESSER:type_str = "IS_LESSER";
      break;
    case TokenType::IS_LESSER_EQUAL:type_str = "IS_LESSER_EQUAL";
      break;
    case TokenType::IS_EQUAL:type_str = "IS_EQUAL";
      break;
    case TokenType::IS_NOT_EQUAL:type_str = "IS_NOT_EQUAL";
      break;
    case TokenType::ADDITION:type_str = "ADDITION";
      break;
    case TokenType::SUBTRACTION:type_str = "SUBTRACTION";
      break;
    case TokenType::MULTIPLICATION:type_str = "MULTIPLICATION";
      break;
    case TokenType::DIVISION:type_str = "DIVISION";
      break;
    case TokenType::MODULUS:type_str = "MODULUS";
      break;
    case TokenType::EQUAL:type_str = "EQUAL";
      break;
    case TokenType::SEMI_COLON:type_str = "SEMI_COLON";
      break;
    case TokenType::READ:type_str = "READ";
      break;
    case TokenType::PRINT:type_str = "PRINT";
      break;
    case TokenType::WHILE:type_str = "WHILE";
      break;
    case TokenType::IF:type_str = "IF";
      break;
    case TokenType::THEN:type_str = "THEN";
      break;
    case TokenType::ELSE:type_str = "ELSE";
      break;
    case TokenType::PROCEDURE:type_str = "PROCEDURE";
      break;
    default:type_str = "UNKNOWN";
  }
  return "{ type: " + type_str + ", value: " + m_value + "}";
}

}

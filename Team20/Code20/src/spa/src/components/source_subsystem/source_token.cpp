#include "source_token.h"

SourceToken::SourceToken(SourceTokenType type, std::string value) {
  this->type = type;
  this->value = value;
}

SourceTokenType SourceToken::GetType() {
  return type;
}

std::string SourceToken::GetTypeStr() {
  std::string type_str;
  switch (type) {
    case SourceTokenType::UNKNOWN:type_str = "UNKNOWN";
      break;
    case SourceTokenType::NEW_LINE:type_str = "NEW_LINE";
      break;
    case SourceTokenType::WHITE_SPACE:type_str = "WHITE_SPACE";
      break;
    case SourceTokenType::DIGIT:type_str = "DIGIT";
      break;
    case SourceTokenType::NAME:type_str = "NAME";
      break;
    case SourceTokenType::OPENED_BRACES:type_str = "OPENED_BRACES";
      break;
    case SourceTokenType::CLOSED_BRACES:type_str = "CLOSED_BRACES";
      break;
    case SourceTokenType::OPENED_PARENTHESIS:type_str = "OPENED_PARENTHESIS";
      break;
    case SourceTokenType::CLOSED_PARENTHESIS:type_str = "CLOSED_PARENTHESIS";
      break;
    case SourceTokenType::AND:type_str = "AND";
      break;
    case SourceTokenType::OR:type_str = "OR";
      break;
    case SourceTokenType::IS_GREATER_EQUAL:type_str = "IS_GREATER_EQUAL";
      break;
    case SourceTokenType::IS_GREATER:type_str = "IS_GREATER";
      break;
    case SourceTokenType::IS_LESSER_EQUAL:type_str = "IS_LESSER_EQUAL";
      break;
    case SourceTokenType::IS_LESSER:type_str = "IS_LESSER";
      break;
    case SourceTokenType::IS_EQUAL:type_str = "IS_EQUAL";
      break;
    case SourceTokenType::IS_NOT_EQUAL:type_str = "IS_NOT_EQUAL";
      break;
    case SourceTokenType::ADDITION:type_str = "ADDITION";
      break;
    case SourceTokenType::SUBTRACTION:type_str = "SUBTRACTION";
      break;
    case SourceTokenType::MULTIPLICATION:type_str = "MULTIPLICATION";
      break;
    case SourceTokenType::DIVISION:type_str = "DIVISION";
      break;
    case SourceTokenType::MODULUS:type_str = "MODULUS";
      break;
    case SourceTokenType::SEMI_COLON:type_str = "SEMI_COLON";
      break;
    case SourceTokenType::EQUAL:type_str = "EQUAL";
      break;
    case SourceTokenType::NOT:type_str = "NOT";
      break;
    case SourceTokenType::PROCEDURE:type_str = "PROCEDURE";
      break;
    case SourceTokenType::READ:type_str = "READ";
      break;
    case SourceTokenType::PRINT:type_str = "PRINT";
      break;
    case SourceTokenType::WHILE:type_str = "WHILE";
      break;
    case SourceTokenType::IF:type_str = "IF";
      break;
    case SourceTokenType::THEN:type_str = "THEN";
      break;
    case SourceTokenType::ELSE:type_str = "ELSE";
      break;
  }
  return type_str;
}

std::string SourceToken::GetValue() {
  return value;
}

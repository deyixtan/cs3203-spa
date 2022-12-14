#include "source_token.h"

namespace source {

SourceToken::SourceToken() : SourceToken(TokenType::UNKNOWN, "") {}

SourceToken::SourceToken(TokenType type, String value) : m_type(type), m_value(std::move(value)) {}

TokenType SourceToken::GetType() {
  return m_type;
}

String SourceToken::GetValue() {
  return m_value;
}

}

#include "source_token.h"

namespace source {

SourceToken::SourceToken(TokenType type, std::string value) : m_type(type), m_value(std::move(value)) {}

TokenType SourceToken::GetType() {
  return m_type;
}

std::string SourceToken::GetValue() {
  return m_value;
}

}

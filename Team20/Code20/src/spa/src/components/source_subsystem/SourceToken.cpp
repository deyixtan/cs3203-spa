#include "SourceToken.h"

SourceToken::SourceToken(TokenType type, std::string value) {
    this->type = type;
    this->value = value;
}

TokenType SourceToken::GetType() {
    return type;
}

std::string SourceToken::GetValue() {
    return value;
}
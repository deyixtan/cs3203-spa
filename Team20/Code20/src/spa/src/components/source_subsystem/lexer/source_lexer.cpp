#include "source_lexer.h"
#include "components/source_subsystem/exceptions/unexpected_token.h"
#include "components/source_subsystem/types/source_token/source_token.h"

namespace source {

SourceLexer::SourceLexer(std::string source) : m_cursor(0), m_source(std::move(source)) {}

void SourceLexer::Tokenize(TokenStream &token_stream) {
  while (HasMoreTokens()) {
    TokenPtr token = GetNextToken();
    token_stream.push_back(token);
  }
  SanitizeTokenStream(token_stream);
  EncodeTokenStream(token_stream);
}

bool SourceLexer::HasMoreTokens() {
  return m_cursor < m_source.length();
}

TokenPtr SourceLexer::GetNextToken() {
  TokenPtr token = std::make_shared<SourceToken>();
  std::string remaining_source = m_source.substr(m_cursor);
  char curr_char = remaining_source.at(0);

  if (remaining_source.length() > 1) {
    char next_char = remaining_source.at(1);
    TwoCharsTokenHandler(token, curr_char, next_char);
    if (token->GetType() != TokenType::UNKNOWN) {
      return token;
    }
  }

  OneCharTokenHandler(token, curr_char);
  if (token->GetType() != TokenType::UNKNOWN) {
    return token;
  }

  MultipleCharsTokenHandler(token, curr_char);
  if (token->GetType() != TokenType::UNKNOWN) {
    return token;
  }

  throw UnexpectedTokenException();
}

void SourceLexer::TwoCharsTokenHandler(TokenPtr &token, char curr_char, char next_char) {
  // attempt to create two chars token
  if (curr_char == '&' && next_char == '&') {
    token = std::make_shared<SourceToken>(TokenType::AND, "");
  } else if (curr_char == '|' && next_char == '|') {
    token = std::make_shared<SourceToken>(TokenType::OR, "");
  } else if (curr_char == '=' && next_char == '=') {
    token = std::make_shared<SourceToken>(TokenType::IS_EQUAL, "");
  } else if (curr_char == '!' && next_char == '=') {
    token = std::make_shared<SourceToken>(TokenType::IS_NOT_EQUAL, "");
  } else if (curr_char == '>' && next_char == '=') {
    token = std::make_shared<SourceToken>(TokenType::IS_GREATER_EQUAL, "");
  } else if (curr_char == '<' && next_char == '=') {
    token = std::make_shared<SourceToken>(TokenType::IS_LESSER_EQUAL, "");
  }

  // check if token is modified
  if (token->GetType() == TokenType::UNKNOWN) {
    return;
  }
  m_cursor += 2;
}

void SourceLexer::OneCharTokenHandler(TokenPtr &token, char curr_char) {
  if (curr_char == ' ') {
    token = std::make_shared<SourceToken>(TokenType::WHITE_SPACE, "");
  } else if (curr_char == '\n') {
    token = std::make_shared<SourceToken>(TokenType::NEW_LINE, "");
  } if (curr_char == '\t') {
    token = std::make_shared<SourceToken>(TokenType::TAB, "");
  } else if (curr_char == '{') {
    token = std::make_shared<SourceToken>(TokenType::OPENED_BRACES, "");
  } else if (curr_char == '}') {
    token = std::make_shared<SourceToken>(TokenType::CLOSED_BRACES, "");
  } else if (curr_char == '(') {
    token = std::make_shared<SourceToken>(TokenType::OPENED_PARENTHESIS, "");
  } else if (curr_char == ')') {
    token = std::make_shared<SourceToken>(TokenType::CLOSED_PARENTHESIS, "");
  } else if (curr_char == '!') {
    token = std::make_shared<SourceToken>(TokenType::NOT, "");
  } else if (curr_char == '>') {
    token = std::make_shared<SourceToken>(TokenType::IS_GREATER, "");
  } else if (curr_char == '<') {
    token = std::make_shared<SourceToken>(TokenType::IS_LESSER, "");
  } else if (curr_char == '+') {
    token = std::make_shared<SourceToken>(TokenType::ADDITION, "");
  } else if (curr_char == '-') {
    token = std::make_shared<SourceToken>(TokenType::SUBTRACTION, "");
  } else if (curr_char == '*') {
    token = std::make_shared<SourceToken>(TokenType::MULTIPLICATION, "");
  } else if (curr_char == '/') {
    token = std::make_shared<SourceToken>(TokenType::DIVISION, "");
  } else if (curr_char == '%') {
    token = std::make_shared<SourceToken>(TokenType::MODULUS, "");
  } else if (curr_char == '=') {
    token = std::make_shared<SourceToken>(TokenType::EQUAL, "");
  } else if (curr_char == ';') {
    token = std::make_shared<SourceToken>(TokenType::SEMI_COLON, "");
  }

  // check if token is modified
  if (token->GetType() == TokenType::UNKNOWN) {
    return;
  }
  m_cursor += 1;
}

void SourceLexer::MultipleCharsTokenHandler(TokenPtr &token, char curr_char) {
  if (std::isdigit(curr_char) != 0) {
    IntegerTokenHandler(token, curr_char);
  } else if (std::isalpha(curr_char) != 0) {
    NameTokenHandler(token, curr_char);
  }
}

void SourceLexer::IntegerTokenHandler(TokenPtr &token, char curr_char) {
  std::string remaining_source;
  std::string value;
  while (std::isdigit(curr_char) != 0) {
    value += curr_char;
    remaining_source = m_source.substr(++m_cursor);

    if (!HasMoreTokens()) {
      break;
    }
    curr_char = remaining_source.at(0);
  }
  token = std::make_shared<SourceToken>(TokenType::INTEGER, value);
}

void SourceLexer::NameTokenHandler(TokenPtr &token, char curr_char) {
  std::string remaining_source;
  std::string value;

  value += curr_char;
  remaining_source = m_source.substr(++m_cursor);

  if (!HasMoreTokens()) {
    token = std::make_shared<SourceToken>(TokenType::NAME, value);
    return;
  }
  curr_char = remaining_source.at(0);

  while (std::isalnum(curr_char) != 0) {
    value += curr_char;
    remaining_source = m_source.substr(++m_cursor);

    if (!HasMoreTokens()) {
      break;
    }
    curr_char = remaining_source.at(0);
  }
  token = std::make_shared<SourceToken>(TokenType::NAME, value);
}

void SourceLexer::SanitizeTokenStream(TokenStream &token_stream) {
  for (auto it = token_stream.begin(); it != token_stream.end(); it++) {
    TokenType type = (*it)->GetType();

    if (type != TokenType::WHITE_SPACE && type != TokenType::NEW_LINE && type != TokenType::TAB) {
      continue;
    }

    token_stream.erase(it--);
  }
}

void SourceLexer::EncodeTokenStream(TokenStream &token_stream) {
  for (auto it = token_stream.begin(); it != token_stream.end(); it++) {
    if ((*it)->GetType() != TokenType::NAME) {
      continue;
    }

    // check iterator does not exceed last
    // second index of token stream
    if (it + 1 >= token_stream.end()) {
      break;
    }

    EncodeTokenHandler(*it, *(it + 1));
  }
}

void SourceLexer::EncodeTokenHandler(TokenPtr &token, TokenPtr &next_token) {
  // attempt to modify (re-encode) current token
  TokenType next_type = next_token->GetType();
  std::string value = token->GetValue();

  if (value == "read" && next_type == TokenType::NAME) {
    token = std::make_shared<SourceToken>(TokenType::READ, "");
  } else if (value == "print" && next_type == TokenType::NAME) {
    token = std::make_shared<SourceToken>(TokenType::PRINT, "");
  } else if (value == "while" && next_type == TokenType::OPENED_PARENTHESIS) {
    token = std::make_shared<SourceToken>(TokenType::WHILE, "");
  } else if (value == "if" && next_type == TokenType::OPENED_PARENTHESIS) {
    token = std::make_shared<SourceToken>(TokenType::IF, "");
  } else if (value == "then" && next_type == TokenType::OPENED_BRACES) {
    token = std::make_shared<SourceToken>(TokenType::THEN, "");
  } else if (value == "else" && next_type == TokenType::OPENED_BRACES) {
    token = std::make_shared<SourceToken>(TokenType::ELSE, "");
  } else if (value == "call" && next_type == TokenType::NAME) {
    token = std::make_shared<SourceToken>(TokenType::CALL, "");
  } else if (value == "procedure" && next_type == TokenType::NAME) {
    token = std::make_shared<SourceToken>(TokenType::PROCEDURE, "");
  }
}

}

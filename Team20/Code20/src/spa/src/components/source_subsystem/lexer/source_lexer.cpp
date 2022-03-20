#include "source_lexer.h"

namespace source {

SourceLexer::SourceLexer(std::string simple_source) : m_cursor(0), m_simple_source(std::move(simple_source)) {}

bool SourceLexer::HasMoreTokens() {
  return m_cursor < m_simple_source.length();
}

std::shared_ptr<SourceToken> SourceLexer::GetNextToken() {
  std::string remaining_source = m_simple_source.substr(m_cursor);
  char lookahead = remaining_source.at(0);
  if (remaining_source.length() > 1) {
    char lookahead2 = remaining_source.at(1);
    if (lookahead == '&' && lookahead2 == '&') {
      m_cursor += 2;
      return std::make_shared<SourceToken>(TokenType::AND, "");
    } else if (lookahead == '|' && lookahead2 == '|') {
      m_cursor += 2;
      return std::make_shared<SourceToken>(TokenType::OR, "");
    } else if (lookahead == '>' && lookahead2 == '=') {
      m_cursor += 2;
      return std::make_shared<SourceToken>(TokenType::IS_GREATER_EQUAL, "");
    } else if (lookahead == '<' && lookahead2 == '=') {
      m_cursor += 2;
      return std::make_shared<SourceToken>(TokenType::IS_LESSER_EQUAL, "");
    } else if (lookahead == '=' && lookahead2 == '=') {
      m_cursor += 2;
      return std::make_shared<SourceToken>(TokenType::IS_EQUAL, "");
    } else if (lookahead == '!' && lookahead2 == '=') {
      m_cursor += 2;
      return std::make_shared<SourceToken>(TokenType::IS_NOT_EQUAL, "");
    }
  }

  if (lookahead == '\n') {
    m_cursor += 1;
    return std::make_shared<SourceToken>(TokenType::NEW_LINE, "");
  } else if (lookahead == '{') {
    m_cursor += 1;
    return std::make_shared<SourceToken>(TokenType::OPENED_BRACES, "");
  } else if (lookahead == '}') {
    m_cursor += 1;
    return std::make_shared<SourceToken>(TokenType::CLOSED_BRACES, "");
  } else if (lookahead == '(') {
    m_cursor += 1;
    return std::make_shared<SourceToken>(TokenType::OPENED_PARENTHESIS, "");
  } else if (lookahead == ')') {
    m_cursor += 1;
    return std::make_shared<SourceToken>(TokenType::CLOSED_PARENTHESIS, "");
  } else if (lookahead == '>') {
    m_cursor += 1;
    return std::make_shared<SourceToken>(TokenType::IS_GREATER, "");
  } else if (lookahead == '<') {
    m_cursor += 1;
    return std::make_shared<SourceToken>(TokenType::IS_LESSER, "");
  } else if (lookahead == '+') {
    m_cursor += 1;
    return std::make_shared<SourceToken>(TokenType::ADDITION, "");
  } else if (lookahead == '-') {
    m_cursor += 1;
    return std::make_shared<SourceToken>(TokenType::SUBTRACTION, "");
  } else if (lookahead == '*') {
    m_cursor += 1;
    return std::make_shared<SourceToken>(TokenType::MULTIPLICATION, "");
  } else if (lookahead == '/') {
    m_cursor += 1;
    return std::make_shared<SourceToken>(TokenType::DIVISION, "");
  } else if (lookahead == '%') {
    m_cursor += 1;
    return std::make_shared<SourceToken>(TokenType::MODULUS, "");
  } else if (lookahead == ';') {
    m_cursor += 1;
    return std::make_shared<SourceToken>(TokenType::SEMI_COLON, "");
  } else if (lookahead == '=') {
    m_cursor += 1;
    return std::make_shared<SourceToken>(TokenType::EQUAL, "");
  } else if (lookahead == '!') {
    m_cursor += 1;
    return std::make_shared<SourceToken>(TokenType::NOT, "");
  } else if (lookahead == ' ') {
    m_cursor += 1;
    return std::make_shared<SourceToken>(TokenType::WHITE_SPACE, "");
  } else if (std::isdigit(lookahead) != 0) {
    std::string value = "";
    while (std::isdigit(lookahead) != 0) {
      value += lookahead;
      m_cursor++;
      remaining_source = m_simple_source.substr(m_cursor);
      if (m_cursor >= m_simple_source.length()) {
        break;
      }
      lookahead = remaining_source.at(0);
    }
    return std::make_shared<SourceToken>(TokenType::INTEGER, value);
  } else if (std::isalpha(lookahead) != 0) {
    std::string value = "";
    value += lookahead;
    m_cursor++;
    remaining_source = m_simple_source.substr(m_cursor);
    if (m_cursor >= m_simple_source.length()) {
      return std::make_shared<SourceToken>(TokenType::NAME, value);
    }
    lookahead = remaining_source.at(0);

    while (std::isalnum(lookahead) != 0) {
      value += lookahead;
      m_cursor++;
      remaining_source = m_simple_source.substr(m_cursor);
      if (m_cursor >= m_simple_source.length()) {
        break;
      }
      lookahead = remaining_source.at(0);
    }
    return std::make_shared<SourceToken>(TokenType::NAME, value);
  }

  throw UnexpectedTokenException();
}

void SourceLexer::RemoveWhiteSpaceTokens(std::vector<std::shared_ptr<SourceToken>> &tokens_ptr) {
  for (std::vector<std::shared_ptr<SourceToken>>::iterator it = tokens_ptr.begin(); it != tokens_ptr.end(); it++) {
    TokenType type = (*it)->GetType();
    if (type == TokenType::WHITE_SPACE || type == TokenType::NEW_LINE) {
      tokens_ptr.erase(it--);
    }
  }
}

void SourceLexer::TranslateKeywordTokens(std::vector<std::shared_ptr<SourceToken>> &tokens_ptr) {
  for (std::vector<std::shared_ptr<SourceToken>>::iterator it = tokens_ptr.begin(); it != tokens_ptr.end(); it++) {
    TokenType type = (*it)->GetType();
    std::string value = (*it)->GetValue();

    if (type != TokenType::NAME) {
      continue;
    }

    if (it + 1 < tokens_ptr.end()) {
      TokenType next_type = (*(it + 1))->GetType();

      if (value == "read" && next_type == TokenType::NAME) {
        *it = std::make_shared<SourceToken>(TokenType::READ, "");
      } else if (value == "print" && next_type == TokenType::NAME) {
        *it = std::make_shared<SourceToken>(TokenType::PRINT, "");
      } else if (value == "while" && next_type == TokenType::OPENED_PARENTHESIS) {
        *it = std::make_shared<SourceToken>(TokenType::WHILE, "");
      } else if (value == "if" && next_type == TokenType::OPENED_PARENTHESIS) {
        *it = std::make_shared<SourceToken>(TokenType::IF, "");
      } else if (value == "then" && next_type == TokenType::OPENED_BRACES) {
        *it = std::make_shared<SourceToken>(TokenType::THEN, "");
      } else if (value == "else" && next_type == TokenType::OPENED_BRACES) {
        *it = std::make_shared<SourceToken>(TokenType::ELSE, "");
      } else if (value == "call" && next_type == TokenType::NAME) {
        *it = std::make_shared<SourceToken>(TokenType::CALL, "");
      } else if (value == "procedure" && next_type == TokenType::NAME) {
        *it = std::make_shared<SourceToken>(TokenType::PROCEDURE, "");
      }

    }
  }
}

void SourceLexer::Tokenize(std::vector<std::shared_ptr<SourceToken>> &tokens_ptr) {
  while (HasMoreTokens()) {
    std::shared_ptr<SourceToken> token_ptr = GetNextToken();
    tokens_ptr.push_back(token_ptr);
  }
  RemoveWhiteSpaceTokens(tokens_ptr);
  TranslateKeywordTokens(tokens_ptr);
}

}

#include "source_lexer.h"

namespace source {

SourceLexer::SourceLexer(std::string simple_source) : m_cursor(0), m_simple_source(std::move(simple_source)) {
  ConstructLexerSpecs();
}

void SourceLexer::ConstructLexerSpecs() {
  m_lexer_specs.emplace_back(std::pair<std::regex, TokenType>("^\\s+", TokenType::WHITE_SPACE));
  m_lexer_specs.emplace_back(std::pair<std::regex, TokenType>("^\\d+", TokenType::INTEGER));
  m_lexer_specs.emplace_back(std::pair<std::regex, TokenType>("^[a-zA-Z]+[a-zA-Z0-9]*", TokenType::NAME));
  m_lexer_specs.emplace_back(std::pair<std::regex, TokenType>("^[{]", TokenType::OPENED_BRACES));
  m_lexer_specs.emplace_back(std::pair<std::regex, TokenType>("^[}]", TokenType::CLOSED_BRACES));
  m_lexer_specs.emplace_back(std::pair<std::regex, TokenType>("^[(]", TokenType::OPENED_PARENTHESIS));
  m_lexer_specs.emplace_back(std::pair<std::regex, TokenType>("^[)]", TokenType::CLOSED_PARENTHESIS));
  m_lexer_specs.emplace_back(std::pair<std::regex, TokenType>("^[&][&]", TokenType::AND));
  m_lexer_specs.emplace_back(std::pair<std::regex, TokenType>("^[|][|]", TokenType::OR));
  m_lexer_specs.emplace_back(std::pair<std::regex, TokenType>("^[>][=]", TokenType::IS_GREATER_EQUAL));
  m_lexer_specs.emplace_back(std::pair<std::regex, TokenType>("^[>]", TokenType::IS_GREATER));
  m_lexer_specs.emplace_back(std::pair<std::regex, TokenType>("^[<][=]", TokenType::IS_LESSER_EQUAL));
  m_lexer_specs.emplace_back(std::pair<std::regex, TokenType>("^[<]", TokenType::IS_LESSER));
  m_lexer_specs.emplace_back(std::pair<std::regex, TokenType>("^[=][=]", TokenType::IS_EQUAL));
  m_lexer_specs.emplace_back(std::pair<std::regex, TokenType>("^[!][=]", TokenType::IS_NOT_EQUAL));
  m_lexer_specs.emplace_back(std::pair<std::regex, TokenType>("^[+]", TokenType::ADDITION));
  m_lexer_specs.emplace_back(std::pair<std::regex, TokenType>("^[-]", TokenType::SUBTRACTION));
  m_lexer_specs.emplace_back(std::pair<std::regex, TokenType>("^[*]", TokenType::MULTIPLICATION));
  m_lexer_specs.emplace_back(std::pair<std::regex, TokenType>("^[/]", TokenType::DIVISION));
  m_lexer_specs.emplace_back(std::pair<std::regex, TokenType>("^[%]", TokenType::MODULUS));
  m_lexer_specs.emplace_back(std::pair<std::regex, TokenType>("^[;]", TokenType::SEMI_COLON));
  m_lexer_specs.emplace_back(std::pair<std::regex, TokenType>("^[=]", TokenType::EQUAL));
  m_lexer_specs.emplace_back(std::pair<std::regex, TokenType>("^[!]", TokenType::NOT));
}

bool SourceLexer::HasMoreTokens() {
  return m_cursor < m_simple_source.length();
}

std::shared_ptr<SourceToken> SourceLexer::GetNextToken() {
  std::string remaining_source = m_simple_source.substr(m_cursor);
  for (std::pair<std::regex, TokenType> lexer_spec : m_lexer_specs) {
    std::regex pattern_expression = lexer_spec.first;
    TokenType token_type = lexer_spec.second;

    std::smatch match;
    std::regex_search(remaining_source, match, pattern_expression);
    if (match.length() == 0) {
      continue;
    }

    std::string token_value = match[0].str();
    m_cursor += match[0].str().length();
    if (token_type == TokenType::NAME || token_type == TokenType::INTEGER) {
      return std::make_shared<SourceToken>(token_type, token_value);
    }
    return std::make_shared<SourceToken>(token_type, "");
  }

  throw UnexpectedTokenException();
}

void SourceLexer::RemoveWhiteSpaceTokens(std::vector<std::shared_ptr<SourceToken>> &tokens_ptr) {
  for (std::vector<std::shared_ptr<SourceToken>>::iterator it = tokens_ptr.begin(); it != tokens_ptr.end(); it++) {
    TokenType type = (*it)->GetType();
    if (type == TokenType::WHITE_SPACE) {
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

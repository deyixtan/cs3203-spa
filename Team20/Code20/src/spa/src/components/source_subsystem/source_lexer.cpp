#include "source_lexer.h"

namespace source {

SourceLexer::SourceLexer(std::string simple_source) : m_cursor(0), m_simple_source(simple_source) {
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
  if (!HasMoreTokens()) {
    return nullptr;
  }

  std::string remaining_source = m_simple_source.substr(m_cursor);
  for (std::pair<std::regex, TokenType> lexer_spec : m_lexer_specs) {
    std::regex expression = lexer_spec.first;
    TokenType token_type = lexer_spec.second;

    std::smatch match;
    std::regex_search(remaining_source, match, expression);
    if (match.length() == 0) {
      continue;
    }

    std::string token_value = match[0].str();
    m_cursor += match[0].str().length();
    if (token_type == TokenType::WHITE_SPACE) {
      return GetNextToken();
    }

    return std::make_shared<SourceToken>(token_type, token_value);
  }

  throw UnexpectedTokenException();
}

void SourceLexer::TryTranslateToKeywordToken(std::shared_ptr<SourceToken> &token_ptr) {
  TokenType token_type = token_ptr->GetType();
  std::string token_value = token_ptr->GetValue();

  if (token_type != TokenType::NAME) {
    return;
  }

  if (token_value == "procedure") {
    token_ptr = std::make_shared<SourceToken>(TokenType::PROCEDURE, "");
  } else if (token_value == "read") {
    token_ptr = std::make_shared<SourceToken>(TokenType::READ, "");
  } else if (token_value == "print") {
    token_ptr = std::make_shared<SourceToken>(TokenType::PRINT, "");
  } else if (token_value == "while") {
    token_ptr = std::make_shared<SourceToken>(TokenType::WHILE, "");
  } else if (token_value == "if") {
    token_ptr = std::make_shared<SourceToken>(TokenType::IF, "");
  } else if (token_value == "then") {
    token_ptr = std::make_shared<SourceToken>(TokenType::THEN, "");
  } else if (token_value == "else") {
    token_ptr = std::make_shared<SourceToken>(TokenType::ELSE, "");
  }
}

void SourceLexer::Tokenize(std::vector<std::shared_ptr<SourceToken>> &tokens_ptr) {
  while (HasMoreTokens()) {
    std::shared_ptr<SourceToken> token_ptr = GetNextToken();
    if (token_ptr != nullptr) {
      TryTranslateToKeywordToken(token_ptr);
      tokens_ptr.push_back(token_ptr);
    }
  }
}

}

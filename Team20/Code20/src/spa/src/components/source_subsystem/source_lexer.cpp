#include "source_lexer.h"

namespace source {

SourceLexer::SourceLexer(std::string program_source) {
  this->program_source = program_source;
  this->cursor = 0;
  ConstructSpecs();
}

void SourceLexer::ConstructSpecs() {
  tokenize_specs.emplace_back(std::pair<std::regex, TokenType>("^\\s+", TokenType::WHITE_SPACE));
  tokenize_specs.emplace_back(std::pair<std::regex, TokenType>("^\\d+", TokenType::DIGIT));
  tokenize_specs.emplace_back(std::pair<std::regex, TokenType>("^[a-zA-Z]+[a-zA-Z0-9]*", TokenType::NAME));
  tokenize_specs.emplace_back(std::pair<std::regex, TokenType>("^[{]", TokenType::OPENED_BRACES));
  tokenize_specs.emplace_back(std::pair<std::regex, TokenType>("^[}]", TokenType::CLOSED_BRACES));
  tokenize_specs.emplace_back(std::pair<std::regex, TokenType>("^[(]", TokenType::OPENED_PARENTHESIS));
  tokenize_specs.emplace_back(std::pair<std::regex, TokenType>("^[)]", TokenType::CLOSED_PARENTHESIS));
  tokenize_specs.emplace_back(std::pair<std::regex, TokenType>("^[&][&]", TokenType::AND));
  tokenize_specs.emplace_back(std::pair<std::regex, TokenType>("^[|][|]", TokenType::OR));
  tokenize_specs.emplace_back(std::pair<std::regex, TokenType>("^[>][=]", TokenType::IS_GREATER_EQUAL));
  tokenize_specs.emplace_back(std::pair<std::regex, TokenType>("^[>]", TokenType::IS_GREATER));
  tokenize_specs.emplace_back(std::pair<std::regex, TokenType>("^[<][=]", TokenType::IS_LESSER_EQUAL));
  tokenize_specs.emplace_back(std::pair<std::regex, TokenType>("^[<]", TokenType::IS_LESSER));
  tokenize_specs.emplace_back(std::pair<std::regex, TokenType>("^[=][=]", TokenType::IS_EQUAL));
  tokenize_specs.emplace_back(std::pair<std::regex, TokenType>("^[!][=]", TokenType::IS_NOT_EQUAL));
  tokenize_specs.emplace_back(std::pair<std::regex, TokenType>("^[+]", TokenType::ADDITION));
  tokenize_specs.emplace_back(std::pair<std::regex, TokenType>("^[-]", TokenType::SUBTRACTION));
  tokenize_specs.emplace_back(std::pair<std::regex, TokenType>("^[*]", TokenType::MULTIPLICATION));
  tokenize_specs.emplace_back(std::pair<std::regex, TokenType>("^[/]", TokenType::DIVISION));
  tokenize_specs.emplace_back(std::pair<std::regex, TokenType>("^[%]", TokenType::MODULUS));
  tokenize_specs.emplace_back(std::pair<std::regex, TokenType>("^[;]", TokenType::SEMI_COLON));
  tokenize_specs.emplace_back(std::pair<std::regex, TokenType>("^[=]", TokenType::EQUAL));
  tokenize_specs.emplace_back(std::pair<std::regex, TokenType>("^[!]", TokenType::NOT));
}

bool SourceLexer::IsEOF() {
  return cursor == program_source.length();
}

bool SourceLexer::HasMoreTokens() {
  return cursor < program_source.length();
}

SourceToken *SourceLexer::GetNextToken() {
  if (!HasMoreTokens()) {
    return nullptr;
  }

  std::string remaining_source = program_source.substr(cursor);

  std::vector<std::pair<std::regex, TokenType>>::iterator itr;
  for (itr = tokenize_specs.begin(); itr != tokenize_specs.end(); ++itr) {
    std::regex expression = itr->first;
    TokenType token_type = itr->second;

    std::smatch match;
    std::regex_search(remaining_source, match, expression);

    if (match.length() == 0) {
      continue;
    }

    std::string token_value = match[0].str();
    cursor += match[0].str().length();

    if (token_type == TokenType::WHITE_SPACE) {
      return GetNextToken();
    }

    return new SourceToken(token_type, token_value);
  }

  // TODO: throw UnexpectedTokenException
  return nullptr;
}

void SourceLexer::TryTranslateToKeywordToken(SourceToken *&token_ptr) {
  TokenType token_type = token_ptr->GetType();
  std::string token_value = token_ptr->GetValue();

  if (token_type != TokenType::NAME) {
    return;
  }

  if (token_value == "procedure") {
    token_ptr = new SourceToken(TokenType::PROCEDURE, (std::string &) "");
  } else if (token_value == "read") {
    token_ptr = new SourceToken(TokenType::READ, (std::string &) "");
  } else if (token_value == "print") {
    token_ptr = new SourceToken(TokenType::PRINT, (std::string &) "");
  } else if (token_value == "while") {
    token_ptr = new SourceToken(TokenType::WHILE, (std::string &) "");
  } else if (token_value == "if") {
    token_ptr = new SourceToken(TokenType::IF, (std::string &) "");
  } else if (token_value == "then") {
    token_ptr = new SourceToken(TokenType::THEN, (std::string &) "");
  } else if (token_value == "else") {
    token_ptr = new SourceToken(TokenType::ELSE, (std::string &) "");
  }

  // TODO: throw TokenTranslationException
}

void SourceLexer::Tokenize(std::vector<SourceToken *> &tokens_ptr) {
  while (!IsEOF()) {
    SourceToken *token_ptr = GetNextToken();
    if (token_ptr != nullptr) {
      TryTranslateToKeywordToken(token_ptr);
      tokens_ptr.push_back(token_ptr);
    }
  }
}

}

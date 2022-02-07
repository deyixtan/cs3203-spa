#include "source_lexer.h"

SourceLexer::SourceLexer(std::string program_source) {
  this->program_source = program_source;
  this->cursor = 0;
  ConstructSpecs();
}

void SourceLexer::ConstructSpecs() {
  tokenize_specs.emplace_back(std::pair<std::regex, SourceTokenType>("^\\n", SourceTokenType::NEW_LINE));
  tokenize_specs.emplace_back(std::pair<std::regex, SourceTokenType>("^\\s+", SourceTokenType::WHITE_SPACE));
  tokenize_specs.emplace_back(std::pair<std::regex, SourceTokenType>("^\\d+", SourceTokenType::DIGIT));
  tokenize_specs.emplace_back(std::pair<std::regex, SourceTokenType>("^[a-zA-Z]+[a-zA-Z0-9]*", SourceTokenType::NAME));
  tokenize_specs.emplace_back(std::pair<std::regex, SourceTokenType>("^[{]", SourceTokenType::OPENED_BRACES));
  tokenize_specs.emplace_back(std::pair<std::regex, SourceTokenType>("^[}]", SourceTokenType::CLOSED_BRACES));
  tokenize_specs.emplace_back(std::pair<std::regex, SourceTokenType>("^[(]", SourceTokenType::OPENED_PARENTHESIS));
  tokenize_specs.emplace_back(std::pair<std::regex, SourceTokenType>("^[)]", SourceTokenType::CLOSED_PARENTHESIS));
  tokenize_specs.emplace_back(std::pair<std::regex, SourceTokenType>("^[&][&]", SourceTokenType::AND));
  tokenize_specs.emplace_back(std::pair<std::regex, SourceTokenType>("^[|][|]", SourceTokenType::OR));
  tokenize_specs.emplace_back(std::pair<std::regex, SourceTokenType>("^[>][=]", SourceTokenType::IS_GREATER_EQUAL));
  tokenize_specs.emplace_back(std::pair<std::regex, SourceTokenType>("^[>]", SourceTokenType::IS_GREATER));
  tokenize_specs.emplace_back(std::pair<std::regex, SourceTokenType>("^[<][=]", SourceTokenType::IS_LESSER_EQUAL));
  tokenize_specs.emplace_back(std::pair<std::regex, SourceTokenType>("^[<]", SourceTokenType::IS_LESSER));
  tokenize_specs.emplace_back(std::pair<std::regex, SourceTokenType>("^[=][=]", SourceTokenType::IS_EQUAL));
  tokenize_specs.emplace_back(std::pair<std::regex, SourceTokenType>("^[!][=]", SourceTokenType::IS_NOT_EQUAL));
  tokenize_specs.emplace_back(std::pair<std::regex, SourceTokenType>("^[+]", SourceTokenType::ADDITION));
  tokenize_specs.emplace_back(std::pair<std::regex, SourceTokenType>("^[-]", SourceTokenType::SUBTRACTION));
  tokenize_specs.emplace_back(std::pair<std::regex, SourceTokenType>("^[*]", SourceTokenType::MULTIPLICATION));
  tokenize_specs.emplace_back(std::pair<std::regex, SourceTokenType>("^[/]", SourceTokenType::DIVISION));
  tokenize_specs.emplace_back(std::pair<std::regex, SourceTokenType>("^[%]", SourceTokenType::MODULUS));
  tokenize_specs.emplace_back(std::pair<std::regex, SourceTokenType>("^[;]", SourceTokenType::SEMI_COLON));
  tokenize_specs.emplace_back(std::pair<std::regex, SourceTokenType>("^[=]", SourceTokenType::EQUAL));
  tokenize_specs.emplace_back(std::pair<std::regex, SourceTokenType>("^[!]", SourceTokenType::NOT));
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

  std::vector<std::pair<std::regex, SourceTokenType>>::iterator itr;
  for (itr = tokenize_specs.begin(); itr != tokenize_specs.end(); ++itr) {
    std::regex expression = itr->first;
    SourceTokenType token_type = itr->second;

    std::smatch match;
    std::regex_search(remaining_source, match, expression);

    if (match.length() == 0) {
      continue;
    }

    std::string token_value = match[0].str();
    cursor += match[0].str().length();

    if (token_type == SourceTokenType::NEW_LINE) {
      token_value = "";
    } else if (token_type == SourceTokenType::WHITE_SPACE) {
      return GetNextToken();
    }

    return new SourceToken(token_type, token_value);
  }

  // TODO: throw UnexpectedTokenException
  return nullptr;
}

void SourceLexer::TryTranslateToKeywordToken(SourceToken *&token_ptr) {
  SourceTokenType token_type = token_ptr->GetType();
  std::string token_value = token_ptr->GetValue();

  if (token_type != SourceTokenType::NAME) {
    return;
  }

  if (token_value == "procedure") {
    token_ptr = new SourceToken(SourceTokenType::PROCEDURE, "");
  } else if (token_value == "read") {
    token_ptr = new SourceToken(SourceTokenType::READ, "");
  } else if (token_value == "print") {
    token_ptr = new SourceToken(SourceTokenType::PRINT, "");
  } else if (token_value == "while") {
    token_ptr = new SourceToken(SourceTokenType::WHILE, "");
  } else if (token_value == "if") {
    token_ptr = new SourceToken(SourceTokenType::IF, "");
  } else if (token_value == "then") {
    token_ptr = new SourceToken(SourceTokenType::THEN, "");
  } else if (token_value == "else") {
    token_ptr = new SourceToken(SourceTokenType::ELSE, "");
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

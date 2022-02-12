#include "components/source_subsystem/source_lexer.h"
#include "catch.hpp"

source::SourceToken source_procedure_token = source::SourceToken(source::TokenType::PROCEDURE, "");
source::SourceToken p_val_token = source::SourceToken(source::TokenType::NAME, "p");
source::SourceToken open_brace_token = source::SourceToken(source::TokenType::OPENED_BRACES, "{");
source::SourceToken close_brace_token = source::SourceToken(source::TokenType::CLOSED_BRACES, "}");
source::SourceToken open_bracket_token = source::SourceToken(source::TokenType::OPENED_PARENTHESIS, "(");
source::SourceToken close_bracket_token = source::SourceToken(source::TokenType::CLOSED_PARENTHESIS, ")");
source::SourceToken x_val_token = source::SourceToken(source::TokenType::NAME, "x");
source::SourceToken y_val_token = source::SourceToken(source::TokenType::NAME, "y");
source::SourceToken z_val_token = source::SourceToken(source::TokenType::NAME, "z");
source::SourceToken equal_token = source::SourceToken(source::TokenType::EQUAL, "=");
source::SourceToken digit_token = source::SourceToken(source::TokenType::DIGIT, "2");
source::SourceToken digit_five_token = source::SourceToken(source::TokenType::DIGIT, "5");
source::SourceToken source_if_token = source::SourceToken(source::TokenType::IF, "");
source::SourceToken source_then_token = source::SourceToken(source::TokenType::THEN, "");
source::SourceToken source_else_token = source::SourceToken(source::TokenType::ELSE, "");
source::SourceToken source_read_token = source::SourceToken(source::TokenType::READ, "");
source::SourceToken source_print_token = source::SourceToken(source::TokenType::PRINT, "");
source::SourceToken source_while_token = source::SourceToken(source::TokenType::WHILE, "");
source::SourceToken source_semicolon_token = source::SourceToken(source::TokenType::SEMI_COLON, ";");
source::SourceToken source_subtract_token = source::SourceToken(source::TokenType::SUBTRACTION, "-");
source::SourceToken source_lesser_token = source::SourceToken(source::TokenType::IS_LESSER, "<");
source::SourceToken source_equal_token = source::SourceToken(source::TokenType::EQUAL, "=");


TEST_CASE("Test source lexer with basic program") {
  std::string program_source = "procedure p { x = 2; }";
  source::SourceLexer lexer = source::SourceLexer(program_source);
  std::vector<std::shared_ptr<source::SourceToken>> tokens_ptr;
  lexer.Tokenize(tokens_ptr);
  std::vector<source::SourceToken> expected_tokens;
  expected_tokens.push_back(source_procedure_token);
  expected_tokens.push_back(p_val_token);
  expected_tokens.push_back(open_brace_token);
  expected_tokens.push_back(x_val_token);
  expected_tokens.push_back(equal_token);
  expected_tokens.push_back(digit_token);
  expected_tokens.push_back(source_semicolon_token);
  expected_tokens.push_back(close_brace_token);
  for(int i = 0; i < expected_tokens.size(); i++) {
      std::shared_ptr<source::SourceToken> token = tokens_ptr[i];
      REQUIRE(token->GetValue() == expected_tokens[i].GetValue());
      REQUIRE(token->GetType() == expected_tokens[i].GetType());
  }
}

TEST_CASE("Test source lexer with if-else") {
  std::string program_source = "procedure p { if(x) then { read y; } else { print z; } }";
  source::SourceLexer lexer = source::SourceLexer(program_source);
  std::vector<std::shared_ptr<source::SourceToken>> tokens_ptr;
  lexer.Tokenize(tokens_ptr);
  std::vector<source::SourceToken> expected_tokens;
  expected_tokens.push_back(source_procedure_token);
  expected_tokens.push_back(p_val_token);
  expected_tokens.push_back(open_brace_token);
  expected_tokens.push_back(source_if_token);
  expected_tokens.push_back(open_bracket_token);
  expected_tokens.push_back(x_val_token);
  expected_tokens.push_back(close_bracket_token);
  expected_tokens.push_back(source_then_token);
  expected_tokens.push_back(open_brace_token);
  expected_tokens.push_back(source_read_token);
  expected_tokens.push_back(y_val_token);
  expected_tokens.push_back(source_semicolon_token);
  expected_tokens.push_back(close_brace_token);
  expected_tokens.push_back(source_else_token);
  expected_tokens.push_back(open_brace_token);
  expected_tokens.push_back(source_print_token);
  expected_tokens.push_back(z_val_token);
  expected_tokens.push_back(source_semicolon_token);
  expected_tokens.push_back(close_brace_token);
  expected_tokens.push_back(close_brace_token);
  for(int i = 0; i < expected_tokens.size(); i++) {
    std::shared_ptr<source::SourceToken> token = tokens_ptr[i];
    REQUIRE(token->GetValue() == expected_tokens[i].GetValue());
    REQUIRE(token->GetType() == expected_tokens[i].GetType());
  }
}

TEST_CASE("Test source lexer with while loop") {
  std::string program_source = "procedure p { x = 2; while(x < 5) { y = z - 2; } }";
  source::SourceLexer lexer = source::SourceLexer(program_source);
  std::vector<std::shared_ptr<source::SourceToken>> tokens_ptr;
  lexer.Tokenize(tokens_ptr);
  std::vector<source::SourceToken> expected_tokens;
  expected_tokens.push_back(source_procedure_token);
  expected_tokens.push_back(p_val_token);
  expected_tokens.push_back(open_brace_token);
  expected_tokens.push_back(x_val_token);
  expected_tokens.push_back(equal_token);
  expected_tokens.push_back(digit_token);
  expected_tokens.push_back(source_semicolon_token);
  expected_tokens.push_back(source_while_token);
  expected_tokens.push_back(open_bracket_token);
  expected_tokens.push_back(x_val_token);
  expected_tokens.push_back(source_lesser_token);
  expected_tokens.push_back(digit_five_token);
  expected_tokens.push_back(close_bracket_token);
  expected_tokens.push_back(open_brace_token);
  expected_tokens.push_back(y_val_token);
  expected_tokens.push_back(source_equal_token);
  expected_tokens.push_back(z_val_token);
  expected_tokens.push_back(source_subtract_token);
  expected_tokens.push_back(digit_token);
  expected_tokens.push_back(source_semicolon_token);
  expected_tokens.push_back(close_brace_token);
  expected_tokens.push_back(close_brace_token);
  for(int i = 0; i < expected_tokens.size(); i++) {
    std::shared_ptr<source::SourceToken> token = tokens_ptr[i];
    REQUIRE(token->GetValue() == expected_tokens[i].GetValue());
    REQUIRE(token->GetType() == expected_tokens[i].GetType());
  }
}
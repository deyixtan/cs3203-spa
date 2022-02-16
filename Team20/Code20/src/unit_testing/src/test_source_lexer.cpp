#include "catch.hpp"
#include "components/source_subsystem/source_lexer.h"
#include "components/source_subsystem/exceptions/unexpected_token.h"

using namespace source;

source::SourceToken source_procedure_token = source::SourceToken(source::TokenType::PROCEDURE, "");
source::SourceToken p_val_token = source::SourceToken(source::TokenType::NAME, "p");
source::SourceToken open_brace_token = source::SourceToken(source::TokenType::OPENED_BRACES, "");
source::SourceToken close_brace_token = source::SourceToken(source::TokenType::CLOSED_BRACES, "");
source::SourceToken open_bracket_token = source::SourceToken(source::TokenType::OPENED_PARENTHESIS, "");
source::SourceToken close_bracket_token = source::SourceToken(source::TokenType::CLOSED_PARENTHESIS, "");
source::SourceToken x_val_token = source::SourceToken(source::TokenType::NAME, "x");
source::SourceToken y_val_token = source::SourceToken(source::TokenType::NAME, "y");
source::SourceToken z_val_token = source::SourceToken(source::TokenType::NAME, "z");
source::SourceToken equal_token = source::SourceToken(source::TokenType::EQUAL, "");
source::SourceToken digit_token = source::SourceToken(source::TokenType::INTEGER, "2");
source::SourceToken digit_five_token = source::SourceToken(source::TokenType::INTEGER, "5");
source::SourceToken source_if_token = source::SourceToken(source::TokenType::IF, "");
source::SourceToken source_then_token = source::SourceToken(source::TokenType::THEN, "");
source::SourceToken source_else_token = source::SourceToken(source::TokenType::ELSE, "");
source::SourceToken source_read_token = source::SourceToken(source::TokenType::READ, "");
source::SourceToken source_print_token = source::SourceToken(source::TokenType::PRINT, "");
source::SourceToken source_while_token = source::SourceToken(source::TokenType::WHILE, "");
source::SourceToken source_semicolon_token = source::SourceToken(source::TokenType::SEMI_COLON, "");
source::SourceToken source_subtract_token = source::SourceToken(source::TokenType::SUBTRACTION, "");
source::SourceToken source_lesser_token = source::SourceToken(source::TokenType::IS_LESSER, "");
source::SourceToken source_equal_token = source::SourceToken(source::TokenType::EQUAL, "");

TEST_CASE("Test empty source") {
  SourceLexer lexer = SourceLexer("");

  std::vector<std::shared_ptr<source::SourceToken>> tokens_ptr;
  lexer.Tokenize(tokens_ptr);

  REQUIRE(tokens_ptr.size() == 0);
}

TEST_CASE("Test single unexpected token") {
  SourceLexer lexer = SourceLexer("@");

  std::vector<std::shared_ptr<source::SourceToken>> tokens_ptr;

  REQUIRE_THROWS_WITH(lexer.Tokenize(tokens_ptr), UnexpectedTokenException().what());
}

TEST_CASE("Test simple read statement") {
  SourceLexer lexer = SourceLexer("read x;");

  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;
  lexer.Tokenize(tokens_ptr);

  std::vector<std::shared_ptr<SourceToken>> expected_tokens_ptr;
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::READ, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::NAME, "x"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::SEMI_COLON, ""));

  REQUIRE(tokens_ptr.size() == expected_tokens_ptr.size());

  for (int i = 0; i < tokens_ptr.size(); i++) {
    std::shared_ptr<source::SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<source::SourceToken> expected_token = expected_tokens_ptr.at(i);
    REQUIRE(token->GetValue() == expected_token->GetValue());
    REQUIRE(token->GetType() == expected_token->GetType());
  }
}

TEST_CASE("Test simple invalid read statement") {
  SourceLexer lexer = SourceLexer("read 1;");

  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;
  lexer.Tokenize(tokens_ptr);

  std::vector<std::shared_ptr<SourceToken>> expected_tokens_ptr;
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::NAME, "read"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::INTEGER, "1"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::SEMI_COLON, ""));

  REQUIRE(tokens_ptr.size() == expected_tokens_ptr.size());

  for (int i = 0; i < tokens_ptr.size(); i++) {
    std::shared_ptr<source::SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<source::SourceToken> expected_token = expected_tokens_ptr.at(i);
    REQUIRE(token->GetValue() == expected_token->GetValue());
    REQUIRE(token->GetType() == expected_token->GetType());
  }
}

TEST_CASE("Test simple print statement") {
  SourceLexer lexer = SourceLexer("print x;");

  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;
  lexer.Tokenize(tokens_ptr);

  std::vector<std::shared_ptr<SourceToken>> expected_tokens_ptr;
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::PRINT, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::NAME, "x"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::SEMI_COLON, ""));

  REQUIRE(tokens_ptr.size() == expected_tokens_ptr.size());

  for (int i = 0; i < tokens_ptr.size(); i++) {
    std::shared_ptr<source::SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<source::SourceToken> expected_token = expected_tokens_ptr.at(i);
    REQUIRE(token->GetValue() == expected_token->GetValue());
    REQUIRE(token->GetType() == expected_token->GetType());
  }
}

TEST_CASE("Test simple invalid print statement") {
  SourceLexer lexer = SourceLexer("print 1;");

  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;
  lexer.Tokenize(tokens_ptr);

  std::vector<std::shared_ptr<SourceToken>> expected_tokens_ptr;
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::NAME, "print"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::INTEGER, "1"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::SEMI_COLON, ""));

  REQUIRE(tokens_ptr.size() == expected_tokens_ptr.size());

  for (int i = 0; i < tokens_ptr.size(); i++) {
    std::shared_ptr<source::SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<source::SourceToken> expected_token = expected_tokens_ptr.at(i);
    REQUIRE(token->GetValue() == expected_token->GetValue());
    REQUIRE(token->GetType() == expected_token->GetType());
  }
}

TEST_CASE("Test simple while-statement") {
  SourceLexer lexer = SourceLexer("while (x == 0) { x = 1; }");

  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;
  lexer.Tokenize(tokens_ptr);

  std::vector<std::shared_ptr<SourceToken>> expected_tokens_ptr;
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::WHILE, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::OPENED_PARENTHESIS, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::NAME, "x"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::IS_EQUAL, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::INTEGER, "0"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::CLOSED_PARENTHESIS, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::OPENED_BRACES, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::NAME, "x"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::EQUAL, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::INTEGER, "1"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::SEMI_COLON, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::CLOSED_BRACES, ""));

  REQUIRE(tokens_ptr.size() == expected_tokens_ptr.size());

  for (int i = 0; i < tokens_ptr.size(); i++) {
    std::shared_ptr<source::SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<source::SourceToken> expected_token = expected_tokens_ptr.at(i);
    REQUIRE(token->GetValue() == expected_token->GetValue());
    REQUIRE(token->GetType() == expected_token->GetType());
  }
}

TEST_CASE("Test simple if-statement (if part)") {
  SourceLexer lexer = SourceLexer("if (x == 0)");

  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;
  lexer.Tokenize(tokens_ptr);

  std::vector<std::shared_ptr<SourceToken>> expected_tokens_ptr;
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::IF, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::OPENED_PARENTHESIS, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::NAME, "x"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::IS_EQUAL, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::INTEGER, "0"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::CLOSED_PARENTHESIS, ""));

  REQUIRE(tokens_ptr.size() == expected_tokens_ptr.size());

  for (int i = 0; i < tokens_ptr.size(); i++) {
    std::shared_ptr<source::SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<source::SourceToken> expected_token = expected_tokens_ptr.at(i);
    REQUIRE(token->GetValue() == expected_token->GetValue());
    REQUIRE(token->GetType() == expected_token->GetType());
  }
}

TEST_CASE("Test simple if-statement (then part)") {
  SourceLexer lexer = SourceLexer("then { x = 1; }");

  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;
  lexer.Tokenize(tokens_ptr);

  std::vector<std::shared_ptr<SourceToken>> expected_tokens_ptr;
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::THEN, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::OPENED_BRACES, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::NAME, "x"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::EQUAL, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::INTEGER, "1"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::SEMI_COLON, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::CLOSED_BRACES, ""));

  REQUIRE(tokens_ptr.size() == expected_tokens_ptr.size());

  for (int i = 0; i < tokens_ptr.size(); i++) {
    std::shared_ptr<source::SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<source::SourceToken> expected_token = expected_tokens_ptr.at(i);
    REQUIRE(token->GetValue() == expected_token->GetValue());
    REQUIRE(token->GetType() == expected_token->GetType());
  }
}

TEST_CASE("Test simple if-statement (else part)") {
  SourceLexer lexer = SourceLexer("else { x = 2; }");

  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;
  lexer.Tokenize(tokens_ptr);

  std::vector<std::shared_ptr<SourceToken>> expected_tokens_ptr;
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::ELSE, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::OPENED_BRACES, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::NAME, "x"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::EQUAL, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::INTEGER, "2"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::SEMI_COLON, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::CLOSED_BRACES, ""));

  REQUIRE(tokens_ptr.size() == expected_tokens_ptr.size());

  for (int i = 0; i < tokens_ptr.size(); i++) {
    std::shared_ptr<source::SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<source::SourceToken> expected_token = expected_tokens_ptr.at(i);
    REQUIRE(token->GetValue() == expected_token->GetValue());
    REQUIRE(token->GetType() == expected_token->GetType());
  }
}

TEST_CASE("Test simple if-statement") {
  SourceLexer lexer = SourceLexer("if (x == 0) then { x = 1; } else { x = 2; }");

  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;
  lexer.Tokenize(tokens_ptr);

  std::vector<std::shared_ptr<SourceToken>> expected_tokens_ptr;
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::IF, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::OPENED_PARENTHESIS, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::NAME, "x"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::IS_EQUAL, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::INTEGER, "0"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::CLOSED_PARENTHESIS, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::THEN, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::OPENED_BRACES, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::NAME, "x"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::EQUAL, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::INTEGER, "1"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::SEMI_COLON, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::CLOSED_BRACES, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::ELSE, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::OPENED_BRACES, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::NAME, "x"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::EQUAL, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::INTEGER, "2"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::SEMI_COLON, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::CLOSED_BRACES, ""));

  REQUIRE(tokens_ptr.size() == expected_tokens_ptr.size());

  for (int i = 0; i < tokens_ptr.size(); i++) {
    std::shared_ptr<source::SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<source::SourceToken> expected_token = expected_tokens_ptr.at(i);
    REQUIRE(token->GetValue() == expected_token->GetValue());
    REQUIRE(token->GetType() == expected_token->GetType());
  }
}

TEST_CASE("Test simple assign statement") {
  SourceLexer lexer = SourceLexer("x = 1;");

  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;
  lexer.Tokenize(tokens_ptr);

  std::vector<std::shared_ptr<SourceToken>> expected_tokens_ptr;
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::NAME, "x"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::EQUAL, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::INTEGER, "1"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::SEMI_COLON, ""));

  REQUIRE(tokens_ptr.size() == expected_tokens_ptr.size());

  for (int i = 0; i < tokens_ptr.size(); i++) {
    std::shared_ptr<source::SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<source::SourceToken> expected_token = expected_tokens_ptr.at(i);
    REQUIRE(token->GetValue() == expected_token->GetValue());
    REQUIRE(token->GetType() == expected_token->GetType());
  }
}

TEST_CASE("Test simple procedure (before stmt_list") {
  SourceLexer lexer = SourceLexer("procedure main {");

  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;
  lexer.Tokenize(tokens_ptr);

  std::vector<std::shared_ptr<SourceToken>> expected_tokens_ptr;
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::PROCEDURE, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::NAME, "main"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::OPENED_BRACES, ""));

  REQUIRE(tokens_ptr.size() == expected_tokens_ptr.size());

  for (int i = 0; i < tokens_ptr.size(); i++) {
    std::shared_ptr<source::SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<source::SourceToken> expected_token = expected_tokens_ptr.at(i);
    REQUIRE(token->GetValue() == expected_token->GetValue());
    REQUIRE(token->GetType() == expected_token->GetType());
  }
}

TEST_CASE("Test simple procedure statement") {
  SourceLexer lexer = SourceLexer("procedure main { x = 1; }");

  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;
  lexer.Tokenize(tokens_ptr);

  std::vector<std::shared_ptr<SourceToken>> expected_tokens_ptr;
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::PROCEDURE, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::NAME, "main"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::OPENED_BRACES, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::NAME, "x"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::EQUAL, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::INTEGER, "1"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::SEMI_COLON, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::CLOSED_BRACES, ""));

  REQUIRE(tokens_ptr.size() == expected_tokens_ptr.size());

  for (int i = 0; i < tokens_ptr.size(); i++) {
    std::shared_ptr<source::SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<source::SourceToken> expected_token = expected_tokens_ptr.at(i);
    REQUIRE(token->GetValue() == expected_token->GetValue());
    REQUIRE(token->GetType() == expected_token->GetType());
  }
}

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
  for (int i = 0; i < expected_tokens.size(); i++) {
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
  for (int i = 0; i < expected_tokens.size(); i++) {
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
  for (int i = 0; i < expected_tokens.size(); i++) {
    std::shared_ptr<source::SourceToken> token = tokens_ptr[i];
    REQUIRE(token->GetValue() == expected_tokens[i].GetValue());
    REQUIRE(token->GetType() == expected_tokens[i].GetType());
  }
}

TEST_CASE("Test source lexer with unrecognised character hash") {
  std::string program_source = "procedure p { x = # }";
  source::SourceLexer lexer = source::SourceLexer(program_source);
  std::vector<std::shared_ptr<source::SourceToken>> tokens_ptr;
  REQUIRE_THROWS_WITH(lexer.Tokenize(tokens_ptr), "Unexpected token.");
}

TEST_CASE("Test source lexer with unrecognised character question") {
  std::string program_source = "procedure p { x = 2 ?";
  source::SourceLexer lexer = source::SourceLexer(program_source);
  std::vector<std::shared_ptr<source::SourceToken>> tokens_ptr;
  REQUIRE_THROWS_WITH(lexer.Tokenize(tokens_ptr), "Unexpected token.");
}

TEST_CASE("Test source lexer with unrecognised characters") {
  std::string program_source = "@$/ p { x = 2 }";
  source::SourceLexer lexer = source::SourceLexer(program_source);
  std::vector<std::shared_ptr<source::SourceToken>> tokens_ptr;
  REQUIRE_THROWS_WITH(lexer.Tokenize(tokens_ptr), "Unexpected token.");
}

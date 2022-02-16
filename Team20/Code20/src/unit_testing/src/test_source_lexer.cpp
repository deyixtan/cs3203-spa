#include "catch.hpp"
#include "components/source_subsystem/source_lexer.h"
#include "components/source_subsystem/exceptions/unexpected_token.h"

using namespace source;

SourceToken source_procedure_token = SourceToken(TokenType::PROCEDURE, "");
SourceToken p_val_token = SourceToken(TokenType::NAME, "p");
SourceToken open_brace_token = SourceToken(TokenType::OPENED_BRACES, "");
SourceToken close_brace_token = SourceToken(TokenType::CLOSED_BRACES, "");
SourceToken open_bracket_token = SourceToken(TokenType::OPENED_PARENTHESIS, "");
SourceToken close_bracket_token = SourceToken(TokenType::CLOSED_PARENTHESIS, "");
SourceToken x_val_token = SourceToken(TokenType::NAME, "x");
SourceToken y_val_token = SourceToken(TokenType::NAME, "y");
SourceToken z_val_token = SourceToken(TokenType::NAME, "z");
SourceToken equal_token = SourceToken(TokenType::EQUAL, "");
SourceToken digit_token = SourceToken(TokenType::INTEGER, "2");
SourceToken digit_five_token = SourceToken(TokenType::INTEGER, "5");
SourceToken source_if_token = SourceToken(TokenType::IF, "");
SourceToken source_then_token = SourceToken(TokenType::THEN, "");
SourceToken source_else_token = SourceToken(TokenType::ELSE, "");
SourceToken source_read_token = SourceToken(TokenType::READ, "");
SourceToken source_print_token = SourceToken(TokenType::PRINT, "");
SourceToken source_while_token = SourceToken(TokenType::WHILE, "");
SourceToken source_semicolon_token = SourceToken(TokenType::SEMI_COLON, "");
SourceToken source_subtract_token = SourceToken(TokenType::SUBTRACTION, "");
SourceToken source_lesser_token = SourceToken(TokenType::IS_LESSER, "");
SourceToken source_equal_token = SourceToken(TokenType::EQUAL, "");

TEST_CASE("Test empty source") {
  SourceLexer lexer = SourceLexer("");

  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;
  lexer.Tokenize(tokens_ptr);

  REQUIRE(tokens_ptr.size() == 0);
}

TEST_CASE("Test single unexpected token") {
  SourceLexer lexer = SourceLexer("@");

  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;

  REQUIRE_THROWS_WITH(lexer.Tokenize(tokens_ptr), UnexpectedTokenException().what());
}

TEST_CASE("Test simple read string") {
  SourceLexer lexer = SourceLexer("read");

  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;
  lexer.Tokenize(tokens_ptr);

  std::vector<std::shared_ptr<SourceToken>> expected_tokens_ptr;
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::NAME, "read"));

  REQUIRE(tokens_ptr.size() == expected_tokens_ptr.size());

  for (int i = 0; i < tokens_ptr.size(); i++) {
    std::shared_ptr<SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<SourceToken> expected_token = expected_tokens_ptr.at(i);
    REQUIRE(token->GetValue() == expected_token->GetValue());
    REQUIRE(token->GetType() == expected_token->GetType());
  }
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
    std::shared_ptr<SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<SourceToken> expected_token = expected_tokens_ptr.at(i);
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
    std::shared_ptr<SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<SourceToken> expected_token = expected_tokens_ptr.at(i);
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
    std::shared_ptr<SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<SourceToken> expected_token = expected_tokens_ptr.at(i);
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
    std::shared_ptr<SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<SourceToken> expected_token = expected_tokens_ptr.at(i);
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
    std::shared_ptr<SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<SourceToken> expected_token = expected_tokens_ptr.at(i);
    REQUIRE(token->GetValue() == expected_token->GetValue());
    REQUIRE(token->GetType() == expected_token->GetType());
  }
}

TEST_CASE("Test simple invalid while statement") {
  SourceLexer lexer = SourceLexer("while 1;");

  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;
  lexer.Tokenize(tokens_ptr);

  std::vector<std::shared_ptr<SourceToken>> expected_tokens_ptr;
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::NAME, "while"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::INTEGER, "1"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::SEMI_COLON, ""));

  REQUIRE(tokens_ptr.size() == expected_tokens_ptr.size());

  for (int i = 0; i < tokens_ptr.size(); i++) {
    std::shared_ptr<SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<SourceToken> expected_token = expected_tokens_ptr.at(i);
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
    std::shared_ptr<SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<SourceToken> expected_token = expected_tokens_ptr.at(i);
    REQUIRE(token->GetValue() == expected_token->GetValue());
    REQUIRE(token->GetType() == expected_token->GetType());
  }
}

TEST_CASE("Test simple invalid if-statement (if part)") {
  SourceLexer lexer = SourceLexer("if 1;");

  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;
  lexer.Tokenize(tokens_ptr);

  std::vector<std::shared_ptr<SourceToken>> expected_tokens_ptr;
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::NAME, "if"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::INTEGER, "1"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::SEMI_COLON, ""));

  REQUIRE(tokens_ptr.size() == expected_tokens_ptr.size());

  for (int i = 0; i < tokens_ptr.size(); i++) {
    std::shared_ptr<SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<SourceToken> expected_token = expected_tokens_ptr.at(i);
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
    std::shared_ptr<SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<SourceToken> expected_token = expected_tokens_ptr.at(i);
    REQUIRE(token->GetValue() == expected_token->GetValue());
    REQUIRE(token->GetType() == expected_token->GetType());
  }
}

TEST_CASE("Test simple invalid if-statement (then part)") {
  SourceLexer lexer = SourceLexer("then 1;");

  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;
  lexer.Tokenize(tokens_ptr);

  std::vector<std::shared_ptr<SourceToken>> expected_tokens_ptr;
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::NAME, "then"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::INTEGER, "1"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::SEMI_COLON, ""));

  REQUIRE(tokens_ptr.size() == expected_tokens_ptr.size());

  for (int i = 0; i < tokens_ptr.size(); i++) {
    std::shared_ptr<SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<SourceToken> expected_token = expected_tokens_ptr.at(i);
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
    std::shared_ptr<SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<SourceToken> expected_token = expected_tokens_ptr.at(i);
    REQUIRE(token->GetValue() == expected_token->GetValue());
    REQUIRE(token->GetType() == expected_token->GetType());
  }
}

TEST_CASE("Test simple invalid if-statement (else part)") {
  SourceLexer lexer = SourceLexer("else 1;");

  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;
  lexer.Tokenize(tokens_ptr);

  std::vector<std::shared_ptr<SourceToken>> expected_tokens_ptr;
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::NAME, "else"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::INTEGER, "1"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::SEMI_COLON, ""));

  REQUIRE(tokens_ptr.size() == expected_tokens_ptr.size());

  for (int i = 0; i < tokens_ptr.size(); i++) {
    std::shared_ptr<SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<SourceToken> expected_token = expected_tokens_ptr.at(i);
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
    std::shared_ptr<SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<SourceToken> expected_token = expected_tokens_ptr.at(i);
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
    std::shared_ptr<SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<SourceToken> expected_token = expected_tokens_ptr.at(i);
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
    std::shared_ptr<SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<SourceToken> expected_token = expected_tokens_ptr.at(i);
    REQUIRE(token->GetValue() == expected_token->GetValue());
    REQUIRE(token->GetType() == expected_token->GetType());
  }
}

TEST_CASE("Test simple invalid procedure") {
  SourceLexer lexer = SourceLexer("procedure = 1;");

  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;
  lexer.Tokenize(tokens_ptr);

  std::vector<std::shared_ptr<SourceToken>> expected_tokens_ptr;
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::NAME, "procedure"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::EQUAL, ""));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::INTEGER, "1"));
  expected_tokens_ptr.push_back(std::make_shared<SourceToken>(TokenType::SEMI_COLON, ""));

  REQUIRE(tokens_ptr.size() == expected_tokens_ptr.size());

  for (int i = 0; i < tokens_ptr.size(); i++) {
    std::shared_ptr<SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<SourceToken> expected_token = expected_tokens_ptr.at(i);
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
    std::shared_ptr<SourceToken> token = tokens_ptr.at(i);
    std::shared_ptr<SourceToken> expected_token = expected_tokens_ptr.at(i);
    REQUIRE(token->GetValue() == expected_token->GetValue());
    REQUIRE(token->GetType() == expected_token->GetType());
  }
}

TEST_CASE("Test source lexer with basic program") {
  std::string program_source = "procedure p { x = 2; }";
  SourceLexer lexer = SourceLexer(program_source);
  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;
  lexer.Tokenize(tokens_ptr);
  std::vector<SourceToken> expected_tokens;
  expected_tokens.push_back(source_procedure_token);
  expected_tokens.push_back(p_val_token);
  expected_tokens.push_back(open_brace_token);
  expected_tokens.push_back(x_val_token);
  expected_tokens.push_back(equal_token);
  expected_tokens.push_back(digit_token);
  expected_tokens.push_back(source_semicolon_token);
  expected_tokens.push_back(close_brace_token);
  for (int i = 0; i < expected_tokens.size(); i++) {
    std::shared_ptr<SourceToken> token = tokens_ptr[i];
    REQUIRE(token->GetValue() == expected_tokens[i].GetValue());
    REQUIRE(token->GetType() == expected_tokens[i].GetType());
  }
}

TEST_CASE("Test source lexer with if-else") {
  std::string program_source = "procedure p { if(x) then { read y; } else { print z; } }";
  SourceLexer lexer = SourceLexer(program_source);
  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;
  lexer.Tokenize(tokens_ptr);
  std::vector<SourceToken> expected_tokens;
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
    std::shared_ptr<SourceToken> token = tokens_ptr[i];
    REQUIRE(token->GetValue() == expected_tokens[i].GetValue());
    REQUIRE(token->GetType() == expected_tokens[i].GetType());
  }
}

TEST_CASE("Test source lexer with while loop") {
  std::string program_source = "procedure p { x = 2; while(x < 5) { y = z - 2; } }";
  SourceLexer lexer = SourceLexer(program_source);
  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;
  lexer.Tokenize(tokens_ptr);
  std::vector<SourceToken> expected_tokens;
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
    std::shared_ptr<SourceToken> token = tokens_ptr[i];
    REQUIRE(token->GetValue() == expected_tokens[i].GetValue());
    REQUIRE(token->GetType() == expected_tokens[i].GetType());
  }
}

TEST_CASE("Test source lexer with unrecognised character hash") {
  std::string program_source = "procedure p { x = # }";
  SourceLexer lexer = SourceLexer(program_source);
  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;
  REQUIRE_THROWS_WITH(lexer.Tokenize(tokens_ptr), "Unexpected token.");
}

TEST_CASE("Test source lexer with unrecognised character question") {
  std::string program_source = "procedure p { x = 2 ?";
  SourceLexer lexer = SourceLexer(program_source);
  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;
  REQUIRE_THROWS_WITH(lexer.Tokenize(tokens_ptr), "Unexpected token.");
}

TEST_CASE("Test source lexer with unrecognised characters") {
  std::string program_source = "@$/ p { x = 2 }";
  SourceLexer lexer = SourceLexer(program_source);
  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;
  REQUIRE_THROWS_WITH(lexer.Tokenize(tokens_ptr), "Unexpected token.");
}

#include "catch.hpp"
#include "components/pkb/pkb.h"
#include "components/pkb/design_extractor/design_extractor.h"
#include "components/source_subsystem/source_lexer.h"
#include "components/source_subsystem/source_parser.h"
#include "components/query_subsystem/pql_parser/parsed_query.h"
#include "components/query_subsystem/pql_lexer/pql_lexer.h"
#include "components/query_subsystem/pql_parser/parsed_query_builder.h"
#include "components/query_subsystem/pql_parser/query_validator.h"
#include "components/query_subsystem/pql_evaluator/query_evaluator.h"

using namespace source;

extern std::string sample_source1;
extern std::string sample_source2;

std::shared_ptr<ProgramNode> GenerateAST(std::string source) {
  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;

  SourceLexer lexer = SourceLexer(source);
  lexer.Tokenize(tokens_ptr);

  SourceParser parser = SourceParser(tokens_ptr);
  std::shared_ptr<ProgramNode> ast = parser.ParseProgram();
  return ast;
}

PKB *GetPopulatedPkb(std::shared_ptr<ProgramNode> ast) {
  PKB *pkb = PKB::GetInstance();
  pkb->Clear();

  DesignExtractor *design_extractor = new DesignExtractor(*ast, pkb);
  design_extractor->TraverseAst();
  return pkb;
}

TEST_CASE("Test components between pql processor and PKB (Sample source 1)") {
  std::shared_ptr<ProgramNode> ast = GenerateAST(sample_source1);
  PKB *pkb = GetPopulatedPkb(ast);
  pql_evaluator::QueryEvaluator *evaluator = new pql_evaluator::QueryEvaluator(pkb);

  SECTION("Test if-statement count and their statement number") {
    PqlLexer lexer{"if ifs; Select ifs"};
    ParsedQueryBuilder pqb;
    std::vector<PqlToken> tokens = lexer.Lex();
    QueryValidator query_validator = QueryValidator(tokens);
    std::vector<PqlToken> validated_tokens = query_validator.CheckValidation();
    ParsedQuery parsed_query = pqb.Build(validated_tokens);
    std::list<std::string> results;
    evaluator->Evaluate(parsed_query, results);
    std::string result = results.front();
    std::string expected_result = "8";

    REQUIRE(result == expected_result);
  }

  SECTION("Test assign statement count and their statement number") {
    PqlLexer lexer{"assign a; Select a"};
    ParsedQueryBuilder pqb;
    std::vector<PqlToken> tokens = lexer.Lex();
    QueryValidator query_validator = QueryValidator(tokens);
    std::vector<PqlToken> validated_tokens = query_validator.CheckValidation();
    ParsedQuery parsed_query = pqb.Build(validated_tokens);
    std::list<std::string> results;
    evaluator->Evaluate(parsed_query, results);
    std::list<std::string> expected_result = {"10", "11", "9", "6", "7", "5", "12", "3", "2", "1"};

    REQUIRE(results.size() == 10);
    REQUIRE(results == expected_result);
  }

  SECTION("Test Follows relationship") {
    std::vector<std::string> expected_result = {"2", "3", "4", "8", "6", "7"};
    for (int i = 1; i < 7; i++) {
      std::string query = "stmt s ; Select s such that Follows(" + std::to_string(i) + " ,s)";
      PqlLexer lexer{query};
      ParsedQueryBuilder pqb;
      std::vector<PqlToken> tokens = lexer.Lex();
      QueryValidator query_validator = QueryValidator(tokens);
      std::vector<PqlToken> validated_tokens = query_validator.CheckValidation();
      ParsedQuery parsed_query = pqb.Build(validated_tokens);
      std::list<std::string> results;
      evaluator->Evaluate(parsed_query, results);
      REQUIRE(results.front() == expected_result[i - 1]);
    }
  }

  SECTION("Test Follows* relationship") {
    std::vector<std::list<std::string>> expected_result = {{"2", "3", "4", "8", "12"}, {"3", "8", "4", "12"},
                                                           {"8", "4", "12"}, {"8", "12"}, {"6", "7"}, {"7"}, {},
                                                           {"12"}, {}, {"11"}, {}, {}};
    for (int i = 1; i < 13; i++) {
      std::string query = "stmt s ; Select s such that Follows*(" + std::to_string(i) + " ,s)";
      PqlLexer lexer{query};
      ParsedQueryBuilder pqb;
      std::vector<PqlToken> tokens = lexer.Lex();
      QueryValidator query_validator = QueryValidator(tokens);
      std::vector<PqlToken> validated_tokens = query_validator.CheckValidation();
      ParsedQuery parsed_query = pqb.Build(validated_tokens);
      std::list<std::string> results;
      evaluator->Evaluate(parsed_query, results);
      REQUIRE(results == expected_result[i - 1]);
    }
  }
}


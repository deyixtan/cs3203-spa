#include "catch.hpp"
#include "components/pkb/pkb.h"
#include "components/pkb/design_extractor/design_extractor.h"
#include "components/source_subsystem/source_lexer.h"
#include "components/source_subsystem/source_parser.h"
#include "components/source_subsystem/exceptions/empty_statement_list.h"

using namespace source;

extern std::string sample_source1;

std::shared_ptr<ProgramNode> GenerateAbstractSyntaxTree(std::string source) {
  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;

  SourceLexer lexer = SourceLexer(source);
  lexer.Tokenize(tokens_ptr);

  SourceParser parser = SourceParser(tokens_ptr);
  std::shared_ptr<ProgramNode> ast = parser.ParseProgram();
  return ast;
}

PKB *GetPopulatedPkbInstance(std::shared_ptr<ProgramNode> ast) {
  PKB *pkb = PKB::GetInstance();
  DesignExtractor *design_extractor = new DesignExtractor(*ast, pkb);
  design_extractor->TraverseAst();
  return pkb;
}

TEST_CASE("Test components between Source and PKB (Sample source 1)") {
  std::shared_ptr<ProgramNode> ast = GenerateAbstractSyntaxTree(sample_source1);
  PKB *pkb = GetPopulatedPkbInstance(ast);

  SECTION("Test read statement count and their statement number") {
    std::unordered_set<std::string> result = pkb->GetStmt(StmtType::READ);

    REQUIRE(result.size() == 0);
  }

  SECTION("Test print statement count and their statement number") {
    std::unordered_set<std::string> result = pkb->GetStmt(StmtType::PRINT);

    REQUIRE(result.size() == 0);
  }

  SECTION("Test while-statement count and their statement number") {
    std::unordered_set<std::string> result = pkb->GetStmt(StmtType::WHILE);
    std::unordered_set<std::string> expected_result = {"4"};

    REQUIRE(result.size() == 1);
    REQUIRE(result == expected_result);
  }

  SECTION("Test if-statement count and their statement number") {
    std::unordered_set<std::string> result = pkb->GetStmt(StmtType::IF);
    std::unordered_set<std::string> expected_result = {"8"};

    REQUIRE(result.size() == 1);
    REQUIRE(result == expected_result);
  }

  SECTION("Test assign statement count and their statement number") {
    std::unordered_set<std::string> result = pkb->GetStmt(StmtType::ASSIGN);
    std::unordered_set<std::string> expected_result = {"1", "2", "3", "5", "6", "7", "9", "10", "11", "12"};

    REQUIRE(result.size() == 10);
    REQUIRE(result == expected_result);
  }

  SECTION("Test Follows relationship") {
    std::string result1 = pkb->GetFollowOf("1");
    std::string result2 = pkb->GetFollowOf("2");
    std::string result3 = pkb->GetFollowOf("3");
    std::string result4 = pkb->GetFollowOf("4");
    std::string result5 = pkb->GetFollowOf("5");
    std::string result6 = pkb->GetFollowOf("6");
    std::string result7 = pkb->GetFollowOf("7");
    std::string result8 = pkb->GetFollowOf("8");
    std::string result9 = pkb->GetFollowOf("9");
    std::string result10 = pkb->GetFollowOf("10");
    std::string result11 = pkb->GetFollowOf("11");
    std::string result12 = pkb->GetFollowOf("12");

    std::string expected_result1 = "2";
    std::string expected_result2 = "3";
    std::string expected_result3 = "4";
    std::string expected_result4 = "8";
    std::string expected_result5 = "6";
    std::string expected_result6 = "7";
    std::string expected_result7 = "0";
    std::string expected_result8 = "12";
    std::string expected_result9 = "0";
    std::string expected_result10 = "11";
    std::string expected_result11 = "0";
    std::string expected_result12 = "0";

    REQUIRE(result1 == expected_result1);
    REQUIRE(result2 == expected_result2);
    REQUIRE(result3 == expected_result3);
    REQUIRE(result4 == expected_result4);
    REQUIRE(result5 == expected_result5);
    REQUIRE(result6 == expected_result6);
    REQUIRE(result7 == expected_result7);
    REQUIRE(result8 == expected_result8);
    REQUIRE(result9 == expected_result9);
    REQUIRE(result10 == expected_result10);
    REQUIRE(result11 == expected_result11);
    REQUIRE(result12 == expected_result12);
  }

  SECTION("Test Follows* relationship") {
    std::unordered_set<std::string> result1 = pkb->GetFollowStarOf("1");
    std::unordered_set<std::string> result2 = pkb->GetFollowStarOf("2");
    std::unordered_set<std::string> result3 = pkb->GetFollowStarOf("3");
    std::unordered_set<std::string> result4 = pkb->GetFollowStarOf("4");
    std::unordered_set<std::string> result5 = pkb->GetFollowStarOf("5");
    std::unordered_set<std::string> result6 = pkb->GetFollowStarOf("6");
    std::unordered_set<std::string> result7 = pkb->GetFollowStarOf("7");
    std::unordered_set<std::string> result8 = pkb->GetFollowStarOf("8");
    std::unordered_set<std::string> result9 = pkb->GetFollowStarOf("9");
    std::unordered_set<std::string> result10 = pkb->GetFollowStarOf("10");
    std::unordered_set<std::string> result11 = pkb->GetFollowStarOf("11");
    std::unordered_set<std::string> result12 = pkb->GetFollowStarOf("12");

    std::unordered_set<std::string> expected_result1 = {"2", "3", "4", "8", "12"};
    std::unordered_set<std::string> expected_result2 = {"3", "4", "8", "12"};
    std::unordered_set<std::string> expected_result3 = {"4", "8", "12"};
    std::unordered_set<std::string> expected_result4 = {"8", "12"};
    std::unordered_set<std::string> expected_result5 = {"6", "7"};
    std::unordered_set<std::string> expected_result6 = {"7"};
    std::unordered_set<std::string> expected_result7 = {};
    std::unordered_set<std::string> expected_result8 = {"12"};
    std::unordered_set<std::string> expected_result9 = {};
    std::unordered_set<std::string> expected_result10 = {"11"};
    std::unordered_set<std::string> expected_result11 = {};
    std::unordered_set<std::string> expected_result12 = {};

    REQUIRE(result1 == expected_result1);
    REQUIRE(result2 == expected_result2);
    REQUIRE(result3 == expected_result3);
    REQUIRE(result4 == expected_result4);
    REQUIRE(result5 == expected_result5);
    REQUIRE(result6 == expected_result6);
    REQUIRE(result7 == expected_result7);
    REQUIRE(result8 == expected_result8);
    REQUIRE(result9 == expected_result9);
    REQUIRE(result10 == expected_result10);
    REQUIRE(result11 == expected_result11);
    REQUIRE(result12 == expected_result12);
  }

//  SECTION("Test Parents relationship") {
//    std::string result1 = pkb->GetParentOf("1");
//    std::string result2 = pkb->GetParentOf("2");
//    std::string result3 = pkb->GetParentOf("3");
//    std::string result4 = pkb->GetParentOf("4");
//    std::string result5 = pkb->GetParentOf("5");
//    std::string result6 = pkb->GetParentOf("6");
//    std::string result7 = pkb->GetParentOf("7");
//    std::string result8 = pkb->GetParentOf("8");
//    std::string result9 = pkb->GetParentOf("9");
//    std::string result10 = pkb->GetParentOf("10");
//    std::string result11 = pkb->GetParentOf("11");
//    std::string result12 = pkb->GetParentOf("12");
//
//    std::string expected_result1 = "0";
//    std::string expected_result2 = "0";
//    std::string expected_result3 = "0";
//    std::string expected_result4 = "0";
//    std::string expected_result5 = "4";
//    std::string expected_result6 = "4";
//    std::string expected_result7 = "4";
//    std::string expected_result8 = "0";
//    std::string expected_result9 = "8";
//    std::string expected_result10 = "8";
//    std::string expected_result11 = "8";
//    std::string expected_result12 = "0";
//
//    REQUIRE(result1 == expected_result1);
//    REQUIRE(result2 == expected_result2);
//    REQUIRE(result3 == expected_result3);
//    REQUIRE(result4 == expected_result4);
//    REQUIRE(result5 == expected_result5);
//    REQUIRE(result6 == expected_result6);
//    REQUIRE(result7 == expected_result7);
//    REQUIRE(result8 == expected_result8);
//    REQUIRE(result9 == expected_result9);
//    REQUIRE(result10 == expected_result10);
//    REQUIRE(result11 == expected_result11);
//    REQUIRE(result12 == expected_result12);
//  }
//
//  SECTION("Test Parents* relationship") {
//    std::unordered_set<std::string> result1 = pkb->GetParentStarOf("1");
//    std::unordered_set<std::string> result2 = pkb->GetParentStarOf("2");
//    std::unordered_set<std::string> result3 = pkb->GetParentStarOf("3");
//    std::unordered_set<std::string> result4 = pkb->GetParentStarOf("4");
//    std::unordered_set<std::string> result5 = pkb->GetParentStarOf("5");
//    std::unordered_set<std::string> result6 = pkb->GetParentStarOf("6");
//    std::unordered_set<std::string> result7 = pkb->GetParentStarOf("7");
//    std::unordered_set<std::string> result8 = pkb->GetParentStarOf("8");
//    std::unordered_set<std::string> result9 = pkb->GetParentStarOf("9");
//    std::unordered_set<std::string> result10 = pkb->GetParentStarOf("10");
//    std::unordered_set<std::string> result11 = pkb->GetParentStarOf("11");
//    std::unordered_set<std::string> result12 = pkb->GetParentStarOf("12");
//
//    std::unordered_set<std::string> expected_result1 = {};
//    std::unordered_set<std::string> expected_result2 = {};
//    std::unordered_set<std::string> expected_result3 = {};
//    std::unordered_set<std::string> expected_result4 = {};
//    std::unordered_set<std::string> expected_result5 = {"4"};
//    std::unordered_set<std::string> expected_result6 = {"4"};
//    std::unordered_set<std::string> expected_result7 = {"4"};
//    std::unordered_set<std::string> expected_result8 = {};
//    std::unordered_set<std::string> expected_result9 = {"8"};
//    std::unordered_set<std::string> expected_result10 = {"8"};
//    std::unordered_set<std::string> expected_result11 = {"8"};
//    std::unordered_set<std::string> expected_result12 = {};
//
//    REQUIRE(result1 == expected_result1);
//    REQUIRE(result2 == expected_result2);
//    REQUIRE(result3 == expected_result3);
//    REQUIRE(result4 == expected_result4);
//    REQUIRE(result5 == expected_result5);
//    REQUIRE(result6 == expected_result6);
//    REQUIRE(result7 == expected_result7);
//    REQUIRE(result8 == expected_result8);
//    REQUIRE(result9 == expected_result9);
//    REQUIRE(result10 == expected_result10);
//    REQUIRE(result11 == expected_result11);
//    REQUIRE(result12 == expected_result12);
//  }
}

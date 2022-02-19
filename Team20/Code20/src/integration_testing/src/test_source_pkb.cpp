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
    std::string result1 = pkb->GetFollowingOf("1");
    std::string result2 = pkb->GetFollowingOf("2");
    std::string result3 = pkb->GetFollowingOf("3");
    std::string result4 = pkb->GetFollowingOf("4");
    std::string result5 = pkb->GetFollowingOf("5");
    std::string result6 = pkb->GetFollowingOf("6");
    std::string result7 = pkb->GetFollowingOf("7");
    std::string result8 = pkb->GetFollowingOf("8");
    std::string result9 = pkb->GetFollowingOf("9");
    std::string result10 = pkb->GetFollowingOf("10");
    std::string result11 = pkb->GetFollowingOf("11");
    std::string result12 = pkb->GetFollowingOf("12");

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
    std::unordered_set<std::string> result1 = pkb->GetFollowingStarOf("1");
    std::unordered_set<std::string> result2 = pkb->GetFollowingStarOf("2");
    std::unordered_set<std::string> result3 = pkb->GetFollowingStarOf("3");
    std::unordered_set<std::string> result4 = pkb->GetFollowingStarOf("4");
    std::unordered_set<std::string> result5 = pkb->GetFollowingStarOf("5");
    std::unordered_set<std::string> result6 = pkb->GetFollowingStarOf("6");
    std::unordered_set<std::string> result7 = pkb->GetFollowingStarOf("7");
    std::unordered_set<std::string> result8 = pkb->GetFollowingStarOf("8");
    std::unordered_set<std::string> result9 = pkb->GetFollowingStarOf("9");
    std::unordered_set<std::string> result10 = pkb->GetFollowingStarOf("10");
    std::unordered_set<std::string> result11 = pkb->GetFollowingStarOf("11");
    std::unordered_set<std::string> result12 = pkb->GetFollowingStarOf("12");

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

  SECTION("Test Parents relationship") {
    std::string result1 = pkb->GetParentOf("1");
    std::string result2 = pkb->GetParentOf("2");
    std::string result3 = pkb->GetParentOf("3");
    std::string result4 = pkb->GetParentOf("4");
    std::string result5 = pkb->GetParentOf("5");
    std::string result6 = pkb->GetParentOf("6");
    std::string result7 = pkb->GetParentOf("7");
    std::string result8 = pkb->GetParentOf("8");
    std::string result9 = pkb->GetParentOf("9");
    std::string result10 = pkb->GetParentOf("10");
    std::string result11 = pkb->GetParentOf("11");
    std::string result12 = pkb->GetParentOf("12");

    std::string expected_result1 = "0";
    std::string expected_result2 = "0";
    std::string expected_result3 = "0";
    std::string expected_result4 = "0";
    std::string expected_result5 = "4";
    std::string expected_result6 = "4";
    std::string expected_result7 = "4";
    std::string expected_result8 = "0";
    std::string expected_result9 = "8";
    std::string expected_result10 = "8";
    std::string expected_result11 = "8";
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

  SECTION("Test Parents* relationship") {
    std::unordered_set<std::string> result1 = pkb->GetAnceOf("1");
    std::unordered_set<std::string> result2 = pkb->GetAnceOf("2");
    std::unordered_set<std::string> result3 = pkb->GetAnceOf("3");
    std::unordered_set<std::string> result4 = pkb->GetAnceOf("4");
    std::unordered_set<std::string> result5 = pkb->GetAnceOf("5");
    std::unordered_set<std::string> result6 = pkb->GetAnceOf("6");
    std::unordered_set<std::string> result7 = pkb->GetAnceOf("7");
    std::unordered_set<std::string> result8 = pkb->GetAnceOf("8");
    std::unordered_set<std::string> result9 = pkb->GetAnceOf("9");
    std::unordered_set<std::string> result10 = pkb->GetAnceOf("10");
    std::unordered_set<std::string> result11 = pkb->GetAnceOf("11");
    std::unordered_set<std::string> result12 = pkb->GetAnceOf("12");

    std::unordered_set<std::string> expected_result1 = {};
    std::unordered_set<std::string> expected_result2 = {};
    std::unordered_set<std::string> expected_result3 = {};
    std::unordered_set<std::string> expected_result4 = {};
    std::unordered_set<std::string> expected_result5 = {"4"};
    std::unordered_set<std::string> expected_result6 = {"4"};
    std::unordered_set<std::string> expected_result7 = {"4"};
    std::unordered_set<std::string> expected_result8 = {};
    std::unordered_set<std::string> expected_result9 = {"8"};
    std::unordered_set<std::string> expected_result10 = {"8"};
    std::unordered_set<std::string> expected_result11 = {"8"};
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

  SECTION("Test Uses relationship for variables") {
    std::unordered_set<std::string> result1 = pkb->GetVarUsedByStmt("1");
    std::unordered_set<std::string> result2 = pkb->GetVarUsedByStmt("2");
    std::unordered_set<std::string> result3 = pkb->GetVarUsedByStmt("3");
    std::unordered_set<std::string> result4 = pkb->GetVarUsedByStmt("4");
    std::unordered_set<std::string> result5 = pkb->GetVarUsedByStmt("5");
    std::unordered_set<std::string> result6 = pkb->GetVarUsedByStmt("6");
    std::unordered_set<std::string> result7 = pkb->GetVarUsedByStmt("7");
    std::unordered_set<std::string> result8 = pkb->GetVarUsedByStmt("8");
    std::unordered_set<std::string> result9 = pkb->GetVarUsedByStmt("9");
    std::unordered_set<std::string> result10 = pkb->GetVarUsedByStmt("10");
    std::unordered_set<std::string> result11 = pkb->GetVarUsedByStmt("11");
    std::unordered_set<std::string> result12 = pkb->GetVarUsedByStmt("12");

    std::unordered_set<std::string> expected_result1 = {};
    std::unordered_set<std::string> expected_result2 = {};
    std::unordered_set<std::string> expected_result3 = {};
    std::unordered_set<std::string> expected_result4 = {"x", "y", "count", "cenX", "x", "cenY", "y"};
    std::unordered_set<std::string> expected_result5 = {"count"};
    std::unordered_set<std::string> expected_result6 = {"cenX", "x"};
    std::unordered_set<std::string> expected_result7 = {"cenY", "y"};
    std::unordered_set<std::string> expected_result8 = {"count", "cenX", "cenY"};
    std::unordered_set<std::string> expected_result9 = {};
    std::unordered_set<std::string> expected_result10 = {"cenX", "count"};
    std::unordered_set<std::string> expected_result11 = {"cenY", "count"};
    std::unordered_set<std::string> expected_result12 = {"cenX", "cenY"};

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

  SECTION("Test Uses relationship for statement number") {
    std::unordered_set<std::string> result1 = pkb->GetStmtUsedByVar("count");
    std::unordered_set<std::string> result2 = pkb->GetStmtUsedByVar("cenX");
    std::unordered_set<std::string> result3 = pkb->GetStmtUsedByVar("cenY");
    std::unordered_set<std::string> result4 = pkb->GetStmtUsedByVar("x");
    std::unordered_set<std::string> result5 = pkb->GetStmtUsedByVar("y");
    std::unordered_set<std::string> result6 = pkb->GetStmtUsedByVar("flag");
    std::unordered_set<std::string> result7 = pkb->GetStmtUsedByVar("normSq");

    std::unordered_set<std::string> expected_result1 = {"4", "5", "8", "10", "11"};
    std::unordered_set<std::string> expected_result2 = {"4", "8", "6", "10", "12"};
    std::unordered_set<std::string> expected_result3 = {"4", "8", "7", "11", "12"};
    std::unordered_set<std::string> expected_result4 = {"4", "6"};
    std::unordered_set<std::string> expected_result5 = {"4", "7"};
    std::unordered_set<std::string> expected_result6 = {};
    std::unordered_set<std::string> expected_result7 = {};

    REQUIRE(result1 == expected_result1);
    REQUIRE(result2 == expected_result2);
    REQUIRE(result3 == expected_result3);
    REQUIRE(result4 == expected_result4);
    REQUIRE(result5 == expected_result5);
    REQUIRE(result6 == expected_result6);
    REQUIRE(result7 == expected_result7);
  }

  SECTION("Test Modifies relationship for variables") {
    std::unordered_set<std::string> result1 = pkb->GetVarModByStmt("1");
    std::unordered_set<std::string> result2 = pkb->GetVarModByStmt("2");
    std::unordered_set<std::string> result3 = pkb->GetVarModByStmt("3");
    std::unordered_set<std::string> result4 = pkb->GetVarModByStmt("4");
    std::unordered_set<std::string> result5 = pkb->GetVarModByStmt("5");
    std::unordered_set<std::string> result6 = pkb->GetVarModByStmt("6");
    std::unordered_set<std::string> result7 = pkb->GetVarModByStmt("7");
    std::unordered_set<std::string> result8 = pkb->GetVarModByStmt("8");
    std::unordered_set<std::string> result9 = pkb->GetVarModByStmt("9");
    std::unordered_set<std::string> result10 = pkb->GetVarModByStmt("10");
    std::unordered_set<std::string> result11 = pkb->GetVarModByStmt("11");
    std::unordered_set<std::string> result12 = pkb->GetVarModByStmt("12");

    std::unordered_set<std::string> expected_result1 = {"count"};
    std::unordered_set<std::string> expected_result2 = {"cenX"};
    std::unordered_set<std::string> expected_result3 = {"cenY"};
    std::unordered_set<std::string> expected_result4 = {"count", "cenX", "cenY"};
    std::unordered_set<std::string> expected_result5 = {"count"};
    std::unordered_set<std::string> expected_result6 = {"cenX"};
    std::unordered_set<std::string> expected_result7 = {"cenY"};
    std::unordered_set<std::string> expected_result8 = {"flag", "cenY", "cenX"};
    std::unordered_set<std::string> expected_result9 = {"flag"};
    std::unordered_set<std::string> expected_result10 = {"cenX"};
    std::unordered_set<std::string> expected_result11 = {"cenY"};
    std::unordered_set<std::string> expected_result12 = {"normSq"};

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

  SECTION("Test Modifies relationship for statement number") {
    std::unordered_set<std::string> result1 = pkb->GetStmtModByVar("count");
    std::unordered_set<std::string> result2 = pkb->GetStmtModByVar("cenX");
    std::unordered_set<std::string> result3 = pkb->GetStmtModByVar("cenY");
    std::unordered_set<std::string> result4 = pkb->GetStmtModByVar("x");
    std::unordered_set<std::string> result5 = pkb->GetStmtModByVar("y");
    std::unordered_set<std::string> result6 = pkb->GetStmtModByVar("flag");
    std::unordered_set<std::string> result7 = pkb->GetStmtModByVar("normSq");

    std::unordered_set<std::string> expected_result1 = {"1", "4", "5"};
    std::unordered_set<std::string> expected_result2 = {"2", "4", "8", "6", "10"};
    std::unordered_set<std::string> expected_result3 = {"4", "8", "3", "7", "11"};
    std::unordered_set<std::string> expected_result4 = {};
    std::unordered_set<std::string> expected_result5 = {};
    std::unordered_set<std::string> expected_result6 = {"8", "9"};
    std::unordered_set<std::string> expected_result7 = {"12"};

    REQUIRE(result1 == expected_result1);
    REQUIRE(result2 == expected_result2);
    REQUIRE(result3 == expected_result3);
    REQUIRE(result4 == expected_result4);
    REQUIRE(result5 == expected_result5);
    REQUIRE(result6 == expected_result6);
    REQUIRE(result7 == expected_result7);
  }
}

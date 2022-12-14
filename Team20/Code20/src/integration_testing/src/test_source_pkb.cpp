#include "catch.hpp"
#include "components/pkb/pkb.h"
#include "components/source_subsystem/iterator/design_extractor.h"
#include "components/source_subsystem/lexer/source_lexer.h"
#include "components/source_subsystem/parser/source_parser.h"
#include "components/source_subsystem/exceptions/empty_statement_list.h"
#include "components/source_subsystem/iterator/cfg_builder.h"
#include "components/pkb/pkb_client/pkb_client.h"
#include "components/source_subsystem/exceptions/cyclic_call.h"
#include "components/source_subsystem/exceptions/invalid_call.h"
#include "components/pkb/stores/uses_store/uses_store.h"
#include "components/pkb/stores/modifies_store/modifies_store.h"
#include "components/pkb/stores/follows_store/follows_store.h"
#include "components/pkb/stores/parent_store/parent_store.h"
#include "components/pkb/stores/pattern_store/pattern_store.h"
#include "components/pkb/stores/call_store/call_store.h"
#include "components/pkb/stores/next_store/next_store.h"

using namespace source;
using namespace pkb;

extern std::string sample_source1;
extern std::string sample_source2;
extern std::string sample_source3;
extern std::string sample_source4;
extern std::string sample_source5;
extern std::string sample_source6;
extern std::string sample_source7;

std::shared_ptr<ProgramNode> GenerateAbstractSyntaxTree(std::string source) {
  std::vector<std::shared_ptr<SourceToken>> tokens_ptr;

  SourceLexer lexer = SourceLexer(source);
  lexer.Tokenize(tokens_ptr);

  SourceParser parser = SourceParser(tokens_ptr);
  std::shared_ptr<ProgramNode> ast = parser.ParseProgram();
  return ast;
}

pkb::PkbPtr GetPopulatedPkbInstance(std::shared_ptr<ProgramNode> ast) {
  PkbPtr pkb = std::make_shared<PKB>();

  std::shared_ptr<PkbClient> pkb_client = std::make_shared<PkbClient>(pkb);
  DesignExtractorPtr design_extractor = std::make_shared<DesignExtractor>(pkb_client);
  design_extractor->IterateAst(ast);
  CfgBuilderPtr cfg_builder = std::make_shared<CfgBuilder>(pkb_client);
  cfg_builder->IterateAst(ast);
  return pkb;
}

PkbPtr GetCfgPopulatedPkbInstance(std::shared_ptr<ProgramNode> ast) {
  PkbPtr pkb = std::make_shared<PKB>();

  std::shared_ptr<PkbClient> pkb_client = std::make_shared<PkbClient>(pkb);
  DesignExtractorPtr design_extractor = std::make_shared<DesignExtractor>(pkb_client);
  CfgBuilderPtr cfg_builder = std::make_shared<CfgBuilder>(pkb_client);
  cfg_builder->IterateAst(ast);
  design_extractor->IterateAst(ast);
  cfg_builder->IterateCfg();
  return pkb;
}

TEST_CASE("Test components between Source and PKB (Sample source 1)") {
  std::shared_ptr<ProgramNode> ast = GenerateAbstractSyntaxTree(sample_source1);
  PkbPtr pkb = GetPopulatedPkbInstance(ast);

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
    std::string result1 = pkb->GetFollowsStore()->GetFollowingOf(STMT, "1");
    std::string result2 = pkb->GetFollowsStore()->GetFollowingOf(STMT, "2");
    std::string result3 = pkb->GetFollowsStore()->GetFollowingOf(STMT, "3");
    std::string result4 = pkb->GetFollowsStore()->GetFollowingOf(STMT, "4");
    std::string result5 = pkb->GetFollowsStore()->GetFollowingOf(STMT, "5");
    std::string result6 = pkb->GetFollowsStore()->GetFollowingOf(STMT, "6");
    std::string result7 = pkb->GetFollowsStore()->GetFollowingOf(STMT, "7");
    std::string result8 = pkb->GetFollowsStore()->GetFollowingOf(STMT, "8");
    std::string result9 = pkb->GetFollowsStore()->GetFollowingOf(STMT, "9");
    std::string result10 = pkb->GetFollowsStore()->GetFollowingOf(STMT, "10");
    std::string result11 = pkb->GetFollowsStore()->GetFollowingOf(STMT, "11");
    std::string result12 = pkb->GetFollowsStore()->GetFollowingOf(STMT, "12");

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
    std::unordered_set<std::string> result1 = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "1");
    std::unordered_set<std::string> result2 = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "2");
    std::unordered_set<std::string> result3 = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "3");
    std::unordered_set<std::string> result4 = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "4");
    std::unordered_set<std::string> result5 = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "5");
    std::unordered_set<std::string> result6 = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "6");
    std::unordered_set<std::string> result7 = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "7");
    std::unordered_set<std::string> result8 = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "8");
    std::unordered_set<std::string> result9 = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "9");
    std::unordered_set<std::string> result10 = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "10");
    std::unordered_set<std::string> result11 = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "11");
    std::unordered_set<std::string> result12 = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "12");

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

  SECTION("Test Parents relationship 1") {
    std::string result1 = pkb->GetParentStore()->GetParentOf(STMT, "1");
    std::string result2 = pkb->GetParentStore()->GetParentOf(STMT, "2");
    std::string result3 = pkb->GetParentStore()->GetParentOf(STMT, "3");
    std::string result4 = pkb->GetParentStore()->GetParentOf(STMT, "4");
    std::string result5 = pkb->GetParentStore()->GetParentOf(STMT, "5");
    std::string result6 = pkb->GetParentStore()->GetParentOf(STMT, "6");
    std::string result7 = pkb->GetParentStore()->GetParentOf(STMT, "7");
    std::string result8 = pkb->GetParentStore()->GetParentOf(STMT, "8");
    std::string result9 = pkb->GetParentStore()->GetParentOf(STMT, "9");
    std::string result10 = pkb->GetParentStore()->GetParentOf(STMT, "10");
    std::string result11 = pkb->GetParentStore()->GetParentOf(STMT, "11");
    std::string result12 = pkb->GetParentStore()->GetParentOf(STMT, "12");

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

  SECTION("Test Parents relationship 2") {
    std::unordered_set<std::string> result1 = pkb->GetParentStore()->GetChildOf(STMT, "1");
    std::unordered_set<std::string> result2 = pkb->GetParentStore()->GetChildOf(STMT, "2");
    std::unordered_set<std::string> result3 = pkb->GetParentStore()->GetChildOf(STMT, "3");
    std::unordered_set<std::string> result4 = pkb->GetParentStore()->GetChildOf(STMT, "4");
    std::unordered_set<std::string> result5 = pkb->GetParentStore()->GetChildOf(STMT, "5");
    std::unordered_set<std::string> result6 = pkb->GetParentStore()->GetChildOf(STMT, "6");
    std::unordered_set<std::string> result7 = pkb->GetParentStore()->GetChildOf(STMT, "7");
    std::unordered_set<std::string> result8 = pkb->GetParentStore()->GetChildOf(STMT, "8");
    std::unordered_set<std::string> result9 = pkb->GetParentStore()->GetChildOf(STMT, "9");
    std::unordered_set<std::string> result10 = pkb->GetParentStore()->GetChildOf(STMT, "10");
    std::unordered_set<std::string> result11 = pkb->GetParentStore()->GetChildOf(STMT, "11");
    std::unordered_set<std::string> result12 = pkb->GetParentStore()->GetChildOf(STMT, "12");

    std::unordered_set<std::string> expected_result1 = {};
    std::unordered_set<std::string> expected_result2 = {};
    std::unordered_set<std::string> expected_result3 = {};
    std::unordered_set<std::string> expected_result4 = {"5", "6", "7"};
    std::unordered_set<std::string> expected_result5 = {};
    std::unordered_set<std::string> expected_result6 = {};
    std::unordered_set<std::string> expected_result7 = {};
    std::unordered_set<std::string> expected_result8 = {"9", "10", "11"};
    std::unordered_set<std::string> expected_result9 = {};
    std::unordered_set<std::string> expected_result10 = {};
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

  SECTION("Test Parents* relationship 1") {
    std::unordered_set<std::string> result1 = pkb->GetParentStore()->GetAllAnceOf(STMT, "1");
    std::unordered_set<std::string> result2 = pkb->GetParentStore()->GetAllAnceOf(STMT, "2");
    std::unordered_set<std::string> result3 = pkb->GetParentStore()->GetAllAnceOf(STMT, "3");
    std::unordered_set<std::string> result4 = pkb->GetParentStore()->GetAllAnceOf(STMT, "4");
    std::unordered_set<std::string> result5 = pkb->GetParentStore()->GetAllAnceOf(STMT, "5");
    std::unordered_set<std::string> result6 = pkb->GetParentStore()->GetAllAnceOf(STMT, "6");
    std::unordered_set<std::string> result7 = pkb->GetParentStore()->GetAllAnceOf(STMT, "7");
    std::unordered_set<std::string> result8 = pkb->GetParentStore()->GetAllAnceOf(STMT, "8");
    std::unordered_set<std::string> result9 = pkb->GetParentStore()->GetAllAnceOf(STMT, "9");
    std::unordered_set<std::string> result10 = pkb->GetParentStore()->GetAllAnceOf(STMT, "10");
    std::unordered_set<std::string> result11 = pkb->GetParentStore()->GetAllAnceOf(STMT, "11");
    std::unordered_set<std::string> result12 = pkb->GetParentStore()->GetAllAnceOf(STMT, "12");

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

  SECTION("Test Parents* relationship 2") {
    std::unordered_set<std::string> result1 = pkb->GetParentStore()->GetAllDescOf(STMT, "1");
    std::unordered_set<std::string> result2 = pkb->GetParentStore()->GetAllDescOf(STMT, "2");
    std::unordered_set<std::string> result3 = pkb->GetParentStore()->GetAllDescOf(STMT, "3");
    std::unordered_set<std::string> result4 = pkb->GetParentStore()->GetAllDescOf(STMT, "4");
    std::unordered_set<std::string> result5 = pkb->GetParentStore()->GetAllDescOf(STMT, "5");
    std::unordered_set<std::string> result6 = pkb->GetParentStore()->GetAllDescOf(STMT, "6");
    std::unordered_set<std::string> result7 = pkb->GetParentStore()->GetAllDescOf(STMT, "7");
    std::unordered_set<std::string> result8 = pkb->GetParentStore()->GetAllDescOf(STMT, "8");
    std::unordered_set<std::string> result9 = pkb->GetParentStore()->GetAllDescOf(STMT, "9");
    std::unordered_set<std::string> result10 = pkb->GetParentStore()->GetAllDescOf(STMT, "10");
    std::unordered_set<std::string> result11 = pkb->GetParentStore()->GetAllDescOf(STMT, "11");
    std::unordered_set<std::string> result12 = pkb->GetParentStore()->GetAllDescOf(STMT, "12");

    std::unordered_set<std::string> expected_result1 = {};
    std::unordered_set<std::string> expected_result2 = {};
    std::unordered_set<std::string> expected_result3 = {};
    std::unordered_set<std::string> expected_result4 = {"5", "6", "7"};
    std::unordered_set<std::string> expected_result5 = {};
    std::unordered_set<std::string> expected_result6 = {};
    std::unordered_set<std::string> expected_result7 = {};
    std::unordered_set<std::string> expected_result8 = {"9", "10", "11"};
    std::unordered_set<std::string> expected_result9 = {};
    std::unordered_set<std::string> expected_result10 = {};
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

  SECTION("Test Uses relationship for variables") {
    std::unordered_set<std::string> result1 = pkb->GetUsesStore()->GetVarUsedByStmt("1");
    std::unordered_set<std::string> result2 = pkb->GetUsesStore()->GetVarUsedByStmt("2");
    std::unordered_set<std::string> result3 = pkb->GetUsesStore()->GetVarUsedByStmt("3");
    std::unordered_set<std::string> result4 = pkb->GetUsesStore()->GetVarUsedByStmt("4");
    std::unordered_set<std::string> result5 = pkb->GetUsesStore()->GetVarUsedByStmt("5");
    std::unordered_set<std::string> result6 = pkb->GetUsesStore()->GetVarUsedByStmt("6");
    std::unordered_set<std::string> result7 = pkb->GetUsesStore()->GetVarUsedByStmt("7");
    std::unordered_set<std::string> result8 = pkb->GetUsesStore()->GetVarUsedByStmt("8");
    std::unordered_set<std::string> result9 = pkb->GetUsesStore()->GetVarUsedByStmt("9");
    std::unordered_set<std::string> result10 = pkb->GetUsesStore()->GetVarUsedByStmt("10");
    std::unordered_set<std::string> result11 = pkb->GetUsesStore()->GetVarUsedByStmt("11");
    std::unordered_set<std::string> result12 = pkb->GetUsesStore()->GetVarUsedByStmt("12");

    std::unordered_set<std::string> expected_result1 = {};
    std::unordered_set<std::string> expected_result2 = {};
    std::unordered_set<std::string> expected_result3 = {};
    std::unordered_set<std::string> expected_result4 = {"x", "y", "count", "cenX", "cenY"};
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
    std::unordered_set<std::string> result1 = pkb->GetUsesStore()->GetStmtUsedByVar(STMT, "count");
    std::unordered_set<std::string> result2 = pkb->GetUsesStore()->GetStmtUsedByVar(STMT, "cenX");
    std::unordered_set<std::string> result3 = pkb->GetUsesStore()->GetStmtUsedByVar(STMT, "cenY");
    std::unordered_set<std::string> result4 = pkb->GetUsesStore()->GetStmtUsedByVar(STMT, "x");
    std::unordered_set<std::string> result5 = pkb->GetUsesStore()->GetStmtUsedByVar(STMT, "y");
    std::unordered_set<std::string> result6 = pkb->GetUsesStore()->GetStmtUsedByVar(STMT, "flag");
    std::unordered_set<std::string> result7 = pkb->GetUsesStore()->GetStmtUsedByVar(STMT, "normSq");

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
    std::unordered_set<std::string> result1 = pkb->GetModifiesStore()->GetVarModByStmt("1");
    std::unordered_set<std::string> result2 = pkb->GetModifiesStore()->GetVarModByStmt("2");
    std::unordered_set<std::string> result3 = pkb->GetModifiesStore()->GetVarModByStmt("3");
    std::unordered_set<std::string> result4 = pkb->GetModifiesStore()->GetVarModByStmt("4");
    std::unordered_set<std::string> result5 = pkb->GetModifiesStore()->GetVarModByStmt("5");
    std::unordered_set<std::string> result6 = pkb->GetModifiesStore()->GetVarModByStmt("6");
    std::unordered_set<std::string> result7 = pkb->GetModifiesStore()->GetVarModByStmt("7");
    std::unordered_set<std::string> result8 = pkb->GetModifiesStore()->GetVarModByStmt("8");
    std::unordered_set<std::string> result9 = pkb->GetModifiesStore()->GetVarModByStmt("9");
    std::unordered_set<std::string> result10 = pkb->GetModifiesStore()->GetVarModByStmt("10");
    std::unordered_set<std::string> result11 = pkb->GetModifiesStore()->GetVarModByStmt("11");
    std::unordered_set<std::string> result12 = pkb->GetModifiesStore()->GetVarModByStmt("12");

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
    std::unordered_set<std::string> result1 = pkb->GetModifiesStore()->GetStmtModByVar(STMT, "count");
    std::unordered_set<std::string> result2 = pkb->GetModifiesStore()->GetStmtModByVar(STMT, "cenX");
    std::unordered_set<std::string> result3 = pkb->GetModifiesStore()->GetStmtModByVar(STMT, "cenY");
    std::unordered_set<std::string> result4 = pkb->GetModifiesStore()->GetStmtModByVar(STMT, "x");
    std::unordered_set<std::string> result5 = pkb->GetModifiesStore()->GetStmtModByVar(STMT,"y");
    std::unordered_set<std::string> result6 = pkb->GetModifiesStore()->GetStmtModByVar(STMT, "flag");
    std::unordered_set<std::string> result7 = pkb->GetModifiesStore()->GetStmtModByVar(STMT, "normSq");

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

TEST_CASE("Test components between Source and PKB (Sample source 2)") {
  std::shared_ptr<ProgramNode> ast = GenerateAbstractSyntaxTree(sample_source2);
  PkbPtr pkb = GetPopulatedPkbInstance(ast);

  SECTION("Test read statement count and their statement number") {
    std::unordered_set<std::string> result = pkb->GetStmt(StmtType::READ);

    REQUIRE(result.size() == 2);
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
    std::unordered_set<std::string> expected_result = {"3", "8"};

    REQUIRE(result.size() == 2);
    REQUIRE(result == expected_result);
  }

  SECTION("Test assign statement count and their statement number") {
    std::unordered_set<std::string> result = pkb->GetStmt(StmtType::ASSIGN);
    std::unordered_set<std::string> expected_result = {"5", "6", "7", "9", "10", "11", "12", "13", "14", "15"};

    REQUIRE(result.size() == 10);
    REQUIRE(result == expected_result);
  }

  SECTION("Test Follows relationship") {
    std::string result1 = pkb->GetFollowsStore()->GetFollowingOf(STMT, "1");
    std::string result2 = pkb->GetFollowsStore()->GetFollowingOf(STMT, "2");
    std::string result3 = pkb->GetFollowsStore()->GetFollowingOf(STMT, "3");
    std::string result4 = pkb->GetFollowsStore()->GetFollowingOf(STMT, "4");
    std::string result5 = pkb->GetFollowsStore()->GetFollowingOf(STMT, "5");
    std::string result6 = pkb->GetFollowsStore()->GetFollowingOf(STMT, "6");
    std::string result7 = pkb->GetFollowsStore()->GetFollowingOf(STMT, "7");
    std::string result8 = pkb->GetFollowsStore()->GetFollowingOf(STMT, "8");
    std::string result9 = pkb->GetFollowsStore()->GetFollowingOf(STMT, "9");
    std::string result10 = pkb->GetFollowsStore()->GetFollowingOf(STMT, "10");
    std::string result11 = pkb->GetFollowsStore()->GetFollowingOf(STMT, "11");
    std::string result12 = pkb->GetFollowsStore()->GetFollowingOf(STMT, "12");
    std::string result13 = pkb->GetFollowsStore()->GetFollowingOf(STMT, "13");
    std::string result14 = pkb->GetFollowsStore()->GetFollowingOf(STMT, "14");
    std::string result15 = pkb->GetFollowsStore()->GetFollowingOf(STMT, "15");

    std::string expected_result1 = "2";
    std::string expected_result2 = "3";
    std::string expected_result3 = "0";
    std::string expected_result4 = "0";
    std::string expected_result5 = "6";
    std::string expected_result6 = "7";
    std::string expected_result7 = "8";
    std::string expected_result8 = "12";
    std::string expected_result9 = "10";
    std::string expected_result10 = "0";
    std::string expected_result11 = "0";
    std::string expected_result12 = "0";
    std::string expected_result13 = "14";
    std::string expected_result14 = "15";
    std::string expected_result15 = "0";

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
    REQUIRE(result13 == expected_result13);
    REQUIRE(result14 == expected_result14);
    REQUIRE(result15 == expected_result15);
  }

  SECTION("Test Follows* relationship") {
    std::unordered_set<std::string> result1 = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "1");
    std::unordered_set<std::string> result2 = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "2");
    std::unordered_set<std::string> result3 = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "3");
    std::unordered_set<std::string> result4 = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "4");
    std::unordered_set<std::string> result5 = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "5");
    std::unordered_set<std::string> result6 = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "6");
    std::unordered_set<std::string> result7 = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "7");
    std::unordered_set<std::string> result8 = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "8");
    std::unordered_set<std::string> result9 = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "9");
    std::unordered_set<std::string> result10 = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "10");
    std::unordered_set<std::string> result11 = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "11");
    std::unordered_set<std::string> result12 = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "12");
    std::unordered_set<std::string> result13 = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "13");
    std::unordered_set<std::string> result14 = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "14");
    std::unordered_set<std::string> result15 = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, "15");

    std::unordered_set<std::string> expected_result1 = {"2", "3"};
    std::unordered_set<std::string> expected_result2 = {"3"};
    std::unordered_set<std::string> expected_result3 = {};
    std::unordered_set<std::string> expected_result4 = {};
    std::unordered_set<std::string> expected_result5 = {"6", "7", "8", "12"};
    std::unordered_set<std::string> expected_result6 = {"7", "8", "12"};
    std::unordered_set<std::string> expected_result7 = {"8", "12"};
    std::unordered_set<std::string> expected_result8 = {"12"};
    std::unordered_set<std::string> expected_result9 = {"10"};
    std::unordered_set<std::string> expected_result10 = {};
    std::unordered_set<std::string> expected_result11 = {};
    std::unordered_set<std::string> expected_result12 = {};
    std::unordered_set<std::string> expected_result13 = {"14", "15"};
    std::unordered_set<std::string> expected_result14 = {"15"};
    std::unordered_set<std::string> expected_result15 = {};

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
    REQUIRE(result13 == expected_result13);
    REQUIRE(result14 == expected_result14);
    REQUIRE(result15 == expected_result15);
  }

  SECTION("Test Parents relationship 1") {
    std::string result1 = pkb->GetParentStore()->GetParentOf(STMT, "1");
    std::string result2 = pkb->GetParentStore()->GetParentOf(STMT, "2");
    std::string result3 = pkb->GetParentStore()->GetParentOf(STMT, "3");
    std::string result4 = pkb->GetParentStore()->GetParentOf(STMT, "4");
    std::string result5 = pkb->GetParentStore()->GetParentOf(STMT, "5");
    std::string result6 = pkb->GetParentStore()->GetParentOf(STMT, "6");
    std::string result7 = pkb->GetParentStore()->GetParentOf(STMT, "7");
    std::string result8 = pkb->GetParentStore()->GetParentOf(STMT, "8");
    std::string result9 = pkb->GetParentStore()->GetParentOf(STMT, "9");
    std::string result10 = pkb->GetParentStore()->GetParentOf(STMT, "10");
    std::string result11 = pkb->GetParentStore()->GetParentOf(STMT, "11");
    std::string result12 = pkb->GetParentStore()->GetParentOf(STMT, "12");
    std::string result13 = pkb->GetParentStore()->GetParentOf(STMT, "13");
    std::string result14 = pkb->GetParentStore()->GetParentOf(STMT, "14");
    std::string result15 = pkb->GetParentStore()->GetParentOf(STMT, "15");

    std::string expected_result1 = "0";
    std::string expected_result2 = "0";
    std::string expected_result3 = "0";
    std::string expected_result4 = "3";
    std::string expected_result5 = "4";
    std::string expected_result6 = "4";
    std::string expected_result7 = "4";
    std::string expected_result8 = "4";
    std::string expected_result9 = "8";
    std::string expected_result10 = "8";
    std::string expected_result11 = "8";
    std::string expected_result12 = "4";
    std::string expected_result13 = "3";
    std::string expected_result14 = "3";
    std::string expected_result15 = "3";

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
    REQUIRE(result13 == expected_result13);
    REQUIRE(result14 == expected_result14);
    REQUIRE(result15 == expected_result15);
  }

  SECTION("Test Parents relationship 2") {
    std::unordered_set<std::string> result1 = pkb->GetParentStore()->GetChildOf(STMT, "1");
    std::unordered_set<std::string> result2 = pkb->GetParentStore()->GetChildOf(STMT, "2");
    std::unordered_set<std::string> result3 = pkb->GetParentStore()->GetChildOf(STMT, "3");
    std::unordered_set<std::string> result4 = pkb->GetParentStore()->GetChildOf(STMT, "4");
    std::unordered_set<std::string> result5 = pkb->GetParentStore()->GetChildOf(STMT, "5");
    std::unordered_set<std::string> result6 = pkb->GetParentStore()->GetChildOf(STMT, "6");
    std::unordered_set<std::string> result7 = pkb->GetParentStore()->GetChildOf(STMT, "7");
    std::unordered_set<std::string> result8 = pkb->GetParentStore()->GetChildOf(STMT, "8");
    std::unordered_set<std::string> result9 = pkb->GetParentStore()->GetChildOf(STMT, "9");
    std::unordered_set<std::string> result10 = pkb->GetParentStore()->GetChildOf(STMT, "10");
    std::unordered_set<std::string> result11 = pkb->GetParentStore()->GetChildOf(STMT, "11");
    std::unordered_set<std::string> result12 = pkb->GetParentStore()->GetChildOf(STMT, "12");
    std::unordered_set<std::string> result13 = pkb->GetParentStore()->GetChildOf(STMT, "13");
    std::unordered_set<std::string> result14 = pkb->GetParentStore()->GetChildOf(STMT, "14");
    std::unordered_set<std::string> result15 = pkb->GetParentStore()->GetChildOf(STMT, "15");

    std::unordered_set<std::string> expected_result1 = {};
    std::unordered_set<std::string> expected_result2 = {};
    std::unordered_set<std::string> expected_result3 = {"4", "13", "14", "15"};
    std::unordered_set<std::string> expected_result4 = {"5", "6", "7", "8", "12"};
    std::unordered_set<std::string> expected_result5 = {};
    std::unordered_set<std::string> expected_result6 = {};
    std::unordered_set<std::string> expected_result7 = {};
    std::unordered_set<std::string> expected_result8 = {"9", "10", "11"};
    std::unordered_set<std::string> expected_result9 = {};
    std::unordered_set<std::string> expected_result10 = {};
    std::unordered_set<std::string> expected_result11 = {};
    std::unordered_set<std::string> expected_result12 = {};
    std::unordered_set<std::string> expected_result13 = {};
    std::unordered_set<std::string> expected_result14 = {};
    std::unordered_set<std::string> expected_result15 = {};

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
    REQUIRE(result13 == expected_result13);
    REQUIRE(result14 == expected_result14);
    REQUIRE(result15 == expected_result15);
  }

  SECTION("Test Parents* relationship 1") {
    std::unordered_set<std::string> result1 = pkb->GetParentStore()->GetAllAnceOf(STMT, "1");
    std::unordered_set<std::string> result2 = pkb->GetParentStore()->GetAllAnceOf(STMT, "2");
    std::unordered_set<std::string> result3 = pkb->GetParentStore()->GetAllAnceOf(STMT, "3");
    std::unordered_set<std::string> result4 = pkb->GetParentStore()->GetAllAnceOf(STMT, "4");
    std::unordered_set<std::string> result5 = pkb->GetParentStore()->GetAllAnceOf(STMT, "5");
    std::unordered_set<std::string> result6 = pkb->GetParentStore()->GetAllAnceOf(STMT, "6");
    std::unordered_set<std::string> result7 = pkb->GetParentStore()->GetAllAnceOf(STMT, "7");
    std::unordered_set<std::string> result8 = pkb->GetParentStore()->GetAllAnceOf(STMT, "8");
    std::unordered_set<std::string> result9 = pkb->GetParentStore()->GetAllAnceOf(STMT, "9");
    std::unordered_set<std::string> result10 = pkb->GetParentStore()->GetAllAnceOf(STMT, "10");
    std::unordered_set<std::string> result11 = pkb->GetParentStore()->GetAllAnceOf(STMT, "11");
    std::unordered_set<std::string> result12 = pkb->GetParentStore()->GetAllAnceOf(STMT, "12");
    std::unordered_set<std::string> result13 = pkb->GetParentStore()->GetAllAnceOf(STMT, "13");
    std::unordered_set<std::string> result14 = pkb->GetParentStore()->GetAllAnceOf(STMT, "14");
    std::unordered_set<std::string> result15 = pkb->GetParentStore()->GetAllAnceOf(STMT, "15");

    std::unordered_set<std::string> expected_result1 = {};
    std::unordered_set<std::string> expected_result2 = {};
    std::unordered_set<std::string> expected_result3 = {};
    std::unordered_set<std::string> expected_result4 = {"3"};
    std::unordered_set<std::string> expected_result5 = {"3", "4"};
    std::unordered_set<std::string> expected_result6 = {"3", "4"};
    std::unordered_set<std::string> expected_result7 = {"3", "4"};
    std::unordered_set<std::string> expected_result8 = {"3", "4"};
    std::unordered_set<std::string> expected_result9 = {"3", "4", "8"};
    std::unordered_set<std::string> expected_result10 = {"3", "4", "8"};
    std::unordered_set<std::string> expected_result11 = {"3", "4", "8"};
    std::unordered_set<std::string> expected_result12 = {"3", "4"};
    std::unordered_set<std::string> expected_result13 = {"3"};
    std::unordered_set<std::string> expected_result14 = {"3"};
    std::unordered_set<std::string> expected_result15 = {"3"};

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
    REQUIRE(result13 == expected_result13);
    REQUIRE(result14 == expected_result14);
    REQUIRE(result15 == expected_result15);
  }

  SECTION("Test Parents* relationship 2") {
    std::unordered_set<std::string> result1 = pkb->GetParentStore()->GetAllDescOf(STMT, "1");
    std::unordered_set<std::string> result2 = pkb->GetParentStore()->GetAllDescOf(STMT, "2");
    std::unordered_set<std::string> result3 = pkb->GetParentStore()->GetAllDescOf(STMT, "3");
    std::unordered_set<std::string> result4 = pkb->GetParentStore()->GetAllDescOf(STMT, "4");
    std::unordered_set<std::string> result5 = pkb->GetParentStore()->GetAllDescOf(STMT, "5");
    std::unordered_set<std::string> result6 = pkb->GetParentStore()->GetAllDescOf(STMT, "6");
    std::unordered_set<std::string> result7 = pkb->GetParentStore()->GetAllDescOf(STMT, "7");
    std::unordered_set<std::string> result8 = pkb->GetParentStore()->GetAllDescOf(STMT, "8");
    std::unordered_set<std::string> result9 = pkb->GetParentStore()->GetAllDescOf(STMT, "9");
    std::unordered_set<std::string> result10 = pkb->GetParentStore()->GetAllDescOf(STMT, "10");
    std::unordered_set<std::string> result11 = pkb->GetParentStore()->GetAllDescOf(STMT, "11");
    std::unordered_set<std::string> result12 = pkb->GetParentStore()->GetAllDescOf(STMT, "12");
    std::unordered_set<std::string> result13 = pkb->GetParentStore()->GetAllDescOf(STMT, "13");
    std::unordered_set<std::string> result14 = pkb->GetParentStore()->GetAllDescOf(STMT, "14");
    std::unordered_set<std::string> result15 = pkb->GetParentStore()->GetAllDescOf(STMT, "15");

    std::unordered_set<std::string> expected_result1 = {};
    std::unordered_set<std::string> expected_result2 = {};
    std::unordered_set<std::string>
        expected_result3 = {"4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"};
    std::unordered_set<std::string> expected_result4 = {"5", "6", "7", "8", "9", "10", "11", "12"};
    std::unordered_set<std::string> expected_result5 = {};
    std::unordered_set<std::string> expected_result6 = {};
    std::unordered_set<std::string> expected_result7 = {};
    std::unordered_set<std::string> expected_result8 = {"9", "10", "11"};
    std::unordered_set<std::string> expected_result9 = {};
    std::unordered_set<std::string> expected_result10 = {};
    std::unordered_set<std::string> expected_result11 = {};
    std::unordered_set<std::string> expected_result12 = {};
    std::unordered_set<std::string> expected_result13 = {};
    std::unordered_set<std::string> expected_result14 = {};
    std::unordered_set<std::string> expected_result15 = {};

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
    REQUIRE(result13 == expected_result13);
    REQUIRE(result14 == expected_result14);
    REQUIRE(result15 == expected_result15);
  }

  SECTION("Test Uses relationship for variables") {
    std::unordered_set<std::string> result1 = pkb->GetUsesStore()->GetVarUsedByStmt("1");
    std::unordered_set<std::string> result2 = pkb->GetUsesStore()->GetVarUsedByStmt("2");
    std::unordered_set<std::string> result3 = pkb->GetUsesStore()->GetVarUsedByStmt("3");
    std::unordered_set<std::string> result4 = pkb->GetUsesStore()->GetVarUsedByStmt("4");
    std::unordered_set<std::string> result5 = pkb->GetUsesStore()->GetVarUsedByStmt("5");
    std::unordered_set<std::string> result6 = pkb->GetUsesStore()->GetVarUsedByStmt("6");
    std::unordered_set<std::string> result7 = pkb->GetUsesStore()->GetVarUsedByStmt("7");
    std::unordered_set<std::string> result8 = pkb->GetUsesStore()->GetVarUsedByStmt("8");
    std::unordered_set<std::string> result9 = pkb->GetUsesStore()->GetVarUsedByStmt("9");
    std::unordered_set<std::string> result10 = pkb->GetUsesStore()->GetVarUsedByStmt("10");
    std::unordered_set<std::string> result11 = pkb->GetUsesStore()->GetVarUsedByStmt("11");
    std::unordered_set<std::string> result12 = pkb->GetUsesStore()->GetVarUsedByStmt("12");
    std::unordered_set<std::string> result13 = pkb->GetUsesStore()->GetVarUsedByStmt("13");
    std::unordered_set<std::string> result14 = pkb->GetUsesStore()->GetVarUsedByStmt("14");
    std::unordered_set<std::string> result15 = pkb->GetUsesStore()->GetVarUsedByStmt("15");

    std::unordered_set<std::string> expected_result1 = {};
    std::unordered_set<std::string> expected_result2 = {};
    std::unordered_set<std::string> expected_result3 = {"x", "y", "length", "sin", "cos", "tan"};
    std::unordered_set<std::string> expected_result4 = {"x", "y", "length", "sin", "cos", "tan"};
    std::unordered_set<std::string> expected_result5 = {"x", "y"};
    std::unordered_set<std::string> expected_result6 = {"y", "length"};
    std::unordered_set<std::string> expected_result7 = {"x", "length"};
    std::unordered_set<std::string> expected_result8 = {"x", "y", "sin", "cos", "tan"};
    std::unordered_set<std::string> expected_result9 = {"y"};
    std::unordered_set<std::string> expected_result10 = {"x"};
    std::unordered_set<std::string> expected_result11 = {};
    std::unordered_set<std::string> expected_result12 = {"x", "y"};
    std::unordered_set<std::string> expected_result13 = {};
    std::unordered_set<std::string> expected_result14 = {};
    std::unordered_set<std::string> expected_result15 = {"x", "y"};

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
    REQUIRE(result13 == expected_result13);
    REQUIRE(result14 == expected_result14);
    REQUIRE(result15 == expected_result15);
  }

  SECTION("Test Uses relationship for statement number") {
    std::unordered_set<std::string> result1 = pkb->GetUsesStore()->GetStmtUsedByVar(STMT, "x");
    std::unordered_set<std::string> result2 = pkb->GetUsesStore()->GetStmtUsedByVar(STMT, "y");
    std::unordered_set<std::string> result3 = pkb->GetUsesStore()->GetStmtUsedByVar(STMT, "length");
    std::unordered_set<std::string> result4 = pkb->GetUsesStore()->GetStmtUsedByVar(STMT, "sin");
    std::unordered_set<std::string> result5 = pkb->GetUsesStore()->GetStmtUsedByVar(STMT, "cos");
    std::unordered_set<std::string> result6 = pkb->GetUsesStore()->GetStmtUsedByVar(STMT, "tan");

    std::unordered_set<std::string> expected_result1 = {"3", "4", "5", "7", "8", "10", "12", "15"};
    std::unordered_set<std::string> expected_result2 = {"3", "4", "5", "6", "8", "9", "12", "15"};
    std::unordered_set<std::string> expected_result3 = {"3", "4", "6", "7"};
    std::unordered_set<std::string> expected_result4 = {"3", "4", "8"};
    std::unordered_set<std::string> expected_result5 = {"3", "4", "8"};
    std::unordered_set<std::string> expected_result6 = {"3", "4", "8"};

    REQUIRE(result1 == expected_result1);
    REQUIRE(result2 == expected_result2);
    REQUIRE(result3 == expected_result3);
    REQUIRE(result4 == expected_result4);
    REQUIRE(result5 == expected_result5);
    REQUIRE(result6 == expected_result6);
  }

  SECTION("Test Modifies relationship for variables") {
    std::unordered_set<std::string> result1 = pkb->GetModifiesStore()->GetVarModByStmt("1");
    std::unordered_set<std::string> result2 = pkb->GetModifiesStore()->GetVarModByStmt("2");
    std::unordered_set<std::string> result3 = pkb->GetModifiesStore()->GetVarModByStmt("3");
    std::unordered_set<std::string> result4 = pkb->GetModifiesStore()->GetVarModByStmt("4");
    std::unordered_set<std::string> result5 = pkb->GetModifiesStore()->GetVarModByStmt("5");
    std::unordered_set<std::string> result6 = pkb->GetModifiesStore()->GetVarModByStmt("6");
    std::unordered_set<std::string> result7 = pkb->GetModifiesStore()->GetVarModByStmt("7");
    std::unordered_set<std::string> result8 = pkb->GetModifiesStore()->GetVarModByStmt("8");
    std::unordered_set<std::string> result9 = pkb->GetModifiesStore()->GetVarModByStmt("9");
    std::unordered_set<std::string> result10 = pkb->GetModifiesStore()->GetVarModByStmt("10");
    std::unordered_set<std::string> result11 = pkb->GetModifiesStore()->GetVarModByStmt("11");
    std::unordered_set<std::string> result12 = pkb->GetModifiesStore()->GetVarModByStmt("12");
    std::unordered_set<std::string> result13 = pkb->GetModifiesStore()->GetVarModByStmt("13");
    std::unordered_set<std::string> result14 = pkb->GetModifiesStore()->GetVarModByStmt("14");
    std::unordered_set<std::string> result15 = pkb->GetModifiesStore()->GetVarModByStmt("15");

    std::unordered_set<std::string> expected_result1 = {"x"};
    std::unordered_set<std::string> expected_result2 = {"y"};
    std::unordered_set<std::string> expected_result3 = {"x", "y", "length", "sin", "cos", "tan"};
    std::unordered_set<std::string> expected_result4 = {"x", "y", "length", "sin", "cos", "tan"};
    std::unordered_set<std::string> expected_result5 = {"tan"};
    std::unordered_set<std::string> expected_result6 = {"sin"};
    std::unordered_set<std::string> expected_result7 = {"cos"};
    std::unordered_set<std::string> expected_result8 = {"x", "y"};
    std::unordered_set<std::string> expected_result9 = {"x"};
    std::unordered_set<std::string> expected_result10 = {"y"};
    std::unordered_set<std::string> expected_result11 = {"x"};
    std::unordered_set<std::string> expected_result12 = {"length"};
    std::unordered_set<std::string> expected_result13 = {"x"};
    std::unordered_set<std::string> expected_result14 = {"y"};
    std::unordered_set<std::string> expected_result15 = {"length"};

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
    REQUIRE(result13 == expected_result13);
    REQUIRE(result14 == expected_result14);
    REQUIRE(result15 == expected_result15);
  }

  SECTION("Test Modifies relationship for statement number") {
    std::unordered_set<std::string> result1 = pkb->GetModifiesStore()->GetStmtModByVar(STMT, "x");
    std::unordered_set<std::string> result2 = pkb->GetModifiesStore()->GetStmtModByVar(STMT, "y");
    std::unordered_set<std::string> result3 = pkb->GetModifiesStore()->GetStmtModByVar(STMT, "length");
    std::unordered_set<std::string> result4 = pkb->GetModifiesStore()->GetStmtModByVar(STMT, "sin");
    std::unordered_set<std::string> result5 = pkb->GetModifiesStore()->GetStmtModByVar(STMT, "cos");
    std::unordered_set<std::string> result6 = pkb->GetModifiesStore()->GetStmtModByVar(STMT, "tan");

    std::unordered_set<std::string> expected_result1 = {"1", "3", "4", "8", "9", "11", "13"};
    std::unordered_set<std::string> expected_result2 = {"2", "3", "4", "8", "10", "14"};
    std::unordered_set<std::string> expected_result3 = {"3", "4", "12", "15"};
    std::unordered_set<std::string> expected_result4 = {"3", "4", "6"};
    std::unordered_set<std::string> expected_result5 = {"3", "4", "7"};
    std::unordered_set<std::string> expected_result6 = {"3", "4", "5"};

    REQUIRE(result1 == expected_result1);
    REQUIRE(result2 == expected_result2);
    REQUIRE(result3 == expected_result3);
    REQUIRE(result4 == expected_result4);
    REQUIRE(result5 == expected_result5);
    REQUIRE(result6 == expected_result6);
  }
}

TEST_CASE("Test components between Source and PKB (Sample source 3)") {
  std::shared_ptr<ProgramNode> ast = GenerateAbstractSyntaxTree(sample_source3);
  PkbPtr pkb = GetPopulatedPkbInstance(ast);

  SECTION("Test Calls") {
    std::unordered_set<std::string> result1 = pkb->GetCallStore()->GetCallersOf("fizz");
    std::unordered_set<std::string> result2 = pkb->GetCallStore()->GetCallersOf("foo");
    std::unordered_set<std::string> result3 = pkb->GetCallStore()->GetCallersStarOf("fizz");
    std::unordered_set<std::string> result4 = pkb->GetCallStore()->GetCalleesStarOf("bar");
    std::unordered_set<std::string> result5 = pkb->GetCallStore()->GetCallersOf("buzz");
    std::unordered_set<std::string> result6 = pkb->GetCallStore()->GetCallersStarOf("buzz");
    std::unordered_set<std::string> result7 = pkb->GetCallStore()->GetCalleesStarOf("buzz");

    std::unordered_set<std::string> expected_result1 = {"foo"};
    std::unordered_set<std::string> expected_result2 = {"bar"};
    std::unordered_set<std::string> expected_result3 = {"foo", "bar"};
    std::unordered_set<std::string> expected_result4 = {"foo", "fizz", "buzz"};
    std::unordered_set<std::string> expected_result5 = {"bar", "fizz"};
    std::unordered_set<std::string> expected_result6 = {"bar", "fizz", "foo"};
    std::unordered_set<std::string> expected_result7 = {};

    REQUIRE(result1 == expected_result1);
    REQUIRE(result2 == expected_result2);
    REQUIRE(result3 == expected_result3);
    REQUIRE(result4 == expected_result4);
    REQUIRE(result5 == expected_result5);
    REQUIRE(result6 == expected_result6);
    REQUIRE(result7 == expected_result7);
  }
}

TEST_CASE("Test components between Source and PKB for Next (Sample source 5)") {
  std::shared_ptr<ProgramNode> ast = GenerateAbstractSyntaxTree(sample_source5);
  PkbPtr pkb = GetCfgPopulatedPkbInstance(ast);
  SECTION("Test Next") {
    std::unordered_set<std::string> result1 = pkb->GetNextStore()->GetNextOf(STMT, "13");
    std::unordered_set<std::string> result2 = pkb->GetNextStore()->GetNextOf(STMT, "2");
    std::unordered_set<std::string> result3 = pkb->GetNextStore()->GetNextOf(STMT, "15");
    std::unordered_set<std::string> result4 = pkb->GetNextStore()->GetBeforeOf(STMT, "12");
    std::unordered_set<std::string> result5 = pkb->GetNextStore()->GetBeforeOf(STMT, "5");
    std::unordered_set<std::string> result6 = pkb->GetNextStore()->GetBeforeOf(STMT, "8");
    std::unordered_set<std::string> result7 = pkb->GetNextStore()->GetNextOf(STMT, "4");
    std::unordered_set<std::string> result8 = pkb->GetNextStore()->GetNextOf(STMT, "12");

    std::unordered_set<std::string> expected_result1 = {"12"};
    std::unordered_set<std::string> expected_result2 = {"3", "7"};
    std::unordered_set<std::string> expected_result3 = {"8"};
    std::unordered_set<std::string> expected_result4 = {"11", "13"};
    std::unordered_set<std::string> expected_result5 = {"3", "6"};
    std::unordered_set<std::string> expected_result6 = {"1", "15"};
    std::unordered_set<std::string> expected_result7 = {};
    std::unordered_set<std::string> expected_result8 = {"13", "14"};

    REQUIRE(result1 == expected_result1);
    REQUIRE(result2 == expected_result2);
    REQUIRE(result3 == expected_result3);
    REQUIRE(result4 == expected_result4);
    REQUIRE(result5 == expected_result5);
    REQUIRE(result6 == expected_result6);
    REQUIRE(result7 == expected_result7);
  }
}

TEST_CASE("Test components between Source and PKB for Next (Sample source 6)") {
  SECTION("Test cyclic call (invalid AST - non-initialised PKB)") {
    std::shared_ptr<ProgramNode> ast;
    PkbPtr pkb;
    try {
      ast = GenerateAbstractSyntaxTree(sample_source6);
      pkb = GetCfgPopulatedPkbInstance(ast);
    } catch (std::exception &e) {
      // catch-all, do nothing
    }

    // sample choice, no pkb population = no cfg
    REQUIRE(pkb == nullptr);

    // to double confirm it should throw an exception
    REQUIRE_THROWS_WITH(GenerateAbstractSyntaxTree(sample_source6), CyclicCallException().what());
  }
}

TEST_CASE("Test components between Source and PKB for Next (Sample source 7)") {
  SECTION("Test invalid call (invalid AST - non-initialised PKB)") {
    std::shared_ptr<ProgramNode> ast;
    PkbPtr pkb;
    try {
      ast = GenerateAbstractSyntaxTree(sample_source7);
      pkb = GetCfgPopulatedPkbInstance(ast);
    } catch (std::exception &e) {
      // catch-all, do nothing
    }

    // sample choice, no pkb population = no cfg
    REQUIRE(pkb == nullptr);

    // to double confirm it should throw an exception
    REQUIRE_THROWS_WITH(GenerateAbstractSyntaxTree(sample_source7), InvalidCallException().what());
  }
}

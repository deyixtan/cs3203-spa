#include "catch.hpp"
#include "components/pkb/pkb.h"
#include "components/query_subsystem/pql_parser/parsed_query.h"
#include "components/query_subsystem/pql_lexer/pql_lexer.h"
#include "components/query_subsystem/pql_parser/parsed_query_builder.h"
#include "components/query_subsystem/pql_parser/query_validator.h"
#include "components/query_subsystem/pql_evaluator/query_evaluator.h"

PKB *set_up_pkb() {
  PKB *pkb = new PKB();
  pkb->AddStmt("main", PROC);
  pkb->AddStmt("foo", PROC);
  pkb->AddStmt("bar", PROC);
  pkb->AddStmt("func", PROC);

  pkb->AddStmt("5", IF);

  pkb->AddStmt("6", WHILE);
  pkb->AddStmt("11", WHILE);

  pkb->AddStmt("dog", VARS);
  pkb->AddStmt("cat", VARS);
  pkb->AddStmt("rabbit", VARS);
  pkb->AddStmt("snake", VARS);
  pkb->AddStmt("tiger", VARS);
  pkb->AddStmt("monkey", VARS);
  pkb->AddStmt("pig", VARS);
  pkb->AddStmt("mouse", VARS);
  pkb->AddStmt("dragon", VARS);
  pkb->AddStmt("ox", VARS);

  pkb->AddStmt("1", STMT);
  pkb->AddStmt("2", STMT);
  pkb->AddStmt("3", STMT);
  pkb->AddStmt("4", STMT);
  pkb->AddStmt("5", STMT);
  pkb->AddStmt("6", STMT);
  pkb->AddStmt("7", STMT);
  pkb->AddStmt("8", STMT);
  pkb->AddStmt("9", STMT);
  pkb->AddStmt("10", STMT);
  pkb->AddStmt("11", STMT);
  pkb->AddStmt("12", STMT);
  pkb->AddStmt("13", STMT);
  pkb->AddStmt("14", STMT);
  pkb->AddStmt("15", STMT);

  pkb->AddStmt("4", ASSIGN);
  pkb->AddStmt("7", ASSIGN);
  pkb->AddStmt("8", ASSIGN);
  pkb->AddStmt("10", ASSIGN);
  pkb->AddStmt("15", ASSIGN);

  pkb->AddStmt("0", CONSTS);
  pkb->AddStmt("10", CONSTS);

  pkb->AddStmt("1", READ);

  pkb->AddStmt("3", PRINT);
  pkb->AddStmt("13", PRINT);

  pkb->AddTypeOfStmt("1", READ);
  pkb->AddTypeOfStmt("2", STMT);
  pkb->AddTypeOfStmt("3", PRINT);
  pkb->AddTypeOfStmt("4", ASSIGN);
  pkb->AddTypeOfStmt("5", STMT);
  pkb->AddTypeOfStmt("6", STMT);
  pkb->AddTypeOfStmt("7", ASSIGN);
  pkb->AddTypeOfStmt("8", ASSIGN);
  pkb->AddTypeOfStmt("9", STMT);
  pkb->AddTypeOfStmt("10", ASSIGN);
  pkb->AddTypeOfStmt("11", STMT);
  pkb->AddTypeOfStmt("12", STMT);
  pkb->AddTypeOfStmt("13", STMT);
  pkb->AddTypeOfStmt("14", STMT);
  pkb->AddTypeOfStmt("15", STMT);

  pkb->GetUsesStore()->AddStmtVar("3", "cat");
  pkb->GetUsesStore()->AddStmtVar("4", "dog");
  pkb->GetUsesStore()->AddStmtVar("4", "mouse");
  pkb->GetUsesStore()->AddStmtVar("4", "cat");
  pkb->GetUsesStore()->AddStmtVar("4", "dragon");
  pkb->GetUsesStore()->AddStmtVar("4", "rabbit");
  pkb->GetUsesStore()->AddStmtVar("5", "dog");
  pkb->GetUsesStore()->AddStmtVar("6", "pig");
  pkb->GetUsesStore()->AddStmtVar("6", "mouse");
  pkb->GetUsesStore()->AddStmtVar("7", "pig");
  pkb->GetUsesStore()->AddStmtVar("7", "ox");
  pkb->GetUsesStore()->AddStmtVar("7", "cat");
  pkb->GetUsesStore()->AddStmtVar("8", "dragon");
  pkb->GetUsesStore()->AddStmtVar("8", "dog");
  pkb->GetUsesStore()->AddStmtVar("8", "rabbit");
  pkb->GetUsesStore()->AddStmtVar("8", "mouse");
  pkb->GetUsesStore()->AddStmtVar("10", "snake");
  pkb->GetUsesStore()->AddStmtVar("10", "dog");
  pkb->GetUsesStore()->AddStmtVar("10", "rabbit");
  pkb->GetUsesStore()->AddStmtVar("11", "cat");
  pkb->GetUsesStore()->AddStmtVar("13", "rabbit");
  pkb->GetUsesStore()->AddStmtVar("15", "monkey");
  pkb->GetUsesStore()->AddStmtVar("15", "tiger");
  pkb->GetUsesStore()->AddStmtVar("15", "dog");

  pkb->GetModifiesStore()->AddStmtVar("1", "dog");
  pkb->GetModifiesStore()->AddStmtVar("4", "dog");
  pkb->GetModifiesStore()->AddStmtVar("7", "pig");
  pkb->GetModifiesStore()->AddStmtVar("8", "dragon");
  pkb->GetModifiesStore()->AddStmtVar("10", "snake");
  pkb->GetModifiesStore()->AddStmtVar("15", "monkey");

  pkb->GetFollowsStore()->AddFollow("1", "2");
  pkb->GetFollowsStore()->AddFollow("2", "3");
  pkb->GetFollowsStore()->AddFollow("3", "4");
  pkb->GetFollowsStore()->AddFollow("4", "5");
  pkb->GetFollowsStore()->AddFollow("8", "9");
  pkb->GetFollowsStore()->AddFollow("10", "11");
  pkb->GetFollowsStore()->AddFollow("13", "14");

  pkb->GetFollowsStore()->AddFollowStar("1", "2");
  pkb->GetFollowsStore()->AddFollowStar("1", "3");
  pkb->GetFollowsStore()->AddFollowStar("1", "4");
  pkb->GetFollowsStore()->AddFollowStar("1", "5");
  pkb->GetFollowsStore()->AddFollowStar("2", "3");
  pkb->GetFollowsStore()->AddFollowStar("2", "4");
  pkb->GetFollowsStore()->AddFollowStar("2", "5");
  pkb->GetFollowsStore()->AddFollowStar("3", "4");
  pkb->GetFollowsStore()->AddFollowStar("3", "5");
  pkb->GetFollowsStore()->AddFollowStar("4", "5");
  pkb->GetFollowsStore()->AddFollowStar("8", "9");
  pkb->GetFollowsStore()->AddFollowStar("10", "11");
  pkb->GetFollowsStore()->AddFollowStar("13", "14");

  pkb->GetParentStore()->AddParent("5", "6");
  pkb->GetParentStore()->AddParent("5", "8");
  pkb->GetParentStore()->AddParent("5", "9");
  pkb->GetParentStore()->AddParent("6", "7");
  pkb->GetParentStore()->AddParent("11", "12");

  pkb->GetPatternStore()->AddStmtWithPattern("4", "dog", "(((mouse+(10*cat))-((dog/mouse)*dragon))+((mouse+rabbit)-cat))");
  pkb->GetPatternStore()->AddStmtWithPattern("7", "pig", "(ox+cat)");
  pkb->GetPatternStore()->AddStmtWithPattern("8", "dragon", "((dog*rabbit)/mouse)");
  pkb->GetPatternStore()->AddStmtWithPattern("10", "snake", "(dog+rabbit)");
  pkb->GetPatternStore()->AddStmtWithPattern("15", "monkey", "(tiger+dog)");

  return pkb;
}

TEST_CASE("Test components between pql processor and PKB (Sample source 1)") {
  PKB *pkb = set_up_pkb();
  pql_evaluator::QueryEvaluator *evaluator = new pql_evaluator::QueryEvaluator(pkb);

  SECTION("Test if-statement count and their statement number") {
    PqlLexer lexer{"if ifs; Select ifs"};
    std::vector<PqlToken> tokens = lexer.Lex();
    QueryValidator query_validator = QueryValidator(tokens);
    std::vector<PqlToken> validated_tokens = query_validator.CheckValidation();
    ParsedQueryBuilder pqb(validated_tokens);
    ParsedQuery parsed_query = pqb.Build();
    std::list<std::string> results;
    evaluator->Evaluate(parsed_query, results);
    std::string result = results.front();
    std::string expected_result = "5";

    REQUIRE(result == expected_result);
  }

  SECTION("Test assign statement count and their statement number") {
    PqlLexer lexer{"assign a; Select a"};
    std::vector<PqlToken> tokens = lexer.Lex();
    QueryValidator query_validator = QueryValidator(tokens);
    std::vector<PqlToken> validated_tokens = query_validator.CheckValidation();
    ParsedQueryBuilder pqb(validated_tokens);
    ParsedQuery parsed_query = pqb.Build();
    std::list<std::string> results;
    evaluator->Evaluate(parsed_query, results);
    std::list<std::string> expected_result = {"15", "10", "8", "7", "4"};

    REQUIRE(results.size() == 5);
    REQUIRE(results == expected_result);
  }

  SECTION("Test Follows relationship") {
    std::vector<std::string> expected_result = {"2", "3", "4", "5", "", "", "", "9"};
    for (int i = 1; i < 9; i++) {
      std::string query = "stmt s ; Select s such that Follows(" + std::to_string(i) + " ,s)";
      PqlLexer lexer{query};
      std::vector<PqlToken> tokens = lexer.Lex();
      QueryValidator query_validator = QueryValidator(tokens);
      std::vector<PqlToken> validated_tokens = query_validator.CheckValidation();
      ParsedQueryBuilder pqb(validated_tokens);
      ParsedQuery parsed_query = pqb.Build();
      std::list<std::string> results;
      evaluator->Evaluate(parsed_query, results);
      REQUIRE(results.front() == expected_result[i - 1]);
    }
  }

  SECTION("Test Follows* relationship") {
    std::vector<std::list<std::string>> expected_result = {{"4", "3", "2", "5"}, {"3", "4", "5"},
                                                           {"4", "5"}, {"5"}, {}, {}, {},
                                                           {"9"}, {}, {"11"}, {}, {}, {"14"}};
    for (int i = 1; i < 14; i++) {
      std::string query = "stmt s ; Select s such that Follows*(" + std::to_string(i) + " ,s)";
      PqlLexer lexer{query};
      std::vector<PqlToken> tokens = lexer.Lex();
      QueryValidator query_validator = QueryValidator(tokens);
      std::vector<PqlToken> validated_tokens = query_validator.CheckValidation();
      ParsedQueryBuilder pqb(validated_tokens);
      ParsedQuery parsed_query = pqb.Build();
      std::list<std::string> results;
      evaluator->Evaluate(parsed_query, results);
      REQUIRE(results == expected_result[i - 1]);
    }
  }
}


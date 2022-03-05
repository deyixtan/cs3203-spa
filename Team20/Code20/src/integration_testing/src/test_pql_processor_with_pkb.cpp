#include "catch.hpp"
#include "components/pkb/pkb.h"
#include "components/source_subsystem/design_extractor.h"
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
  
  pkb->GetUsageStore().AddProcVar("main", "dog");
  pkb->GetUsageStore().AddProcVar("main", "cat");
  pkb->GetUsageStore().AddProcVar("main", "mouse");
  pkb->GetUsageStore().AddProcVar("main", "pig");
  pkb->GetUsageStore().AddProcVar("main", "ox");
  pkb->GetUsageStore().AddProcVar("main", "dragon");
  pkb->GetUsageStore().AddProcVar("main", "rabbit");
  pkb->GetUsageStore().AddProcVar("foo", "snake");
  pkb->GetUsageStore().AddProcVar("foo", "dog");
  pkb->GetUsageStore().AddProcVar("foo", "rabbit");
  pkb->GetUsageStore().AddProcVar("foo", "cat");
  pkb->GetUsageStore().AddProcVar("bar", "rabbit");
  pkb->GetUsageStore().AddProcVar("func", "monkey");
  pkb->GetUsageStore().AddProcVar("func", "tiger");
  pkb->GetUsageStore().AddProcVar("func", "dog");

  pkb->GetUsageStore().AddStmtVar("3", "cat");
  pkb->GetUsageStore().AddStmtVar("4", "dog");
  pkb->GetUsageStore().AddStmtVar("4", "mouse");
  pkb->GetUsageStore().AddStmtVar("4", "cat");
  pkb->GetUsageStore().AddStmtVar("4", "dragon");
  pkb->GetUsageStore().AddStmtVar("4", "rabbit");
  pkb->GetUsageStore().AddStmtVar("5", "dog");
  pkb->GetUsageStore().AddStmtVar("6", "pig");
  pkb->GetUsageStore().AddStmtVar("6", "mouse");
  pkb->GetUsageStore().AddStmtVar("7", "pig");
  pkb->GetUsageStore().AddStmtVar("7", "ox");
  pkb->GetUsageStore().AddStmtVar("7", "cat");
  pkb->GetUsageStore().AddStmtVar("8", "dragon");
  pkb->GetUsageStore().AddStmtVar("8", "dog");
  pkb->GetUsageStore().AddStmtVar("8", "rabbit");
  pkb->GetUsageStore().AddStmtVar("8", "mouse");
  pkb->GetUsageStore().AddStmtVar("10", "snake");
  pkb->GetUsageStore().AddStmtVar("10", "dog");
  pkb->GetUsageStore().AddStmtVar("10", "rabbit");
  pkb->GetUsageStore().AddStmtVar("11", "cat");
  pkb->GetUsageStore().AddStmtVar("13", "rabbit");
  pkb->GetUsageStore().AddStmtVar("15", "monkey");
  pkb->GetUsageStore().AddStmtVar("15", "tiger");
  pkb->GetUsageStore().AddStmtVar("15", "dog");

  pkb->GetModifyStore().AddProcVar("main", "dog");
  pkb->GetModifyStore().AddProcVar("main", "pig");
  pkb->GetModifyStore().AddProcVar("main", "dragon");
  pkb->GetModifyStore().AddProcVar("foo", "snake");
  pkb->GetModifyStore().AddProcVar("func", "monkey");

  pkb->GetModifyStore().AddStmtVar("1", "dog");
  pkb->GetModifyStore().AddStmtVar("4", "dog");
  pkb->GetModifyStore().AddStmtVar("7", "pig");
  pkb->GetModifyStore().AddStmtVar("8", "dragon");
  pkb->GetModifyStore().AddStmtVar("10", "snake");
  pkb->GetModifyStore().AddStmtVar("15", "monkey");

  pkb->GetFollowStore().AddFollow("1", "2");
  pkb->GetFollowStore().AddFollow("2", "3");
  pkb->GetFollowStore().AddFollow("3", "4");
  pkb->GetFollowStore().AddFollow("4", "5");
  pkb->GetFollowStore().AddFollow("8", "9");
  pkb->GetFollowStore().AddFollow("10", "11");
  pkb->GetFollowStore().AddFollow("13", "14");

  pkb->GetFollowStore().AddFollowStar("1", "2");
  pkb->GetFollowStore().AddFollowStar("1", "3");
  pkb->GetFollowStore().AddFollowStar("1", "4");
  pkb->GetFollowStore().AddFollowStar("1", "5");
  pkb->GetFollowStore().AddFollowStar("2", "3");
  pkb->GetFollowStore().AddFollowStar("2", "4");
  pkb->GetFollowStore().AddFollowStar("2", "5");
  pkb->GetFollowStore().AddFollowStar("3", "4");
  pkb->GetFollowStore().AddFollowStar("3", "5");
  pkb->GetFollowStore().AddFollowStar("4", "5");
  pkb->GetFollowStore().AddFollowStar("8", "9");
  pkb->GetFollowStore().AddFollowStar("10", "11");
  pkb->GetFollowStore().AddFollowStar("13", "14");

  pkb->GetParentStore().AddParent("5", "6");
  pkb->GetParentStore().AddParent("5", "8");
  pkb->GetParentStore().AddParent("5", "9");
  pkb->GetParentStore().AddParent("6", "7");
  pkb->GetParentStore().AddParent("11", "12");

  pkb->AddPattern("4", "dog", "(((mouse+(10*cat))-((dog/mouse)*dragon))+((mouse+rabbit)-cat))");
  pkb->AddPattern("7", "pig", "(ox+cat)");
  pkb->AddPattern("8", "dragon", "((dog*rabbit)/mouse)");
  pkb->AddPattern("10", "snake", "(dog+rabbit)");
  pkb->AddPattern("15", "monkey", "(tiger+dog)");

  return pkb;
}

TEST_CASE("Test components between pql processor and PKB (Sample source 1)") {
  PKB *pkb = set_up_pkb();
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
    std::string expected_result = "5";

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
    std::list<std::string> expected_result = {"15", "10", "8", "7", "4"};

    REQUIRE(results.size() == 5);
    REQUIRE(results == expected_result);
  }

  SECTION("Test Follows relationship") {
    std::vector<std::string> expected_result = {"2", "3", "4", "5", "", "", "", "9"};
    for (int i = 1; i < 9; i++) {
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
    std::vector<std::list<std::string>> expected_result = {{"3", "4", "2", "5"}, {"3", "4", "5"},
                                                           {"4", "5"}, {"5"}, {}, {}, {},
                                                           {"9"}, {}, {"11"}, {}, {}, {"14"}};
    for (int i = 1; i < 14; i++) {
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


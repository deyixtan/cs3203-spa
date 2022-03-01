#include "catch.hpp"
#include "components/pkb/pkb.h"
#include "components/pkb/design_extractor/design_extractor.h"
#include "components/query_subsystem/pql_parser/parsed_query.h"
#include "components/query_subsystem/pql_lexer/pql_lexer.h"
#include "components/query_subsystem/pql_parser/parsed_query_builder.h"
#include "components/query_subsystem/pql_parser/query_validator.h"
#include "components/query_subsystem/pql_evaluator/query_evaluator.h"

using namespace source;

void set_up_pkb() {
  PKB *pkb = PKB::GetInstance();
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

  pkb->AddUsageProcVar("main", "dog");
  pkb->AddUsageProcVar("main", "cat");
  pkb->AddUsageProcVar("main", "mouse");
  pkb->AddUsageProcVar("main", "pig");
  pkb->AddUsageProcVar("main", "ox");
  pkb->AddUsageProcVar("main", "dragon");
  pkb->AddUsageProcVar("main", "rabbit");
  pkb->AddUsageProcVar("foo", "snake");
  pkb->AddUsageProcVar("foo", "dog");
  pkb->AddUsageProcVar("foo", "rabbit");
  pkb->AddUsageProcVar("foo", "cat");
  pkb->AddUsageProcVar("bar", "rabbit");
  pkb->AddUsageProcVar("func", "monkey");
  pkb->AddUsageProcVar("func", "tiger");
  pkb->AddUsageProcVar("func", "dog");

  pkb->AddUsageStmtVar("3", "cat");
  pkb->AddUsageStmtVar("4", "dog");
  pkb->AddUsageStmtVar("4", "mouse");
  pkb->AddUsageStmtVar("4", "cat");
  pkb->AddUsageStmtVar("4", "dragon");
  pkb->AddUsageStmtVar("4", "rabbit");
  pkb->AddUsageStmtVar("5", "dog");
  pkb->AddUsageStmtVar("6", "pig");
  pkb->AddUsageStmtVar("6", "mouse");
  pkb->AddUsageStmtVar("7", "pig");
  pkb->AddUsageStmtVar("7", "ox");
  pkb->AddUsageStmtVar("7", "cat");
  pkb->AddUsageStmtVar("8", "dragon");
  pkb->AddUsageStmtVar("8", "dog");
  pkb->AddUsageStmtVar("8", "rabbit");
  pkb->AddUsageStmtVar("8", "mouse");
  pkb->AddUsageStmtVar("10", "snake");
  pkb->AddUsageStmtVar("10", "dog");
  pkb->AddUsageStmtVar("10", "rabbit");
  pkb->AddUsageStmtVar("11", "cat");
  pkb->AddUsageStmtVar("13", "rabbit");
  pkb->AddUsageStmtVar("15", "monkey");
  pkb->AddUsageStmtVar("15", "tiger");
  pkb->AddUsageStmtVar("15", "dog");

  pkb->AddModifyProcVar("main", "dog");
  pkb->AddModifyProcVar("main", "pig");
  pkb->AddModifyProcVar("main", "dragon");
  pkb->AddModifyProcVar("foo", "snake");
  pkb->AddModifyProcVar("func", "monkey");

  pkb->AddModifyStmtVar("1", "dog");
  pkb->AddModifyStmtVar("4", "dog");
  pkb->AddModifyStmtVar("7", "pig");
  pkb->AddModifyStmtVar("8", "dragon");
  pkb->AddModifyStmtVar("10", "snake");
  pkb->AddModifyStmtVar("15", "monkey");

  pkb->AddFollowStmt("1", "2");
  pkb->AddFollowStmt("2", "3");
  pkb->AddFollowStmt("3", "4");
  pkb->AddFollowStmt("4", "5");
  pkb->AddFollowStmt("8", "9");
  pkb->AddFollowStmt("10", "11");
  pkb->AddFollowStmt("13", "14");

  pkb->AddFollowStarStmt("1", "2");
  pkb->AddFollowStarStmt("1", "3");
  pkb->AddFollowStarStmt("1", "4");
  pkb->AddFollowStarStmt("1", "5");
  pkb->AddFollowStarStmt("2", "3"),
  pkb->AddFollowStarStmt("2", "4"),
  pkb->AddFollowStarStmt("2", "5"),
  pkb->AddFollowStarStmt("3", "4"),
  pkb->AddFollowStarStmt("3", "5"),
  pkb->AddFollowStarStmt("4", "5"),
  pkb->AddFollowStarStmt("8", "9"),
  pkb->AddFollowStarStmt("10", "11");
  pkb->AddFollowStarStmt("13", "14");

  pkb->AddParentStmt("5", "6");
  pkb->AddParentStmt("5", "8");
  pkb->AddParentStmt("5", "9");
  pkb->AddParentStmt("6", "7");
  pkb->AddParentStmt("11", "12");

  pkb->AddPattern("4", "dog", "(((mouse+(10*cat))-((dog/mouse)*dragon))+((mouse+rabbit)-cat))");
  pkb->AddPattern("7", "pig", "(ox+cat)");
  pkb->AddPattern("8", "dragon", "((dog*rabbit)/mouse)");
  pkb->AddPattern("10", "snake", "(dog+rabbit)");
  pkb->AddPattern("15", "monkey", "(tiger+dog)");
}

TEST_CASE("Test components between pql processor and PKB (Sample source 1)") {
  set_up_pkb();
  PKB *pkb = PKB::GetInstance();
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


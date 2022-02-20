#include "query_evaluator.h"

namespace pql_evaluator {

void QueryEvaluator::Evaluate(ParsedQuery &query, std::list<std::string> &results) {
  result.clear();

  // Evaluation has 4 cases
  // 1. Select
  // 2. Select + Relationship
  // 3. Select + Pattern
  // 4. Select + Relationship + Pattern

  // Draft algo for future iterations
  // eval(Relationship) * n -> Table has restrictions on synonym values (single or pair)
  // eval(Pattern) * n -> Table has restrictions on synonym values (single or pair)
  // Select should find tables with the synonyms and then merge the tables
  if (query.GetRelationships().empty() && query.GetPatterns().empty()) {
    // only select
    EvaluateSelectOnly(query);
  } else if (!query.GetRelationships().empty() && query.GetPatterns().empty()) {
    // select + 1 relationship
    EvaluateSelectWithRelationship(query);
  } else if (query.GetRelationships().empty() && !query.GetPatterns().empty()) {
    // select + 1 pattern
    EvaluateSelectWithPattern(query);
  } else {
    // select + 1 relationship + 1 pattern
    EvaluateSelectWithRelationshipAndPattern(query);
  }

  for (auto i = result.begin(); i!=result.end(); ++i) {
    results.emplace_back(*i);
  }
}

void QueryEvaluator::EvaluateSelectOnly(ParsedQuery &query) {
  const PqlToken select_synonym = query.GetSynonym();
  PqlTokenType select_synonym_design_entity;
  const auto declarations = query.GetDeclaration();

  // assume semantically valid and declaration must contain the select_synonym
  for (auto declaration : declarations) {
    if (declaration.GetSynonym().value==select_synonym.value) {
      select_synonym_design_entity = declaration.GetDesignEntity().type;
    }
  }

  // assume that only select
  // evaluation with relationship and pattern may default to this in case of true boolean
  std::unordered_set<std::string> result_to_add;
  switch (select_synonym_design_entity) {
    case PqlTokenType::STMT: {
      result_to_add = pkb->GetStmt(StmtType::STMT);
      break;
    }
    case PqlTokenType::READ: {
      result_to_add = pkb->GetStmt(StmtType::READ);
      break;
    }
    case PqlTokenType::PRINT: {
      result_to_add = pkb->GetStmt(StmtType::PRINT);
      break;
    }
    case PqlTokenType::CALL: {
      result_to_add = pkb->GetStmt(StmtType::CALL);
      break;
    }
    case PqlTokenType::WHILE: {
      result_to_add = pkb->GetStmt(StmtType::WHILE);
      break;
    }
    case PqlTokenType::IF: {
      result_to_add = pkb->GetStmt(StmtType::IF);
      break;
    }
    case PqlTokenType::ASSIGN: {
      result_to_add = pkb->GetStmt(StmtType::ASSIGN);
      break;
    }
    case PqlTokenType::VARIABLE: {
      result_to_add = pkb->GetStmt(StmtType::VARS);
      break;
    }
    case PqlTokenType::CONSTANT: {
      result_to_add = pkb->GetStmt(StmtType::CONSTS);
      break;
    }
    case PqlTokenType::PROCEDURE: {
      result_to_add = pkb->GetStmt(StmtType::PROC);
      break;
    }
    default: {
      // TODO:throw exception
    }
  }

  result.insert(result_to_add.begin(), result_to_add.end());
}

void QueryEvaluator::EvaluateSelectWithRelationship(ParsedQuery &query) {
  // assume only select with 1 relationship
  PqlToken select_synonym = query.GetSynonym();
  const auto declarations = query.GetDeclaration();
  PqlTokenType select_synonym_design_entity;
  Relationship relationship = query.GetRelationships().front();
  PqlTokenType rel_ref = relationship.GetRelRef().type;
  PqlToken first_arg = relationship.GetFirst();
  PqlToken second_arg = relationship.GetSecond();

  for (auto declaration : declarations) {
    if (declaration.GetSynonym().value==select_synonym.value) {
      select_synonym_design_entity = declaration.GetDesignEntity().type;
    }
  }

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> pair_result;
  std::unordered_set<std::string> result_to_add;
  switch (rel_ref) {
    case PqlTokenType::USES: {
      // 6 Total Cases (Handles UsesS now) TODO: refactor Uses to UsesS and UsesP
      // UsesS is a relationship between statement and variables
      // Statements: assign, while, if, call, print, read (total 6 types of statements)

      if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::SYNONYM) {
        // 1. Uses(s, v)
        if (select_synonym.value==first_arg.value) {
          // Select s such that Uses(s, v)
          switch (select_synonym_design_entity) {
            case PqlTokenType::STMT: {
              pair_result = pkb->GetAllUsesStmt(StmtType::STMT);
              break;
            }
            case PqlTokenType::ASSIGN: {
              pair_result = pkb->GetAllUsesStmt(StmtType::ASSIGN);
              break;
            }
            case PqlTokenType::WHILE: {
              pair_result = pkb->GetAllUsesStmt(StmtType::WHILE);
              break;
            }
            case PqlTokenType::IF: {
              pair_result = pkb->GetAllUsesStmt(StmtType::IF);
              break;
            }
            case PqlTokenType::PRINT: {
              pair_result = pkb->GetAllUsesStmt(StmtType::PRINT);
              break;
            }
            default: {
              // do nothing
              return;
            }
          }
          for (auto pair : pair_result) {
            result_to_add.insert(pair.first);
          }
        } else if (select_synonym.value==second_arg.value) {
          // Select v such that Uses(s, v)
          PqlTokenType first_arg_design_entity;
          for (auto declaration : declarations) {
            if (declaration.GetSynonym().value==first_arg.value) {
              first_arg_design_entity = declaration.GetDesignEntity().type;
            }
          }

          switch (first_arg_design_entity) {
            case PqlTokenType::STMT: {
              pair_result = pkb->GetAllUsesStmt(StmtType::STMT);
              break;
            }
            case PqlTokenType::ASSIGN: {
              pair_result = pkb->GetAllUsesStmt(StmtType::ASSIGN);
              break;
            }
            case PqlTokenType::WHILE: {
              pair_result = pkb->GetAllUsesStmt(StmtType::WHILE);
              break;
            }
            case PqlTokenType::IF: {
              pair_result = pkb->GetAllUsesStmt(StmtType::IF);
              break;
            }
            case PqlTokenType::PRINT: {
              pair_result = pkb->GetAllUsesStmt(StmtType::PRINT);
              break;
            }
            default: {
              // do nothing
              return;
            }
          }
          for (auto pair : pair_result) {
            result_to_add.insert(pair.second);
          }
        } else {
          // check if Uses(s, v)
          // is empty -> return None
          // else -> Evaluate the select

          PqlTokenType first_arg_design_entity;
          for (auto declaration : declarations) {
            if (declaration.GetSynonym().value==first_arg.value) {
              first_arg_design_entity = declaration.GetDesignEntity().type;
            }
          }

          switch (first_arg_design_entity) {
            case PqlTokenType::STMT: {
              pair_result = pkb->GetAllUsesStmt(StmtType::STMT);
              break;
            }
            case PqlTokenType::ASSIGN: {
              pair_result = pkb->GetAllUsesStmt(StmtType::ASSIGN);
              break;
            }
            case PqlTokenType::WHILE: {
              pair_result = pkb->GetAllUsesStmt(StmtType::WHILE);
              break;
            }
            case PqlTokenType::IF: {
              pair_result = pkb->GetAllUsesStmt(StmtType::IF);
              break;
            }
            case PqlTokenType::PRINT: {
              pair_result = pkb->GetAllUsesStmt(StmtType::PRINT);
              break;
            }
            default: {
              // do nothing
              return;
            }
          }

          if (!pair_result.empty()) {
            EvaluateSelectOnly(query);
          }

        }
      } else if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::UNDERSCORE) {
        // 2. Uses(s, _)
        if (select_synonym.value==first_arg.value) {
          switch (select_synonym_design_entity) {
            case PqlTokenType::STMT: {
              pair_result = pkb->GetAllUsesStmt(StmtType::STMT);
              break;
            }
            case PqlTokenType::ASSIGN: {
              pair_result = pkb->GetAllUsesStmt(StmtType::ASSIGN);
              break;
            }
            case PqlTokenType::WHILE: {
              pair_result = pkb->GetAllUsesStmt(StmtType::WHILE);
              break;
            }
            case PqlTokenType::IF: {
              pair_result = pkb->GetAllUsesStmt(StmtType::IF);
              break;
            }
            case PqlTokenType::PRINT: {
              pair_result = pkb->GetAllUsesStmt(StmtType::PRINT);
              break;
            }
            default: {
              // do nothing
              return;
            }
          }
          for (auto pair : pair_result) {
            result_to_add.insert(pair.first);
          }
        } else {
          // Check is Uses(s, _) is non-empty

          PqlTokenType first_arg_design_entity;
          for (auto declaration : declarations) {
            if (declaration.GetSynonym().value==first_arg.value) {
              first_arg_design_entity = declaration.GetDesignEntity().type;
            }
          }

          switch (first_arg_design_entity) {
            case PqlTokenType::STMT: {
              pair_result = pkb->GetAllUsesStmt(StmtType::STMT);
              break;
            }
            case PqlTokenType::ASSIGN: {
              pair_result = pkb->GetAllUsesStmt(StmtType::ASSIGN);
              break;
            }
            case PqlTokenType::WHILE: {
              pair_result = pkb->GetAllUsesStmt(StmtType::WHILE);
              break;
            }
            case PqlTokenType::IF: {
              pair_result = pkb->GetAllUsesStmt(StmtType::IF);
              break;
            }
            case PqlTokenType::PRINT: {
              pair_result = pkb->GetAllUsesStmt(StmtType::PRINT);
              break;
            }
            default: {
              // do nothing
              return;
            }
          }

          if (!pair_result.empty()) {
            EvaluateSelectOnly(query);
          }

        }
      } else if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::IDENT_WITH_QUOTES) {
        // 3. Uses(s, "x")
        std::string ident_without_quotes = second_arg.value.substr(1, second_arg.value.length() - 2);
        if (select_synonym.value==first_arg.value) {
          switch (select_synonym_design_entity) {
            case PqlTokenType::STMT: {
              pair_result = pkb->GetAllUsesStmt(StmtType::STMT);
              break;
            }
            case PqlTokenType::ASSIGN: {
              pair_result = pkb->GetAllUsesStmt(StmtType::ASSIGN);
              break;
            }
            case PqlTokenType::WHILE: {
              pair_result = pkb->GetAllUsesStmt(StmtType::WHILE);
              break;
            }
            case PqlTokenType::IF: {
              pair_result = pkb->GetAllUsesStmt(StmtType::IF);
              break;
            }
            case PqlTokenType::PRINT: {
              pair_result = pkb->GetAllUsesStmt(StmtType::PRINT);
              break;
            }
            default: {
              // do nothing
              return;
            }
          }

          for (auto pair : pair_result) {
            if (pair.second==ident_without_quotes) {
              result_to_add.insert(pair.first);
            }
          }

        } else {
          // selected synonym is not in the Uses clause
          // Check if Uses(s, "x") is non-empty

          PqlTokenType first_arg_design_entity;
          for (auto declaration : declarations) {
            if (declaration.GetSynonym().value==first_arg.value) {
              first_arg_design_entity = declaration.GetDesignEntity().type;
            }
          }

          switch (first_arg_design_entity) {
            case PqlTokenType::STMT: {
              pair_result = pkb->GetAllUsesStmt(StmtType::STMT);
              break;
            }
            case PqlTokenType::ASSIGN: {
              pair_result = pkb->GetAllUsesStmt(StmtType::ASSIGN);
              break;
            }
            case PqlTokenType::WHILE: {
              pair_result = pkb->GetAllUsesStmt(StmtType::WHILE);
              break;
            }
            case PqlTokenType::IF: {
              pair_result = pkb->GetAllUsesStmt(StmtType::IF);
              break;
            }
            case PqlTokenType::PRINT: {
              pair_result = pkb->GetAllUsesStmt(StmtType::PRINT);
              break;
            }
            default: {
              // do nothing
              return;
            }
          }

          bool is_empty = true;
          for (auto pair : pair_result) {
            if (pair.second==ident_without_quotes) {
              is_empty = false;
            }
          }

          if (!is_empty) {
            EvaluateSelectOnly(query);
          }
        }
      } else if (first_arg.type==PqlTokenType::NUMBER && second_arg.type==PqlTokenType::SYNONYM) {
        // 4. Uses(1, v)
        if (select_synonym.value==second_arg.value) {
          result_to_add = pkb->GetVarUsedByStmt(first_arg.value);
        } else {
          // selected synonym is not in the Uses clause
          result_to_add = pkb->GetVarUsedByStmt(first_arg.value);

          if (!result_to_add.empty()) {
            EvaluateSelectOnly(query);
          }
        }
      } else if (first_arg.type==PqlTokenType::NUMBER && second_arg.type==PqlTokenType::UNDERSCORE) {
        // 5. Uses(1, _)
        if (!pkb->GetVarUsedByStmt(first_arg.value).empty()) {
          // Line uses some variable
          EvaluateSelectOnly(query);
        } else {
          // Line does not use some variable
          return;
        }
      } else if (first_arg.type==PqlTokenType::NUMBER && second_arg.type==PqlTokenType::IDENT_WITH_QUOTES) {
        // 6. Uses(1, "x")
        std::string ident_without_quotes = second_arg.value.substr(1, second_arg.value.length() - 2);
        std::pair arg_pair(first_arg.value, ident_without_quotes);
        bool is_true = pkb->IsUsageStmtVarExist(arg_pair);
        if (is_true) { // defaults to case with just select
          EvaluateSelectOnly(query);
        } else { // none
          return;
        }
      }

      break;
    }

    case PqlTokenType::MODIFIES: {
      // 6 Total cases for ModifiesS
      // ModifiesS is a relationship between statement and variables
      // Statements: assign, while, if, call, print, read (total 6 types of statements)

      if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::SYNONYM) {
        // 1. Modifies(s, v)
        if (select_synonym.value==first_arg.value) {
          // Select s such that Modifies(s, v)
          switch (select_synonym_design_entity) {
            case PqlTokenType::STMT: {
              pair_result = pkb->GetAllModStmt(StmtType::STMT);
              break;
            }
            case PqlTokenType::ASSIGN: {
              pair_result = pkb->GetAllModStmt(StmtType::ASSIGN);
              break;
            }
            case PqlTokenType::WHILE: {
              pair_result = pkb->GetAllModStmt(StmtType::WHILE);
              break;
            }
            case PqlTokenType::IF: {
              pair_result = pkb->GetAllModStmt(StmtType::IF);
              break;
            }
            case PqlTokenType::READ: {
              pair_result = pkb->GetAllModStmt(StmtType::READ);
              break;
            }
            default: {
              // do nothing
              return;
            }
          }
          for (auto pair : pair_result) {
            result_to_add.insert(pair.first);
          }
        } else if (select_synonym.value==second_arg.value) {
          // Select v such that Modifies(s, v)
          PqlTokenType first_arg_design_entity;
          for (auto declaration : declarations) {
            if (declaration.GetSynonym().value==first_arg.value) {
              first_arg_design_entity = declaration.GetDesignEntity().type;
            }
          }

          switch (first_arg_design_entity) {
            case PqlTokenType::STMT: {
              pair_result = pkb->GetAllModStmt(StmtType::STMT);
              break;
            }
            case PqlTokenType::ASSIGN: {
              pair_result = pkb->GetAllModStmt(StmtType::ASSIGN);
              break;
            }
            case PqlTokenType::WHILE: {
              pair_result = pkb->GetAllModStmt(StmtType::WHILE);
              break;
            }
            case PqlTokenType::IF: {
              pair_result = pkb->GetAllModStmt(StmtType::IF);
              break;
            }
            case PqlTokenType::READ: {
              pair_result = pkb->GetAllModStmt(StmtType::READ);
              break;
            }
            default: {
              // do nothing
              return;
            }
          }
          for (auto pair : pair_result) {
            result_to_add.insert(pair.second);
          }
        } else {
          // check if Modifies(s, v)
          // is empty -> return None
          // else -> Evaluate the select

          PqlTokenType first_arg_design_entity;
          for (auto declaration : declarations) {
            if (declaration.GetSynonym().value==first_arg.value) {
              first_arg_design_entity = declaration.GetDesignEntity().type;
            }
          }

          switch (first_arg_design_entity) {
            case PqlTokenType::STMT: {
              pair_result = pkb->GetAllModStmt(StmtType::STMT);
              break;
            }
            case PqlTokenType::ASSIGN: {
              pair_result = pkb->GetAllModStmt(StmtType::ASSIGN);
              break;
            }
            case PqlTokenType::WHILE: {
              pair_result = pkb->GetAllModStmt(StmtType::WHILE);
              break;
            }
            case PqlTokenType::IF: {
              pair_result = pkb->GetAllModStmt(StmtType::IF);
              break;
            }
            case PqlTokenType::READ: {
              pair_result = pkb->GetAllModStmt(StmtType::READ);
              break;
            }
            default: {
              // do nothing
              return;
            }
          }

          if (!pair_result.empty()) {
            EvaluateSelectOnly(query);
          }

        }
      } else if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::UNDERSCORE) {
        // 2. Modifies(s, _)
        if (select_synonym.value==first_arg.value) {
          switch (select_synonym_design_entity) {
            case PqlTokenType::STMT: {
              pair_result = pkb->GetAllModStmt(StmtType::STMT);
              break;
            }
            case PqlTokenType::ASSIGN: {
              pair_result = pkb->GetAllModStmt(StmtType::ASSIGN);
              break;
            }
            case PqlTokenType::WHILE: {
              pair_result = pkb->GetAllModStmt(StmtType::WHILE);
              break;
            }
            case PqlTokenType::IF: {
              pair_result = pkb->GetAllModStmt(StmtType::IF);
              break;
            }
            case PqlTokenType::READ: {
              pair_result = pkb->GetAllModStmt(StmtType::READ);
              break;
            }
            default: {
              // do nothing
              return;
            }
          }
          for (auto pair : pair_result) {
            result_to_add.insert(pair.first);
          }
        } else {
          // Check is Modifies(s, _) is non-empty

          PqlTokenType first_arg_design_entity;
          for (auto declaration : declarations) {
            if (declaration.GetSynonym().value==first_arg.value) {
              first_arg_design_entity = declaration.GetDesignEntity().type;
            }
          }

          switch (first_arg_design_entity) {
            case PqlTokenType::STMT: {
              pair_result = pkb->GetAllModStmt(StmtType::STMT);
              break;
            }
            case PqlTokenType::ASSIGN: {
              pair_result = pkb->GetAllModStmt(StmtType::ASSIGN);
              break;
            }
            case PqlTokenType::WHILE: {
              pair_result = pkb->GetAllModStmt(StmtType::WHILE);
              break;
            }
            case PqlTokenType::IF: {
              pair_result = pkb->GetAllModStmt(StmtType::IF);
              break;
            }
            case PqlTokenType::READ: {
              pair_result = pkb->GetAllModStmt(StmtType::READ);
              break;
            }
            default: {
              // do nothing
              return;
            }
          }

          if (!pair_result.empty()) {
            EvaluateSelectOnly(query);
          }

        }
      } else if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::IDENT_WITH_QUOTES) {
        // 3. Modifies(s, "x")
        std::string ident_without_quotes = second_arg.value.substr(1, second_arg.value.length() - 2);
        if (select_synonym.value==first_arg.value) {
          switch (select_synonym_design_entity) {
            case PqlTokenType::STMT: {
              pair_result = pkb->GetAllModStmt(StmtType::STMT);
              break;
            }
            case PqlTokenType::ASSIGN: {
              pair_result = pkb->GetAllModStmt(StmtType::ASSIGN);
              break;
            }
            case PqlTokenType::WHILE: {
              pair_result = pkb->GetAllModStmt(StmtType::WHILE);
              break;
            }
            case PqlTokenType::IF: {
              pair_result = pkb->GetAllModStmt(StmtType::IF);
              break;
            }
            case PqlTokenType::READ: {
              pair_result = pkb->GetAllModStmt(StmtType::READ);
              break;
            }
            default: {
              // do nothing
              return;
            }
          }

          for (auto pair : pair_result) {
            if (pair.second==ident_without_quotes) {
              result_to_add.insert(pair.first);
            }
          }

        } else {
          // selected synonym is not in the Modifies clause
          // Check if Modifies(s, "x") is non-empty

          PqlTokenType first_arg_design_entity;
          for (auto declaration : declarations) {
            if (declaration.GetSynonym().value==first_arg.value) {
              first_arg_design_entity = declaration.GetDesignEntity().type;
            }
          }

          switch (first_arg_design_entity) {
            case PqlTokenType::STMT: {
              pair_result = pkb->GetAllModStmt(StmtType::STMT);
              break;
            }
            case PqlTokenType::ASSIGN: {
              pair_result = pkb->GetAllModStmt(StmtType::ASSIGN);
              break;
            }
            case PqlTokenType::WHILE: {
              pair_result = pkb->GetAllModStmt(StmtType::WHILE);
              break;
            }
            case PqlTokenType::IF: {
              pair_result = pkb->GetAllModStmt(StmtType::IF);
              break;
            }
            case PqlTokenType::READ: {
              pair_result = pkb->GetAllModStmt(StmtType::READ);
              break;
            }
            default: {
              // do nothing
              return;
            }
          }

          bool is_empty = true;
          for (auto pair : pair_result) {
            if (pair.second==ident_without_quotes) {
              is_empty = false;
            }
          }

          if (!is_empty) {
            EvaluateSelectOnly(query);
          }
        }
      } else if (first_arg.type==PqlTokenType::NUMBER && second_arg.type==PqlTokenType::SYNONYM) {
        // 4. Modifies(1, v)
        if (select_synonym.value==second_arg.value) {
          result_to_add = pkb->GetVarModByStmt(first_arg.value);
        } else {
          // selected synonym is not in the Modifies clause
          result_to_add = pkb->GetVarModByStmt(first_arg.value);

          if (!result_to_add.empty()) {
            EvaluateSelectOnly(query);
          }
        }
      } else if (first_arg.type==PqlTokenType::NUMBER && second_arg.type==PqlTokenType::UNDERSCORE) {
        // 5. Modifies(1, _)
        if (!pkb->GetVarModByStmt(first_arg.value).empty()) {
          // Line modifies some variable
          EvaluateSelectOnly(query);
        } else {
          // Line does not modify some variable
          return;
        }
      } else if (first_arg.type==PqlTokenType::NUMBER && second_arg.type==PqlTokenType::IDENT_WITH_QUOTES) {
        // 6. Modifies(1, "x")
        std::string ident_without_quotes = second_arg.value.substr(1, second_arg.value.length() - 2);
        std::pair arg_pair(first_arg.value, ident_without_quotes);
        bool is_true = pkb->IsModifyStmtVarExist(arg_pair);
        if (is_true) { // defaults to case with just select
          EvaluateSelectOnly(query);
        } else { // none
          return;
        }
      }
      break;
    }
    case PqlTokenType::FOLLOWS: {
      // Follows is a relationship between statements
      // Total 6 types of statements (assign, while, if, call, print, read)
      // Total 9 cases

      if (first_arg.type==PqlTokenType::NUMBER && second_arg.type==PqlTokenType::NUMBER) {
        // 1. Follows(1, 2)
        if (pkb->GetFollowingOf(first_arg.value)==second_arg.value) {
          // clause is true
          EvaluateSelectOnly(query);
        }
      } else if (first_arg.type==PqlTokenType::NUMBER && second_arg.type==PqlTokenType::UNDERSCORE) {
        // 2. Follows(1, _)
        if (pkb->GetFollowingOf(first_arg.value)!="0") {
          EvaluateSelectOnly(query);
        }
      } else if (first_arg.type==PqlTokenType::NUMBER && second_arg.type==PqlTokenType::SYNONYM) {
        // 3. Follows(9, s)

        PqlTokenType second_arg_design_entity;
        for (auto declaration : declarations) {
          if (declaration.GetSynonym().value==second_arg.value) {
            second_arg_design_entity = declaration.GetDesignEntity().type;
          }
        }

        const bool is_empty = pkb->GetFollowingOf(first_arg.value)=="0";
        if (!is_empty) {
          if (select_synonym.value==second_arg.value) {
            result_to_add.insert(pkb->GetFollowingOf(first_arg.value));
          } else {
            EvaluateSelectOnly(query);
          }
        }
      } else if (first_arg.type==PqlTokenType::UNDERSCORE && second_arg.type==PqlTokenType::NUMBER) {
        // 4. Follows(_, 9)
        pair_result = pkb->GetAllFollowStmt(StmtType::STMT);
        bool is_empty = true;
        for (auto pair : pair_result) {
          if (pair.second==second_arg.value) {
            is_empty = false;
          }
        }
        if (!is_empty) {
          EvaluateSelectOnly(query);
        }
      } else if (first_arg.type==PqlTokenType::UNDERSCORE && second_arg.type==PqlTokenType::UNDERSCORE) {
        // 5. Follows(_, _)
        pair_result = pkb->GetAllFollowStmt(StmtType::STMT, StmtType::STMT);
        if (!pair_result.empty()) {
          EvaluateSelectOnly(query);
        }
      } else if (first_arg.type==PqlTokenType::UNDERSCORE && second_arg.type==PqlTokenType::SYNONYM) {
        // 6. Follows(_, s)
        PqlTokenType second_arg_design_entity;
        for (auto declaration : declarations) {
          if (declaration.GetSynonym().value==second_arg.value) {
            second_arg_design_entity = declaration.GetDesignEntity().type;
          }
        }

        pair_result = pkb->GetAllFollowStmt(StmtType::STMT, GetStmtType(second_arg_design_entity));
        if (select_synonym.value==second_arg.value) {
          for (auto pair : pair_result) {
            result_to_add.insert(pair.second);
          }
        } else if (!pair_result.empty()) {
          EvaluateSelectOnly(query);
        }

      } else if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::NUMBER) {
        // 7. Follows(s, 8)
        PqlTokenType first_arg_design_entity;
        for (auto declaration : declarations) {
          if (declaration.GetSynonym().value==first_arg.value) {
            first_arg_design_entity = declaration.GetDesignEntity().type;
          }
        }

        const bool is_empty = pkb->GetFollowerOf(second_arg.value)=="0";
        if (!is_empty) {
          if (select_synonym.value==first_arg.value) {
            result_to_add.insert(pkb->GetFollowerOf(second_arg.value));
          } else {
            // clause is false no possible s
            EvaluateSelectOnly(query);
          }
        }

      } else if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::UNDERSCORE) {
        // 8. Follows(s, _)
        PqlTokenType first_arg_design_entity;
        for (auto declaration : declarations) {
          if (declaration.GetSynonym().value==first_arg.value) {
            first_arg_design_entity = declaration.GetDesignEntity().type;
          }
        }

        pair_result = pkb->GetAllFollowStmt(GetStmtType(first_arg_design_entity), StmtType::STMT);
        if (select_synonym.value==first_arg.value) {
          for (auto pair : pair_result) {
            result_to_add.insert(pair.first);
          }
        } else if (!pair_result.empty()) {
          EvaluateSelectOnly(query);
        }

      } else if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::SYNONYM) {
        // 9. Follows(s1, s2)
        PqlTokenType first_arg_design_entity;
        PqlTokenType second_arg_design_entity;
        for (auto declaration : declarations) {
          if (declaration.GetSynonym().value==first_arg.value) {
            first_arg_design_entity = declaration.GetDesignEntity().type;
          }
          if (declaration.GetSynonym().value==second_arg.value) {
            second_arg_design_entity = declaration.GetDesignEntity().type;
          }
        }

        pair_result =
            pkb->GetAllFollowStmt(GetStmtType(first_arg_design_entity), GetStmtType(second_arg_design_entity));
        if (select_synonym.value==first_arg.value) {
          for (auto pair : pair_result) {
            result_to_add.insert(pair.first);
          }
        } else if (select_synonym.value==second_arg.value) {
          for (auto pair : pair_result) {
            result_to_add.insert(pair.second);
          }
        } else if (!pair_result.empty()) {
          EvaluateSelectOnly(query);
        }
      }
      break;
    }
    case PqlTokenType::FOLLOWS_T: {
      // Follows* is a relationship between statements
      // Total 6 types of statements (assign, while, if, call, print, read)
      // Total 9 cases

      std::unordered_set<std::string> followings;
      std::unordered_set<std::string> followers;

      if (first_arg.type==PqlTokenType::NUMBER && second_arg.type==PqlTokenType::NUMBER) {
        // 1. Follows*(1, 2)
        followings = pkb->GetFollowingStarOf(first_arg.value);
        if (followings.find(second_arg.value) != followings.end()) {
          // clause is true
          EvaluateSelectOnly(query);
        }
      } else if (first_arg.type==PqlTokenType::NUMBER && second_arg.type==PqlTokenType::UNDERSCORE) {
        // 2. Follows*(1, _)
        followings = pkb->GetFollowingStarOf(first_arg.value);
        if (!followings.empty()) {
          EvaluateSelectOnly(query);
        }
      } else if (first_arg.type==PqlTokenType::NUMBER && second_arg.type==PqlTokenType::SYNONYM) {
        // 3. Follows*(9, s)

        PqlTokenType second_arg_design_entity;
        for (auto declaration : declarations) {
          if (declaration.GetSynonym().value==second_arg.value) {
            second_arg_design_entity = declaration.GetDesignEntity().type;
          }
        }

        followings = pkb->GetFollowingStarOf(first_arg.value);
        if (select_synonym.value==second_arg.value) {
          result_to_add.insert(followings.begin(), followings.end());
        } else if (!followings.empty()) {
          EvaluateSelectOnly(query);
        }

      } else if (first_arg.type==PqlTokenType::UNDERSCORE && second_arg.type==PqlTokenType::NUMBER) {
        // 4. Follows*(_, 9)
        followers = pkb->GetFollowerStarOf(second_arg.value);
        if (!followers.empty()) {
          EvaluateSelectOnly(query);
        }
      } else if (first_arg.type==PqlTokenType::UNDERSCORE && second_arg.type==PqlTokenType::UNDERSCORE) {
        // 5. Follows*(_, _)
        pair_result = pkb->GetAllFollowStarStmt(StmtType::STMT, StmtType::STMT);
        if (!pair_result.empty()) {
          EvaluateSelectOnly(query);
        }
      } else if (first_arg.type==PqlTokenType::UNDERSCORE && second_arg.type==PqlTokenType::SYNONYM) {
        // 6. Follows*(_, s)
        PqlTokenType second_arg_design_entity;
        for (auto declaration : declarations) {
          if (declaration.GetSynonym().value==second_arg.value) {
            second_arg_design_entity = declaration.GetDesignEntity().type;
          }
        }

        pair_result = pkb->GetAllFollowStarStmt(StmtType::STMT, GetStmtType(second_arg_design_entity));
        if (select_synonym.value==second_arg.value) {
          for (auto pair : pair_result) {
            result_to_add.insert(pair.second);
          }
        } else if (!pair_result.empty()) {
          EvaluateSelectOnly(query);
        }

      } else if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::NUMBER) {
        // 7. Follows*(s, 8)
        PqlTokenType first_arg_design_entity;
        for (auto declaration : declarations) {
          if (declaration.GetSynonym().value==first_arg.value) {
            first_arg_design_entity = declaration.GetDesignEntity().type;
          }
        }

        followers = pkb->GetFollowerStarOf(second_arg.value);
        if (select_synonym.value==first_arg.value) {
          result_to_add.insert(followers.begin(), followers.end());
        } else if (!followers.empty()) {
          EvaluateSelectOnly(query);
        }

      } else if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::UNDERSCORE) {
        // 8. Follows*(s, _)
        PqlTokenType first_arg_design_entity;
        for (auto declaration : declarations) {
          if (declaration.GetSynonym().value==first_arg.value) {
            first_arg_design_entity = declaration.GetDesignEntity().type;
          }
        }

        pair_result = pkb->GetAllFollowStarStmt(GetStmtType(first_arg_design_entity), StmtType::STMT);
        if (select_synonym.value==first_arg.value) {
          for (auto pair : pair_result) {
            result_to_add.insert(pair.first);
          }
        } else if (!pair_result.empty()) {
          EvaluateSelectOnly(query);
        }

      } else if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::SYNONYM) {
        // 9. Follows*(s1, s2)
        PqlTokenType first_arg_design_entity;
        PqlTokenType second_arg_design_entity;
        for (auto declaration : declarations) {
          if (declaration.GetSynonym().value==first_arg.value) {
            first_arg_design_entity = declaration.GetDesignEntity().type;
          }
          if (declaration.GetSynonym().value==second_arg.value) {
            second_arg_design_entity = declaration.GetDesignEntity().type;
          }
        }

        pair_result =
            pkb->GetAllFollowStarStmt(GetStmtType(first_arg_design_entity), GetStmtType(second_arg_design_entity));
        if (select_synonym.value==first_arg.value) {
          for (auto pair : pair_result) {
            result_to_add.insert(pair.first);
          }
        } else if (select_synonym.value==second_arg.value) {
          for (auto pair : pair_result) {
            result_to_add.insert(pair.second);
          }
        } else if (!pair_result.empty()) {
          EvaluateSelectOnly(query);
        }
      }
      break;
    }
    case PqlTokenType::PARENT_T: {
      // Parent* is a relationship between statements
      // Total 6 types of statements (assign, while, if, call, print, read)
      // Total 9 cases

      std::unordered_set<std::string> descendants;
      std::unordered_set<std::string> ancestors;

      if (first_arg.type==PqlTokenType::NUMBER && second_arg.type==PqlTokenType::NUMBER) {
        // 1. Parent*(1, 2)
        descendants = pkb->GetDescOf(first_arg.value);
        if (descendants.find(second_arg.value) != descendants.end()) {
          // clause is true
          EvaluateSelectOnly(query);
        }
      } else if (first_arg.type==PqlTokenType::NUMBER && second_arg.type==PqlTokenType::UNDERSCORE) {
        // 2. Parent*(1, _)
        descendants = pkb->GetDescOf(first_arg.value);
        if (!descendants.empty()) {
          EvaluateSelectOnly(query);
        }
      } else if (first_arg.type==PqlTokenType::NUMBER && second_arg.type==PqlTokenType::SYNONYM) {
        // 3. Parent*(9, s)

        PqlTokenType second_arg_design_entity;
        for (auto declaration : declarations) {
          if (declaration.GetSynonym().value==second_arg.value) {
            second_arg_design_entity = declaration.GetDesignEntity().type;
          }
        }

        pair_result = pkb->GetAllParentStarStmt(StmtType::STMT, GetStmtType(second_arg_design_entity));
        bool is_empty = true;
        for (auto pair : pair_result) {
          if (pair.first==first_arg.value) {
            is_empty = false;
            break;
          }
        }
        if (!is_empty) {
          if (select_synonym.value==second_arg.value) {
            for (auto pair : pair_result) {
              if (pair.first==first_arg.value) {
                result_to_add.insert(pair.second);
              }
            }
          } else {
            EvaluateSelectOnly(query);
          }
        }

      } else if (first_arg.type==PqlTokenType::UNDERSCORE && second_arg.type==PqlTokenType::NUMBER) {
        // 4. Parent*(_, 9)
        ancestors = pkb->GetAnceOf(second_arg.value);
        if (!ancestors.empty()) {
          EvaluateSelectOnly(query);
        }
      } else if (first_arg.type==PqlTokenType::UNDERSCORE && second_arg.type==PqlTokenType::UNDERSCORE) {
        // 5. Parent*(_, _)
        pair_result = pkb->GetAllParentStarStmt(StmtType::STMT, StmtType::STMT);
        if (!pair_result.empty()) {
          EvaluateSelectOnly(query);
        }
      } else if (first_arg.type==PqlTokenType::UNDERSCORE && second_arg.type==PqlTokenType::SYNONYM) {
        // 6. Parent*(_, s)
        PqlTokenType second_arg_design_entity;
        for (auto declaration : declarations) {
          if (declaration.GetSynonym().value==second_arg.value) {
            second_arg_design_entity = declaration.GetDesignEntity().type;
          }
        }

        pair_result = pkb->GetAllParentStarStmt(StmtType::STMT, GetStmtType(second_arg_design_entity));
        if (select_synonym.value==second_arg.value) {
          for (auto pair : pair_result) {
            result_to_add.insert(pair.second);
          }
        } else if (!pair_result.empty()) {
          EvaluateSelectOnly(query);
        }

      } else if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::NUMBER) {
        // 7. Parent*(s, 8)
        PqlTokenType first_arg_design_entity;
        for (auto declaration : declarations) {
          if (declaration.GetSynonym().value==first_arg.value) {
            first_arg_design_entity = declaration.GetDesignEntity().type;
          }
        }

        ancestors = pkb->GetAnceOf(second_arg.value);
        if (select_synonym.value==first_arg.value) {
          result_to_add.insert(ancestors.begin(), ancestors.end());
        } else if (!ancestors.empty()) {
          EvaluateSelectOnly(query);
        }

      } else if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::UNDERSCORE) {
        // 8. Parent*(s, _)
        PqlTokenType first_arg_design_entity;
        for (auto declaration : declarations) {
          if (declaration.GetSynonym().value==first_arg.value) {
            first_arg_design_entity = declaration.GetDesignEntity().type;
          }
        }

        pair_result = pkb->GetAllParentStarStmt(GetStmtType(first_arg_design_entity), StmtType::STMT);
        if (select_synonym.value==first_arg.value) {
          for (auto pair : pair_result) {
            result_to_add.insert(pair.first);
          }
        } else if (!pair_result.empty()) {
          EvaluateSelectOnly(query);
        }

      } else if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::SYNONYM) {
        // 9. Parent*(s1, s2)
        PqlTokenType first_arg_design_entity;
        PqlTokenType second_arg_design_entity;
        for (auto declaration : declarations) {
          if (declaration.GetSynonym().value==first_arg.value) {
            first_arg_design_entity = declaration.GetDesignEntity().type;
          }
          if (declaration.GetSynonym().value==second_arg.value) {
            second_arg_design_entity = declaration.GetDesignEntity().type;
          }
        }

        pair_result =
            pkb->GetAllParentStarStmt(GetStmtType(first_arg_design_entity), GetStmtType(second_arg_design_entity));
        if (select_synonym.value==first_arg.value) {
          for (auto pair : pair_result) {
            result_to_add.insert(pair.first);
          }
        } else if (select_synonym.value==second_arg.value) {
          for (auto pair : pair_result) {
            result_to_add.insert(pair.second);
          }
        } else if (!pair_result.empty()) {
          EvaluateSelectOnly(query);
        }
      }
      break;
    }
    case PqlTokenType::PARENT: {
      // Parent is a relationship between statements
      // Total 6 types of statements (assign, while, if, call, print, read)
      // Total 9 cases

      if (first_arg.type==PqlTokenType::NUMBER && second_arg.type==PqlTokenType::NUMBER) {
        // 1. Parent(1, 2)
        if (pkb->ParentChildExists(first_arg.value, second_arg.value)) {
          // clause is true
          EvaluateSelectOnly(query);
        }
      } else if (first_arg.type==PqlTokenType::NUMBER && second_arg.type==PqlTokenType::UNDERSCORE) {
        // 2. Parent(1, _)
        if (!pkb->GetChildOf(first_arg.value).empty()) {
          EvaluateSelectOnly(query);
        }
      } else if (first_arg.type==PqlTokenType::NUMBER && second_arg.type==PqlTokenType::SYNONYM) {
        // 3. Parent(9, s)

        PqlTokenType second_arg_design_entity;
        for (auto declaration : declarations) {
          if (declaration.GetSynonym().value==second_arg.value) {
            second_arg_design_entity = declaration.GetDesignEntity().type;
          }
        }

        pair_result = pkb->GetAllParentStmt(StmtType::STMT, GetStmtType(second_arg_design_entity));
        bool is_empty = true;
        for (auto pair : pair_result) {
          if (pair.first==first_arg.value) {
            is_empty = false;
            break;
          }
        }
        if (!is_empty) {
          if (select_synonym.value==second_arg.value) {
            for (auto pair : pair_result) {
              if (pair.first==first_arg.value) {
                result_to_add.insert(pair.second);
              }
            }
          } else {
            EvaluateSelectOnly(query);
          }
        }

      } else if (first_arg.type==PqlTokenType::UNDERSCORE && second_arg.type==PqlTokenType::NUMBER) {
        // 4. Parent(_, 9)
        if (pkb->GetParentOf(second_arg.value)!="0") {
          EvaluateSelectOnly(query);
        }
      } else if (first_arg.type==PqlTokenType::UNDERSCORE && second_arg.type==PqlTokenType::UNDERSCORE) {
        // 5. Parent(_, _)
        pair_result = pkb->GetAllParentStmt(StmtType::STMT, StmtType::STMT);
        if (!pair_result.empty()) {
          EvaluateSelectOnly(query);
        }
      } else if (first_arg.type==PqlTokenType::UNDERSCORE && second_arg.type==PqlTokenType::SYNONYM) {
        // 6. Parent(_, s)
        PqlTokenType second_arg_design_entity;
        for (auto declaration : declarations) {
          if (declaration.GetSynonym().value==second_arg.value) {
            second_arg_design_entity = declaration.GetDesignEntity().type;
          }
        }

        pair_result = pkb->GetAllParentStmt(StmtType::STMT, GetStmtType(second_arg_design_entity));
        if (select_synonym.value==second_arg.value) {
          for (auto pair : pair_result) {
            result_to_add.insert(pair.second);
          }
        } else if (!pair_result.empty()) {
          EvaluateSelectOnly(query);
        }

      } else if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::NUMBER) {
        // 7. Parent(s, 8)
        PqlTokenType first_arg_design_entity;
        for (auto declaration : declarations) {
          if (declaration.GetSynonym().value==first_arg.value) {
            first_arg_design_entity = declaration.GetDesignEntity().type;
          }
        }

        const bool is_empty = pkb->GetParentOf(second_arg.value)=="0";
        if (!is_empty) {
          if (select_synonym.value==first_arg.value) {
            result_to_add.insert(pkb->GetParentOf(second_arg.value));
          } else {
            EvaluateSelectOnly(query);
          }
        }

      } else if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::UNDERSCORE) {
        // 8. Parent(s, _)
        PqlTokenType first_arg_design_entity;
        for (auto declaration : declarations) {
          if (declaration.GetSynonym().value==first_arg.value) {
            first_arg_design_entity = declaration.GetDesignEntity().type;
          }
        }

        pair_result = pkb->GetAllParentStmt(GetStmtType(first_arg_design_entity), StmtType::STMT);
        if (select_synonym.value==first_arg.value) {
          for (auto pair : pair_result) {
            result_to_add.insert(pair.first);
          }
        } else if (!pair_result.empty()) {
          EvaluateSelectOnly(query);
        }

      } else if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::SYNONYM) {
        // 9. Parent(s1, s2)
        PqlTokenType first_arg_design_entity;
        PqlTokenType second_arg_design_entity;
        for (auto declaration : declarations) {
          if (declaration.GetSynonym().value==first_arg.value) {
            first_arg_design_entity = declaration.GetDesignEntity().type;
          }
          if (declaration.GetSynonym().value==second_arg.value) {
            second_arg_design_entity = declaration.GetDesignEntity().type;
          }
        }

        pair_result =
            pkb->GetAllParentStmt(GetStmtType(first_arg_design_entity), GetStmtType(second_arg_design_entity));
        if (select_synonym.value==first_arg.value) {
          for (auto pair : pair_result) {
            result_to_add.insert(pair.first);
          }
        } else if (select_synonym.value==second_arg.value) {
          for (auto pair : pair_result) {
            result_to_add.insert(pair.second);
          }
        } else if (!pair_result.empty()) {
          EvaluateSelectOnly(query);
        }
      }
      break;
    }
    default: {

    }
  }

  result.insert(result_to_add.begin(), result_to_add.end());
}

void QueryEvaluator::EvaluateSelectWithPattern(ParsedQuery &query) {
  // For iteration 1 we only have patterns for assignment statements
  // Assume we only evaluate a syntactically and semantically valid query
  // i.e. the pattern synonym is of assign design-entity and if first argument is a synonym it must be a variable design-entity
  // Total of 6 possible cases given that 3 entRef possibilities * 2 exp-spec possibilitiesd

  PqlToken select_synonym = query.GetSynonym();
  const auto declarations = query.GetDeclaration();
  PqlTokenType select_synonym_design_entity;
  Pattern pattern = query.GetPatterns().front();
  PqlToken pattern_synonym = pattern.GetSynAssign();
  PqlToken first_arg = pattern.GetFirst();
  PqlToken second_arg = pattern.GetSecond();

  for (auto declaration : declarations) {
    if (declaration.GetSynonym().value==select_synonym.value) {
      select_synonym_design_entity = declaration.GetDesignEntity().type;
    }
  }

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> pair_result;
  std::unordered_set<std::string> result_to_add;

  if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::UNDERSCORE) {
    // 1. pattern a(v, _)
    if (select_synonym.value==pattern_synonym.value) {
      // Select a pattern a(v, _)
      pair_result = pkb->GetStmtWithPatternSynonym("_");
      for (auto pair : pair_result) {
        result_to_add.insert(pair.first);
      }
    } else if (select_synonym.value==first_arg.value) {
      // Select v pattern a(v, _)
      pair_result = pkb->GetStmtWithPatternSynonym("_");
      for (auto pair : pair_result) {
        result_to_add.insert(pair.second);
      }
    } else {
      // check if pattern a(v, _) is non-empty
      if (!pkb->GetStmtWithPatternSynonym("_").empty()) {
        EvaluateSelectOnly(query);
      }
    }
  } else if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::SUB_EXPRESSION) {
    // 2. pattern a(v, _"x"_)
    if (select_synonym.value==pattern_synonym.value) {
      // Select a pattern a(v, _"x"_)
      pair_result = pkb->GetStmtWithPatternSynonym(second_arg.value);
      for (auto pair : pair_result) {
        result_to_add.insert(pair.first);
      }
    } else if (select_synonym.value==first_arg.value) {
      // Select v pattern a(v, _)
      pair_result = pkb->GetStmtWithPatternSynonym(second_arg.value);
      for (auto pair : pair_result) {
        result_to_add.insert(pair.second);
      }
    } else {
      // check if pattern a(v, _) is non-empty
      if (!pkb->GetStmtWithPatternSynonym(second_arg.value).empty()) {
        EvaluateSelectOnly(query);
      }
    }
  } else if (first_arg.type==PqlTokenType::UNDERSCORE && second_arg.type==PqlTokenType::UNDERSCORE) {
    // 3. pattern a(_, _)
    if (select_synonym.value==pattern_synonym.value) {
      // Select a pattern a(_, _)
      result_to_add = pkb->GetStmtWithPattern("_", "_");
    } else {
      // check if pattern a(_, _) is non-empty
      if (!pkb->GetStmtWithPattern("_", "_").empty()) {
        EvaluateSelectOnly(query);
      }
    }
  } else if (first_arg.type==PqlTokenType::UNDERSCORE && second_arg.type==PqlTokenType::SUB_EXPRESSION) {
    // 4. pattern a(_, _"x"_)
    if (select_synonym.value==pattern_synonym.value) {
      // Select a pattern a(_, _"x"_)
      result_to_add = pkb->GetStmtWithPattern("_", second_arg.value);
    } else {
      // check if pattern a(_, _"x"_) is non-empty
      if (!pkb->GetStmtWithPattern("_", second_arg.value).empty()) {
        EvaluateSelectOnly(query);
      }
    }
  } else if (first_arg.type==PqlTokenType::IDENT_WITH_QUOTES && second_arg.type==PqlTokenType::UNDERSCORE) {
    // 5. pattern a("x", _)
    std::string ident_without_quotes = first_arg.value.substr(1, first_arg.value.length() - 2);
    if (select_synonym.value==pattern_synonym.value) {
      // Select a pattern a("x", _)
      result_to_add = pkb->GetStmtWithPattern(ident_without_quotes, "_");
    } else {
      // check if pattern a("x", _) is non-empty
      if (!pkb->GetStmtWithPattern(ident_without_quotes, "_").empty()) {
        EvaluateSelectOnly(query);
      }
    }
  } else if (first_arg.type==PqlTokenType::IDENT_WITH_QUOTES && second_arg.type==PqlTokenType::SUB_EXPRESSION) {
    // 6. pattern a("x", _"y"_)
    std::string ident_without_quotes = first_arg.value.substr(1, first_arg.value.length() - 2);
    if (select_synonym.value==pattern_synonym.value) {
      // Select a pattern a("x", _"y"_)
      result_to_add = pkb->GetStmtWithPattern(ident_without_quotes, second_arg.value);
    } else {
      // check if pattern a("x", _"y"_) is non-empty
      if (!pkb->GetStmtWithPattern(ident_without_quotes, second_arg.value).empty()) {
        EvaluateSelectOnly(query);
      }
    }
  }

  result.insert(result_to_add.begin(), result_to_add.end());
}

void QueryEvaluator::EvaluateSelectWithRelationshipAndPattern(ParsedQuery &query) {
  PqlToken selected_synonym = query.GetSynonym();
  std::vector<Declaration> declarations = query.GetDeclaration();
  Relationship relationship = query.GetRelationships().front();
  Pattern pattern = query.GetPatterns().front();

  PqlTokenType relationship_type = relationship.GetRelRef().type;
  PqlToken rel_first_arg = relationship.GetFirst();
  PqlToken rel_second_arg = relationship.GetSecond();

  if (rel_first_arg.value == rel_second_arg.value) {
    return;
  }

  if (relationship.GetFirst().type != PqlTokenType::SYNONYM &&
      relationship.GetSecond().type != PqlTokenType::SYNONYM) { // no synonym in such that clause
    bool such_that_bool_result;
    std::string rel_second_no_quote;
    if (relationship.GetSecond().type != PqlTokenType::UNDERSCORE) {
      rel_second_no_quote = rel_second_arg.value.substr(1, rel_second_arg.value.length() - 2);
    }
    if (relationship_type == PqlTokenType::USES) {
      such_that_bool_result = pkb->IsUsageStmtVarExist(std::make_pair(rel_first_arg.value, rel_second_no_quote));
    } else if (relationship_type == PqlTokenType::MODIFIES) {
      such_that_bool_result = pkb->IsModifyStmtVarExist(std::make_pair(rel_first_arg.value, rel_second_no_quote));
    } else if (relationship_type == PqlTokenType::PARENT) {
      such_that_bool_result = pkb->GetParentOf(rel_first_arg.value) == rel_second_arg.value;
    } else if (relationship_type == PqlTokenType::PARENT_T) {
      such_that_bool_result = pkb->GetAnceOf(rel_first_arg.value).count(rel_second_arg.value);
    } else if (relationship_type == PqlTokenType::FOLLOWS) {
      such_that_bool_result = pkb->IsFollowExist(std::make_pair(rel_first_arg.value, rel_second_arg.value));
    } else if (relationship_type == PqlTokenType::FOLLOWS_T) {
      such_that_bool_result = pkb->IsFollowStarExist(std::make_pair(rel_first_arg.value, rel_second_arg.value));
    }

    if (such_that_bool_result) { // resort to select pattern
      EvaluateSelectWithPattern(query);
    } else { // none
      return;
    }

  } else { // there is a synonym in such that clause
    PqlToken pattern_ass_arg = pattern.GetSynAssign();
    PqlToken pattern_first_arg = pattern.GetFirst();

    QueryCondition rel_condition = QueryCondition(rel_first_arg, rel_second_arg);
    QueryCondition pattern_condition = QueryCondition(pattern_ass_arg, pattern_first_arg);

    std::unordered_set<std::pair<std::string, std::string>, pair_hash> rel_result_set;
    std::unordered_set<std::pair<std::string, std::string>, pair_hash> temp_set;
    std::unordered_set<std::string> single_result_set;

    if (relationship.GetFirst().type == PqlTokenType::SYNONYM &&
        relationship.GetSecond().type != PqlTokenType::SYNONYM) {  // (s, "x")
      std::string second_arg = relationship.GetSecond().value;

      PqlTokenType first_arg_design_entity;
      for (auto declaration : declarations) {
        if (declaration.GetSynonym().value==relationship.GetFirst().value) {
          first_arg_design_entity = declaration.GetDesignEntity().type;
        }
      }

      if (relationship_type == PqlTokenType::USES) {
        if (relationship.GetSecond().type != PqlTokenType::UNDERSCORE) {
          second_arg = second_arg.substr(1, second_arg.length() - 2);
        }
        temp_set = pkb->GetAllUsesStmt(GetStmtType(first_arg_design_entity));
      } else if (relationship_type == PqlTokenType::MODIFIES) {
        if (relationship.GetSecond().type != PqlTokenType::UNDERSCORE) {
          second_arg = second_arg.substr(1, second_arg.length() - 2);
        }
        temp_set = pkb->GetAllModStmt(GetStmtType(first_arg_design_entity));
      } else if (relationship_type == PqlTokenType::PARENT) {
        temp_set = pkb->GetAllParentStmt(GetStmtType(first_arg_design_entity));
      } else if (relationship_type == PqlTokenType::PARENT_T) {
        temp_set = pkb->GetAllParentStarStmt(GetStmtType(first_arg_design_entity));
      } else if (relationship_type == PqlTokenType::FOLLOWS) {
        temp_set = pkb->GetAllFollowStmt(GetStmtType(first_arg_design_entity));
      } else if (relationship_type == PqlTokenType::FOLLOWS_T) {
        temp_set = pkb->GetAllFollowStarStmt(GetStmtType(first_arg_design_entity));
      }

      for (auto pair : temp_set) {
        if (second_arg == "_") {
          single_result_set.insert(pair.first);
        } else if (pair.second == second_arg) {
          single_result_set.insert(pair.first);
        }
      }

      for (auto single_result : single_result_set) {
        rel_result_set.insert(std::make_pair( single_result, relationship.GetSecond().value));
      }
    } else if (relationship.GetFirst().type != PqlTokenType::SYNONYM &&
        relationship.GetSecond().type == PqlTokenType::SYNONYM) {  // (1, v)

      PqlTokenType second_arg_design_entity;
      for (auto declaration : declarations) {
        if (declaration.GetSynonym().value==relationship.GetSecond().value) {
          second_arg_design_entity = declaration.GetDesignEntity().type;
        }
      }

      if (relationship_type == PqlTokenType::USES) {
        single_result_set = pkb->GetVarUsedByStmt(relationship.GetFirst().value);
      } else if (relationship_type == PqlTokenType::MODIFIES) {
        single_result_set = pkb->GetVarModByStmt(relationship.GetFirst().value);
      } else if (relationship_type == PqlTokenType::PARENT) {
        temp_set = pkb->GetAllParentStmt(GetStmtType(second_arg_design_entity));
      } else if (relationship_type == PqlTokenType::PARENT_T) {
        temp_set = pkb->GetAllParentStarStmt(GetStmtType(second_arg_design_entity));
      } else if (relationship_type == PqlTokenType::FOLLOWS) {
        temp_set = pkb->GetAllFollowStmt(GetStmtType(second_arg_design_entity));
      } else if (relationship_type == PqlTokenType::FOLLOWS_T) {
        temp_set = pkb->GetAllFollowStarStmt(GetStmtType(second_arg_design_entity));
      }

      if (!temp_set.empty()) {
        for (auto pair : temp_set) {
          if (pair.first == relationship.GetFirst().value) {
            single_result_set.insert(pair.second);
          }
        }
      }

      for (auto single_result : single_result_set) {
        rel_result_set.insert(std::make_pair(relationship.GetFirst().value, single_result));
      }
    } else if (relationship.GetFirst().type == PqlTokenType::SYNONYM &&
        relationship.GetSecond().type == PqlTokenType::SYNONYM) {
      std::string second_arg = relationship.GetSecond().value;

      PqlTokenType first_arg_design_entity;
      PqlTokenType second_arg_design_entity;
      for (auto declaration : declarations) {
        if (declaration.GetSynonym().value==relationship.GetFirst().value) {
          first_arg_design_entity = declaration.GetDesignEntity().type;
        }
        if (declaration.GetSynonym().value==relationship.GetSecond().value) {
          second_arg_design_entity = declaration.GetDesignEntity().type;
        }
      }

      if (relationship_type == PqlTokenType::USES) {
        second_arg = second_arg.substr(1, second_arg.length() - 2);
        rel_result_set = pkb->GetAllUsesStmt(GetStmtType(first_arg_design_entity));
      } else if (relationship_type == PqlTokenType::MODIFIES) {
        second_arg = second_arg.substr(1, second_arg.length() - 2);
        rel_result_set = pkb->GetAllModStmt(GetStmtType(first_arg_design_entity));
      } else if (relationship_type == PqlTokenType::PARENT) {
        rel_result_set = pkb->GetAllParentStmt(GetStmtType(first_arg_design_entity),
                                               GetStmtType(second_arg_design_entity));
      } else if (relationship_type == PqlTokenType::PARENT_T) {
        rel_result_set = pkb->GetAllParentStarStmt(GetStmtType(first_arg_design_entity),
                                                   GetStmtType(second_arg_design_entity));
      } else if (relationship_type == PqlTokenType::FOLLOWS) {
        rel_result_set = pkb->GetAllFollowStmt(GetStmtType(first_arg_design_entity),
                                               GetStmtType(second_arg_design_entity));
      } else if (relationship_type == PqlTokenType::FOLLOWS_T) {
        rel_result_set = pkb->GetAllFollowStarStmt(GetStmtType(first_arg_design_entity),
                                                   GetStmtType(second_arg_design_entity));
      }
    }
    if (rel_result_set.empty() &&
        rel_first_arg.value != selected_synonym.value &&
        rel_second_arg.value != selected_synonym.value) {
      return;
    }

    std::unordered_set<std::pair<std::string, std::string>, pair_hash> pattern_result_set;
    if (pattern_first_arg.type == PqlTokenType::SYNONYM) { // a(v, "x") OR a(v, _)
      std::string second_pattern_arg_value = pattern.GetSecond().value;
      if (pattern.GetSecond().type != PqlTokenType::SUB_EXPRESSION &&
          pattern.GetSecond().type != PqlTokenType::UNDERSCORE) {
        second_pattern_arg_value = pattern.GetSecond().value.substr(1, pattern.GetSecond().value.length() - 2);
      }
      pattern_result_set = pkb->GetStmtWithPatternSynonym(second_pattern_arg_value);
    } else if (pattern_first_arg.type == PqlTokenType::UNDERSCORE) { // a(_, "x")
      std::string pattern_second_arg_value = pattern.GetSecond().value;
      if (pattern.GetSecond().type == PqlTokenType::EXPR || pattern.GetSecond().type == PqlTokenType::IDENT_WITH_QUOTES) {
        pattern_second_arg_value = pattern.GetSecond().value.substr(1, pattern.GetSecond().value.length() - 2);
      }
      std::unordered_set<std::string> pattern_single_set = pkb->GetStmtWithPattern("_", pattern_second_arg_value);
      for (auto pattern_single : pattern_single_set) {
        pattern_result_set.insert(std::make_pair( pattern_single, pattern.GetFirst().value));
      }
    } else { // a("x", "x")
      std::string pattern_first_arg_value = pattern_first_arg.value.substr(1, pattern_first_arg.value.length() - 2);
      std::string pattern_second_arg_value = pattern.GetSecond().value;
      if (pattern.GetSecond().type == PqlTokenType::EXPR || pattern.GetSecond().type == PqlTokenType::IDENT_WITH_QUOTES) {
        pattern_second_arg_value = pattern.GetSecond().value.substr(1, pattern.GetSecond().value.length() - 2);
      }
      std::unordered_set<std::string> pattern_single_set = pkb->GetStmtWithPattern(pattern_first_arg_value, pattern_second_arg_value);
      for (auto pattern_single : pattern_single_set) {
        pattern_result_set.insert(std::make_pair( pattern_single, pattern.GetFirst().value));
      }
    }

    std::pair<QueryCondition, std::unordered_set<std::pair<std::string, std::string>, pair_hash>> rel_column =
        std::make_pair(rel_condition, rel_result_set);
    std::pair<QueryCondition, std::unordered_set<std::pair<std::string, std::string>, pair_hash>> pattern_column =
        std::make_pair(pattern_condition, pattern_result_set);

    std::vector<std::pair<QueryCondition, std::unordered_set<std::pair<std::string, std::string>, pair_hash>>> result_table =
        {rel_column, pattern_column};
    std::unordered_set<std::string> add_result = QueryResult(result_table).GetResult(selected_synonym);

    PqlTokenType selected_synonym_design_entity;
    for (auto declaration : declarations) {
      if (declaration.GetSynonym().value==selected_synonym.value) {
        selected_synonym_design_entity = declaration.GetDesignEntity().type;
      }
    }

    // selected synonym not in clause
    if (rel_first_arg.value != selected_synonym.value &&
        rel_second_arg.value != selected_synonym.value &&
        pattern.GetSynAssign().value != selected_synonym.value &&
        pattern.GetFirst().value != selected_synonym.value &&
        !rel_result_set.empty() &&
        !pattern_result_set.empty()) {
      std::unordered_set<std::string> add_result = pkb->GetStmt(GetStmtType(selected_synonym_design_entity));
      result.insert(add_result.begin(), add_result.end());
      return;
    }

    result.insert(add_result.begin(), add_result.end());
  }
}

StmtType QueryEvaluator::GetStmtType(PqlTokenType token_type) {
  switch (token_type) {
    case PqlTokenType::STMT: {
      return StmtType::STMT;
    }
    case PqlTokenType::ASSIGN: {
      return StmtType::ASSIGN;
    }
    case PqlTokenType::WHILE: {
      return StmtType::WHILE;
    }
    case PqlTokenType::IF: {
      return StmtType::IF;
    }
    case PqlTokenType::PRINT: {
      return StmtType::PRINT;
    }
    case PqlTokenType::READ: {
      return StmtType::READ;
    }
    case PqlTokenType::CALL: {
      return StmtType::CALL;
    }
    case PqlTokenType::VARIABLE: {
      return StmtType::VARS;
    }
    case PqlTokenType::CONSTANT: {
      return StmtType::CONSTS;
    }
  }
}

}

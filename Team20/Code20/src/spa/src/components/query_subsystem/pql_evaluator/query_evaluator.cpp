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
    case PqlTokenType::PARENT: {
      break;
    }
    case PqlTokenType::PARENT_T: {
      break;
    }
    case PqlTokenType::FOLLOWS: {
      break;
    }
    case PqlTokenType::FOLLOWS_T: {
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

  } else if (first_arg.type==PqlTokenType::UNDERSCORE && second_arg.type==PqlTokenType::UNDERSCORE) {
    // 3. pattern a(_, _)

  } else if (first_arg.type==PqlTokenType::UNDERSCORE && second_arg.type==PqlTokenType::SUB_EXPRESSION) {
    // 4. pattern a(_, _"x"_)

  } else if (first_arg.type==PqlTokenType::IDENT_WITH_QUOTES && second_arg.type==PqlTokenType::UNDERSCORE) {
    // 5. pattern a("x", _)

  } else if (first_arg.type==PqlTokenType::IDENT_WITH_QUOTES && second_arg.type==PqlTokenType::SUB_EXPRESSION) {
    // 6. pattern a("x", _"y"_)

  }

  result.insert(result_to_add.begin(), result_to_add.end());
}

void QueryEvaluator::EvaluateSelectWithRelationshipAndPattern(ParsedQuery &query) {
  //TODO: implement

  PqlToken selected_synonym = query.GetSynonym();
  std::vector<Declaration> declarations = query.GetDeclaration();
  Relationship relationship = query.GetRelationships().front();
  Pattern pattern = query.GetPatterns().front();

  PqlTokenType relationship_type = relationship.GetRelRef().type;
  PqlToken rel_first_arg = relationship.GetFirst();
  PqlToken rel_second_arg = relationship.GetSecond();

  if (relationship.GetFirst().type!=PqlTokenType::SYNONYM &&
      relationship.GetSecond().type!=PqlTokenType::SYNONYM) { // no synonym in such that clause
    bool such_that_bool_result;
    std::string rel_second_no_quote = rel_second_arg.value.substr(1, 1);
    if (relationship_type==PqlTokenType::USES) {
      such_that_bool_result = pkb->IsUsageStmtVarExist(std::make_pair(rel_first_arg.value, rel_second_no_quote));
    } else if (relationship_type==PqlTokenType::MODIFIES) {
      such_that_bool_result = pkb->IsModifyStmtVarExist(std::make_pair(rel_first_arg.value, rel_second_no_quote));
    } else if (relationship_type==PqlTokenType::PARENT) {
      // TODO: modify such_that_bool_result by calling from pkb
    } else if (relationship_type==PqlTokenType::PARENT_T) {
      // TODO: modify such_that_bool_result by calling from pkb
    } else if (relationship_type==PqlTokenType::FOLLOWS) {
      // TODO: modify such_that_bool_result by calling from pkb
    } else if (relationship_type==PqlTokenType::FOLLOWS_T) {
      // TODO: modify such_that_bool_result by calling from pkb
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
    std::unordered_set<std::string> single_result_set;

    if (relationship.GetFirst().type==PqlTokenType::SYNONYM &&
        relationship.GetSecond().type!=PqlTokenType::SYNONYM) {
      std::string second_arg = relationship.GetSecond().value;
      if (relationship_type==PqlTokenType::USES) {
        second_arg = second_arg.substr(1, 1);
        single_result_set = pkb->GetStmtUsedByVar(second_arg);
      } else if (relationship_type==PqlTokenType::MODIFIES) {
        second_arg = second_arg.substr(1, 1);
        single_result_set = pkb->GetStmtModByVar(second_arg);
      } else if (relationship_type==PqlTokenType::PARENT) {
        single_result_set; // TODO
      } else if (relationship_type==PqlTokenType::PARENT_T) {
        single_result_set; // TODO
      } else if (relationship_type==PqlTokenType::FOLLOWS) {
        single_result_set; // TODO
      } else if (relationship_type==PqlTokenType::FOLLOWS_T) {
        single_result_set; // TODO
      }
      for (auto single_result : single_result_set) {
        rel_result_set.insert(std::make_pair(single_result, second_arg));
      }
    } else if (relationship.GetFirst().type!=PqlTokenType::SYNONYM &&
        relationship.GetSecond().type==PqlTokenType::SYNONYM) {
      if (relationship_type==PqlTokenType::USES) {
        single_result_set = pkb->GetVarUsedByStmt(relationship.GetFirst().value);
      } else if (relationship_type==PqlTokenType::MODIFIES) {
        single_result_set = pkb->GetVarModByStmt(relationship.GetFirst().value);
      } else if (relationship_type==PqlTokenType::PARENT) {
        single_result_set; // TODO
      } else if (relationship_type==PqlTokenType::PARENT_T) {
        single_result_set; // TODO
      } else if (relationship_type==PqlTokenType::FOLLOWS) {
        single_result_set; // TODO
      } else if (relationship_type==PqlTokenType::FOLLOWS_T) {
        single_result_set; // TODO
      }
      for (auto single_result : single_result_set) {
        rel_result_set.insert(std::make_pair(relationship.GetFirst().value, single_result));
      }
    } else if (relationship.GetFirst().type==PqlTokenType::SYNONYM &&
        relationship.GetSecond().type==PqlTokenType::SYNONYM) {
      if (relationship_type==PqlTokenType::USES) {
        rel_result_set; // TODO
      } else if (relationship_type==PqlTokenType::MODIFIES) {
        rel_result_set; // TODO
      } else if (relationship_type==PqlTokenType::PARENT) {
        rel_result_set; // TODO
      } else if (relationship_type==PqlTokenType::PARENT_T) {
        rel_result_set; // TODO
      } else if (relationship_type==PqlTokenType::FOLLOWS) {
        rel_result_set; // TODO
      } else if (relationship_type==PqlTokenType::FOLLOWS_T) {
        rel_result_set; // TODO
      }
    }

    std::unordered_set<std::pair<std::string, std::string>, pair_hash> pattern_result_set; //TODO

    std::pair<QueryCondition, std::unordered_set<std::pair<std::string, std::string>, pair_hash>> rel_column =
        std::make_pair(rel_condition, rel_result_set);
    std::pair<QueryCondition, std::unordered_set<std::pair<std::string, std::string>, pair_hash>> pattern_column =
        std::make_pair(pattern_condition, pattern_result_set);

    std::vector<std::pair<QueryCondition, std::unordered_set<std::pair<std::string, std::string>, pair_hash>>>
        result_table =
        {rel_column, pattern_column};
    std::unordered_set<std::string> add_result = QueryResult(result_table).GetResult(selected_synonym);
    result.insert(add_result.begin(), add_result.end());
  }
}

}

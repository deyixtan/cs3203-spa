#include "query_evaluator.h"

namespace pql_evaluator {

void QueryEvaluator::Evaluate(ParsedQuery &query, std::list<std::string> &results) {
  result.clear();

  if (query.GetRelationships().empty() && query.GetPatterns().empty()) { // only select
    EvaluateSelectOnly(query);
  } else if (!query.GetRelationships().empty() && query.GetPatterns().empty()) { // select + 1 relationship
    EvaluateSelectWithRelationship(query);
  } else if (query.GetRelationships().empty() && !query.GetPatterns().empty()) { // select + 1 pattern
    EvaluateSelectWithPattern(query);
  } else { // select + 1 relationship + 1 pattern
    EvaluateSelectWithRelationshipAndPattern(query);
  }

  for (auto i = result.begin(); i!=result.end(); ++i) {
    results.emplace_back(*i);
  }
}

void QueryEvaluator::EvaluateSelectOnly(ParsedQuery &query) {
  const PqlToken select_synonym = query.GetSynonym();
  const auto declarations = query.GetDeclaration();
  PqlToken token;

  // assume semantically valid and declaration must contain the select_synonym
  // TODO: refactor ParsedQuery to ValidQuery
  for (auto declaration : declarations) {
    if (declaration.GetSynonym().value==select_synonym.value) {
      token = declaration.GetDesignEntity();
    }
  }

  // assume that only select
  // evaluation with relationship and pattern may default to this in case of true boolean
  std::unordered_set<std::string> add_result;
  switch (token.type) {
    case PqlTokenType::STMT: {
      add_result = pkb->GetStmt(StmtType::STMT);
      result.insert(add_result.begin(), add_result.end());
      break;
    }
    case PqlTokenType::READ: {
      add_result = pkb->GetStmt(StmtType::READ);
      result.insert(add_result.begin(), add_result.end());
      break;
    }
    case PqlTokenType::PRINT: {
      add_result = pkb->GetStmt(StmtType::PRINT);
      result.insert(add_result.begin(), add_result.end());
      break;
    }
    case PqlTokenType::WHILE: {
      add_result = pkb->GetStmt(StmtType::WHILE);
      result.insert(add_result.begin(), add_result.end());
      break;
    }
    case PqlTokenType::IF: {
      add_result = pkb->GetStmt(StmtType::IF);
      result.insert(add_result.begin(), add_result.end());
      break;
    }
    case PqlTokenType::ASSIGN: {
      add_result = pkb->GetStmt(StmtType::ASSIGN);
      result.insert(add_result.begin(), add_result.end());
      break;
    }
    case PqlTokenType::VARIABLE: {
      add_result = pkb->GetStmt(StmtType::VARS);
      result.insert(add_result.begin(), add_result.end());
      break;
    }
    case PqlTokenType::CONSTANT: {
      add_result = pkb->GetStmt(StmtType::CONSTS);
      result.insert(add_result.begin(), add_result.end());
      break;
    }
    case PqlTokenType::PROCEDURE: {
      add_result = pkb->GetStmt(StmtType::PROC);
      result.insert(add_result.begin(), add_result.end());
      break;
    }
    default: {
      // TODO:throw exception
    }
  }
}

void QueryEvaluator::EvaluateSelectWithRelationship(ParsedQuery &query) {
  // assume only select with 1 relationship
  PqlToken select_syonym = query.GetSynonym();
  Relationship relationship = query.GetRelationships().front();
  PqlTokenType rel_ref = relationship.GetRelRef().type;
  PqlToken first_arg = relationship.GetFirst();
  PqlToken second_arg = relationship.GetSecond();

  std::unordered_set<std::string> add_result;
  if (rel_ref==PqlTokenType::USES) {
    // 6 Total Cases (Handles UsesS now) TODO: refactor Uses to UsesS and UsesP
    if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::SYNONYM) {
      // 1. Uses(s, v)
      if (select_syonym.value==first_arg.value) {
        //TODO: need pkb to get all statements that use some variable
      } else if (select_syonym.value==second_arg.value) {
        //TODO: need pkb to get all variables that are used in statements
      } else { // not equal to any
        // is this correct handling?
        // TODO: do we need to check if Uses(s, v) is non-empty???
        EvaluateSelectOnly(query);
      }
    } else if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::UNDERSCORE) {
      // 2. Uses(s, _)
      if (select_syonym.value==first_arg.value) {
        //TODO: need pkb to get all statements that use any variable
      } else {
        EvaluateSelectOnly(query);
      }
    } else if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::IDENT_WITH_QUOTES) {
      // 3. Uses(s, "x")
      if (select_syonym.value==first_arg.value) {
        add_result = pkb->GetStmtUsedByVar(second_arg.value);
        result.insert(add_result.begin(), add_result.end());
      } else { // selected synonym is not in the Uses clause
        EvaluateSelectOnly(query);
      }
    } else if (first_arg.type==PqlTokenType::NUMBER && second_arg.type==PqlTokenType::SYNONYM) {
      // 4. Uses(1, v)
      if (select_syonym.value==second_arg.value) {
        add_result = pkb->GetVarUsedByStmt(first_arg.value);
        result.insert(add_result.begin(), add_result.end());
      } else { // selected synonym is not in the Uses clause
        EvaluateSelectOnly(query);
      }
    }
  } else if (first_arg.type==PqlTokenType::NUMBER && second_arg.type==PqlTokenType::UNDERSCORE) {
    // 5. Uses(1, _)
    if (!pkb->GetVarUsedByStmt(first_arg.value).empty()) {
      // line does not use any variables
      // false
      return;
    } else {
      // line uses some variable
      // true
      EvaluateSelectOnly(query);
    }
  } else if (first_arg.type==PqlTokenType::NUMBER && second_arg.type==PqlTokenType::IDENT_WITH_QUOTES) {
    // 6. Uses(1, "x")
    std::pair arg_pair(first_arg.value, second_arg.value);
    bool is_true = pkb->IsUsageStmtVarExist(arg_pair);
    if (is_true) { // defaults to case with just select
      EvaluateSelectOnly(query);
    } else { // none
      return;
    }
  } else if (rel_ref==PqlTokenType::MODIFIES) {
    // 6 Total Cases same as UsesS
    // Only handles ModifiesS TODO: need to specialize Modifies -> ModifiesS vs ModifiesP
    if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::SYNONYM) {
      // 1. Modifies(s, v)
      if (select_syonym.value==first_arg.value) {
        //TODO: need pkb to get all statements that modify some variable
      } else if (select_syonym.value==second_arg.value) {
        //TODO: need pkb to get all variables that are modified in statements
      } else { // not equal to any
        // is this correct handling?
        // TODO: do we need to check if Modifies(s, v) is non-empty???
        EvaluateSelectOnly(query);
      }
    } else if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::UNDERSCORE) {
      // 2. Modifies(s, _)
      if (select_syonym.value==first_arg.value) {
        //TODO: need pkb to get all statements that modify any variable
      } else {
        EvaluateSelectOnly(query);
      }
    } else if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::IDENT_WITH_QUOTES) {
      // 3. Modifies(s, "x")
      if (select_syonym.value==first_arg.value) {
        add_result = pkb->GetStmtModByVar(second_arg.value);
        result.insert(add_result.begin(), add_result.end());
      } else { // selected synonym is not in the Uses clause
        EvaluateSelectOnly(query);
      }
    } else if (first_arg.type==PqlTokenType::NUMBER && second_arg.type==PqlTokenType::SYNONYM) {
      // 4. Modifies(1, v)
      if (select_syonym.value==second_arg.value) {
        add_result = pkb->GetVarModByStmt(first_arg.value);
        result.insert(add_result.begin(), add_result.end());
      } else { // selected synonym is not in the Uses clause
        EvaluateSelectOnly(query);
      }
    }
  } else if (first_arg.type==PqlTokenType::NUMBER && second_arg.type==PqlTokenType::UNDERSCORE) {
    // 5. Modifies(1, _)
    if (!pkb->GetVarModByStmt(first_arg.value).empty()) {
      // line does not use any variables
      // false
      return;
    } else {
      // line uses some variable
      // true
      EvaluateSelectOnly(query);
    }
  } else if (first_arg.type==PqlTokenType::NUMBER && second_arg.type==PqlTokenType::IDENT_WITH_QUOTES) {
    // 6. Modifies(1, "x")
    std::pair arg_pair(first_arg.value, second_arg.value);
    bool is_true = pkb->IsModifyStmtVarExist(arg_pair);
    if (is_true) { // defaults to case with just select
      EvaluateSelectOnly(query);
    } else { // none
      return;
    }
  } else { // TODO: exception

  }
}

void QueryEvaluator::EvaluateSelectWithPattern(ParsedQuery &query) {
  //TODO: implement
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

  if (relationship.GetFirst().type != PqlTokenType::SYNONYM &&
      relationship.GetSecond().type != PqlTokenType::SYNONYM) { // no synonym in such that clause
    bool such_that_bool_result;
    std::string rel_second_no_quote = rel_second_arg.value.substr(1, 1);
    if (relationship_type == PqlTokenType::USES) {
      such_that_bool_result = pkb->IsUsageStmtVarExist(std::make_pair(rel_first_arg.value, rel_second_no_quote));
    } else if (relationship_type == PqlTokenType::MODIFIES) {
      such_that_bool_result = pkb->IsModifyStmtVarExist(std::make_pair(rel_first_arg.value, rel_second_no_quote));
    } else if (relationship_type == PqlTokenType::PARENT) {
      // TODO: modify such_that_bool_result by calling from pkb
    } else if (relationship_type == PqlTokenType::PARENT_T) {
      // TODO: modify such_that_bool_result by calling from pkb
    } else if (relationship_type == PqlTokenType::FOLLOWS) {
      // TODO: modify such_that_bool_result by calling from pkb
    } else if (relationship_type == PqlTokenType::FOLLOWS_T) {
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
        second_arg = second_arg.substr(1, 1);
        temp_set = pkb->GetAllUsesStmt(GetStmtType(first_arg_design_entity));
      } else if (relationship_type == PqlTokenType::MODIFIES) {
        second_arg = second_arg.substr(1, 1);
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
        if (pair.second == second_arg) {
          single_result_set.insert(pair.first);
        }
      }

      for (auto single_result : single_result_set) {
        rel_result_set.insert(std::make_pair( single_result, relationship.GetSecond().value));
      }
    } else if (relationship.GetFirst().type != PqlTokenType::SYNONYM &&
               relationship.GetSecond().type == PqlTokenType::SYNONYM) {  // (1, v)
      if (relationship_type == PqlTokenType::USES) {
        single_result_set = pkb->GetVarUsedByStmt(relationship.GetFirst().value);
      } else if (relationship_type == PqlTokenType::MODIFIES) {
        single_result_set = pkb->GetVarModByStmt(relationship.GetFirst().value);
      } else if (relationship_type == PqlTokenType::PARENT) {
        single_result_set; // TODO
      } else if (relationship_type == PqlTokenType::PARENT_T) {
        single_result_set; // TODO
      } else if (relationship_type == PqlTokenType::FOLLOWS) {
        single_result_set; // TODO
      } else if (relationship_type == PqlTokenType::FOLLOWS_T) {
        single_result_set; // TODO
      }
      for (auto single_result : single_result_set) {
        rel_result_set.insert(std::make_pair(relationship.GetFirst().value, single_result));
      }
    } else if (relationship.GetFirst().type == PqlTokenType::SYNONYM &&
               relationship.GetSecond().type == PqlTokenType::SYNONYM) {
      std::string second_arg = relationship.GetSecond().value;

      PqlTokenType first_arg_design_entity;
      for (auto declaration : declarations) {
        if (declaration.GetSynonym().value==relationship.GetFirst().value) {
          first_arg_design_entity = declaration.GetDesignEntity().type;
        }
      }

      if (relationship_type == PqlTokenType::USES) {
        second_arg = second_arg.substr(1, 1);
        rel_result_set = pkb->GetAllUsesStmt(GetStmtType(first_arg_design_entity));
      } else if (relationship_type == PqlTokenType::MODIFIES) {
        second_arg = second_arg.substr(1, 1);
        rel_result_set = pkb->GetAllModStmt(GetStmtType(first_arg_design_entity));
      } else if (relationship_type == PqlTokenType::PARENT) {
        rel_result_set = pkb->GetAllParentStmt(GetStmtType(first_arg_design_entity));
      } else if (relationship_type == PqlTokenType::PARENT_T) {
        rel_result_set = pkb->GetAllParentStarStmt(GetStmtType(first_arg_design_entity));
      } else if (relationship_type == PqlTokenType::FOLLOWS) {
        rel_result_set = pkb->GetAllFollowStmt(GetStmtType(first_arg_design_entity));
      } else if (relationship_type == PqlTokenType::FOLLOWS_T) {
        rel_result_set = pkb->GetAllFollowStarStmt(GetStmtType(first_arg_design_entity));
      }
    }

    std::unordered_set<std::pair<std::string, std::string>, pair_hash> pattern_result_set;
    if (pattern_first_arg.type == PqlTokenType::SYNONYM) { // a(v, "x")
      //TODO
    } else { // a("x", "x")
      std::string pattern_first_arg_value = pattern_first_arg.value.substr(1, 1);
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
  }
}

}

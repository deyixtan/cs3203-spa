#include "clause_factory.h"
#include "modifiesp_clause.h"
#include "modifiess_clause.h"
#include "usesp_clause.h"
#include "usess_clause.h"
#include "follows_clause.h"
#include "followst_clause.h"
#include "parent_clause.h"
#include "parentt_clause.h"
#include "calls_clause.h"
#include "callst_clause.h"
#include "next_clause.h"
#include "nextt_clause.h"
#include "affects_clause.h"
#include "affectst_clause.h"
#include "pattern_assign_clause.h"
#include "pattern_while_clause.h"
#include "pattern_if_clause.h"
#include "with_clause.h"
#include "select_clause.h"
#include "select_boolean_clause.h"
#include "select_tuple_clause.h"
#include "clause_util.h"
#include "components/query_subsystem/pql_parser/parsed_query.h"

namespace pql {

using namespace ::clause_util;

std::shared_ptr<Clause> ClauseFactory::Create(Relationship relationship,
                                              const std::unordered_map<std::string, DesignEntityType> &declarations,
                                              const pkb::PkbPtr &pkb) {
  switch (relationship.GetRelRef().type) {
    case PqlTokenType::MODIFIES: {
      if (IsArgIdent(relationship.GetFirst()) || IsFirstArgProcRelationship(relationship.GetFirst(), declarations)) {
        return std::make_shared<ModifiesPClause>(declarations, relationship.GetFirst(), relationship.GetSecond(), pkb);
      } else if (IsArgInteger(relationship.GetFirst())
          || IsFirstArgStmtRelationship(relationship.GetFirst(), declarations)) {
        return std::make_shared<ModifiesSClause>(declarations, relationship.GetFirst(), relationship.GetSecond(), pkb);
      } else {
        return nullptr;
      }
    }
    case PqlTokenType::USES: {
      if (IsArgIdent(relationship.GetFirst()) || IsFirstArgProcRelationship(relationship.GetFirst(), declarations)) {
        return std::make_shared<UsesPClause>(declarations, relationship.GetFirst(), relationship.GetSecond(), pkb);
      } else if (IsArgInteger(relationship.GetFirst())
          || IsFirstArgStmtRelationship(relationship.GetFirst(), declarations)) {
        return std::make_shared<UsesSClause>(declarations, relationship.GetFirst(), relationship.GetSecond(), pkb);
      } else {
        return nullptr;
      }
    }
    case PqlTokenType::FOLLOWS: {
      return std::make_shared<FollowsClause>(declarations, relationship.GetFirst(), relationship.GetSecond(), pkb);
    }
    case PqlTokenType::FOLLOWS_T: {
      return std::make_shared<FollowsTClause>(declarations, relationship.GetFirst(), relationship.GetSecond(), pkb);
    }
    case PqlTokenType::PARENT: {
      return std::make_shared<ParentClause>(declarations, relationship.GetFirst(), relationship.GetSecond(), pkb);
    }
    case PqlTokenType::PARENT_T: {
      return std::make_shared<ParentTClause>(declarations, relationship.GetFirst(), relationship.GetSecond(), pkb);
    }
    case PqlTokenType::CALLS: {
      return std::make_shared<CallsClause>(relationship.GetFirst(), relationship.GetSecond(), pkb);
    }
    case PqlTokenType::CALLS_T: {
      return std::make_shared<CallsTClause>(relationship.GetFirst(), relationship.GetSecond(), pkb);
    }
    case PqlTokenType::NEXT: {
      return std::make_shared<NextClause>(declarations, relationship.GetFirst(), relationship.GetSecond(), pkb);
    }
    case PqlTokenType::NEXT_T: {
      return std::make_shared<NextTClause>(declarations, relationship.GetFirst(), relationship.GetSecond(), pkb);
    }
    case PqlTokenType::AFFECTS: {
      return std::make_shared<AffectsClause>(relationship.GetFirst(), relationship.GetSecond(), pkb);
    }
    case PqlTokenType::AFFECTS_T: {
      return std::make_shared<AffectsTClause>(relationship.GetFirst(), relationship.GetSecond(), pkb);
    }
    default: {
      return nullptr;
    }
  }
}

std::shared_ptr<Clause> ClauseFactory::Create(Pattern pattern,
                                              const std::unordered_map<std::string, DesignEntityType> &declarations,
                                              const pkb::PkbPtr &pkb) {
  auto pattern_synonym_design_entity_type = GetSynonymDesignEntity(pattern.GetSynonym(), declarations);
  switch (pattern_synonym_design_entity_type) {
    case DesignEntityType::ASSIGN: {
      return std::make_shared<PatternAssignClause>(pattern.GetSynonym().value,
                                                   pattern.GetFirst(),
                                                   pattern.GetSecond(),
                                                   pkb);
    }
    case DesignEntityType::WHILE: {
      return std::make_shared<PatternWhileClause>(pattern.GetSynonym().value,
                                                  pattern.GetFirst(),
                                                  pkb);
    }
    case DesignEntityType::IF: {
      return std::make_shared<PatternIfClause>(pattern.GetSynonym().value,
                                               pattern.GetFirst(),
                                               pkb);
    }
    default: {
      return nullptr;
    }
  }
}

std::shared_ptr<Clause> ClauseFactory::Create(const PqlToken &selected_synonym,
                                              const std::unordered_map<std::string, DesignEntityType> &declarations,
                                              const pkb::PkbPtr &pkb) {
  return std::make_shared<SelectClause>(selected_synonym, declarations, pkb);
}

std::shared_ptr<Clause> ClauseFactory::Create(With with,
                                              const std::unordered_map<std::string, DesignEntityType> &declarations,
                                              const pkb::PkbPtr &pkb) {
  return std::make_shared<WithClause>(declarations, with.GetFirst(), with.GetSecond(), pkb);
}

std::shared_ptr<Clause> ClauseFactory::Create(ResultClause result_clause,
                                              const std::unordered_map<std::string, DesignEntityType> &declarations,
                                              const pkb::PkbPtr &pkb) {
  ResultClauseType result_clause_type = result_clause.GetType();
  if (result_clause_type==ResultClauseType::BOOLEAN) {
    return std::make_shared<SelectBooleanClause>();
  } else if (result_clause_type==ResultClauseType::SYNONYM) {
    return std::make_shared<SelectClause>(result_clause.GetValues().front(), declarations, pkb);
  } else if (result_clause_type==ResultClauseType::ATTRIBUTE) {
    return std::make_shared<SelectClause>(result_clause.GetValues().front(), declarations, pkb);
  } else { // result_clause_type==ResultClauseType::TUPLE
    auto selected_tuple = result_clause.GetValues();
    return std::make_shared<SelectTupleClause>(selected_tuple, declarations, pkb);
  }
}
std::shared_ptr<Clause> ClauseFactory::Create(const std::unordered_set<std::string> &table_synonyms,
                                              ResultClause result_clause,
                                              const std::unordered_map<std::string, DesignEntityType> &declarations,
                                              const pkb::PkbPtr &pkb) {
  ResultClauseType result_clause_type = result_clause.GetType();
  if (result_clause_type==ResultClauseType::BOOLEAN) {
    return std::make_shared<SelectBooleanClause>();
  } else if (result_clause_type==ResultClauseType::SYNONYM) {
    return std::make_shared<SelectClause>(table_synonyms, result_clause.GetValues().front(), declarations, pkb);
  } else if (result_clause_type==ResultClauseType::ATTRIBUTE) {
    return std::make_shared<SelectClause>(table_synonyms, result_clause.GetValues().front(), declarations, pkb);
  } else { // result_clause_type==ResultClauseType::TUPLE
    auto selected_tuple = result_clause.GetValues();
    return std::make_shared<SelectTupleClause>(table_synonyms, selected_tuple, declarations, pkb);
  }
}

}

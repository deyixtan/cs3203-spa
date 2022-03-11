#include "clause_factory.h"
#include "modifiesp_clause.h"
#include "modifiess_clause.h"
#include "usesp_clause.h"
#include "usess_clause.h"
#include "follows_clause.h"
#include "followst_clause.h"
#include "parent_clause.h"
#include "select_clause.h"
#include "clause_util.h"

namespace pql {

using namespace ::clause_util;

std::unique_ptr<Clause> ClauseFactory::Create(Relationship relationship,
                                              std::vector<Declaration> declarations,
                                              PKB *pkb) {
  switch (relationship.GetRelRef().type) {
    case PqlTokenType::MODIFIES: {
      if (IsArgInteger(relationship.GetFirst()) || IsArgProcSynonym(relationship.GetFirst(), declarations)) {
        return std::make_unique<ModifiesPClause>(declarations, relationship.GetFirst(), relationship.GetSecond(), pkb);
      } else if (IsArgIdent(relationship.GetFirst()) || IsArgStmtSynonym(relationship.GetFirst(), declarations)) {
        return std::make_unique<ModifiesSClause>(declarations, relationship.GetFirst(), relationship.GetSecond(), pkb);
      } else {
        return nullptr;
      }
    }
    case PqlTokenType::USES: {
      if (IsArgInteger(relationship.GetFirst()) || IsArgProcSynonym(relationship.GetFirst(), declarations)) {
        return std::make_unique<UsesPClause>(declarations, relationship.GetFirst(), relationship.GetSecond(), pkb);
      } else if (IsArgIdent(relationship.GetFirst()) || IsArgStmtSynonym(relationship.GetFirst(), declarations)) {
        return std::make_unique<UsesSClause>(declarations, relationship.GetFirst(), relationship.GetSecond(), pkb);
      } else {
        return nullptr;
      }
    }
    case PqlTokenType::FOLLOWS: {
      return std::make_unique<FollowsClause>(declarations, relationship.GetFirst(), relationship.GetSecond(), pkb);
    }
    case PqlTokenType::FOLLOWS_T: {
      return std::make_unique<FollowsTClause>(declarations, relationship.GetFirst(), relationship.GetSecond(), pkb);
    }
    case PqlTokenType::PARENT: {
      return std::make_unique<ParentClause>(declarations, relationship.GetFirst(), relationship.GetSecond(), pkb);
    }
    default: {
      return nullptr;
    }
  }
}

std::unique_ptr<Clause> ClauseFactory::Create(Pattern pattern, std::vector<Declaration> declarations, PKB *pkb) {
  return std::unique_ptr<Clause>();
}
std::unique_ptr<Clause> ClauseFactory::Create(PqlToken selected_synonym,
                                              std::vector<Declaration> declarations,
                                              PKB *pkb) {
  return std::make_unique<SelectClause>(selected_synonym, declarations, pkb);
}

}

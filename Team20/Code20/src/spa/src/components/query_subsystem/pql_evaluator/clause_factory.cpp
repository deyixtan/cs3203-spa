#include "clause_factory.h"
#include "modifiesp_clause.h"
#include "modifiess_clause.h"
#include "usesp_clause.h"
#include "usess_clause.h"

namespace pql {

ClauseFactory::ClauseFactory(PKB *pkb_) {
  pkb = pkb_;
}

std::unique_ptr<Clause> ClauseFactory::Create(Relationship relationship, std::vector<Declaration> declarations) {
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
    default: {
      return nullptr;
    }
  }
}

std::unique_ptr<Clause> ClauseFactory::Create(Pattern pattern, std::vector<Declaration> declarations) {
  return std::unique_ptr<Clause>();
}
std::unique_ptr<Clause> ClauseFactory::Create(PqlToken selected_synonym, std::vector<Declaration> declarations) {
  return std::unique_ptr<Clause>();
}

bool ClauseFactory::IsArgIdent(const PqlToken &arg) {
  return arg.type==PqlTokenType::IDENT_WITH_QUOTES;
}

bool ClauseFactory::IsArgInteger(const PqlToken &arg) {
  return arg.type==PqlTokenType::NUMBER;
}

bool ClauseFactory::IsArgProcSynonym(const PqlToken &arg, const std::vector<Declaration> &declarations) {
  return GetSynonymDesignEntity(arg, declarations)==PqlTokenType::PROCEDURE;
}

bool ClauseFactory::IsArgStmtSynonym(const PqlToken &arg, const std::vector<Declaration> &declarations) {
  return GetSynonymDesignEntity(arg, declarations)==PqlTokenType::STMT ||
      GetSynonymDesignEntity(arg, declarations)==PqlTokenType::ASSIGN ||
      GetSynonymDesignEntity(arg, declarations)==PqlTokenType::CALL ||
      GetSynonymDesignEntity(arg, declarations)==PqlTokenType::READ ||
      GetSynonymDesignEntity(arg, declarations)==PqlTokenType::PRINT ||
      GetSynonymDesignEntity(arg, declarations)==PqlTokenType::WHILE ||
      GetSynonymDesignEntity(arg, declarations)==PqlTokenType::IF;
}

PqlTokenType ClauseFactory::GetSynonymDesignEntity(const PqlToken &arg, const std::vector<Declaration> &declarations) {
  assert(arg.type==PqlTokenType::SYNONYM);
  for (auto declaration : declarations) {
    if (arg.value==declaration.GetSynonym().value) {
      return declaration.GetDesignEntity().type;
    }
  }
  throw std::out_of_range("Synonym not declared");
}

}

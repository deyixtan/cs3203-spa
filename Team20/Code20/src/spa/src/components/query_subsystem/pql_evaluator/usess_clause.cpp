#include "usess_clause.h"

#include <stdexcept>

namespace pql {

UsesSClause::UsesSClause(const std::vector<Declaration> &declarations_,
                         const PqlToken &first_arg_,
                         const PqlToken &second_arg_,
                         PKB *pkb_) :
    declarations(declarations_), first_arg(first_arg_), second_arg(second_arg_), pkb(pkb_) {}

Table UsesSClause::Execute() {
  if (IsArgSynonym(first_arg) && IsArgSynonym(second_arg)) {
    // UsesS(s, v)
    return HandleSynonymSynonym();
  } else if (IsArgSynonym(first_arg) && IsArgWildcard(second_arg)) {
    // UsesS(s, _)
    return HandleSynonymWildcard();
  } else if (IsArgSynonym(first_arg) && IsArgIdent(second_arg)) {
    // UsesS(s, "x")
    return HandleSynonymIdent();
  } else if (IsArgInteger(first_arg) && IsArgSynonym(second_arg)) {
    // UsesS(1, v)
    return HandleIntegerSynonym();
  } else if (IsArgInteger(first_arg) && IsArgWildcard(second_arg)) {
    // UsesS(1, _)
    return HandleIntegerWildcard();
  } else if (IsArgInteger(first_arg) && IsArgIdent(second_arg)) {
    // UsesS(1, "x")
    return HandleIntegerIdent();
  }
}

bool UsesSClause::IsArgSynonym(const PqlToken &arg) {
  return arg.type==PqlTokenType::SYNONYM;
}

bool UsesSClause::IsArgWildcard(const PqlToken &arg) {
  return arg.type==PqlTokenType::UNDERSCORE;
}

bool UsesSClause::IsArgIdent(const PqlToken &arg) {
  return arg.type==PqlTokenType::IDENT_WITH_QUOTES;
}

bool UsesSClause::IsArgInteger(const PqlToken &arg) {
  return arg.type==PqlTokenType::NUMBER;
}

PqlTokenType UsesSClause::GetSynonymDesignEntity(const PqlToken &arg) {
  assert(arg.type==PqlTokenType::SYNONYM);
  for (auto declaration : declarations) {
    if (arg.value==declaration.GetSynonym().value) {
      return declaration.GetDesignEntity().type;
    }
  }
  throw std::out_of_range("Synonym not declared");
}

StmtType UsesSClause::GetStmtType(const PqlTokenType &design_entity) {
  switch (design_entity) {
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
    case PqlTokenType::PROCEDURE: {
      return StmtType::PROC;
    }
  }
};

std::string UsesSClause::GetIdentWithoutQuotes(const std::string& ident) {
  return ident.substr(1, ident.length() - 2);
}

Table UsesSClause::HandleSynonymSynonym() {
  auto pair_constraints = pkb->GetUsageStore()->GetAllUsesStmt(GetStmtType(GetSynonymDesignEntity(first_arg)));
  return {first_arg.value, second_arg.value, pair_constraints};
}

Table UsesSClause::HandleSynonymWildcard() {
  auto pair_constraints = pkb->GetUsageStore()->GetAllUsesStmt(GetStmtType(GetSynonymDesignEntity(first_arg)));
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }
  return {first_arg.value, single_constraints};
}

Table UsesSClause::HandleSynonymIdent() {
  auto pair_constraints = pkb->GetUsageStore()->GetAllUsesStmt(GetStmtType(GetSynonymDesignEntity(first_arg)));
  std::unordered_set<std::string> single_constraints;
  std::string ident_without_quotes = GetIdentWithoutQuotes(second_arg.value);
  for (const auto &pair_constraint : pair_constraints) {
    if (pair_constraint.second==ident_without_quotes) {
      single_constraints.insert(pair_constraint.first);
    }
  }
  return {first_arg.value, single_constraints};
}

Table UsesSClause::HandleIntegerSynonym() {
  auto single_constraints = pkb->GetUsageStore()->GetVarUsedByStmt(first_arg.value);
  return {second_arg.value, single_constraints};
}

Table UsesSClause::HandleIntegerWildcard() {
  bool is_empty = pkb->GetUsageStore()->GetVarUsedByStmt(first_arg.value).empty();
  Table table;
  if (is_empty) {
    table.EncounteredFalseClause();
  }
  return table;
}

Table UsesSClause::HandleIntegerIdent() {
  std::string ident_without_quotes = GetIdentWithoutQuotes(second_arg.value);
  std::pair arg_pair(first_arg.value, ident_without_quotes);
  bool is_empty = pkb->GetUsageStore()->StmtVarExists(arg_pair);
  Table table;
  if (is_empty) {
    table.EncounteredFalseClause();
  }
  return table;
}

}
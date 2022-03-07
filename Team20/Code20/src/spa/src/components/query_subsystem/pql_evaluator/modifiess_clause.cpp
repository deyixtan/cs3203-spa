#include "modifiess_clause.h"

namespace pql {

ModifiesSClause::ModifiesSClause(const std::vector<Declaration> &declarations_,
                                 const PqlToken &first_arg_,
                                 const PqlToken &second_arg_,
                                 PKB *pkb_) :
    declarations(declarations_), first_arg(first_arg_), second_arg(second_arg_), pkb(pkb_) {}

Table ModifiesSClause::Execute() {
  if (IsArgSynonym(first_arg) && IsArgSynonym(second_arg)) {
    // ModifiesS(s, v)
    return HandleSynonymSynonym();
  } else if (IsArgSynonym(first_arg) && IsArgWildcard(second_arg)) {
    // ModifiesS(s, _)
    return HandleSynonymWildcard();
  } else if (IsArgSynonym(first_arg) && IsArgIdent(second_arg)) {
    // ModifiesS(s, "x")
    return HandleSynonymIdent();
  } else if (IsArgInteger(first_arg) && IsArgSynonym(second_arg)) {
    // ModifiesS(1, v)
    return HandleIntegerSynonym();
  } else if (IsArgInteger(first_arg) && IsArgWildcard(second_arg)) {
    // ModifiesS(1, _)
    return HandleIntegerWildcard();
  } else if (IsArgInteger(first_arg) && IsArgIdent(second_arg)) {
    // ModifiesS(1, "x")
    return HandleIntegerIdent();
  }
}

bool ModifiesSClause::IsArgSynonym(const PqlToken &arg) {
  return arg.type==PqlTokenType::SYNONYM;
}

bool ModifiesSClause::IsArgWildcard(const PqlToken &arg) {
  return arg.type==PqlTokenType::UNDERSCORE;
}

bool ModifiesSClause::IsArgIdent(const PqlToken &arg) {
  return arg.type==PqlTokenType::IDENT_WITH_QUOTES;
}

bool ModifiesSClause::IsArgInteger(const PqlToken &arg) {
  return arg.type==PqlTokenType::NUMBER;
}

PqlTokenType ModifiesSClause::GetSynonymDesignEntity(const PqlToken &arg) {
  assert(arg.type==PqlTokenType::SYNONYM);
  for (auto declaration : declarations) {
    if (arg.value==declaration.GetSynonym().value) {
      return declaration.GetDesignEntity().type;
    }
  }
  throw std::out_of_range("Synonym not declared");
}

StmtType ModifiesSClause::GetStmtType(const PqlTokenType &design_entity) {
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

std::string ModifiesSClause::GetIdentWithoutQuotes(const std::string &ident) {
  return ident.substr(1, ident.length() - 2);
}

Table ModifiesSClause::HandleSynonymSynonym() {
  auto pair_constraints = pkb->GetModifyStore()->GetAllModStmt(GetStmtType(GetSynonymDesignEntity(first_arg)));
  return {first_arg.value, second_arg.value, pair_constraints};
}

Table ModifiesSClause::HandleSynonymWildcard() {
  auto pair_constraints = pkb->GetModifyStore()->GetAllModStmt(GetStmtType(GetSynonymDesignEntity(first_arg)));
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }
  return {first_arg.value, single_constraints};
}

Table ModifiesSClause::HandleSynonymIdent() {
  auto pair_constraints = pkb->GetModifyStore()->GetAllModStmt(GetStmtType(GetSynonymDesignEntity(first_arg)));
  std::unordered_set<std::string> single_constraints;
  std::string ident_without_quotes = GetIdentWithoutQuotes(second_arg.value);
  for (const auto &pair_constraint : pair_constraints) {
    if (pair_constraint.second==ident_without_quotes) {
      single_constraints.insert(pair_constraint.first);
    }
  }
  return {first_arg.value, single_constraints};
}

Table ModifiesSClause::HandleIntegerSynonym() {
  auto single_constraints = pkb->GetModifyStore()->GetVarModByStmt(first_arg.value);
  return {second_arg.value, single_constraints};
}

Table ModifiesSClause::HandleIntegerWildcard() {
  bool is_empty = pkb->GetModifyStore()->GetVarModByStmt(first_arg.value).empty();
  Table table;
  if (is_empty) {
    table.EncounteredFalseClause();
  }
  return table;
}

Table ModifiesSClause::HandleIntegerIdent() {
  std::string ident_without_quotes = GetIdentWithoutQuotes(second_arg.value);
  std::pair arg_pair(first_arg.value, ident_without_quotes);
  bool is_empty = pkb->GetModifyStore()->StmtVarExists(arg_pair);
  Table table;
  if (is_empty) {
    table.EncounteredFalseClause();
  }
  return table;
}

}

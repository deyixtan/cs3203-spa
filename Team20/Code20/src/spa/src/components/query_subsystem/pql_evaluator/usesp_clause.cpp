#include "usesp_clause.h"

namespace pql {

UsesPClause::UsesPClause(const std::vector<Declaration> &declarations_,
                         const PqlToken &first_arg_,
                         const PqlToken &second_arg_,
                         PKB *pkb_)
    : declarations(declarations_), first_arg(first_arg_), second_arg(second_arg_), pkb(pkb_) {}

Table pql::UsesPClause::Execute() {
  if (IsArgSynonym(first_arg) && IsArgSynonym(second_arg)) {
    // UsesP(p, v)
    return HandleSynonymSynonym();
  } else if (IsArgSynonym(first_arg) && IsArgWildcard(second_arg)) {
    // UsesP(p, _)
    return HandleSynonymWildcard();
  } else if (IsArgSynonym(first_arg) && IsArgIdent(second_arg)) {
    // UsesP(p, "x")
    return HandleSynonymIdent();
  } else if (IsArgIdent(first_arg) && IsArgSynonym(second_arg)) {
    // UsesP("main", v)
    return HandleIdentSynonym();
  } else if (IsArgIdent(first_arg) && IsArgWildcard(second_arg)) {
    // UsesP("main", _)
    return HandleIdentWildcard();
  } else if (IsArgIdent(first_arg) && IsArgIdent(second_arg)) {
    // UsesP("main", "x")
    return HandleIdentIdent();
  }
}

bool UsesPClause::IsArgSynonym(const PqlToken &arg) {
  return arg.type==PqlTokenType::SYNONYM;
}

bool UsesPClause::IsArgWildcard(const PqlToken &arg) {
  return arg.type==PqlTokenType::UNDERSCORE;
}

bool UsesPClause::IsArgIdent(const PqlToken &arg) {
  return arg.type==PqlTokenType::IDENT_WITH_QUOTES;
}

bool UsesPClause::IsArgInteger(const PqlToken &arg) {
  return arg.type==PqlTokenType::NUMBER;
}

PqlTokenType UsesPClause::GetSynonymDesignEntity(const PqlToken &arg) {
  assert(arg.type==PqlTokenType::SYNONYM);
  for (auto declaration : declarations) {
    if (arg.value==declaration.GetSynonym().value) {
      return declaration.GetDesignEntity().type;
    }
  }
  throw std::out_of_range("Synonym not declared");
}

StmtType UsesPClause::GetStmtType(const PqlTokenType &design_entity) {
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

std::string UsesPClause::GetIdentWithoutQuotes(const std::string &ident) {
  return ident.substr(1, ident.length() - 2);
}

Table UsesPClause::HandleSynonymSynonym() {
  auto pair_constraints = pkb->GetUsageStore()->GetAllProcVar();
  return {first_arg.value, second_arg.value, pair_constraints};
}

Table UsesPClause::HandleSynonymWildcard() {
  auto single_constraints = pkb->GetUsageStore()->GetAllProcUsing();
  return {first_arg.value, single_constraints};
}

Table UsesPClause::HandleSynonymIdent() {
  std::string ident_without_quotes = GetIdentWithoutQuotes(second_arg.value);
  auto single_constraints = pkb->GetUsageStore()->GetProcUsedByVar(ident_without_quotes);
  return {first_arg.value, single_constraints};
}

Table UsesPClause::HandleIdentSynonym() {
  std::string ident_without_quotes = GetIdentWithoutQuotes(first_arg.value);
  auto single_constraints = pkb->GetUsageStore()->GetVarUsedByProc(ident_without_quotes);
  return {second_arg.value, single_constraints};
}

Table UsesPClause::HandleIdentWildcard() {
  std::string ident_without_quotes = GetIdentWithoutQuotes(first_arg.value);
  bool is_empty = pkb->GetUsageStore()->GetVarUsedByProc(ident_without_quotes).empty();
  Table table;
  if (is_empty) {
    table.EncounteredFalseClause();
  }
  return table;
}

Table UsesPClause::HandleIdentIdent() {
  std::string first_arg_without_quotes = GetIdentWithoutQuotes(first_arg.value);
  std::string second_arg_without_quotes = GetIdentWithoutQuotes(second_arg.value);
  std::pair arg_pair(first_arg_without_quotes, second_arg_without_quotes);
  bool is_empty = pkb->GetUsageStore()->ProcVarExists(arg_pair);
  Table table;
  if (is_empty) {
    table.EncounteredFalseClause();
  }
  return table;
}

}
#include "select_clause.h"

namespace pql {

SelectClause::SelectClause(const PqlToken &selected_synonym_, const std::vector<Declaration> &declarations_, PKB *pkb_)
    : selected_synonym(selected_synonym_), declarations(declarations_), pkb(pkb_) {}

Table SelectClause::Execute() {
  auto single_constraints = pkb->GetStmt(GetStmtType(GetSynonymDesignEntity(selected_synonym)));
  return {selected_synonym.value, single_constraints};
}

PqlTokenType SelectClause::GetSynonymDesignEntity(const PqlToken &arg) {
  assert(arg.type==PqlTokenType::SYNONYM);
  for (auto declaration : declarations) {
    if (arg.value==declaration.GetSynonym().value) {
      return declaration.GetDesignEntity().type;
    }
  }

  throw std::out_of_range("Synonym not declared");
}

StmtType SelectClause::GetStmtType(const PqlTokenType &design_entity) {
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

}

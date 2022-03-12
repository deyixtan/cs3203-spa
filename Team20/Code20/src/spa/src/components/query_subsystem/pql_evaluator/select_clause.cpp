#include "select_clause.h"

namespace pql {

SelectClause::SelectClause(const PqlToken &selected_synonym_, const std::unordered_map<std::string, DesignEntityType> &declarations_, PKB *pkb_)
    : selected_synonym(selected_synonym_), declarations(declarations_), pkb(pkb_) {}

Table SelectClause::Execute() {
  auto single_constraints = pkb->GetStmt(GetStmtType(GetSynonymDesignEntity(selected_synonym)));
  return {selected_synonym.value, single_constraints};
}

DesignEntityType SelectClause::GetSynonymDesignEntity(const PqlToken &arg) {
  assert(arg.type==PqlTokenType::SYNONYM);
  for (auto declaration : declarations) {
    if (arg.value==declaration.first) {
      return declaration.second;
    }
  }

  throw std::out_of_range("Synonym not declared");
}

StmtType SelectClause::GetStmtType(const DesignEntityType &design_entity) {
  switch (design_entity) {
    case DesignEntityType::STMT: {
      return StmtType::STMT;
    }
    case DesignEntityType::ASSIGN: {
      return StmtType::ASSIGN;
    }
    case DesignEntityType::WHILE: {
      return StmtType::WHILE;
    }
    case DesignEntityType::IF: {
      return StmtType::IF;
    }
    case DesignEntityType::PRINT: {
      return StmtType::PRINT;
    }
    case DesignEntityType::READ: {
      return StmtType::READ;
    }
    case DesignEntityType::CALL: {
      return StmtType::CALL;
    }
    case DesignEntityType::VARIABLE: {
      return StmtType::VARS;
    }
    case DesignEntityType::CONSTANT: {
      return StmtType::CONSTS;
    }
    case DesignEntityType::PROCEDURE: {
      return StmtType::PROC;
    }
  }
};

}

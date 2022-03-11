#include "clause_util.h"

bool clause_util::IsArgSynonym(const PqlToken &arg) {
  return arg.type==PqlTokenType::SYNONYM;
}

bool clause_util::IsArgWildcard(const PqlToken &arg) {
  return arg.type==PqlTokenType::UNDERSCORE;
}

bool clause_util::IsArgIdent(const PqlToken &arg) {
  return arg.type==PqlTokenType::IDENT_WITH_QUOTES;
}

bool clause_util::IsArgInteger(const PqlToken &arg) {
  return arg.type==PqlTokenType::NUMBER;
}

PqlTokenType clause_util::GetSynonymDesignEntity(const PqlToken &arg, const std::vector<Declaration> &declarations) {
  assert(arg.type==PqlTokenType::SYNONYM);
  for (auto declaration : declarations) {
    if (arg.value==declaration.GetSynonym().value) {
      return declaration.GetDesignEntity().type;
    }
  }
  throw std::out_of_range("Synonym not declared");
}

bool clause_util::IsArgProcSynonym(const PqlToken &arg, const std::vector<Declaration> &declarations) {
  return GetSynonymDesignEntity(arg, declarations)==PqlTokenType::PROCEDURE;
}

bool clause_util::IsArgStmtSynonym(const PqlToken &arg, const std::vector<Declaration> &declarations) {
  return GetSynonymDesignEntity(arg, declarations)==PqlTokenType::STMT ||
      GetSynonymDesignEntity(arg, declarations)==PqlTokenType::ASSIGN ||
      GetSynonymDesignEntity(arg, declarations)==PqlTokenType::CALL ||
      GetSynonymDesignEntity(arg, declarations)==PqlTokenType::READ ||
      GetSynonymDesignEntity(arg, declarations)==PqlTokenType::PRINT ||
      GetSynonymDesignEntity(arg, declarations)==PqlTokenType::WHILE ||
      GetSynonymDesignEntity(arg, declarations)==PqlTokenType::IF;
}

StmtType clause_util::GetStmtType(const PqlTokenType &design_entity) {
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
    default: {
      throw std::out_of_range("Not valid design entity");
    }
  }
}

std::string clause_util::GetIdentWithoutQuotes(const std::string &ident) {
  return ident.substr(1, ident.length() - 2);
}

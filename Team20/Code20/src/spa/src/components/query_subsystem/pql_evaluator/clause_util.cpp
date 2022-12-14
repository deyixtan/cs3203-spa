#include "clause_util.h"

bool clause_util::IsArgSynonym(const PqlToken &arg) {
  return arg.type==PqlTokenType::SYNONYM;
}

bool clause_util::IsArgIdent(const PqlToken &arg) {
  return arg.type==PqlTokenType::IDENT_WITH_QUOTES;
}

bool clause_util::IsArgInteger(const PqlToken &arg) {
  return arg.type==PqlTokenType::NUMBER;
}

bool clause_util::IsArgAttribute(const PqlToken &arg) {
  return arg.type==PqlTokenType::ATTRIBUTE;
}

DesignEntityType clause_util::GetSynonymDesignEntity(const PqlToken &arg,
                                                     const std::unordered_map<std::string,
                                                                              DesignEntityType> &declarations) {
  assert(arg.type==PqlTokenType::SYNONYM);
  if (declarations.count(arg.value)!=0) {
    return declarations.at(arg.value);
  }
  throw std::out_of_range("Synonym not declared");
}

bool clause_util::IsFirstArgProcRelationship(const PqlToken &first_arg,
                                             const std::unordered_map<std::string, DesignEntityType> &declarations) {
  return IsArgSynonym(first_arg) &&
      GetSynonymDesignEntity(first_arg, declarations)==DesignEntityType::PROCEDURE;
}

bool clause_util::IsFirstArgStmtRelationship(const PqlToken &first_arg,
                                             const std::unordered_map<std::string, DesignEntityType> &declarations) {
  return IsArgSynonym(first_arg) &&
      (GetSynonymDesignEntity(first_arg, declarations)==DesignEntityType::STMT ||
          GetSynonymDesignEntity(first_arg, declarations)==DesignEntityType::ASSIGN ||
          GetSynonymDesignEntity(first_arg, declarations)==DesignEntityType::READ ||
          GetSynonymDesignEntity(first_arg, declarations)==DesignEntityType::PRINT ||
          GetSynonymDesignEntity(first_arg, declarations)==DesignEntityType::WHILE ||
          GetSynonymDesignEntity(first_arg, declarations)==DesignEntityType::IF ||
          GetSynonymDesignEntity(first_arg, declarations)==DesignEntityType::CALL);
}

StmtType clause_util::GetStmtType(const DesignEntityType &design_entity) {
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
    default: {
      throw std::out_of_range("Not valid design entity");
    }
  }
}

pql::Table clause_util::ConstructEmptyTable(bool is_false_clause) {
  pql::Table table;
  if (is_false_clause) {
    table.ToggleFalseClause();
  }
  return table;
}

pql::Table clause_util::ConstructEmptyTable() {
    pql::Table table;
    table.ToggleFalseClause();
    return table;
}




#ifndef CLAUSE_UTIL_H_
#define CLAUSE_UTIL_H_

#include "components/pkb/stores/store.h"
#include "components/query_subsystem/pql_parser/parsed_query.h"
#include "components/query_subsystem/pql_evaluator/table.h"
#include "components/query_subsystem/pql_evaluator/clauses/clause.h"
#include <cassert>

namespace clause_util {

bool IsArgSynonym(const PqlToken &arg);
bool IsArgIdent(const PqlToken &arg);
bool IsArgInteger(const PqlToken &arg);
bool IsArgAttribute(const PqlToken &arg);
DesignEntityType GetSynonymDesignEntity(const PqlToken &arg,
                                        const std::unordered_map<std::string, DesignEntityType> &declarations);
StmtType GetStmtType(const DesignEntityType &design_entity);
bool IsFirstArgProcRelationship(const PqlToken &first_arg,
                                const std::unordered_map<std::string, DesignEntityType> &declarations);
bool IsFirstArgStmtRelationship(const PqlToken &first_arg,
                                const std::unordered_map<std::string, DesignEntityType> &declarations);
pql::Table ConstructEmptyTable(bool is_false_clause);
pql::Table ConstructEmptyTable();

}

#endif //CLAUSE_UTIL_H_

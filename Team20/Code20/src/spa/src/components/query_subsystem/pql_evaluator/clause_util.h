#ifndef CLAUSE_UTIL_H_
#define CLAUSE_UTIL_H_


#include "components/pkb/stores/store.h"
#include "components/query_subsystem/pql_parser/parsed_query.h"

namespace clause_util {

bool IsArgSynonym(const PqlToken &arg);
bool IsArgWildcard(const PqlToken &arg);
bool IsArgIdent(const PqlToken &arg);
bool IsArgInteger(const PqlToken &arg);
bool IsArgPartialMatch(const PqlToken &arg);
bool IsArgExactMatch(const PqlToken &arg);
bool IsArgAttribute(const PqlToken &arg);
DesignEntityType GetSynonymDesignEntity(const PqlToken &arg, const std::unordered_map<std::string, DesignEntityType> &declarations);
bool IsArgProcSynonym(const PqlToken &arg, const std::unordered_map<std::string, DesignEntityType> &declarations);
bool IsArgStmtSynonym(const PqlToken &arg, const std::unordered_map<std::string, DesignEntityType> &declarations);
StmtType GetStmtType(const DesignEntityType &design_entity);
//std::string GetIdentWithoutQuotes(const std::string &ident);

}

#endif //CLAUSE_UTIL_H_

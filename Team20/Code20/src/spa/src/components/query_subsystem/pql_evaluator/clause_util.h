#ifndef CLAUSE_UTIL_H_
#define CLAUSE_UTIL_H_

#include "components/query_subsystem/pql_parser/declaration.h"
#include "components/pkb/stores/store.h"

namespace clause_util {

bool IsArgSynonym(const PqlToken &arg);
bool IsArgWildcard(const PqlToken &arg);
bool IsArgIdent(const PqlToken &arg);
bool IsArgInteger(const PqlToken &arg);
PqlTokenType GetSynonymDesignEntity(const PqlToken &arg, const std::vector<Declaration> &declarations);
bool IsArgProcSynonym(const PqlToken &arg, const std::vector<Declaration> &declarations);
bool IsArgStmtSynonym(const PqlToken &arg, const std::vector<Declaration> &declarations);
StmtType GetStmtType(const PqlTokenType &design_entity);
std::string GetIdentWithoutQuotes(const std::string &ident);

}

#endif //CLAUSE_UTIL_H_

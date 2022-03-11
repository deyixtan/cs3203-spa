#include "result_clause.h"

ResultClause::ResultClause() {}
ResultClause::ResultClause(ResultClauseType type, std::vector<PqlToken> values): type(type), values(values) {}
ResultClauseType ResultClause::GetType() {
  return type;
}
std::vector<PqlToken> ResultClause::GetValues() {
  return values;
}
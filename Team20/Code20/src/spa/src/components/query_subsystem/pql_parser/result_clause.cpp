#include "result_clause.h"

using namespace pql;

ResultClause::ResultClause(): type(ResultClauseType()), values(std::vector<PqlToken>()) {}
ResultClause::ResultClause(ResultClauseType type): type(type), values(std::vector<PqlToken>()) {}
ResultClause::ResultClause(ResultClauseType type, std::vector<PqlToken> values): type(type), values(values) {}
ResultClauseType ResultClause::GetType() {
  return type;
}
std::vector<PqlToken> ResultClause::GetValues() {
  return values;
}

void ResultClause::AddValue(PqlToken token) {
  values.push_back(token);
}

void ResultClause::ModifyType(ResultClauseType new_type) {
  type = new_type;
}
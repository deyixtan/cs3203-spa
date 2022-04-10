#include "callst_clause.h"
#include "clause_util.h"

namespace pql {

using namespace clause_util;

CallsTClause::CallsTClause(const PqlToken &first_arg, const PqlToken &second_arg, const pkb::PkbPtr &pkb)
    : first_arg(first_arg), second_arg(second_arg), pkb(pkb) {}

Table CallsTClause::Execute() {
  return (this->*execute_handler.at({first_arg.type, second_arg.type}))();
}

Table CallsTClause::HandleSynonymSynonym() {
  if (first_arg.value==second_arg.value) {
    return ConstructEmptyTable();
  }

  auto pair_constraints = pkb->GetCallStore()->GetStarPairByType(PROC, PROC);
  return {first_arg.value, second_arg.value, pair_constraints};
}

Table CallsTClause::HandleSynonymWildcard() {
  auto pair_constraints = pkb->GetCallStore()->GetStarPairByType(PROC, PROC);
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }

  return {first_arg.value, single_constraints};
}

Table CallsTClause::HandleSynonymIdent() {
  auto single_constraints = pkb->GetCallStore()->GetCallersStarOf(second_arg.value);
  return {first_arg.value, single_constraints};
}

Table CallsTClause::HandleWildcardSynonym() {
  auto pair_constraints = pkb->GetCallStore()->GetStarPairByType(PROC, PROC);
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.second);
  }

  return {second_arg.value, single_constraints};
}

Table CallsTClause::HandleWildcardWildcard() {
  bool is_false_clause = pkb->GetCallStore()->GetStarPairByType(PROC, PROC).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table CallsTClause::HandleWildcardIdent() {
  bool is_false_clause = pkb->GetCallStore()->GetCallersStarOf(second_arg.value).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table CallsTClause::HandleIdentSynonym() {
  auto single_constraints = pkb->GetCallStore()->GetCalleesStarOf(first_arg.value);
  return {second_arg.value, single_constraints};
}

Table CallsTClause::HandleIdentWildcard() {
  bool is_false_clause = pkb->GetCallStore()->GetCalleesStarOf(first_arg.value).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table CallsTClause::HandleIdentIdent() {
  bool is_false_clause = !pkb->GetCallStore()->IsCallsStarPairValid(std::pair<std::string, std::string>(first_arg.value,
                                                                                                        second_arg.value));
  return ConstructEmptyTable(is_false_clause);
}

bool CallsTClause::ExecuteBool() {
  return (this->*execute_bool_handler.at({first_arg.type, second_arg.type}))();
}

bool CallsTClause::HandleSynonymSynonymBool() {
  if (first_arg.value==second_arg.value) {
    return true;
  }

  auto pair_constraints = pkb->GetCallStore()->GetStarPairByType(PROC, PROC);
  return pair_constraints.empty();
}

bool CallsTClause::HandleSynonymWildcardBool() {
  auto pair_constraints = pkb->GetCallStore()->GetStarPairByType(PROC, PROC);
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }

  return pair_constraints.empty();
}

bool CallsTClause::HandleSynonymIdentBool() {
  auto single_constraints = pkb->GetCallStore()->GetCallersStarOf(second_arg.value);
  return single_constraints.empty();
}

bool CallsTClause::HandleWildcardSynonymBool() {
  auto pair_constraints = pkb->GetCallStore()->GetStarPairByType(PROC, PROC);
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.second);
  }

  return single_constraints.empty();
}

bool CallsTClause::HandleWildcardWildcardBool() {
  bool is_false_clause = pkb->GetCallStore()->GetStarPairByType(PROC, PROC).empty();
  return is_false_clause;
}

bool CallsTClause::HandleWildcardIdentBool() {
  bool is_false_clause = pkb->GetCallStore()->GetCallersStarOf(second_arg.value).empty();
  return is_false_clause;
}

bool CallsTClause::HandleIdentSynonymBool() {
  auto single_constraints = pkb->GetCallStore()->GetCalleesStarOf(first_arg.value);
  return single_constraints.empty();
}

bool CallsTClause::HandleIdentWildcardBool() {
  bool is_false_clause = pkb->GetCallStore()->GetCalleesStarOf(first_arg.value).empty();
  return is_false_clause;
}

bool CallsTClause::HandleIdentIdentBool() {
  bool is_false_clause =
      !pkb->GetCallStore()->IsCallsStarPairValid(std::pair<std::string, std::string>(first_arg.value, second_arg.value));
  return is_false_clause;
}

std::set<std::string> CallsTClause::GetSynonyms() {
  std::set<std::string> synonyms;
  if (IsArgSynonym(first_arg)) {
    synonyms.emplace(first_arg.value);
  }
  if (IsArgSynonym(second_arg)) {
    synonyms.emplace(second_arg.value);
  }

  return synonyms;
}

size_t CallsTClause::GetSynonymsSize() {
  size_t size = 0;
  if (IsArgSynonym(first_arg)) {
    size++;
  }
  if (IsArgSynonym(second_arg)) {
    size++;
  }
  return size;
}

size_t CallsTClause::GetWeight() {
  return weight;
}

}

#include "calls_clause.h"
#include "components/query_subsystem/pql_evaluator/clause_util.h"
#include "components/pkb/stores/call_store/call_store.h"

namespace pql {

using namespace clause_util;

CallsClause::CallsClause(const PqlToken &first_arg, const PqlToken &second_arg, const pkb::PkbPtr &pkb)
    : first_arg(first_arg), second_arg(second_arg), pkb(pkb) {}

Table CallsClause::Execute() {
  return (this->*execute_handler.at({first_arg.type, second_arg.type}))();
}

Table CallsClause::HandleSynonymSynonym() {
  if (first_arg.value==second_arg.value) {
    return ConstructEmptyTable();
  }

  auto pair_constraints = pkb->GetCallStore()->GetPairByType(PROC, PROC);
  return {first_arg.value, second_arg.value, pair_constraints};
}

Table CallsClause::HandleSynonymWildcard() {
  auto pair_constraints = pkb->GetCallStore()->GetPairByType(PROC, PROC);
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }

  return {first_arg.value, single_constraints};
}

Table CallsClause::HandleSynonymIdent() {
  auto single_constraints = pkb->GetCallStore()->GetCallersOf(second_arg.value);
  return {first_arg.value, single_constraints};
}

Table CallsClause::HandleWildcardSynonym() {
  auto pair_constraints = pkb->GetCallStore()->GetPairByType(PROC, PROC);
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.second);
  }

  return {second_arg.value, single_constraints};
}

Table CallsClause::HandleWildcardWildcard() {
  bool is_false_clause = pkb->GetCallStore()->GetPairByType(PROC, PROC).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table CallsClause::HandleWildcardIdent() {
  bool is_false_clause = pkb->GetCallStore()->GetCallersOf(second_arg.value).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table CallsClause::HandleIdentSynonym() {
  auto single_constraints = pkb->GetCallStore()->GetCalleesOf(first_arg.value);
  return {second_arg.value, single_constraints};
}

Table CallsClause::HandleIdentWildcard() {
  bool is_false_clause = pkb->GetCallStore()->GetCalleesOf(first_arg.value).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table CallsClause::HandleIdentIdent() {
  bool is_false_clause =
      !pkb->GetCallStore()->IsCallsPairValid(std::pair<std::string, std::string>(first_arg.value, second_arg.value));
  return ConstructEmptyTable(is_false_clause);
}

bool CallsClause::ExecuteBool() {
  return (this->*execute_bool_handler.at({first_arg.type, second_arg.type}))();
}

bool CallsClause::HandleSynonymSynonymBool() {
  if (first_arg.value==second_arg.value) {
    return true;
  }

  auto pair_constraints = pkb->GetCallStore()->GetPairByType(PROC, PROC);
  return pair_constraints.empty();
}

bool CallsClause::HandleSynonymWildcardBool() {
  auto pair_constraints = pkb->GetCallStore()->GetPairByType(PROC, PROC);
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }

  return pair_constraints.empty();
}

bool CallsClause::HandleSynonymIdentBool() {
  auto single_constraints = pkb->GetCallStore()->GetCallersOf(second_arg.value);
  return single_constraints.empty();
}

bool CallsClause::HandleWildcardSynonymBool() {
  auto pair_constraints = pkb->GetCallStore()->GetPairByType(PROC, PROC);
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.second);
  }

  return single_constraints.empty();
}

bool CallsClause::HandleWildcardWildcardBool() {
  bool is_false_clause = pkb->GetCallStore()->GetPairByType(PROC, PROC).empty();
  return is_false_clause;
}

bool CallsClause::HandleWildcardIdentBool() {
  bool is_false_clause = pkb->GetCallStore()->GetCallersOf(second_arg.value).empty();
  return is_false_clause;
}

bool CallsClause::HandleIdentSynonymBool() {
  auto single_constraints = pkb->GetCallStore()->GetCalleesOf(first_arg.value);
  return single_constraints.empty();
}

bool CallsClause::HandleIdentWildcardBool() {
  bool is_false_clause = pkb->GetCallStore()->GetCalleesOf(first_arg.value).empty();
  return is_false_clause;
}

bool CallsClause::HandleIdentIdentBool() {
  bool is_false_clause =
      !pkb->GetCallStore()->IsCallsPairValid(std::pair<std::string, std::string>(first_arg.value, second_arg.value));
  return is_false_clause;
}

std::set<std::string> CallsClause::GetSynonyms() {
  std::set<std::string> synonyms;
  if (IsArgSynonym(first_arg)) {
    synonyms.emplace(first_arg.value);
  }
  if (IsArgSynonym(second_arg)) {
    synonyms.emplace(second_arg.value);
  }

  return synonyms;
}

size_t CallsClause::GetSynonymsSize() {
  size_t size = 0;
  if (IsArgSynonym(first_arg)) {
    size++;
  }
  if (IsArgSynonym(second_arg)) {
    size++;
  }
  return size;
}

size_t CallsClause::GetWeight() {
  return weight;
}

}

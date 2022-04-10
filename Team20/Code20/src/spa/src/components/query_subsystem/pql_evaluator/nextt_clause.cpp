#include "nextt_clause.h"
#include "clause_util.h"
#include "components/pkb/stores/next_store/next_store.h"

namespace pql {

using namespace clause_util;

NextTClause::NextTClause(const std::unordered_map<std::string, DesignEntityType> &declarations,
                         const PqlToken &first_arg,
                         const PqlToken &second_arg,
                         const pkb::PkbPtr &pkb)
    : declarations(declarations), first_arg(first_arg), second_arg(second_arg), pkb(pkb) {}

Table NextTClause::Execute() {
  if (!pkb->GetNextStore()->IsNextStoreComputed()) {
    pkb->GetNextStore()->ComputeNextStore();
  }
  return (this->*execute_handler.at({first_arg.type, second_arg.type}))();
}

Table NextTClause::HandleSynonymSynonym() {
  if (first_arg.value==second_arg.value) {
    auto single_constraints =
        pkb->GetNextStore()->GetNextStarSameStmt(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)));
    return {first_arg.value, single_constraints};
  }

  auto pair_constraints = pkb->GetNextStore()->GetAllNextStarStmt(
      GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
      GetStmtType(GetSynonymDesignEntity(second_arg, declarations))
  );
  return {first_arg.value, second_arg.value, pair_constraints};
}

Table NextTClause::HandleSynonymWildcard() {
  auto pair_constraints = pkb->GetNextStore()->GetAllNextStarStmt(
      GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
      StmtType::STMT
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }
  return {first_arg.value, single_constraints};
}

Table NextTClause::HandleSynonymInteger() {
  auto single_constraints =
      pkb->GetNextStore()->GetBeforeStarOf(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
                                           second_arg.value);
  return {first_arg.value, single_constraints};
}

Table NextTClause::HandleWildcardSynonym() {
  auto pair_constraints = pkb->GetNextStore()->GetAllNextStarStmt(
      StmtType::STMT,
      GetStmtType(GetSynonymDesignEntity(second_arg, declarations))
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.second);
  }
  return {second_arg.value, single_constraints};
}

Table NextTClause::HandleWildcardWildcard() {
  bool is_false_clause = pkb->GetNextStore()->GetAllNextStarStmt(StmtType::STMT, StmtType::STMT).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table NextTClause::HandleWildcardInteger() {
  bool is_false_clause = pkb->GetNextStore()->GetBeforeStarOf(STMT, second_arg.value).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table NextTClause::HandleIntegerSynonym() {
  auto single_constraints =
      pkb->GetNextStore()->GetNextStarOf(GetStmtType(GetSynonymDesignEntity(second_arg, declarations)),
                                         first_arg.value);
  return {second_arg.value, single_constraints};
}

Table NextTClause::HandleIntegerWildcard() {
  bool is_false_clause = pkb->GetNextStore()->GetNextStarOf(STMT, first_arg.value).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table NextTClause::HandleIntegerInteger() {
  bool is_false_clause = !pkb->GetNextStore()->IsNextStarPairValid({first_arg.value, second_arg.value});
  return ConstructEmptyTable(is_false_clause);
}

bool NextTClause::ExecuteBool() {
  if (!pkb->GetNextStore()->IsNextStoreComputed()) {
    pkb->GetNextStore()->ComputeNextStore();
  }
  return (this->*execute_bool_handler.at({first_arg.type, second_arg.type}))();
}

bool NextTClause::HandleSynonymSynonymBool() {
  if (first_arg.value==second_arg.value) {
    auto single_constraints =
        pkb->GetNextStore()->GetNextStarSameStmt(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)));
    return single_constraints.empty();
  }

  auto pair_constraints = pkb->GetNextStore()->GetAllNextStarStmt(
      GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
      GetStmtType(GetSynonymDesignEntity(second_arg, declarations))
  );
  return pair_constraints.empty();
}

bool NextTClause::HandleSynonymWildcardBool() {
  auto pair_constraints = pkb->GetNextStore()->GetAllNextStarStmt(
      GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
      StmtType::STMT
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }
  return single_constraints.empty();
}

bool NextTClause::HandleSynonymIntegerBool() {
  auto single_constraints =
      pkb->GetNextStore()->GetBeforeStarOf(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
                                           second_arg.value);
  return single_constraints.empty();
}

bool NextTClause::HandleWildcardSynonymBool() {
  auto pair_constraints = pkb->GetNextStore()->GetAllNextStarStmt(
      StmtType::STMT,
      GetStmtType(GetSynonymDesignEntity(second_arg, declarations))
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.second);
  }
  return single_constraints.empty();
}

bool NextTClause::HandleWildcardWildcardBool() {
  bool is_false_clause = pkb->GetNextStore()->GetAllNextStarStmt(StmtType::STMT, StmtType::STMT).empty();
  return is_false_clause;
}

bool NextTClause::HandleWildcardIntegerBool() {
  bool is_false_clause = pkb->GetNextStore()->GetBeforeStarOf(STMT, second_arg.value).empty();
  return is_false_clause;
}

bool NextTClause::HandleIntegerSynonymBool() {
  auto single_constraints =
      pkb->GetNextStore()->GetNextStarOf(GetStmtType(GetSynonymDesignEntity(second_arg, declarations)),
                                         first_arg.value);
  return single_constraints.empty();
}

bool NextTClause::HandleIntegerWildcardBool() {
  bool is_false_clause = pkb->GetNextStore()->GetNextStarOf(STMT, first_arg.value).empty();
  return is_false_clause;
}

bool NextTClause::HandleIntegerIntegerBool() {
  bool is_false_clause = !pkb->GetNextStore()->IsNextStarPairValid({first_arg.value, second_arg.value});
  return is_false_clause;
}

std::set<std::string> NextTClause::GetSynonyms() {
  std::set<std::string> synonyms;
  if (IsArgSynonym(first_arg)) {
    synonyms.emplace(first_arg.value);
  }
  if (IsArgSynonym(second_arg)) {
    synonyms.emplace(second_arg.value);
  }

  return synonyms;
}

size_t NextTClause::GetSynonymsSize() {
  size_t size = 0;
  if (IsArgSynonym(first_arg)) {
    size++;
  }
  if (IsArgSynonym(second_arg)) {
    size++;
  }
  return size;
}

size_t NextTClause::GetWeight() {
  return weight;
}

}
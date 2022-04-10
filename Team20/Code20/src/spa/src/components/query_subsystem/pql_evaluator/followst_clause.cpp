#include "followst_clause.h"
#include "clause_util.h"
#include "components/pkb/stores/follows_store/follows_store.h"

namespace pql {

using namespace clause_util;

FollowsTClause::FollowsTClause(const std::unordered_map<std::string, DesignEntityType> &declarations,
                               const PqlToken &first_arg,
                               const PqlToken &second_arg,
                               const pkb::PkbPtr &pkb)
    : declarations(declarations), first_arg(first_arg), second_arg(second_arg), pkb(pkb) {}

Table FollowsTClause::Execute() {
  return (this->*execute_handler.at({first_arg.type, second_arg.type}))();
}

Table FollowsTClause::HandleSynonymSynonym() {
  if (first_arg.value==second_arg.value) {
    return ConstructEmptyTable();
  }

  auto pair_constraints = pkb->GetFollowsStore()->GetAllFollowStarStmt(
      GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
      GetStmtType(GetSynonymDesignEntity(second_arg, declarations))
  );
  return {first_arg.value, second_arg.value, pair_constraints};
}

Table FollowsTClause::HandleSynonymWildcard() {
  auto pair_constraints = pkb->GetFollowsStore()->GetAllFollowStarStmt(
      GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
      StmtType::STMT
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }
  return {first_arg.value, single_constraints};
}

Table FollowsTClause::HandleSynonymInteger() {
  auto single_constraints =
      pkb->GetFollowsStore()->GetFollowerStarOf(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
                                                second_arg.value);
  return {first_arg.value, single_constraints};
}

Table FollowsTClause::HandleWildcardSynonym() {
  auto pair_constraints = pkb->GetFollowsStore()->GetAllFollowStarStmt(
      StmtType::STMT,
      GetStmtType(GetSynonymDesignEntity(second_arg, declarations))
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.second);
  }
  return {second_arg.value, single_constraints};
}

Table FollowsTClause::HandleWildcardWildcard() {
  bool is_false_clause = pkb->GetFollowsStore()->GetAllFollowStarStmt(StmtType::STMT, StmtType::STMT).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table FollowsTClause::HandleWildcardInteger() {
  bool is_false_clause = pkb->GetFollowsStore()->GetFollowerStarOf(STMT, second_arg.value).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table FollowsTClause::HandleIntegerSynonym() {
  auto single_constraints =
      pkb->GetFollowsStore()->GetFollowingStarOf(GetStmtType(GetSynonymDesignEntity(second_arg, declarations)),
                                                 first_arg.value);
  return {second_arg.value, single_constraints};
}

Table FollowsTClause::HandleIntegerWildcard() {
  bool is_false_clause = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, first_arg.value).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table FollowsTClause::HandleIntegerInteger() {
  bool is_false_clause = !pkb->GetFollowsStore()->IsFollowsStarPairValid({first_arg.value, second_arg.value});
  return ConstructEmptyTable(is_false_clause);
}

bool FollowsTClause::ExecuteBool() {
  return (this->*execute_bool_handler.at({first_arg.type, second_arg.type}))();
}

bool FollowsTClause::HandleSynonymSynonymBool() {
  if (first_arg.value==second_arg.value) {
    return true;
  }

  auto pair_constraints = pkb->GetFollowsStore()->GetAllFollowStarStmt(
      GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
      GetStmtType(GetSynonymDesignEntity(second_arg, declarations))
  );
  return pair_constraints.empty();
}

bool FollowsTClause::HandleSynonymWildcardBool() {
  auto pair_constraints = pkb->GetFollowsStore()->GetAllFollowStarStmt(
      GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
      StmtType::STMT
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }
  return single_constraints.empty();
}

bool FollowsTClause::HandleSynonymIntegerBool() {
  auto single_constraints =
      pkb->GetFollowsStore()->GetFollowerStarOf(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
                                                second_arg.value);
  return single_constraints.empty();
}

bool FollowsTClause::HandleWildcardSynonymBool() {
  auto pair_constraints = pkb->GetFollowsStore()->GetAllFollowStarStmt(
      StmtType::STMT,
      GetStmtType(GetSynonymDesignEntity(second_arg, declarations))
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.second);
  }
  return single_constraints.empty();
}

bool FollowsTClause::HandleWildcardWildcardBool() {
  bool is_false_clause = pkb->GetFollowsStore()->GetAllFollowStarStmt(StmtType::STMT, StmtType::STMT).empty();
  return is_false_clause;
}

bool FollowsTClause::HandleWildcardIntegerBool() {
  bool is_false_clause = pkb->GetFollowsStore()->GetFollowerStarOf(STMT, second_arg.value).empty();
  return is_false_clause;
}

bool FollowsTClause::HandleIntegerSynonymBool() {
  auto single_constraints =
      pkb->GetFollowsStore()->GetFollowingStarOf(GetStmtType(GetSynonymDesignEntity(second_arg, declarations)),
                                                 first_arg.value);
  return single_constraints.empty();
}

bool FollowsTClause::HandleIntegerWildcardBool() {
  bool is_false_clause = pkb->GetFollowsStore()->GetFollowingStarOf(STMT, first_arg.value).empty();
  return is_false_clause;
}

bool FollowsTClause::HandleIntegerIntegerBool() {
  bool is_false_clause = !pkb->GetFollowsStore()->IsFollowsStarPairValid({first_arg.value, second_arg.value});
  return is_false_clause;
}

std::set<std::string> FollowsTClause::GetSynonyms() {
  std::set<std::string> synonyms;
  if (IsArgSynonym(first_arg)) {
    synonyms.emplace(first_arg.value);
  }
  if (IsArgSynonym(second_arg)) {
    synonyms.emplace(second_arg.value);
  }

  return synonyms;
}

size_t FollowsTClause::GetSynonymsSize() {
  size_t size = 0;
  if (IsArgSynonym(first_arg)) {
    size++;
  }
  if (IsArgSynonym(second_arg)) {
    size++;
  }
  return size;
}

size_t FollowsTClause::GetWeight() {
  return weight;
}

}
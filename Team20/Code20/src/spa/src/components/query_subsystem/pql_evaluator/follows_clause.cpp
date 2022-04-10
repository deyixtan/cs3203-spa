#include "follows_clause.h"
#include "clause_util.h"

namespace pql {

using namespace clause_util;

FollowsClause::FollowsClause(const std::unordered_map<std::string, DesignEntityType> &declarations,
                             const PqlToken &first_arg,
                             const PqlToken &second_arg,
                             const PkbPtr &pkb)
    : declarations(declarations), first_arg(first_arg), second_arg(second_arg), pkb(pkb) {}

Table FollowsClause::Execute() {
  return (this->*execute_handler.at({first_arg.type, second_arg.type}))();
}

Table FollowsClause::HandleSynonymSynonym() {
  if (first_arg.value==second_arg.value) {
    return ConstructEmptyTable();
  }

  auto pair_constraints = pkb->GetFollowsStore()->GetAllFollowStmt(
      GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
      GetStmtType(GetSynonymDesignEntity(second_arg, declarations))
  );
  return {first_arg.value, second_arg.value, pair_constraints};
}

Table FollowsClause::HandleSynonymWildcard() {
  auto pair_constraints = pkb->GetFollowsStore()->GetAllFollowStmt(
      GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
      StmtType::STMT
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }
  return {first_arg.value, single_constraints};
}

Table FollowsClause::HandleSynonymInteger() {
  std::unordered_set<std::string> single_constraints;
  std::string follower =
      pkb->GetFollowsStore()->GetFollowerOf(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
                                            second_arg.value);
  if (follower!="0") {
    single_constraints.emplace(follower);
  }
  return {first_arg.value, single_constraints};
}

Table FollowsClause::HandleWildcardSynonym() {
  auto pair_constraints = pkb->GetFollowsStore()->GetAllFollowStmt(
      StmtType::STMT,
      GetStmtType(GetSynonymDesignEntity(second_arg, declarations))
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.second);
  }
  return {second_arg.value, single_constraints};
}

Table FollowsClause::HandleWildcardWildcard() {
  bool is_false_clause = pkb->GetFollowsStore()->GetAllFollowStmt(StmtType::STMT, StmtType::STMT).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table FollowsClause::HandleWildcardInteger() {
  bool is_false_clause = pkb->GetFollowsStore()->GetFollowerOf(STMT, second_arg.value)=="0";
  return ConstructEmptyTable(is_false_clause);
}

Table FollowsClause::HandleIntegerSynonym() {
  std::unordered_set<std::string> single_constraints;
  std::string following =
      pkb->GetFollowsStore()->GetFollowingOf(GetStmtType(GetSynonymDesignEntity(second_arg, declarations)),
                                             first_arg.value);
  if (following!="0") {
    single_constraints.emplace(following);
  }
  return {second_arg.value, single_constraints};
}

Table FollowsClause::HandleIntegerWildcard() {
  bool is_false_clause = pkb->GetFollowsStore()->GetFollowingOf(STMT, first_arg.value)=="0";
  return ConstructEmptyTable(is_false_clause);
}

Table FollowsClause::HandleIntegerInteger() {
  bool is_false_clause = !pkb->GetFollowsStore()->IsFollowsPairValid({first_arg.value, second_arg.value});
  return ConstructEmptyTable(is_false_clause);
}

bool FollowsClause::ExecuteBool() {
  return (this->*execute_bool_handler.at({first_arg.type, second_arg.type}))();
}

bool FollowsClause::HandleSynonymSynonymBool() {
  if (first_arg.value==second_arg.value) {
    return true;
  }

  auto pair_constraints = pkb->GetFollowsStore()->GetAllFollowStmt(
      GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
      GetStmtType(GetSynonymDesignEntity(second_arg, declarations))
  );
  return pair_constraints.empty();
}

bool FollowsClause::HandleSynonymWildcardBool() {
  auto pair_constraints = pkb->GetFollowsStore()->GetAllFollowStmt(
      GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
      StmtType::STMT
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }
  return single_constraints.empty();
}

bool FollowsClause::HandleSynonymIntegerBool() {
  std::unordered_set<std::string> single_constraints;
  std::string follower =
      pkb->GetFollowsStore()->GetFollowerOf(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
                                            second_arg.value);
  if (follower!="0") {
    single_constraints.emplace(follower);
  }
  return single_constraints.empty();
}

bool FollowsClause::HandleWildcardSynonymBool() {
  auto pair_constraints = pkb->GetFollowsStore()->GetAllFollowStmt(
      StmtType::STMT,
      GetStmtType(GetSynonymDesignEntity(second_arg, declarations))
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.second);
  }
  return single_constraints.empty();
}

bool FollowsClause::HandleWildcardWildcardBool() {
  bool is_false_clause = pkb->GetFollowsStore()->GetAllFollowStmt(StmtType::STMT, StmtType::STMT).empty();
  return is_false_clause;
}

bool FollowsClause::HandleWildcardIntegerBool() {
  bool is_false_clause = pkb->GetFollowsStore()->GetFollowerOf(STMT, second_arg.value)=="0";
  return is_false_clause;
}

bool FollowsClause::HandleIntegerSynonymBool() {
  std::unordered_set<std::string> single_constraints;
  std::string following =
      pkb->GetFollowsStore()->GetFollowingOf(GetStmtType(GetSynonymDesignEntity(second_arg, declarations)),
                                             first_arg.value);
  if (following!="0") {
    single_constraints.emplace(following);
  }
  return single_constraints.empty();
}

bool FollowsClause::HandleIntegerWildcardBool() {
  bool is_false_clause = pkb->GetFollowsStore()->GetFollowingOf(STMT, first_arg.value)=="0";
  return is_false_clause;
}

bool FollowsClause::HandleIntegerIntegerBool() {
  bool is_false_clause = !pkb->GetFollowsStore()->IsFollowsPairValid({first_arg.value, second_arg.value});
  return is_false_clause;
}

std::set<std::string> FollowsClause::GetSynonyms() {
  std::set<std::string> synonyms;
  if (IsArgSynonym(first_arg)) {
    synonyms.emplace(first_arg.value);
  }
  if (IsArgSynonym(second_arg)) {
    synonyms.emplace(second_arg.value);
  }

  return synonyms;
}

size_t FollowsClause::GetSynonymsSize() {
  size_t size = 0;
  if (IsArgSynonym(first_arg)) {
    size++;
  }
  if (IsArgSynonym(second_arg)) {
    size++;
  }
  return size;
}

size_t FollowsClause::GetWeight() {
  return weight;
}

}


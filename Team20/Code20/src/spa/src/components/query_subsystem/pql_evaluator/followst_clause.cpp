#include "followst_clause.h"
#include "clause_util.h"

namespace pql {

using namespace clause_util;

FollowsTClause::FollowsTClause(const std::unordered_map<std::string, DesignEntityType> &declarations,
                               const PqlToken &first_arg,
                               const PqlToken &second_arg,
                               PKB *pkb)
    : declarations(declarations), first_arg(first_arg), second_arg(second_arg), pkb(pkb) {}

Table FollowsTClause::Execute() {
  if (IsArgSynonym(first_arg) && IsArgSynonym(second_arg)) {
    // Follows*(s1, s2)
    return HandleSynonymSynonym();
  } else if (IsArgSynonym(first_arg) && IsArgWildcard(second_arg)) {
    // Follows*(s, _)
    return HandleSynonymWildcard();
  } else if (IsArgSynonym(first_arg) && IsArgInteger(second_arg)) {
    // Follows*(s, 2)
    return HandleSynonymInteger();
  } else if (IsArgWildcard(first_arg) && IsArgSynonym(second_arg)) {
    // Follows*(_, s)
    return HandleWildcardSynonym();
  } else if (IsArgWildcard(first_arg) && IsArgWildcard(second_arg)) {
    // Follows*(_, _)
    return HandleWildcardWildcard();
  } else if (IsArgWildcard(first_arg) && IsArgInteger(second_arg)) {
    // Follows*(_, 2)
    return HandleWildcardInteger();
  } else if (IsArgInteger(first_arg) && IsArgSynonym(second_arg)) {
    // Follows*(1, s)
    return HandleIntegerSynonym();
  } else if (IsArgInteger(first_arg) && IsArgWildcard(second_arg)) {
    // Follows*(1, _)
    return HandleIntegerWildcard();
  } else if (IsArgInteger(first_arg) && IsArgInteger(second_arg)) {
    // Follows*(1, 2)
    return HandleIntegerInteger();
  } else {
    // throw exception???
    // return empty table???
    return {};
  }
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
  auto is_false_clause = pkb->GetFollowsStore()->GetFollowerStarOf(STMT, second_arg.value).empty();
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
  auto is_false_clause = !pkb->GetFollowsStore()->IsFollowsStarPairValid({first_arg.value, second_arg.value});
  return ConstructEmptyTable(is_false_clause);
}

}
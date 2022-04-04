#include "follows_clause.h"
#include "clause_util.h"

namespace pql {

using namespace clause_util;

FollowsClause::FollowsClause(const std::unordered_map<std::string, DesignEntityType> &declarations,
                             const PqlToken &first_arg,
                             const PqlToken &second_arg,
                             PKB *pkb)
    : declarations(declarations), first_arg(first_arg), second_arg(second_arg), pkb(pkb) {}

Table FollowsClause::Execute() {
  if (IsArgSynonym(first_arg) && IsArgSynonym(second_arg)) {
    // Follows(s1, s2)
    return HandleSynonymSynonym();
  } else if (IsArgSynonym(first_arg) && IsArgWildcard(second_arg)) {
    // Follows(s, _)
    return HandleSynonymWildcard();
  } else if (IsArgSynonym(first_arg) && IsArgInteger(second_arg)) {
    // Follows(s, 2)
    return HandleSynonymInteger();
  } else if (IsArgWildcard(first_arg) && IsArgSynonym(second_arg)) {
    // Follows(_, s)
    return HandleWildcardSynonym();
  } else if (IsArgWildcard(first_arg) && IsArgWildcard(second_arg)) {
    // Follows(_, _)
    return HandleWildcardWildcard();
  } else if (IsArgWildcard(first_arg) && IsArgInteger(second_arg)) {
    // Follows(_, 2)
    return HandleWildcardInteger();
  } else if (IsArgInteger(first_arg) && IsArgSynonym(second_arg)) {
    // Follows(1, s)
    return HandleIntegerSynonym();
  } else if (IsArgInteger(first_arg) && IsArgWildcard(second_arg)) {
    // Follows(1, _)
    return HandleIntegerWildcard();
  } else if (IsArgInteger(first_arg) && IsArgInteger(second_arg)) {
    // Follows(1, 2)
    return HandleIntegerInteger();
  } else {
    // throw exception???
    // return empty table???
    return {};
  }
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
  auto single_constraints =
      std::unordered_set{
          pkb->GetFollowsStore()->GetFollowingOf(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
                                                 second_arg.value)};
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
  auto single_constraints =
      std::unordered_set{
          pkb->GetFollowsStore()->GetFollowingOf(GetStmtType(GetSynonymDesignEntity(second_arg, declarations)),
                                                 first_arg.value)};
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

}


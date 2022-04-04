#include "nextt_clause.h"
#include "clause_util.h"

namespace pql {

using namespace clause_util;

NextTClause::NextTClause(const std::unordered_map<std::string, DesignEntityType> &declarations,
                         const PqlToken &first_arg,
                         const PqlToken &second_arg,
                         PKB *pkb)
    : declarations(declarations), first_arg(first_arg), second_arg(second_arg), pkb(pkb) {}

Table NextTClause::Execute() {
  if (IsArgSynonym(first_arg) && IsArgSynonym(second_arg)) {
    // Next(s1, s2)
    return HandleSynonymSynonym();
  } else if (IsArgSynonym(first_arg) && IsArgWildcard(second_arg)) {
    // Next(s, _)
    return HandleSynonymWildcard();
  } else if (IsArgSynonym(first_arg) && IsArgInteger(second_arg)) {
    // Next(s, 2)
    return HandleSynonymInteger();
  } else if (IsArgWildcard(first_arg) && IsArgSynonym(second_arg)) {
    // Next(_, s)
    return HandleWildcardSynonym();
  } else if (IsArgWildcard(first_arg) && IsArgWildcard(second_arg)) {
    // Next(_, _)
    return HandleWildcardWildcard();
  } else if (IsArgWildcard(first_arg) && IsArgInteger(second_arg)) {
    // Next(_, 2)
    return HandleWildcardInteger();
  } else if (IsArgInteger(first_arg) && IsArgSynonym(second_arg)) {
    // Next(1, s)
    return HandleIntegerSynonym();
  } else if (IsArgInteger(first_arg) && IsArgWildcard(second_arg)) {
    // Next(1, _)
    return HandleIntegerWildcard();
  } else if (IsArgInteger(first_arg) && IsArgInteger(second_arg)) {
    // Next(1, 2)
    return HandleIntegerInteger();
  } else {
    // throw exception???
    // return empty table???
    return {};
  }
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
  // TODO: IsNextStarPairValid()
  auto possible_next_values = pkb->GetNextStore()->GetNextStarOf(STMT, first_arg.value);
  bool is_false_clause = possible_next_values.find(second_arg.value)==possible_next_values.end();
  return ConstructEmptyTable(is_false_clause);
}

}
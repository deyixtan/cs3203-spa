#include "call_store.h"
#include "../../pkb_relationship.h"

CallStore::CallStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                     std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type) :
    StmtStmtStore(move(stmt_vector), move(stmt_type)) {}

void CallStore::AddCalls(std::string const &caller, std::string const &callee) {
  ExhaustiveAddAllStmt(PROC, caller, PROC, callee, false);

  ExhaustiveAddAllStmt(PROC, caller, PROC, callee, true);
  //NESTED_STMT_STMT_MAP_PTR pair_map = std::make_shared<NESTED_STMT_STMT_MAP>(star_type_pair_map);

  if (IsMapContains(PROC, PROC, &star_type_pair_map) == 1) {
    for (auto &pair : star_type_pair_map.at(PROC).at(PROC).GetPairVector()) {
      if (caller == pair.second) {
        ExhaustiveAddAllStmt(PROC, pair.first, PROC, callee, true);
      }
      if (callee == pair.first) {
        ExhaustiveAddAllStmt(PROC, caller, PROC, pair.second, true);
      }
    }
  }
}

bool CallStore::IsCallsPairValid(IDENT_PAIR const &pair) {
  return IsValid(pair);
}

bool CallStore::IsCallsStarPairValid(IDENT_PAIR const &pair) {
  return IsStarValid(pair);
}

IDENT_SET CallStore::GetCallStmtOf(IDENT proc) {
  if (call_stmt_map.find(proc) == call_stmt_map.end()) {
    return {};
  }
  return call_stmt_map.at(proc);
}

void CallStore::AddCallStmtMap(IDENT proc, IDENT stmt) {
  if (call_stmt_map.find(proc) == call_stmt_map.end()) {
    call_stmt_map.insert({proc, {stmt}});
  } else {
    call_stmt_map.at(proc).insert(stmt);
  }
}

IDENT_SET CallStore::GetCallersOf(IDENT const &proc){
  return GetUpperSetOf(CALLS, PROC, proc);
}

IDENT_SET CallStore::GetCalleesOf(IDENT const &proc) {
  return GetLowerSetOf(PROC, proc);
}

IDENT_SET CallStore::GetCallersStarOf(IDENT const &proc) {
  return GetUpperStarOf(PROC, proc);
}

IDENT_SET CallStore::GetCalleesStarOf(IDENT const &proc) {
  return GetLowerStarOf(PROC, proc);
}

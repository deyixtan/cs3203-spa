#include "call_store.h"

CallStore::CallStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                     std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type) :
    StmtStmtStore(move(stmt_vector), move(stmt_type)) {}

void CallStore::AddCallerHelper(std::string const &caller, std::string const &callee) {
  AddUpperLower(CALLS, caller, callee);
  AddUpperLowerStar(CALLS, caller, callee, std::vector<std::string>());
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
  return GetLowerSetOf(CALLS, PROC, proc);
}

IDENT_SET CallStore::GetCallersStarOf(IDENT const &proc) {
  return GetUpperStarOf(CALLS, PROC, proc);
}

IDENT_SET CallStore::GetCalleesStarOf(IDENT const &proc) {
  return GetLowerStarOf(CALLS, PROC, proc);
}

IDENT_PAIR_VECTOR CallStore::GetAllCalls() {
  return GetAllPairs();
}

IDENT_PAIR_VECTOR CallStore::GetAllCallsStar() {
  return GetAllStarPairs();
}

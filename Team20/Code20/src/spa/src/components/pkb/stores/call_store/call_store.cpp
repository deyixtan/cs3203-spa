#include "call_store.h"

CallStore::CallStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                     std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type) :
    StmtStmtStore(move(stmt_vector), move(stmt_type)) {}

void CallStore::AddCallerHelper(std::string const &caller, std::string const &callee) {
  AddUpperLower(CALLS, caller, callee);
  AddUpperLowerStar(CALLS, caller, callee, std::vector<std::string>());
}

bool CallStore::IsCallsPairValid(std::pair<std::string, std::string> const &pair) {
  return IsValid(pair);
}

bool CallStore::IsCallsStarPairValid(std::pair<std::string, std::string> const &pair) {
  return IsStarValid(pair);
}

std::unordered_set<std::string> CallStore::GetCallStmtOf(std::string proc) {
  if (call_stmt_map.find(proc) == call_stmt_map.end()) {
    return {};
  }
  return call_stmt_map.at(proc);
}

void CallStore::AddCallStmtMap(std::string proc, std::string stmt) {
  if (call_stmt_map.find(proc) == call_stmt_map.end()) {
    call_stmt_map.insert({proc, {stmt}});
  } else {
    call_stmt_map.at(proc).insert(stmt);
  }
}

std::unordered_set<std::string> CallStore::GetCallersOf(std::string const &proc){
  return GetUpperSetOf(CALLS, PROC, proc);
}

std::unordered_set<std::string> CallStore::GetCalleesOf(std::string const &proc) {
  return GetLowerSetOf(CALLS, PROC, proc);
}

std::unordered_set<std::string> CallStore::GetCallersStarOf(std::string const &proc) {
  return GetUpperStarOf(CALLS, PROC, proc);
}

std::unordered_set<std::string> CallStore::GetCalleesStarOf(std::string const &proc) {
  return GetLowerStarOf(CALLS, PROC, proc);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> CallStore::GetAllCalls() {
  return GetAllPairs();
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> CallStore::GetAllCallsStar() {
  return GetAllStarPairs();
}

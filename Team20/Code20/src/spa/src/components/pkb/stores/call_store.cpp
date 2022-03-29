#include "call_store.h"

CallStore::CallStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector) :
    BidirectionalStore(move(stmt_vector)) {}

void CallStore::AddCallerHelper(std::string const &caller, std::string const &callee) {
  AddUpperLower(CALLS, caller, callee);
  AddUpperLowerStar(CALLS, caller, callee, std::vector<std::string>());
}

bool CallStore::IsCallsPairExists(std::string const &first_proc, std::string const &second_proc) {
  return IsExists(std::make_pair(first_proc, second_proc));
}

bool CallStore::IsCallsStarPairExists(const std::string &first_proc, const std::string &second_proc) {
  return IsStarExists(std::make_pair(first_proc, second_proc));
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
  return GetUpperOf(proc);
}

std::unordered_set<std::string> CallStore::GetCalleesOf(std::string const &proc) {
  return GetLowerOf(CALLS, proc);
}

std::unordered_set<std::string> CallStore::GetCallersStarOf(std::string const &proc) {
  return GetUpperStarOf(CALLS, proc);
}

std::unordered_set<std::string> CallStore::GetCalleesStarOf(std::string const &proc) {
  return GetLowerStarOf(CALLS, proc);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> CallStore::GetAllCalls() {
  return GetAllPairs();
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> CallStore::GetAllCallsStar(){
  return GetAllStarPairs();
}

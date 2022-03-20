#include "call_store.h"

CallStore::CallStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector) :
    Store(move(stmt_vector)) {}

void CallStore::AddCallerHelper(std::string const &caller, std::string const &callee) {
  if (rs_map.find(caller) == rs_map.end()) {
    rs_map.insert({caller, {std::unordered_set<std::string>(), std::unordered_set<std::string>(), std::unordered_set<std::string>(),
                            std::unordered_set<std::string>()}});
  }

  if (rs_map.find(callee) == rs_map.end()) {
    rs_map.insert({callee, {std::unordered_set<std::string>(), std::unordered_set<std::string>(), std::unordered_set<std::string>(),
                           std::unordered_set<std::string>()}});
  }
    rs_map.at(caller).callees_set.insert(callee);
    rs_map.at(callee).callers_set.insert(caller);
    all_calls_pairs.emplace(std::pair<std::string, std::string>(caller, callee));
    // Calls star
    rs_map.at(caller).callees_star_set.insert(callee);
    rs_map.at(callee).callers_star_set.insert(caller);
    all_calls_star_pairs.emplace(std::pair<std::string, std::string>(caller, callee));
  if(rs_map.find(callee) != rs_map.end()){
    CallNode node = rs_map.at(callee);
    for(auto calleeStar : node.callees_star_set) {
      all_calls_star_pairs.emplace(std::pair<std::string, std::string>(caller, calleeStar));
      rs_map.at(calleeStar).callers_star_set.insert(caller);
      rs_map.at(caller).callees_star_set.insert(calleeStar);
    }
  }
  if(rs_map.find(caller) != rs_map.end()){
    CallNode node = rs_map.at(caller);
    for(auto callerStar : node.callers_star_set) {
      all_calls_star_pairs.emplace(std::pair<std::string, std::string>(callerStar, callee));
      rs_map.at(callerStar).callees_star_set.insert(callee);
      rs_map.at(callee).callers_star_set.insert(callerStar);
    }
  }
}

bool CallStore::IsCallsPairExists(std::string const &first_proc, std::string const &second_proc) {
  return all_calls_pairs.find({first_proc, second_proc}) != all_calls_pairs.end();
}

bool CallStore::IsCallsStarPairExists(const std::string &first_proc, const std::string &second_proc) {
  return all_calls_star_pairs.find({first_proc, second_proc}) != all_calls_star_pairs.end();
}

std::unordered_set<std::string> CallStore::GetCallersOf(std::string const &proc){
  if (rs_map.find(proc) != rs_map.end()) {
    return rs_map.at(proc).callers_set;
  }
  return {};
}

std::unordered_set<std::string> CallStore::GetCalleesOf(std::string const &proc) {
  if (rs_map.find(proc) != rs_map.end()) {
    return rs_map.at(proc).callees_set;
  }
  return {};
}

std::unordered_set<std::string> CallStore::GetCallersStarOf(std::string const &proc) {
  if (rs_map.find(proc) != rs_map.end()) {
    return rs_map.at(proc).callers_star_set;
  }
  return {};
}

std::unordered_set<std::string> CallStore::GetCalleesStarOf(std::string const &proc) {
  if (rs_map.find(proc) != rs_map.end()) {
    return rs_map.at(proc).callees_star_set;
  }
  return {};
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> CallStore::GetAllCalls() {
  return all_calls_pairs;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> CallStore::GetAllCallsStar(){
  return all_calls_star_pairs;
}

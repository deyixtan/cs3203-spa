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
    if(rs_map.find(caller) != rs_map.end()){
      CallNode node = rs_map.at(caller);
      for(auto callerStar : node.callers_star_set){
        rs_map.at(callee).callers_star_set.insert(callerStar);
        all_calls_star_pairs.emplace(std::pair<std::string, std::string>(callerStar, callee));
        rs_map.at(callerStar).callees_star_set.insert(callee);
      }
    }

}

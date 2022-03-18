#include "next_store.h"

NextStore::NextStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector) :
    Store(move(stmt_vector)) {}

void NextStore::AddNextMap(std::unordered_map<std::string, std::unordered_set<std::string>> rs_map) {
  next_map = rs_map;
}

void NextStore::AddBeforeMap(std::unordered_map<std::string, std::unordered_set<std::string>> rs_map) {
  std::unordered_map<std::string, std::unordered_set<std::string>> new_map;
  for(auto pairs : rs_map) {
    std::string before = pairs.first;
    for(auto stmt : pairs.second) {
      if(new_map.find(stmt) == new_map.end()) {
        std::unordered_set<std::string> before_set = std::unordered_set<std::string>();
        before_set.insert(before);
        new_map.insert({stmt, before_set});
      } else {
        std::unordered_set<std::string> vals = new_map[stmt];
        vals.insert(before);
      }
    }
  }
  before_map = new_map;
}

void NextStore::AddNextPair(std::string const &before, std::string const &next) {
  all_next_pairs.emplace(std::pair<std::string, std::string>(before, next));
}

bool NextStore::IsNext(std::string const &stmt) {
  for(auto pairs : all_next_pairs) {
    if(pairs.second == stmt) {
      return true;
    }
  }
  return false;
}

bool NextStore::IsBefore(std::string const &stmt) {
  for(auto pairs : all_next_pairs) {
    if(pairs.first == stmt) {
      return true;
    }
  }
  return false;
}

bool NextStore::IsNextStar(std::string const &stmt) {
  return IsNext(stmt);
}

bool NextStore::IsBeforeStar(const std::string &stmt) {
  return IsBefore(stmt);
}

bool NextStore::NextExists(std::pair<std::string, std::string> const &pair) {
  if(all_next_pairs.find(pair) != all_next_pairs.end()) {
    return true;
  } else {
    return false;
  }
}

bool NextStore::NextStarExists(std::pair<std::string, std::string> const &pair) {
  std::unordered_set<std::string> res;
  FindNextStarOf(pair.first, res);
  if(res.find(pair.second) != res.end()) {
    return true;
  } else {
    return false;
  }
}

std::string NextStore::GetBeforeOf(std::string const &stmt) {
  for(auto pairs : all_next_pairs) {
    if(pairs.second == stmt) {
      return pairs.first;
    }
  }
  return "";
}

std::unordered_set<std::string> NextStore::GetNextOf(std::string const &stmt) {
  std::unordered_set<std::string> next_set;
  for(auto pairs : all_next_pairs) {
    if(pairs.first == stmt) {
      next_set.insert(pairs.second);
    }
  }
  return next_set;
}

std::unordered_set<std::string> NextStore::GetBeforeStarOf(std::string const &stmt) {
  std::unordered_set<std::string> res;
  FindBeforeStarOf(stmt, res);
  return res;
}

void NextStore::FindBeforeStarOf(std::string const &stmt, std::unordered_set<std::string> res) {
  if(before_map.find(stmt) == before_map.end()) {
    return;
  }
  std::unordered_set<std::string> before_set = before_map[stmt];
  for(auto before : before_set) {
    res.insert(before);
    FindBeforeStarOf(before, res);
  }
}

std::unordered_set<std::string> NextStore::GetNextStarOf(std::string const &stmt) {
  std::unordered_set<std::string> res;
  FindNextStarOf(stmt, res);
  return res;
}

void NextStore::FindNextStarOf(std::string const &stmt, std::unordered_set<std::string> res) {
  if(next_map.find(stmt) == next_map.end()) {
    return;
  }
  std::unordered_set<std::string> next_set = next_map[stmt];
  for(auto next : next_set) {
    res.insert(next);
    FindNextStarOf(next, res);
  }
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> NextStore::GetNextPairs() {
  return all_next_pairs;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> NextStore::GetNextStarPairs() {
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> res;
  for(auto pair : next_map) {
    AddNextStarPairs(pair.first, pair.first, res);
  }
}

void NextStore::AddNextStarPairs(std::string first, std::string curr, std::unordered_set<std::pair<std::string, std::string>, pair_hash> res) {
  if(next_map.find(curr) == next_map.end()) {
    return;
  }
  std::unordered_set<std::string> next_set = next_map[first];
  for(auto next : next_set) {
    res.insert({ first, next});
    AddNextStarPairs(first, next, res);
  }
}

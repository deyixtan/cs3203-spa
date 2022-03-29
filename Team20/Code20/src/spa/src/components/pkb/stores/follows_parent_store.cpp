#include "follows_parent_store.h"

FollowsParentStore::FollowsParentStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector)
    : Store(move(stmt_vector)) {}

void FollowsParentStore::AddUpperLower(bool isFollows, std::string const &upper, std::string const &lower) {
  if (isFollows) {
    AddFollowsUpperLower(upper, lower);
  } else {
    AddParentUpperLower(upper, lower);
  }
}

void FollowsParentStore::AddUpperLowerStar(bool isFollows, std::string const &upper, std::string const &lower, std::vector<std::string> const &visited) {
  if (isFollows) {
    AddFollowsUpperLowerStar(upper, lower);
  } else {
    AddParentUpperLowerStar(lower, visited);
  }
}

void FollowsParentStore::AddFollowsUpperLower(std::string const &upper, std::string const &lower) {
  if (follows_rs_map.find(upper) == follows_rs_map.end()) {
    follows_rs_map.insert({upper, {"0", "0", std::unordered_set<std::string>(), std::unordered_set<std::string>()}});
  }

  if (follows_rs_map.find(lower) == follows_rs_map.end()) {
    follows_rs_map.insert({lower, {"0", "0", std::unordered_set<std::string>(), std::unordered_set<std::string>()}});
  }

  follows_rs_map.at(upper).following = lower;
  follows_rs_map.at(lower).follower = upper;
  upper_set.insert(upper);
  lower_set.insert(lower);
  all_pairs.emplace(std::pair<std::string, std::string>(upper, lower));
}

void FollowsParentStore::AddParentUpperLower(std::string const &upper, std::string const &lower) {
  if (parent_rs_map.find(upper) == parent_rs_map.end()) {
    parent_rs_map.insert({upper, {"0", std::unordered_set<std::string>(), std::unordered_set<std::string>(),
                            std::unordered_set<std::string>()}});
  }

  if (parent_rs_map.find(lower) == parent_rs_map.end()) {
    parent_rs_map.insert({lower, {"0", std::unordered_set<std::string>(), std::unordered_set<std::string>(),
                           std::unordered_set<std::string>()}});
  }

  all_pairs.insert(std::make_pair(upper, lower));
  upper_set.insert(upper);
  lower_set.insert(lower);
  parent_rs_map.at(upper).child.insert(lower);
  parent_rs_map.at(lower).parent = upper;
}

void FollowsParentStore::AddFollowsUpperLowerStar(std::string const &upper, std::string const &lower) {
  if (follows_rs_map.find(upper) == follows_rs_map.end()) {
    follows_rs_map.insert({upper, {"0", "0", std::unordered_set<std::string>(), std::unordered_set<std::string>()}});
  }

  if (follows_rs_map.find(lower) == follows_rs_map.end()) {
    follows_rs_map.insert({lower, {"0", "0", std::unordered_set<std::string>(), std::unordered_set<std::string>()}});
  }

  follows_rs_map.at(upper).following_star.insert(lower);
  follows_rs_map.at(lower).follower_star.insert(upper);
  upper_star_set.insert(upper);
  lower_star_set.insert(lower);
  all_star_pairs.emplace(std::pair<std::string, std::string>(upper, lower));
}

void FollowsParentStore::AddParentUpperLowerStar(std::string const &stmt, std::vector<std::string> const &visited) {
  for (std::string const &s : visited) {
    if (parent_rs_map.find(stmt) == parent_rs_map.end()) {
      parent_rs_map.insert({stmt, {"0", std::unordered_set<std::string>(), std::unordered_set<std::string>(),
                            std::unordered_set<std::string>()}});
    }

    if (parent_rs_map.find(s) == parent_rs_map.end()) {
      parent_rs_map.insert({s, {"0", std::unordered_set<std::string>(), std::unordered_set<std::string>(),
                         std::unordered_set<std::string>()}});
    }

    if (s != stmt) {
      all_star_pairs.insert(std::make_pair(s, stmt));
      parent_rs_map.at(stmt).ance.insert(s);
      parent_rs_map.at(s).desc.insert(stmt);
      upper_star_set.insert(s);
      lower_star_set.insert(stmt);
    }
  }
}

bool FollowsParentStore::IsUpper(std::string const &stmt) {
  return upper_set.find(stmt) != upper_set.end();
}

bool FollowsParentStore::IsLower(std::string const &stmt) {
  return lower_set.find(stmt) != lower_set.end();
}

bool FollowsParentStore::IsUpperStar(std::string const &stmt) {
  return upper_star_set.find(stmt) != upper_star_set.end();
}

bool FollowsParentStore::IsLowerStar(std::string const &stmt) {
  return lower_star_set.find(stmt) != lower_star_set.end();
}

bool FollowsParentStore::IsExists(std::pair<std::string, std::string> const &pair) {
  return all_pairs.find(pair) != all_pairs.end();
}

bool FollowsParentStore::IsStarExists(std::pair<std::string, std::string> const &pair) {
  return all_star_pairs.find(pair) != all_star_pairs.end();
}

std::string FollowsParentStore::GetUpperOf(bool isFollows, std::string const &stmt) {
  if (isFollows) {
    if (follows_rs_map.find(stmt) != follows_rs_map.end()) {
      return follows_rs_map.at(stmt).follower;
    }
    return "0";
  } else {
    if (parent_rs_map.find(stmt) != parent_rs_map.end()) {
      return parent_rs_map.at(stmt).parent;
    }
    return "0";
  }
}

std::string FollowsParentStore::GetFollowingOf(std::string const &stmt) {
  if (follows_rs_map.find(stmt) != follows_rs_map.end()) {
    return follows_rs_map.at(stmt).following;
  }
  return "0";
}

std::unordered_set<std::string> FollowsParentStore::GetChildOf(std::string const &stmt) {
  if (parent_rs_map.find(stmt) != parent_rs_map.end()) {
    return parent_rs_map.at(stmt).child;
  }
  return {};
}

std::unordered_set<std::string> FollowsParentStore::GetUpperStarOf(bool isFollows, std::string const &stmt) {
  if (isFollows) {
    if (follows_rs_map.find(stmt) != follows_rs_map.end()) {
      return follows_rs_map.at(stmt).follower_star;
    }
    return {};
  } else {
    if (parent_rs_map.find(stmt) != parent_rs_map.end()) {
      return parent_rs_map.at(stmt).ance;
    }
    return {};
  }
}

std::unordered_set<std::string> FollowsParentStore::GetLowerStarOf(bool isFollows, std::string const &stmt) {
  if (isFollows) {
    if (follows_rs_map.find(stmt) != follows_rs_map.end()) {
      return follows_rs_map.at(stmt).following_star;
    }
    return {};
  } else {
    if (parent_rs_map.find(stmt) != parent_rs_map.end()) {
      return parent_rs_map.at(stmt).desc;
    }
    return {};
  }
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> FollowsParentStore::GetAllPairs() {
  return all_pairs;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> FollowsParentStore::GetAllStarPairs() {
  return all_star_pairs;
}
#ifndef FOLLOW_STORE_STUB_H
#define FOLLOW_STORE_STUB_H

#include "components/pkb/stores/follows_store/follows_store.h"

struct FollowsNode {
  std::string follower;
  std::string following;
  std::unordered_set<std::string> follower_star;
  std::unordered_set<std::string> following_star;
};

std::unordered_set<std::pair<std::string, std::string>, pair_hash> follow_set =
    {
        {"1", "2"},
        {"2", "3"},
        {"3", "4"},
        {"4", "7"},
        {"5", "6"},
        {"6", "7"},
        {"7", "8"},
        {"8", "10"},
        {"9", "10"},
    };

std::unordered_map<std::string, FollowsNode> rs_map =
    {
        {"1", {"0", "2", {}, {}}},
        {"2", {"1", "3", {}, {}}},
        {"3", {"2", "4", {}, {}}},
        {"4", {"3", "7", {}, {}}},
        {"5", {"0", "6", {}, {}}},
        {"6", {"5", "7", {}, {}}},
        {"7", {"6", "8", {}, {}}},
        {"8", {"7", "10", {}, {}}},
        {"9", {"0", "10", {}, {}}},
        {"10", {"0", "0", {}, {}}},
    };

#endif //FOLLOW_STORE_STUB_H

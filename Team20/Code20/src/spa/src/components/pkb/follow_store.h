#ifndef FOLLOW_STORE_H
#define FOLLOW_STORE_H

#include <string>
#include <unordered_map>
#include <unordered_set>

struct hashFunction {
  size_t operator()(const std::pair<int, int> &x) const {
    return x.first ^ x.second;
  }
};

// A struct to maintain every node's relationships
struct node {
  int follower;
  int following;
  std::unordered_set<int> follower_star;
  std::unordered_set<int> following_star;
};

// A store class that maintains all Parent APIs and relationships
class FollowStore {
 public:
  FollowStore();

  static bool is_follower(int stmt);

  static bool is_following(int stmt);

  static bool is_follower_star(int stmt);

  static bool is_following_star(int stmt);

  static bool follow_exists(int stmt1, int stmt2);

  static bool follower_star_exists(int curr, int ance);

  static bool following_star_exists(int curr, int desc);

  static void rs_init(int num_stmts);

  static void add_follow(int parent, int child);

  static int get_follower_of(int stmt);

  static int get_following_of(int stmt);

  static std::unordered_set<int> get_follower_star_of(int stmt);

  static std::unordered_set<int> get_following_star_of(int stmt);

  static std::unordered_set<std::pair<int, int>, hashFunction> get_follow_pairs();

 private:
  static std::unordered_map<int, node> rs_map;
  static std::unordered_set<std::pair<int, int>, hashFunction> follow_set;
  static std::unordered_set<int> follower_star_set;
  static std::unordered_set<int> following_star_set;
  static std::unordered_set<int> follower_set;
  static std::unordered_set<int> following_set;
};

#endif //FOLLOW_STORE_H

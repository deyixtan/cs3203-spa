#ifndef FOLLOW_STORE_H
#define FOLLOW_STORE_H

#include <string>
#include <unordered_map>
#include <unordered_set>

struct pair_hash {
  template <class T1, class T2>
  std::size_t operator () (const std::pair<T1,T2> &p) const {
    auto h1 = std::hash<T1>{}(p.first);
    auto h2 = std::hash<T2>{}(p.second);

    // Mainly for demonstration purposes, i.e. works but is overly simple
    // In the real world, use sth. like boost.hash_combine
    return h1 ^ h2;
  }
};

// A store class that maintains all Parent APIs and relationships
class FollowStore {
 public:
  FollowStore();

  // A struct to maintain every node's relationships
  struct node {
    int follower;
    int following;
    std::unordered_set<int> follower_star;
    std::unordered_set<int> following_star;
  };

  bool is_follower(int stmt);

  bool is_following(int stmt);

  bool is_follower_star(int stmt);

  bool is_following_star(int stmt);

  bool follow_exists(std::pair<int, int> pair);

  bool follow_star_exists(std::pair<int, int> pair);

  void rs_init(int num_stmts);

  void add_follow(int parent, int child);

  int get_follower_of(int stmt);

  int get_following_of(int stmt);

  std::unordered_set<int> get_follower_star_of(int stmt);

  std::unordered_set<int> get_following_star_of(int stmt);

  //std::unordered_set<std::pair<int, int>, pair_hash> get_follow_pairs();

 private:
  std::unordered_map<int, node> rs_map;
  std::unordered_set<int> follower_star_set;
  std::unordered_set<int> following_star_set;
  std::unordered_set<int> follower_set;
  std::unordered_set<int> following_set;
  std::unordered_set<std::pair<int, int>, pair_hash > all_follow_pairs;
  std::unordered_set<std::pair<int, int>, pair_hash > all_follow_star_pairs;
};

#endif //FOLLOW_STORE_H

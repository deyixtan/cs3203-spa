#include "components/pkb/stores/next_store.h"
#include "next_store_stub.h"
#include "../test_util.h"
#include "catch.hpp"

NextStore set_up_next() {
  std::vector<std::unordered_set<std::string>> stmt_vector;
  for (int i = 0; i < COUNT; i++) {
    stmt_vector.push_back(std::unordered_set<std::string>());
  }
  NextStore next_store = NextStore(std::make_shared<std::vector<std::unordered_set<std::string>>>(stmt_vector));
  next_store.AddNextMap(next_map);
  next_store.AddBeforeMap(before_map);
  next_store.ConstructNextPairs();

  return next_store;
}


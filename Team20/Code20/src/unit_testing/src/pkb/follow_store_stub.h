#ifndef FOLLOW_STORE_STUB_H
#define FOLLOW_STORE_STUB_H

#include "components/pkb/follow_store.h"

std::unordered_set<int> empty_set = {};

std::unordered_map<int, FollowStore::node> rs_map =
    {
        {1, {0, 2, empty_set, empty_set}},
        {2, {1, 3, empty_set, empty_set}},
        {3, {2, 4, empty_set, empty_set}},
        {4, {3, 7, empty_set, empty_set}},
        {5, {0, 6, empty_set, empty_set}},
        {6, {5, 7, empty_set, empty_set}},
        {7, {6, 8, empty_set, empty_set}},
        {8, {7, 10, empty_set, empty_set}},
        {9, {0, 10, empty_set, empty_set}},
    };

#endif //FOLLOW_STORE_STUB_H

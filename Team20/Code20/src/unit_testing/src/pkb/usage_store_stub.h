#ifndef USAGE_STORE_STUB_H
#define USAGE_STORE_STUB_H

#include "components/pkb/usage_store.h"

std::unordered_map<int, std::unordered_set<std::string> > stmt_var_map =
    {
      {1, {"a", "c", "e", "j"}},
      {2, {"b", "d"}},
      {3, {"h"}},
      {4, {"b", "j"}},
      {5, {"a", "b", "c", "d", "e", "f", "g"}},
      {6, {"c", "f", "g", "h"}},
      {7, {"d", "i"}},
      {8, {"f", "h", "j"}},
      {9, {"b", "e", "g"}},
      {10, {"b", "e"}},
    };

std::unordered_map<std::string, std::unordered_set<int> > var_stmt_map =
    {
        {"a", {1, 5}},
        {"b", {2, 4, 5, 9, 10}},
        {"c", {1, 5}},
        {"d", {2, 5}},
        {"e", {1, 5, 9, 10}},
        {"f", {5, 6, 8}},
        {"g", {5, 6, 9}},
        {"h", {3, 6, 8}},
        {"i", {7}},
        {"j", {1, 4, 8}},
    };

std::unordered_map<std::string, std::unordered_set<std::string> > proc_var_map =
    {
        //to be populated
    };

std::unordered_map<std::string, std::unordered_set<std::string> > var_proc_map =
    {
        //to be populated
    };

#endif //USAGE_STORE_STUB_H

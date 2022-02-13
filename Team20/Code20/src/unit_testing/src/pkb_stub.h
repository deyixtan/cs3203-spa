#ifndef PKB_STUB_H
#define PKB_STUB_H
#include "components/pkb/pkb.h"

std::unordered_set<std::string> proc_list = {"main", "foo", "bar", "funcX", "funcY", "funcZ"};
std::unordered_set<std::string> stmt_list = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"};
std::unordered_set<std::string> var_list = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v"};
std::unordered_set<std::string> assign_list = {"1", "2", "3"};
std::unordered_set<std::string> if_list = {"4", "5", "6"};
std::unordered_set<std::string> while_list = {"7"};
std::unordered_set<std::string> const_list = {"1", "4", "7", "9", "11"};
std::unordered_set<std::string> read_list = {"8", "9", "10", "11"};
std::unordered_set<std::string> print_list = {"12", "13", "14", "15"};

std::unordered_map<std::string, std::unordered_set<std::string>> proc_to_stmt =
    {
      {"main", {"1", "2", "3", "4", "5", "6"}},
      {"foo", {"7"}},
      {"bar", {"8", "9"}},
      {"funcX", {"10", "11", "12"}},
      {"funcY", {"13", "14"}},
      {"funcZ", {"15"}}
    };

std::unordered_map<std::string, std::unordered_set<std::string>> stmt_to_proc =
    {
        {"main", {"1", "2", "3", "4", "5", "6"}},
        {"foo", {"7"}},
        {"bar", {"8", "9"}},
        {"funcX", {"10", "11", "12"}},
        {"funcY", {"13", "14"}},
        {"funcZ", {"15"}}
    };

std::unordered_map<std::string, std::unordered_set<std::string>> stmt_to_var =
    {
        {"1", {"a", "b", "f", "i"}},
        {"2", {"c"}},
        {"3", {}},
        {"4", {"d", "k", "n", "s"}},
        {"5", {"e", "o"}},
        {"6", {"a", "f", "g", "h", "i", "j"}},
        {"7", {"c", "k", "l"}},
        {"8", {"m", "p"}},
        {"9", {"c", "n"}},
        {"10", {"o"}},
        {"11", {"c", "p", "q"}},
        {"12", {"r"}},
        {"13", {}},
        {"14", {"s", "t", "u"}},
        {"15", {"v"}},
    };

std::unordered_map<std::string, std::unordered_set<std::string>> var_to_stmt =
    {
        {"a", {"1", "6"}},
        {"b", {"1"}},
        {"c", {"2", "7", "9", "11"}},
        {"d", {"4"}},
        {"e", {"5"}},
        {"f", {"1", "6"}},
        {"g", {"6"}},
        {"h", {"6"}},
        {"i", {"1", "6"}},
        {"j", {"6"}},
        {"k", {"4", "7"}},
        {"l", {"7"}},
        {"m", {"8"}},
        {"n", {"4", "9"}},
        {"o", {"5", "10"}},
        {"p", {"8", "11"}},
        {"q", {"11"}},
        {"r", {"12"}},
        {"s", {"4", "14"}},
        {"t", {"14"}},
        {"u", {"14"}},
        {"v", {"15"}},
    };

#endif //PKB_STUB_H

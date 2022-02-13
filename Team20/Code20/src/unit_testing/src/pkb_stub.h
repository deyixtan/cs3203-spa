#ifndef PKB_STUB_H
#define PKB_STUB_H
#include "components/pkb/pkb.h"

std::unordered_set<std::string> proc_list = {"main", "foo", "bar", "funcX", "funcY", "funcZ"};
std::unordered_set<std::string>
    stmt_list = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"};
std::unordered_set<std::string> var_list =
    {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v"};
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
        {"1", {"main"}},
        {"2", {"main"}},
        {"3", {"main"}},
        {"4", {"main"}},
        {"5", {"main"}},
        {"6", {"main"}},
        {"7", {"foo"}},
        {"8", {"bar"}},
        {"9", {"bar"}},
        {"10", {"funcX"}},
        {"11", {"funcX"}},
        {"12", {"funcX"}},
        {"13", {"funcY"}},
        {"14", {"funcY"}},
        {"15", {"funcZ"}},
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

std::unordered_set<std::pair<std::string, std::string>, pair_hash> proc_var_pairs =
    {
        {"main", "a"},
        {"main", "b"},
        {"main", "e"},
        {"main", "f"},
        {"main", "i"},
        {"main", "k"},
        {"foo", "g"},
        {"bar", "c"},
        {"bar", "d"},
        {"funcX", "a"},
        {"funcX", "b"},
        {"funcX", "c"},
        {"funcY", "h"},
        {"funcY", "j"},
        {"funcZ", "c"},
    };

std::unordered_set<std::pair<std::string, std::string>, pair_hash> stmt_var_pairs =
    {
        {"1", "a"},
        {"6", "a"},
        {"1", "b"},
        {"2", "c"},
        {"7", "c"},
        {"9", "c"},
        {"11", "c"},
        {"4", "d"},
        {"5", "e"},
        {"1", "f"},
        {"6", "f"},
        {"6", "g"},
        {"6", "h"},
        {"1", "i"},
        {"6", "i"},
        {"6", "j"},
        {"4", "k"},
        {"7", "k"},
        {"7", "l"},
        {"8", "m"},
        {"4", "n"},
        {"9", "n"},
        {"5", "o"},
        {"10", "o"},
        {"8", "p"},
        {"11", "p"},
        {"11", "q"},
        {"12", "r"},
        {"4", "s"},
        {"14", "s"},
        {"14", "t"},
        {"14", "u"},
        {"15", "v"},
    };


#endif //PKB_STUB_H

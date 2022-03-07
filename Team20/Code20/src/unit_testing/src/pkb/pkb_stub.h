#ifndef PKB_STUB_H
#define PKB_STUB_H
#include "components/pkb/pkb.h"

/*
	procedure main {
1		read dog;
2		call foo;
3		print cat;
4		dog = mouse + 10 * cat - (dog / mouse) * dragon + mouse + rabbit - cat;
5		if (dog <= 0) then {
6			while ( pig > mouse ) {
7				pig = ox + cat;
			}
		} else {
8			dragon = dog * rabbit / mouse;
9			call bar;
		}
	}
	procedure foo {
10		snake = dog + rabbit;
11		while (10 > cat) {
12			call bar;
		}
	}
	procedure bar {
13		print rabbit;
14		call func;
	}
	procedure func {
15		monkey = tiger + dog;
	}
*/

std::unordered_set<std::string>
    stmt_list = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"};

std::unordered_set<std::string> proc_list = {"main", "foo", "bar", "func"};

std::unordered_map<std::string, std::vector<std::string>> proc_to_stmt =
    {
        {"main", {"1", "2", "3", "4", "5", "6", "7", "8", "9"}},
        {"foo", {"10", "11", "12"}},
        {"bar", {"13", "14"}},
        {"func", {"15"}}
    };

std::unordered_map<std::string, std::string> stmt_to_proc =
    {
        {"1", "main"},
        {"2", "main"},
        {"3", "main"},
        {"4", "main"},
        {"5", "main"},
        {"6", "main"},
        {"7", "main"},
        {"8", "main"},
        {"9", "main"},
        {"10", "foo"},
        {"11", "foo"},
        {"12", "foo"},
        {"13", "bar"},
        {"14", "bar"},
        {"15", "func"}
    };

/* USES STORE */

std::unordered_map<std::string, std::unordered_set<std::string>> uses_proc_to_var =
    {
        {"main", {"dog", "cat", "mouse", "pig", "ox", "dragon", "rabbit"}},
        {"foo", {"dog", "cat", "snake", "rabbit"}},
        {"bar", {"rabbit"}},
        {"func", {"monkey", "tiger", "dog"}}
    };

std::unordered_map<std::string, std::unordered_set<std::string>> uses_var_to_proc =
    {
        {"dog", {"main", "foo", "func"}},
        {"cat", {"main", "foo"}},
        {"mouse", {"main"}},
        {"pig", {"main"}},
        {"ox", {"main"}},
        {"dragon", {"main"}},
        {"rabbit", {"main", "foo", "bar"}},
        {"tiger", {"func"}},
        {"snake", {"main", "foo"}},
        {"monkey", {"func"}}
    };

std::unordered_map<std::string, std::unordered_set<std::string>> uses_stmt_to_var =
    {
        {"3", {"cat"}},
        {"4", {"dog", "mouse", "cat", "dragon", "rabbit"}},
        {"5", {"dog"}},
        {"6", {"pig", "mouse"}},
        {"7", {"pig", "ox", "cat"}},
        {"8", {"dragon", "dog", "rabbit", "mouse"}},
        {"10", {"snake", "dog", "rabbit"}},
        {"11", {"cat"}},
        {"13", {"rabbit"}},
        {"15", {"monkey", "tiger", "dog"}},
    };

std::unordered_map<std::string, std::unordered_set<std::string>> uses_var_to_stmt =
    {
        {"cat", {"3", "4", "7", "11"}},
        {"rabbit", {"4", "8", "10", "13"}},
        {"snake", {"10"}},
        {"tiger", {"15"}},
        {"monkey", {"15"}},
        {"pig", {"6", "7"}},
        {"mouse", {"4", "6", "8"}},
        {"dragon", {"4", "8"}},
        {"ox", {"7"}},
    };

std::unordered_set<std::pair<std::string, std::string>, pair_hash> uses_proc_var_pairs =
    {
        {"main", "dog"},
        {"main", "cat"},
        {"main", "mouse"},
        {"main", "pig"},
        {"main", "ox"},
        {"main", "dragon"},
        {"main", "rabbit"},
        {"foo", "dog"},
        {"foo", "cat"},
        {"foo", "snake"},
        {"foo", "rabbit"},
        {"bar", "rabbit"},
        {"func", "monkey"},
        {"func", "tiger"},
        {"func", "dog"},
    };

std::unordered_set<std::pair<std::string, std::string>, pair_hash> uses_stmt_var_pairs =
    {
        {"3", "cat"},
        {"4", "dog"},
        {"4", "mouse"},
        {"4", "cat"},
        {"4", "dragon"},
        {"4", "mouse"},
        {"4", "rabbit"},
        {"5", "dog"},
        {"6", "pig"},
        {"6", "mouse"},
        {"7", "pig"},
        {"7", "ox"},
        {"7", "cat"},
        {"8", "dragon"},
        {"8", "dog"},
        {"8", "rabbit"},
        {"8", "mouse"},
        {"10", "snake"},
        {"10", "dog"},
        {"10", "rabbit"},
        {"11", "cat"},
        {"13", "rabbit"},
        {"15", "monkey"},
        {"15", "tiger"},
        {"15", "dog"},
    };

std::unordered_set<std::string> all_stmt_using = {"3", "4", "5", "6", "7", "8", "10", "11", "13", "15"};
std::unordered_set<std::string> all_proc_using = {"main", "foo", "bar", "func"};

/* MODIFY STORE */

std::unordered_map<std::string, std::unordered_set<std::string>> mod_proc_to_var =
    {
        {"main", {"dog", "pig", "dragon"}},
        {"foo", {"snake"}},
        {"func", {"monkey"}}
    };

std::unordered_map<std::string, std::unordered_set<std::string>> mod_var_to_proc =
    {
        {"dog", {"main"}},
        {"pig", {"main"}},
        {"dragon", {"main"}},
        {"snake", {"foo"}},
        {"monkey", {"func"}}
    };

std::unordered_map<std::string, std::unordered_set<std::string>> mod_stmt_to_var =
    {   //1,4,7,8,10,15
        {"1", {"dog"}},
        {"4", {"dog"}},
        {"7", {"pig"}},
        {"8", {"dragon"}},
        {"10", {"snake"}},
        {"15", {"monkey"}},
    };

std::unordered_map<std::string, std::unordered_set<std::string>> mod_var_to_stmt =
    {
        {"dog", {"1", "4"}},
        {"pig", {"7"}},
        {"dragon", {"8"}},
        {"snake", {"10"}},
        {"monkey", {"15"}},
    };

std::unordered_set<std::pair<std::string, std::string>, pair_hash> mod_proc_var_pairs =
    {
        {"main", "dog"},
        {"main", "pig"},
        {"main", "dragon"},
        {"foo", "snake"},
        {"func", "monkey"},
    };

std::unordered_set<std::pair<std::string, std::string>, pair_hash> mod_stmt_var_pairs =
    {
        {"1", {"dog"}},
        {"4", {"dog"}},
        {"7", {"pig"}},
        {"8", {"dragon"}},
        {"10", {"snake"}},
        {"15", {"monkey"}},
    };

std::unordered_set<std::string> all_stmt_mod = {"1", "4", "7", "8", "10", "15"};
std::unordered_set<std::string> all_proc_mod = {"main", "foo", "func"};

/* FOLLOW STORE */

std::unordered_map<std::string, FollowNode> follows_rs =
    {
        {"1", {"0", "2", {}, {"2", "3", "4", "5"}}},
        {"2", {"1", "3", {"1"}, {"3", "4", "5"}}},
        {"3", {"2", "4", {"1", "2"}, {"4", "5"}}},
        {"4", {"3", "5", {"1", "2", "3"}, {"5"}}},
        {"5", {"4", "0", {"1", "2", "3", "4"}, {}}},
        {"6", {"0", "0", {}, {}}},
        {"7", {"0", "0", {}, {}}},
        {"8", {"0", "9", {}, {"9"}}},
        {"9", {"8", "0", {"8"}, {}}},
        {"10", {"0", "11", {}, {"11"}}},
        {"11", {"10", "0", {"10"}, {}}},
        {"12", {"0", "0", {}, {}}},
        {"13", {"0", "14", {}, {"14"}}},
        {"14", {"13", "0", {"13"}, {}}},
        {"15", {"0", "0", {}, {}}},
    };

std::unordered_set<std::string> follower_stars = {"1", "2", "3", "4", "8", "10", "13"};

std::unordered_set<std::string> following_stars = {"2", "3", "4", "5", "9", "11", "14"};

std::unordered_set<std::string> followers = {"1", "2", "3", "4", "8", "10", "13"};

std::unordered_set<std::string> followings = {"2", "3", "4", "5", "9", "11", "14"};

std::unordered_set<std::pair<std::string, std::string>, pair_hash> follow_pairs =
    {
        {"1", "2"},
        {"2", "3"},
        {"3", "4"},
        {"8", "9"},
        {"10", "11"},
        {"13", "14"},
    };

std::unordered_set<std::pair<std::string, std::string>, pair_hash> follow_star_pairs =
    {
        {"1", "2"},
        {"1", "3"},
        {"1", "4"},
        {"1", "5"},
        {"2", "3"},
        {"2", "4"},
        {"2", "5"},
        {"3", "4"},
        {"3", "5"},
        {"4", "5"},
        {"8", "9"},
        {"10", "11"},
        {"13", "14"},
    };

/* PARENT STORE */
std::unordered_map<std::string, parent_child> parent_rs =
    {
        {"1", {"0", {}, {}, {}}},
        {"2", {"0", {}, {}, {}}},
        {"3", {"0", {}, {}, {}}},
        {"4", {"0", {}, {}, {}}},
        {"5", {"0", {"6"}, {}, {"6", "7"}}},
        {"6", {"5", {"7"}, {"5"}, {"7"}}},
        {"7", {"6", {}, {"5", "6"}, {}}},
        {"8", {"5", {}, {"5"}, {}}},
        {"9", {"5", {}, {"5"}, {}}},
        {"10", {"0", {}, {}, {}}},
        {"11", {"0", {"12"}, {}, {"12"}}},
        {"12", {"11", {}, {"11"}, {}}},
        {"13", {"0", {}, {}, {}}},
        {"14", {"0", {}, {}, {}}},
        {"15", {"0", {}, {}, {}}},
    };

std::unordered_set<std::pair<std::string, std::string>, pair_hash> parent_child_pairs =
    {
        {"5", "6"},
        {"5", "8"},
        {"5", "9"},
        {"6", "7"},
        {"11", "12"},
    };

std::unordered_set<std::pair<std::string, std::string>, pair_hash> ance_desc_pairs =
    {
        {"5", "6"},
        {"5", "7"},
        {"5", "8"},
        {"5", "9"},
        {"6", "7"},
        {"11", "12"},
    };

std::unordered_set<std::string> ancestors = {"5", "6", "11"};

std::unordered_set<std::string> descendants = {"6", "7", "8", "9", "12"};

std::unordered_set<std::string> parents = {"5", "6", "11"};

std::unordered_set<std::string> children = {"6", "7", "8", "9", "12"};

/* GENERAL */

std::unordered_set<std::string> var_list = {"dog", "cat", "mouse", "pig", "ox", "dragon",
                                            "rabbit", "snake", "monkey", "tiger"};

std::unordered_set<std::string> const_list = {"10", "0"};
std::unordered_set<std::string> read_list = {"1"};
std::unordered_set<std::string> print_list = {"3", "13"};
std::unordered_set<std::string> assign_list = {"4", "7", "8", "10", "15"};
std::unordered_set<std::string> if_list = {"5"};
std::unordered_set<std::string> while_list = {"6", "11"};

std::unordered_set<std::pair<std::string, std::string>, pair_hash> read_pairs =
    {std::pair<std::string, std::string>("1", "dog")};
std::unordered_set<std::pair<std::string, std::string>, pair_hash> print_pairs =
    {
        std::pair<std::string, std::string>("3", "cat"),
        std::pair<std::string, std::string>("13", "rabbit")
    };

std::unordered_map<std::string, std::pair<std::string, std::string> > stmt_to_pattern =
    {
        {"4", {"dog", "(((mouse+(10*cat))-((dog/mouse)*dragon))+((mouse+rabbit)-cat))"}},
        {"7", {"pig", "(ox+cat)"}},
        {"8", {"dragon", "((dog*rabbit)/mouse)"}},
        {"10", {"snake", "(dog+rabbit)"}},
        {"15", {"monkey", "(tiger+dog)"}}
    };

std::unordered_map<std::pair<std::string, std::string>, std::string, pair_hash> pattern_to_stmt =
    {
        {{"dog", "(((mouse+(10*cat))-((dog/mouse)*dragon))+((mouse+rabbit)-cat))"}, "4"},
        {{"pig", "(ox+cat)"}, "7"},
        {{"dragon", "((dog*rabbit)/mouse)"}, "8"},
        {{"snake", "(dog+rabbit)"}, "10"},
        {{"monkey", "(tiger+dog)"}, "15"}
    };

std::unordered_set<std::pair<std::string, std::string>, pair_hash> pattern_pairs =
    {
        {"dog", "(((mouse+(10*cat))-((dog/mouse)*dragon))+((mouse+rabbit)-cat))"},
        {"pig", "(ox+cat)"},
        {"dragon", "((dog*rabbit)/mouse)"},
        {"snake", "(dog+rabbit)"},
        {"monkey", "(tiger+dog)"}
    };

#endif //PKB_STUB_H

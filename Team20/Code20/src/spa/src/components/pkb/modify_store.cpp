#include "modify_store.h"

ModifyStore::ModifyStore() {}

bool ModifyStore::add_stmt_var(int stmt, std::string var) {
    stmt_var_pairs.insert(std::make_pair(stmt, var));
}

bool ModifyStore::add_proc_var(std::string proc, std::string var) {
    proc_var_pairs.insert(std::make_pair(proc, var));
}

bool ModifyStore::stmt_var_exists(std::pair<int, std::string> pair) {
    return stmt_var_pairs.find(pair) != stmt_var_pairs.end();
}

bool ModifyStore::proc_var_exists(std::pair<std::string, std::string> pair) {
    return proc_var_pairs.find(pair) != proc_var_pairs.end();
}

std::unordered_set<std::string> ModifyStore::get_var_mod_by_stmt(int stmt) {
    if (stmt_var_map.find(stmt) != stmt_var_map.end()) {
        return stmt_var_map.at(stmt);
    }
    return std::unordered_set<std::string>();
}

std::unordered_set<int> ModifyStore::get_stmt_mod_by_var(std::string var) {
    if (var_stmt_map.find(var) != var_stmt_map.end()) {
        return var_stmt_map.at(var);
    }
    return std::unordered_set<int>();
}

std::unordered_set<std::string> ModifyStore::get_var_mod_by_proc(std::string proc) {
    if (proc_var_map.find(proc) != proc_var_map.end()) {
        return proc_var_map.at(proc);
    }
    return std::unordered_set<std::string>();
}

std::unordered_set<std::string> ModifyStore::get_proc_mod_by_var(std::string var) {
    if (var_proc_map.find(var) != var_proc_map.end()) {
        return var_proc_map.at(var);
    }
    return std::unordered_set<std::string>();
}

std::unordered_set<std::pair<int, std::string>, hashFunction> ModifyStore::get_all_stmt_var() {
    return stmt_var_pairs;
}

std::unordered_set<std::pair<std::string, std::string>, hashFunction> ModifyStore::get_all_proc_var() {
    return proc_var_pairs;
}

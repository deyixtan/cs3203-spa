#include "ModifyStore.h"

static std::unordered_set<std::pair<int, std::string>> ModifyStore::stmtVarPairs;
static std::unordered_map<int, std::unordered_set<std::string>> ModifyStore::stmtVarTable;
static std::unordered_map<std::string, std::unordered_set<int>> ModifyStore::varStmtTable;

UsageStore::UsageStore() {}

bool UsageStore::getModify(int stmt, std::string variable) {
    //code goes here
}

std::unordered_set<std::string> UsageStore::getModifyByStmt(int stmt) {
    //code goes here
}

std::unordered_set<int> UsageStore::getModifyByVar(std::string variable) {
    //code goes here
}

std::unordered_set<std::pair<int, std::string>> getAllModify() {
    //code goes here
}


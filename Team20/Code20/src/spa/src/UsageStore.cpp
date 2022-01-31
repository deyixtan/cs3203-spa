#include "UsageStore.h"

static std::unordered_set<std::pair<int, std::string>> UsageStore::stmtVarPairs;
static std::unordered_map<int, std::unordered_set<std::string>> UsageStore::stmtVarTable;
static std::unordered_map<std::string, std::unordered_set<int>> UsageStore::varStmtTable;

UsageStore::UsageStore() {}

bool UsageStore::getUsage(int stmt, std::string variable) {
    //code goes here
}

std::unordered_set<std::string> UsageStore::getUsageByStmt(int stmt) {
    //code goes here
}

std::unordered_set<int> UsageStore::getUsageByVar(std::string variable) {
    //code goes here
}

std::unordered_set<std::pair<int, std::string>> getAllUsage() {
    //code goes here
}
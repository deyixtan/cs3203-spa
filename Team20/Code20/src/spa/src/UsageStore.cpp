#include "UsageStore.h"

std::unordered_set<std::pair<int, std::string>> UsageStore::stmtVarPairs;
std::unordered_map<int, std::unordered_set<std::string>> UsageStore::stmtVarTable;
std::unordered_map<std::string, std::unordered_set<int>> UsageStore::varStmtTable;

UsageStore::UsageStore() = default;

bool UsageStore::addUsage(int stmt, std::string variable) {
    //code goes here
}

bool UsageStore::usageExists(std::pair<int, std::string> pair) {
    return stmtVarPairs.find(pair) != stmtVarPairs.end();
}

std::unordered_set<std::string> UsageStore::getUsageByStmt(int stmt) {
    auto findStmt = stmtVarTable.find(stmt);
    if (findStmt != stmtVarTable.end()) {
		return stmtVarTable.at(stmt);
	}
	return {};
}

std::unordered_set<int> UsageStore::getUsageByVar(std::string variable) {
    auto findStmt = varStmtTable.find(variable);
    if (findStmt != varStmtTable.end()) {
		return varStmtTable.at(variable);
	}
	return {};
}

std::unordered_set<std::pair<int, std::string>> UsageStore::getAllUsage() {
    return stmtVarPairs;
}
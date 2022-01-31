#include "UsageStore.h"

std::unordered_set<std::pair<int, std::string>, hashFunction> UsageStore::stmtVarPairs;
std::unordered_map<int, std::unordered_set<std::string> > UsageStore::stmtVarTable;
std::unordered_map<std::string, std::unordered_set<int> > UsageStore::varStmtTable;

UsageStore::UsageStore() = default;

bool UsageStore::addUsage(int stmt, std::string variable) {
    //code goes here
}

bool UsageStore::usageExists(std::pair<int, std::string> pair) {
    return stmtVarPairs.find(pair) != stmtVarPairs.end();
}

std::unordered_set<std::string> UsageStore::getUsageByStmt(int stmt) {
    if (stmtVarTable.find(stmt) != stmtVarTable.end()) {
		return stmtVarTable.at(stmt);
	}
	return std::unordered_set<std::string>();
}

std::unordered_set<int> UsageStore::getUsageByVar(std::string variable) {
    if (varStmtTable.find(variable) != varStmtTable.end()) {
		return varStmtTable.at(variable);
	}
	return std::unordered_set<int>();
}

std::unordered_set<std::pair<int, std::string>, hashFunction> UsageStore::getAllUsage() {
    return stmtVarPairs;
}
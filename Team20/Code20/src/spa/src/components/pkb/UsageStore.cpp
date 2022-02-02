#include "UsageStore.h"

UsageStore::UsageStore() {}

bool UsageStore::addStmtVar(int stmt, std::string var) {
    stmtVarPairs.insert(std::make_pair(stmt, var));
}

bool UsageStore::addProcVar(std::string proc, std::string var) {
    procVarPairs.insert(std::make_pair(proc, var));
}

bool UsageStore::stmtVarExists(std::pair<int, std::string> pair) {
    return stmtVarPairs.find(pair) != stmtVarPairs.end();
}

bool UsageStore::procVarExists(std::pair<std::string, std::string> pair) {
    return procVarPairs.find(pair) != procVarPairs.end();
}

std::unordered_set<std::string> UsageStore::getVarUsedByStmt(int stmt) {
    if (stmtVarMap.find(stmt) != stmtVarMap.end()) {
		return stmtVarMap.at(stmt);
	}
	return std::unordered_set<std::string>();
}

std::unordered_set<int> UsageStore::getStmtUsedByVar(std::string var) {
    if (varStmtMap.find(var) != varStmtMap.end()) {
        return varStmtMap.at(var);
    }
    return std::unordered_set<int>();
}

std::unordered_set<std::string> UsageStore::getVarUsedByProc(std::string proc) {
    if (procVarMap.find(proc) != procVarMap.end()) {
        return procVarMap.at(proc);
    }
    return std::unordered_set<std::string>();
}

std::unordered_set<std::string> UsageStore::getProcUsedByVar(std::string var) {
    if (varProcMap.find(var) != varProcMap.end()) {
        return varProcMap.at(var);
    }
    return std::unordered_set<std::string>();
}

std::unordered_set<std::pair<int, std::string>, hashFunction> UsageStore::getAllStmtVar() {
    return stmtVarPairs;
}

std::unordered_set<std::pair<std::string, std::string>, hashFunction> UsageStore::getAllProcVar() {
    return procVarPairs;
}

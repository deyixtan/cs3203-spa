#include "ModifyStore.h"

std::unordered_set<std::pair<int, std::string>> ModifyStore::stmtVarPairs;
std::unordered_map<int, std::unordered_set<std::string>> ModifyStore::stmtVarTable;
std::unordered_map<std::string, std::unordered_set<int>> ModifyStore::varStmtTable;

ModifyStore::ModifyStore() = default;

bool ModifyStore::addModify(int stmt, std::string variable) {
    //code goes here
}

bool ModifyStore::modifyExists(std::pair<int, std::string> pair) {
    return stmtVarPairs.find(pair) != stmtVarPairs.end();
}

std::unordered_set<std::string> ModifyStore::getModifyByStmt(int stmt) {
    auto findStmt = stmtVarTable.find(stmt);
    if (findStmt != stmtVarTable.end()) {
		return stmtVarTable.at(stmt);
	}
	return {};
}

std::unordered_set<int> ModifyStore::getModifyByVar(std::string variable) {
    auto findStmt = varStmtTable.find(variable);
    if (findStmt != varStmtTable.end()) {
		return varStmtTable.at(variable);
	}
	return {};
}

std::unordered_set<std::pair<int, std::string>> getAllModify() {
    //code goes here
}


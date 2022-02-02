#include "ModifyStore.h"

std::unordered_set<std::pair<int, std::string>, hashFunction> ModifyStore::stmtVarPairs;
std::unordered_set<std::pair<std::string, std::string>, hashFunction> ModifyStore::procVarPairs;
std::unordered_map<int, std::unordered_set<std::string> > ModifyStore::stmtVarMap;
std::unordered_map<std::string, std::unordered_set<int> > ModifyStore::varStmtMap;
std::unordered_map<std::string, std::unordered_set<std::string> > ModifyStore::procVarMap;
std::unordered_map<std::string, std::unordered_set<std::string> > ModifyStore::varProcMap;

ModifyStore::ModifyStore() {}

bool ModifyStore::addStmtVar(int stmt, std::string var) {
    //code goes here
}

bool ModifyStore::addProcVar(std::string proc, std::string var) {
    //code goes here
}

bool ModifyStore::stmtVarExists(std::pair<int, std::string> pair) {
    return stmtVarPairs.find(pair) != stmtVarPairs.end();
}

bool ModifyStore::procVarExists(std::pair<std::string, std::string> pair) {
    return procVarPairs.find(pair) != procVarPairs.end();
}

std::unordered_set<std::string> ModifyStore::getVarUsedByStmt(int stmt) {
    if (stmtVarMap.find(stmt) != stmtVarMap.end()) {
        return stmtVarMap.at(stmt);
    }
    return std::unordered_set<std::string>();
}

std::unordered_set<int> ModifyStore::getStmtUsedByVar(std::string var) {
    if (varStmtMap.find(var) != varStmtMap.end()) {
        return varStmtMap.at(var);
    }
    return std::unordered_set<int>();
}

std::unordered_set<std::string> ModifyStore::getVarUsedByProc(std::string proc) {
    if (procVarMap.find(proc) != procVarMap.end()) {
        return procVarMap.at(proc);
    }
    return std::unordered_set<std::string>();
}

std::unordered_set<std::string> ModifyStore::getProcUsedByVar(std::string var) {
    if (varProcMap.find(var) != varProcMap.end()) {
        return varProcMap.at(var);
    }
    return std::unordered_set<std::string>();
}

std::unordered_set<std::pair<int, std::string>, hashFunction> ModifyStore::getAllStmtVar() {
    return stmtVarPairs;
}

std::unordered_set<std::pair<std::string, std::string>, hashFunction> ModifyStore::getAllProcVar() {
    return procVarPairs;
}
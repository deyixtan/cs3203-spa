#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "PKB.h"
#include "components/source_subsystem/TNode.h"

/*
int PKB::setProcToAST(PROC p, TNode* r) {
	return 0;
}

TNode* PKB::getRootAST (PROC p){
	return nullptr;
}
*/

//Keywords = [procedure, while, read, print, call, if-else-then, assign] -> Need to maintain 1 list for each of these

std::unordered_set<std::string> PKB::procList;
std::unordered_set<int> PKB::whileStmtList;
std::unordered_set<int> PKB::readStmtList;
std::unordered_set<int> PKB::printStmtList;
std::unordered_set<int> PKB::callStmtList;
std::unordered_set<int> PKB::ifStmtList; //need to create separate else-then or treat this as a block?
std::unordered_set<int> PKB::assignStmtList;

std::unordered_set<std::string> PKB::varList;
std::unordered_set<std::string> PKB::constList;

//FollowStore PKB::followStore;
//ParentStore PKB::parentStore;
//UsageStore PKB::usageStore;
//ModifyStore PKB::modifyStore;

PKB::PKB() {}

/* Adders */

void PKB::addProcStmt(int stmtNo, std::string procedure) {
    //code goes here
}

void PKB::addWhileStmt(int stmtNo, std::string procedure) {
    //code goes here
}

void PKB::addReadStmt(int stmtNo, std::string procedure) {
    //code goes here
}

void PKB::addPrintStmt(int stmtNo, std::string procedure) {
    //code goes here
}

void PKB::addCallStmt(int stmtNo, std::string procedure) {
    //code goes here
}

void PKB::addIfStmt(int stmtNo, std::string procedure) {
    //code goes here
}

void PKB::addAssignStmt(int stmtNo, std::string procedure) {
    //code goes here
}

void PKB::addVar(std::string name) {

}

void PKB::addConst(std::string value) {
    //code goes here
}

bool PKB::addUsageStmt(int stmt, std::string variable) {
    //code goes here
}

bool PKB::addModifyStmt(int stmt, std::string variable) {
    //code goes here
}

/* Getters */

std::unordered_set<int> PKB::getProcStmts() {
    //code goes here
}

std::unordered_set<int> PKB::getWhileStmts() {
    //code goes here
}

std::unordered_set<int> PKB::getReadStmts() {
    //code goes here
}

std::unordered_set<int> PKB::getPrintStmts() {
    //code goes here
}

std::unordered_set<int> PKB::getCallStmts() {
    //code goes here
}

std::unordered_set<int> PKB::getIfStmts() {
    //code goes here
}

std::unordered_set<int> PKB::getAssignStmts() {
    //code goes here
}

std::unordered_set<std::string> PKB::getVars() {
    //code goes here
}

std::unordered_set<std::string> PKB::getConsts() {
    //code goes here
}

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

PKB::PKB() {}

/* Adders */

void PKB::addStmt(int stmt, std::string name, stmtType type) {
    switch (type) {
        case PROC:
            procList.insert(name);
        case WHILE:
            whileStmtList.insert(stmt);
        case READ:
            readStmtList.insert(stmt);
        case PRINT:
            printStmtList.insert(stmt);
        case CALL:
            callStmtList.insert(stmt);
        case IF:
            ifStmtList.insert(stmt);
        case ASSIGN:
            assignStmtList.insert(stmt);
        case VARS:
            varList.insert(name);
        case CONSTS:
            constList.insert(name);
        default:
            break;
    }
}

bool PKB::addUsageStmtVar(int stmt, std::string var) {
    usageStore.addStmtVar(stmt, var);
}

bool PKB::addUsageProcVar(std::string proc, std::string var) {
    usageStore.addProcVar(proc, var);
}

bool PKB::addModifyStmtVar(int stmt, std::string var) {
    modifyStore.addStmtVar(stmt, var);
}

bool PKB::addModifyProcVar(std::string proc, std::string var) {
    modifyStore.addProcVar(proc, var);
}

/* Getters */

template <typename T> T PKB::getStmt(stmtType type) {
    switch (type) {
        case PROC:
            return procList;
        case WHILE:
            return whileStmtList;
        case READ:
            return readStmtList;
        case PRINT:
            return printStmtList;
        case CALL:
            return callStmtList;
        case IF:
            return ifStmtList;
        case ASSIGN:
            return assignStmtList;
        case VARS:
            return varList;
        case CONSTS:
            return constList;
        default:
            break;
    }
}

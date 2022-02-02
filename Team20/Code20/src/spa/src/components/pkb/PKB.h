#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include "UsageStore.h"
#include "ModifyStore.h"

//typedef short PROC;

//class TNode;

//class VarTable;  // no need to #include "VarTable.h" as all I need is pointer

struct hashFunction {
    size_t operator()(const std::pair<int , int> &x) const {
        return x.first ^ x.second;
    }
};

enum stmtType {
    NONE,
    PROC,
    WHILE,
    READ,
    PRINT,
    CALL,
    IF,
    ASSIGN,
    VARS,
    CONSTS
};

class PKB {
public:
	//static VarTable* varTable;
	//static int setProcToAST(PROC p, TNode* r);
	//static TNode* getRootAST (PROC p);
    PKB();

    /* Adders */
    void addStmt(int stmt = 0, std::string name = "", stmtType type = NONE);

    bool addUsageStmtVar(int stmt, std::string variable);
    bool addUsageProcVar(std::string proc, std::string var);

    bool addModifyStmtVar(int stmt, std::string variable);
    bool addModifyProcVar(std::string proc, std::string var);

    /* Getters */

    template <typename T> T getStmt(stmtType type);

    std::unordered_set<std::string> getVarUsedByStmt(int stmt);
    std::unordered_set<int> getStmtUsedByVar(std::string var);
    std::unordered_set<std::string> getVarUsedByProc(std::string proc);
    std::unordered_set<std::string> getProcUsedByVar(std::string var);
    std::unordered_set<std::pair<int, std::string>, hashFunction> getAllUsageStmtVar();
    std::unordered_set<std::pair<std::string, std::string>, hashFunction> getAllUsageProcVar();

    std::unordered_set<std::string> getVarModByStmt(int stmt);
    std::unordered_set<int> getStmtModByVar(std::string var);
    std::unordered_set<std::string> getVarModByProc(std::string proc);
    std::unordered_set<std::string> getProcModByVar(std::string var);
    std::unordered_set<std::pair<int, std::string>, hashFunction> getAllModStmtVar();
    std::unordered_set<std::pair<std::string, std::string>, hashFunction> getAllModProcVar();

    /* Checkers */
    bool usageStmtVarExists(std::pair<int, std::string> pair);
    bool usageProcVarExists(std::pair<std::string, std::string> pair);

    bool modifyStmtVarExists(std::pair<int, std::string> pair);
    bool modifyProcVarExists(std::pair<std::string, std::string> pair);

private:
    static std::unordered_set<std::string> procList;
    static std::unordered_set<int> whileStmtList;
    static std::unordered_set<int> readStmtList;
    static std::unordered_set<int> printStmtList;
    static std::unordered_set<int> callStmtList;
    static std::unordered_set<int> ifStmtList; //need to create separate else-then or treat this as a block?
    static std::unordered_set<int> assignStmtList;

    static std::unordered_set<std::string> varList;
    static std::unordered_set<std::string> constList;

    //static FollowStore fStore;
    //static ParentStore pStore;
    static UsageStore usageStore;
    static ModifyStore modifyStore;
};
#pragma once

#include <string>
#include <unordered_set>
#include <vector>

//#include "FollowStorage.h"
//#include "ParentStorage.h"
//#include "UseStorage.h"
//#include "ModifyStorage.h"

//typedef short PROC;

//class TNode;

//class VarTable;  // no need to #include "VarTable.h" as all I need is pointer

class PKB {
public:
	//static VarTable* varTable;
	//static int setProcToAST(PROC p, TNode* r);
	//static TNode* getRootAST (PROC p);
    PKB();

    /* Adders */

    void addProcStmt(int stmtNo, std::string procedure);

    void addWhileStmt(int stmtNo, std::string procedure);

    void addReadStmt(int stmtNo, std::string procedure);

    void addPrintStmt(int stmtNo, std::string procedure);

    void addCallStmt(int stmtNo, std::string procedure);

    void addIfStmt(int stmtNo, std::string procedure);

    void addAssignStmt(int stmtNo, std::string procedure);

    void addVar(std::string name);

    void addConst(std::string value);

    bool addUsageStmt(int stmt, std::string variable);

    bool addModifyStmt(int stmt, std::string variable);

    /* Getters */

    std::unordered_set<int> getProcStmts();

    std::unordered_set<int> getWhileStmts();

    std::unordered_set<int> getReadStmts();

    std::unordered_set<int> getPrintStmts();

    std::unordered_set<int> getCallStmts();

    std::unordered_set<int> getIfStmts();

    std::unordered_set<int> getAssignStmts();

    std::unordered_set<std::string> getVars();

    std::unordered_set<std::string> getConsts();

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
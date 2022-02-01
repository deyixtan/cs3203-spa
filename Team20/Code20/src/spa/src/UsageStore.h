#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

struct hashFunction {
    size_t operator()(const std::pair<int , int> &x) const {
        return x.first ^ x.second;
    }
};

/*
	A store class that maintains all Uses APIs and relationships
*/
class UsageStore {
    public:
        UsageStore();

        bool addStmtVar(int stmt, std::string var);

        bool addProcVar(std::string proc, std::string var);

        bool stmtVarExists(std::pair<int, std::string> pair);

        bool procVarExists(std::pair<std::string, std::string> pair);

        std::unordered_set<std::string> getVarUsedByStmt(int stmt);

        std::unordered_set<int> getStmtUsedByVar(std::string var);

        std::unordered_set<std::string> getVarUsedByProc(std::string proc);

        std::unordered_set<std::string> getProcUsedByVar(std::string var);

        std::unordered_set<std::pair<int, std::string>, hashFunction> getAllStmtVar();

        std::unordered_set<std::pair<std::string, std::string>, hashFunction> getAllProcVar();

    private:
        static std::unordered_set<std::pair<int, std::string>, hashFunction> stmtVarPairs;
        static std::unordered_set<std::pair<std::string, std::string>, hashFunction> procVarPairs;
        static std::unordered_map<int, std::unordered_set<std::string> > stmtVarMap;
        static std::unordered_map<std::string, std::unordered_set<int> > varStmtMap;
        static std::unordered_map<std::string, std::unordered_set<std::string> > procVarMap;
        static std::unordered_map<std::string, std::unordered_set<std::string> > varProcMap;
};
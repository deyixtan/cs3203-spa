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
	A store class that maintains all Modifies APIs and relationships
*/
class ModifyStore {
    public:
        ModifyStore();

        bool addStmtVar(int stmt, std::string var);

        bool addProcVar(std::string proc, std::string var);

        bool stmtVarExists(std::pair<int, std::string> pair);

        bool procVarExists(std::pair<std::string, std::string> pair);

        std::unordered_set<std::string> getVarModByStmt(int stmt);

        std::unordered_set<int> getStmtModByVar(std::string var);

        std::unordered_set<std::string> getVarModByProc(std::string proc);

        std::unordered_set<std::string> getProcModByVar(std::string var);

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
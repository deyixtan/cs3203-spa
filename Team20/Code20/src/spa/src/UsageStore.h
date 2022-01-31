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

        static bool addUsage(int stmt, std::string variable);

        bool usageExists(std::pair<int, std::string> pair);

        std::unordered_set<std::string> getUsageByStmt(int stmt);

        std::unordered_set<int> getUsageByVar(std::string variable);

        std::unordered_set<std::pair<int, std::string>, hashFunction> getAllUsage();

    private:
        static std::unordered_set<std::pair<int, std::string>, hashFunction> stmtVarPairs;
        static std::unordered_map<int, std::unordered_set<std::string> > stmtVarTable;
        static std::unordered_map<std::string, std::unordered_set<int> > varStmtTable;
};
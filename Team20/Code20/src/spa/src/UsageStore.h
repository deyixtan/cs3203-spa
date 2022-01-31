#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

/*
	A store class that maintains all Uses APIs and relationships
*/
class UsageStore {
    public:
        UsageStore();

        bool addUsage(int stmt, std::string variable);

        bool usageExists(std::pair<int, std::string> pair);

        std::unordered_set<std::string> getUsageByStmt(int stmt);

        std::unordered_set<int> getUsageByVar(std::string variable);

        std::unordered_set<std::pair<int, std::string>> getAllUsage();

    private:
        static std::unordered_set<std::pair<int, std::string>> stmtVarPairs;
        static std::unordered_map<int, std::unordered_set<std::string>> stmtVarTable;
        static std::unordered_map<std::string, std::unordered_set<int>> varStmtTable;
};
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

        bool getUsage(int stmt, std::string variable);

        std::unordered_set<std::string> getUsageByStmt(int stmt);

        std::unordered_set<int> getUsageByVar(std::string variable);

        std::unordered_set<std::pair<int, std::string>> getAllUsage();

    private:
        static std::unordered_set<std::pair<int, std::string>> stmtVarPairs;
        static std::unordered_map<int, std::vector<std::string>> stmtVarTable;
        static std::unordered_map<std::string, std::vector<int>> varStmtTable;
};
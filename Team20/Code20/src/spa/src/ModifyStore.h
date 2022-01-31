#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

/*
	A store class that maintains all Modifies APIs and relationships
*/
class ModifyStore {
    public:
        ModifyStore();

        static bool addModify(int stmt, std::string variable);

        bool modifyExists(std::pair<int, std::string> pair);

        std::unordered_set<std::string> getModifyByStmt(int stmt);

        std::unordered_set<int> getModifyByVar(std::string variable);

        std::unordered_set<std::pair<int, std::string>> getAllModify();

    private:
        static std::unordered_set<std::pair<int, std::string> > stmtVarPairs;
        static std::unordered_map<int, std::unordered_set<std::string> > stmtVarTable;
        static std::unordered_map<std::string, std::unordered_set<int> > varStmtTable;
};
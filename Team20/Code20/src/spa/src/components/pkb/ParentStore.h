#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

struct hashFunction {
    size_t operator()(const std::pair<int , int> &x) const {
        return x.first ^ x.second;
    }
};

// A struct to maintain every node's relationships
struct parentChild {
    int parent;
    int child;
    std::unordered_set<int> ance;
    std::unordered_set<int> desc;
};

/*
    A store class that maintains all Parent APIs and relationships
*/
class ParentStore {
public:
    ParentStore();

    bool isParent(int stmt);

    bool isChild(int stmt);

    bool isAnce(int stmt);

    bool isDesc(int stmt);

    bool parentChildExists(int stmt1, int stmt2);

    bool anceExists(int curr, int ance);

    bool descExists(int curr, int desc);

    void rsInit(int numStmts);

    void addParentStmt(int parent, int child);

    int getParentOf(int stmt);

    int getChildOf(int stmt);

    std::unordered_set<int> getAllAnceOf(int stmt);

    std::unordered_set<int> getAllDescOf(int stmt);

    std::unordered_set< std::pair<int, int>, hashFunction> getParentChildPairs();

private:
    static std::unordered_map<int, parentChild> rsMap;
    static std::unordered_set< std::pair<int, int>, hashFunction> parentChildSet;
    static std::unordered_set<int> anceSet;
    static std::unordered_set<int> descSet;
    static std::unordered_set<int> parentSet;
    static std::unordered_set<int> childSet;
};

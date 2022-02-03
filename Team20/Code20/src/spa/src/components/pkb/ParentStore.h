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
struct parentChild
{
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

    bool isAnce(int stmt); // For Parent*

    bool isDesc(int stmt); // For Parent*

    bool addParentStmt(int parent, int child);

    bool parentChildExists(int stmt1, int stmt2);

    int getParent(int stmt);

    int getChild(int stmt);

    int getAllAnce(int stmt);

    int getAllDesc(int stmt);

    std::unordered_set< std::pair<int, int>, hashFunction> getParentChildPairs();

private:
    static std::unordered_map<int, parentChild> relationshipMap;
    static std::unordered_set< std::pair<int, int>, hashFunction> parentChildSet;
    static std::unordered_map<int, std::unordered_set<int> > anceDescMap;
    static std::unordered_map<int, std::unordered_set<int> > descAnceMap;
    static std::unordered_set<int> parentSet;
    static std::unordered_set<int> childSet;
};

#import "ParentStore.h"

ParentStore::ParentStore() {}

bool ParentStore::isParent(int stmt) {
    return parentSet.find(stmt) != parentSet.end();
}

bool ParentStore::isChild(int stmt) {
    return childSet.find(stmt) != childSet.end();
}

bool ParentStore::isAnce(int stmt) {
    return anceSet.find(stmt) != anceSet.end();
}

bool ParentStore::isDesc(int stmt) {
    return descSet.find(stmt) != descSet.end();
}

void ParentStore::rsInit(int numStmts) {
    parentChild newPC = {0, 0, std::unordered_set<int>(), std::unordered_set<int>()};

    for (int i = 1; i <= numStmts; i++) {
        rsMap[i] = newPC;
    }
}

void ParentStore::addParentStmt(int parent, int child) {
    if (!parentChildExists(parent, child)) {
        parentChildSet.insert(std::make_pair(parent, child));
        rsMap.at(parent).child = child;
        rsMap.at(child).parent = parent;
    }

    rsMap.at(parent).desc.insert(child);
    rsMap.at(child).ance.insert(parent);
}

bool ParentStore::parentChildExists(int stmt1, int stmt2) {
    std::pair<int, int> p = std::make_pair(stmt1, stmt2);
    return parentChildSet.find(p) != parentChildSet.end();
}

int ParentStore::getParent(int stmt) {
    if (rsMap.find(stmt) != rsMap.end()) {
        parentChild pc = rsMap.at(stmt);
        return pc.parent;
    }
    return 0;
}

int ParentStore::getChild(int stmt) {
    if (rsMap.find(stmt) != rsMap.end()) {
        parentChild pc = rsMap.at(stmt);
        return pc.child;
    }
    return 0;
}

std::unordered_set<int> ParentStore::getAllAnce(int stmt) {
    if (rsMap.find(stmt) != rsMap.end()) {
        parentChild pc = rsMap.at(stmt);
        return pc.ance;
    }
    return std::unordered_set<int>();
}

std::unordered_set<int> ParentStore::getAllDesc(int stmt) {
    if (rsMap.find(stmt) != rsMap.end()) {
        parentChild pc = rsMap.at(stmt);
        return pc.desc;
    }
    return std::unordered_set<int>();
}

std::unordered_set< std::pair<int, int>, hashFunction> ParentStore::getParentChildPairs() {
    return parentChildSet;
}

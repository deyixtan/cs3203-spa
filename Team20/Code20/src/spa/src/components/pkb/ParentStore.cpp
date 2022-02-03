#import "ParentStore.h"

ParentStore::ParentStore() {}

bool ParentStore::isParent(int stmt) {

}

bool ParentStore::isChild(int stmt) {

}

bool ParentStore::isAnce(int stmt) {

} // For Parent*

bool ParentStore::isDesc(int stmt) {

}// For Parent*

bool ParentStore::addParentStmt(int parent, int child) {

}

bool ParentStore::parentChildExists(int stmt1, int stmt2) {

}

int ParentStore::getParent(int stmt) {

}

int ParentStore::getChild(int stmt) {

}

int ParentStore::getAllAnce(int stmt) {

}

int ParentStore::getAllDesc(int stmt) {

}

std::unordered_set< std::pair<int, int>, hashFunction> ParentStore::getParentChildPairs() {

}
#include "call_store.h"

CallStore::CallStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector) :
    Store(move(stmt_vector)) {}

void CallStore::AddCaller(std::string const &caller, std::string const &callee) {
}

void CallStore::AddCallerStar(std::string const &caller, std::string const &callee) {
}
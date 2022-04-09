#include "store.h"

Store::Store(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
             std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type)
    : m_stmt_vector(move(stmt_vector)), m_stmt_type(move(stmt_type)) {}

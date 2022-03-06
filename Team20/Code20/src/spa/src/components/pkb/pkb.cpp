#include "pkb.h"

PKB::PKB() : m_stmt_vector(std::make_shared<std::vector<std::unordered_set<std::string>>>(COUNT)) {
  InitStatementVector();
  InitRelationshipStores();
}

void PKB::InitStatementVector() {
  for (int i = 0; i < COUNT; i++) {
    m_stmt_vector->push_back(std::unordered_set<std::string>());
  }
}

void PKB::InitRelationshipStores() {
  m_follow_store = std::make_shared<FollowStore>(m_stmt_vector);
  m_modify_store = std::make_shared<ModifyStore>(m_stmt_vector);
  m_parent_store = std::make_shared<ParentStore>(m_stmt_vector);
  m_usage_store = std::make_shared<UsageStore>(m_stmt_vector);
}

std::shared_ptr<FollowStore> PKB::GetFollowStore() {
  return m_follow_store;
}

std::shared_ptr<ModifyStore> PKB::GetModifyStore() {
  return m_modify_store;
}

std::shared_ptr<ParentStore> PKB::GetParentStore() {
  return m_parent_store;
}

std::shared_ptr<UsageStore> PKB::GetUsageStore() {
  return m_usage_store;
}

std::unordered_set<std::string> PKB::GetStmt(StmtType type) {
  return m_stmt_vector->at(type);
}

void PKB::AddStmt(std::string stmt, StmtType type) {
  m_stmt_vector->at(type).insert(stmt);
}

void PKB::AddPattern(std::string stmt, std::string lhs, std::string rhs) {
  m_pattern_map[stmt] = {lhs, rhs};
}

std::unordered_set<std::string> PKB::GetStmtWithPattern(std::string lhs, std::string rhs) {
  std::unordered_set<std::string> result = {};
  rhs.erase(remove(rhs.begin(), rhs.end(), ' '), rhs.end());
  //rhs = "(" + rhs + ")";
  //TODO: Tree builder and parser

  for (auto const&[key, val] : m_pattern_map) {
    if (lhs == "_" && rhs == "_") {
      result.insert(key);
    }

    if (lhs == "_" && rhs != "_" && rhs.find("_") != std::string::npos) {
      auto first = rhs.find("_\"");
      auto last = rhs.find("\"_");
      auto sub_pattern = rhs.substr(first + 2, last - 2);
      if (val.second.find(sub_pattern) != std::string::npos) {
        result.insert(key);
      }
    }

    if (lhs == "_" && rhs != "_" && rhs.find("_") == std::string::npos) {
      if (val.second == rhs) {
        result.insert(key);
      }
    }

    if (lhs != "_" && rhs == "_") {
      if (lhs == val.first) {
        result.insert(key);
      }
    }

    if (lhs != "_" && rhs != "_" && rhs.find("_") != std::string::npos) {
      auto first = rhs.find("_\"");
      auto last = rhs.find("\"_");
      auto sub_pattern = rhs.substr(first + 2, last - 2);
      if (lhs == val.first && val.second.find(sub_pattern) != std::string::npos) {
        result.insert(key);
      }
    }

    if (lhs != "_" && rhs != "_" && rhs.find("_") == std::string::npos) {
      if (lhs == val.first && val.second == rhs) {
        result.insert(key);
      }
    }
  }
  return result;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PKB::GetStmtWithPatternSynonym(std::string rhs) {
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> result;
  rhs.erase(remove(rhs.begin(), rhs.end(), ' '), rhs.end());

  for (auto const&[key, val] : m_pattern_map) {
    if (rhs == "_") {
      result.insert({key, val.first});
    }

    // Exact match
    if (rhs != "_" && rhs.find("_") == std::string::npos) {
      if (val.second == rhs) {
        result.insert({key, val.first});
      }
    }

    // Partial match
    if (rhs != "_" && rhs.find("_") != std::string::npos) {
      auto first = rhs.find("_\"");
      auto last = rhs.find("\"_");
      auto sub_pattern = rhs.substr(first + 2, last - 2);
      if (val.second.find(sub_pattern) != std::string::npos) {
        result.insert({key, val.first});
      }
    }
  }

  return result;
}

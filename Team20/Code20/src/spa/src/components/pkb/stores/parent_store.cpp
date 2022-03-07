#import "parent_store.h"

ParentStore::ParentStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector) : Store(move(stmt_vector)) {}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> ParentStore::GetAllParentStmt(StmtType type) {
  std::vector<StmtType> supported_types = {STMT, READ, PRINT, WHILE, IF, ASSIGN};
  return GetAllStmt(type, supported_types, GetParentChildPairs(), false);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> ParentStore::GetAllParentStmt(StmtType type1,
                                                                                                 StmtType type2) {
  std::vector<StmtType> supported_types = {STMT, WHILE, IF};
  return GetAllStmt(type1, type2, supported_types, GetAllParentStmt(type2), true);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> ParentStore::GetAllParentStarStmt(StmtType type) {
  std::vector<StmtType> supported_types = {STMT, READ, PRINT, WHILE, IF, ASSIGN};
  return GetAllStmt(type, supported_types, GetAnceDescPairs(), false);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> ParentStore::GetAllParentStarStmt(StmtType type1,
                                                                                                     StmtType type2) {
  std::vector<StmtType> supported_types = {STMT, WHILE, IF};
  return GetAllStmt(type1, type2, supported_types, GetAllParentStarStmt(type2), true);
}

bool ParentStore::IsParent(std::string const &stmt) {
  return parent_set.find(stmt) != parent_set.end();
}

bool ParentStore::IsChild(std::string const &stmt) {
  return child_set.find(stmt) != child_set.end();
}

bool ParentStore::IsAnce(std::string const &stmt) {
  return ance_set.find(stmt) != ance_set.end();
}

bool ParentStore::IsDesc(std::string const &stmt) {
  return desc_set.find(stmt) != desc_set.end();
}

void ParentStore::Init(int num_stmts) {
  parent_child new_PC =
      {"0", std::unordered_set<std::string>(), std::unordered_set<std::string>(), std::unordered_set<std::string>()};

  for (int i = 1; i <= num_stmts; i++) {
    rs_map[std::to_string(i)] = new_PC;
  }
}

void ParentStore::AddParent(std::string const &parent, std::string const &child) {
  if (rs_map.find(parent) == rs_map.end()) {
    rs_map.insert({parent, {"0", std::unordered_set<std::string>(), std::unordered_set<std::string>(),
                            std::unordered_set<std::string>()}});
  }

  if (rs_map.find(child) == rs_map.end()) {
    rs_map.insert({child, {"0", std::unordered_set<std::string>(), std::unordered_set<std::string>(),
                           std::unordered_set<std::string>()}});
  }

  parent_child_set.insert(std::make_pair(parent, child));
  parent_set.insert(parent);
  child_set.insert(child);
  rs_map.at(parent).child.insert(child);
  rs_map.at(child).parent = parent;
}

void ParentStore::AddParentStar(std::string const &stmt, std::vector<std::string> const &visited) {
  for (std::string const &s : visited) {
    if (rs_map.find(stmt) == rs_map.end()) {
      rs_map.insert({stmt, {"0", std::unordered_set<std::string>(), std::unordered_set<std::string>(),
                            std::unordered_set<std::string>()}});
    }

    if (s != stmt) {
      ance_desc_set.insert(std::make_pair(s, stmt));
      rs_map.at(stmt).ance.insert(s);
      rs_map.at(s).desc.insert(stmt);
      ance_set.insert(s);
      desc_set.insert(stmt);
    }
  }
}

// Used for Parent(s1, s2)
bool ParentStore::ParentChildExists(std::string const &stmt1, std::string const &stmt2) {
  std::pair<std::string, std::string> p = std::make_pair(stmt1, stmt2);
  return parent_child_set.find(p) != parent_child_set.end();
}

// Used for Parent*(s1, s2)
bool ParentStore::AnceExists(std::string const &curr, std::string const &ance) {
  std::unordered_set<std::string> all_ance = GetAllAnceOf(curr);
  if (all_ance.find(ance) != all_ance.end()) {
    return true;
  }
  return false;
}

// Used for Parent*(s1, s2)
bool ParentStore::DescExists(std::string const &curr, std::string const &desc) {
  std::unordered_set<std::string> all_desc = GetAllDescOf(curr);
  if (all_desc.find(desc) != all_desc.end()) {
    return true;
  }
  return false;
}

std::unordered_set<std::string> ParentStore::GetAllParents() {
  return parent_set;
}

std::string ParentStore::GetParentOf(std::string const &stmt) {
  if (rs_map.find(stmt) != rs_map.end()) {
    parent_child pc = rs_map.at(stmt);
    return pc.parent;
  }
  return "0";
}

std::unordered_set<std::string> ParentStore::GetChildOf(std::string const &stmt) {
  if (rs_map.find(stmt) != rs_map.end()) {
    parent_child pc = rs_map.at(stmt);
    return pc.child;
  }
  return {};
}

std::unordered_set<std::string> ParentStore::GetAllAnceOf(std::string const &stmt) {
  if (rs_map.find(stmt) != rs_map.end()) {
    parent_child pc = rs_map.at(stmt);
    return pc.ance;
  }
  return {};
}

std::unordered_set<std::string> ParentStore::GetAllDescOf(std::string const &stmt) {
  if (rs_map.find(stmt) != rs_map.end()) {
    parent_child pc = rs_map.at(stmt);
    return pc.desc;
  }
  return {};
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> ParentStore::GetParentChildPairs() {
  return parent_child_set;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> ParentStore::GetAnceDescPairs() {
  return ance_desc_set;
}

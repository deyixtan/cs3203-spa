#import "parent_store.h"

ParentStore::ParentStore() {}

bool ParentStore::is_parent(int stmt) {
  return parent_set.find(stmt) != parent_set.end();
}

bool ParentStore::is_child(int stmt) {
  return child_set.find(stmt) != child_set.end();
}

bool ParentStore::is_ance(int stmt) {
  return ance_set.find(stmt) != ance_set.end();
}

bool ParentStore::is_desc(int stmt) {
  return desc_set.find(stmt) != desc_set.end();
}

void ParentStore::rs_init(int num_stmts) {
  parent_child new_PC = {0, 0, std::unordered_set<int>(), std::unordered_set<int>()};

  for (int i = 1; i <= num_stmts; i++) {
    rs_map[i] = new_PC;
  }
}

void ParentStore::add_parent_stmt(int parent, int child) {
  if (!parent_child_exists(parent, child)) {
    parent_child_set.insert(std::make_pair(parent, child));
    rs_map.at(parent).child = child;
    rs_map.at(child).parent = parent;
  }

  rs_map.at(parent).desc.insert(child);
  rs_map.at(child).ance.insert(parent);
}

// Used for Parent(s1, s2)
bool ParentStore::parent_child_exists(int stmt1, int stmt2) {
  std::pair<int, int> p = std::make_pair(stmt1, stmt2);
  return parent_child_set.find(p) != parent_child_set.end();
}

// Used for Parent*(s1, s2)
bool ParentStore::ance_exists(int curr, int ance) {
  std::unordered_set<int> all_ance = get_all_ance_of(curr);
  if (all_ance.find(ance) != all_ance.end()) {
    return true;
  }
  return false;
}

// Used for Parent*(s1, s2)
bool ParentStore::desc_exists(int curr, int desc) {
  std::unordered_set<int> all_desc = get_all_desc_of(curr);
  if (all_desc.find(desc) != all_desc.end()) {
    return true;
  }
  return false;
}

int ParentStore::get_parent_of(int stmt) {
  if (rs_map.find(stmt) != rs_map.end()) {
    parent_child pc = rs_map.at(stmt);
    return pc.parent;
  }
  return 0;
}

int ParentStore::get_child_of(int stmt) {
  if (rs_map.find(stmt) != rs_map.end()) {
    parent_child pc = rs_map.at(stmt);
    return pc.child;
  }
  return 0;
}

std::unordered_set<int> ParentStore::get_all_ance_of(int stmt) {
  if (rs_map.find(stmt) != rs_map.end()) {
    parent_child pc = rs_map.at(stmt);
    return pc.ance;
  }
  return {};
}

std::unordered_set<int> ParentStore::get_all_desc_of(int stmt) {
  if (rs_map.find(stmt) != rs_map.end()) {
    parent_child pc = rs_map.at(stmt);
    return pc.desc;
  }
  return {};
}

std::unordered_set<std::pair<int, int>, pair_hash> ParentStore::get_parent_child_pairs() {
  return parent_child_set;
}

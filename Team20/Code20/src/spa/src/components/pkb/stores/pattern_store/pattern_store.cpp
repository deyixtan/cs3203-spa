#include "pattern_store.h"

PatternStore::PatternStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                           std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type) :
    Store(move(stmt_vector), move(stmt_type)) {}

void PatternStore::AddStmtWithPattern(std::string const &stmt, std::string const &lhs, std::string const &rhs) {
  m_stmt_pattern_pairs.push_back({stmt, lhs});
  m_stmt_pattern_map[stmt] = {lhs, rhs};
}

void PatternStore::AddWhileWithPattern(std::string const &stmt, std::string const &expr) {
  std::string var;

  for (char const &c : expr) {
    if (isalpha(c)) {
      var += c;
    } else if (!var.empty()) {
      m_while_pattern_pairs.push_back({stmt, var});
      var.clear();
    }
  }
}

void PatternStore::AddIfWithPattern(std::string const &stmt, std::string const &expr) {
  std::string var;

  for (char const &c : expr) {
    if (isalpha(c)) {
      var += c;
    } else if (!var.empty()) {
      m_if_pattern_pairs.push_back({stmt, var});
      var.clear();
    }
  }
}

IDENT_SET PatternStore::GetStmtWithPatternExact(std::string lhs, std::string rhs) {
  IDENT_SET result = {};
  rhs = "(" + rhs + ")";

  ExpressionTree expr_tree = ExpressionTree();
  expr_tree.build(rhs);
  std::string sub_pattern = expr_tree.GetPattern(expr_tree.GetRoot());

  for (auto const&[key, val] : m_stmt_pattern_map) {
    //EXACT MATCH RHS: pattern a(_, "x+1")
    if (lhs == "_" && rhs != "_") {
      if (val.second == sub_pattern) {
        result.insert(key);
      }
    }

    //EXACT MATCH LHS + EXACT MATCH RHS: pattern a("x", "x+1")
    if (lhs != "_" && rhs != "_") {
      if (lhs == val.first && val.second == sub_pattern) {
        result.insert(key);
      }
    }
  }
  return result;
}

IDENT_SET PatternStore::GetStmtWithPatternPartial(IDENT lhs, IDENT rhs) {
  IDENT_SET result = {};
  rhs = "(" + rhs + ")";

  ExpressionTree expr_tree = ExpressionTree();
  expr_tree.build(rhs);
  std::string sub_pattern = expr_tree.GetPattern(expr_tree.GetRoot());

  for (auto const&[key, val] : m_stmt_pattern_map) {
    //PARTIAL MATCH RHS: pattern a(_, _"x+1"_)
    if (lhs == "_") {
      if (val.second.find(sub_pattern) != std::string::npos) {
        result.insert(key);
      }
    } else { //EXACT MATCH LHS + PARTIAL MATCH RHS: pattern a("x", _"x+1"_)
      if (lhs == val.first && val.second.find(sub_pattern) != std::string::npos) {
        result.insert(key);
      }
    }
  }
  return result;
}

IDENT_SET PatternStore::GetStmtWithPatternWildcard(std::string lhs) {
  IDENT_SET result = {};

  for (auto const&[key, val] : m_stmt_pattern_map) {
    //pattern a(_, _)
    if (lhs == "_") {
      result.insert(key);
    } else { //EXACT MATCH LHS: pattern a("x", _)
      if (lhs == val.first) {
        result.insert(key);
      }
    }
  }
  return result;
}

IDENT_PAIR_VECTOR PatternStore::GetStmtWithPatternSynonymExact(std::string expr) {
  IDENT_PAIR_VECTOR result = {};
  expr = "(" + expr + ")";

  ExpressionTree expr_tree = ExpressionTree();
  expr_tree.build(expr);
  std::string sub_pattern = expr_tree.GetPattern(expr_tree.GetRoot());

  for (auto const&[key, val] : m_stmt_pattern_map) {
    //EXACT MATCH RHS: pattern a(v, "x+1")
    if (val.second == sub_pattern) {
      result.push_back({key, val.first});
    }
  }
  return result;
}

IDENT_PAIR_VECTOR PatternStore::GetStmtWithPatternSynonymPartial(std::string expr) {
  IDENT_PAIR_VECTOR result = {};
  expr = "(" + expr + ")";

  ExpressionTree expr_tree = ExpressionTree();
  expr_tree.build(expr);
  std::string sub_pattern = expr_tree.GetPattern(expr_tree.GetRoot());

  for (auto const&[key, val] : m_stmt_pattern_map) {
    //PARTIAL MATCH RHS: pattern a(v, _"x+1"_)
    if (val.second.find(sub_pattern) != std::string::npos) {
      result.push_back({key, val.first});
    }
  }
  return result;
}

IDENT_PAIR_VECTOR PatternStore::GetStmtWithPatternSynonymWildcard() {
  return m_stmt_pattern_pairs;
}

IDENT_SET PatternStore::GetIfWithPattern(std::string var) {
  IDENT_SET result = {};

  for (auto pair : m_if_pattern_pairs) {
    //pattern ifs(_, _, _)
    if (var == "_") {
      result.insert(pair.first);
    }

    //pattern ifs("x", _, _)
    if (var != "_" && pair.second.find(var) != std::string::npos) {
      result.insert(pair.first);
    }
  }
  return result;
}

IDENT_SET PatternStore::GetWhileWithPattern(std::string var) {
  IDENT_SET result = {};

  for (auto pair : m_while_pattern_pairs) {
    //pattern w(_, _)
    if (var == "_") {
      result.insert(pair.first);
    }

    //pattern w("x", _)
    if (var != "_" && pair.second.find(var) != std::string::npos) {
      result.insert(pair.first);
    }
  }
  return result;
}

IDENT_PAIR_VECTOR PatternStore::GetIfWithPatternSynonym() {
  return m_if_pattern_pairs;
}

IDENT_PAIR_VECTOR PatternStore::GetWhileWithPatternSynonym() {
  return m_while_pattern_pairs;
}

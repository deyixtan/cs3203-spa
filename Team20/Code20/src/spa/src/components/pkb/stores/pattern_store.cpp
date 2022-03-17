#include "pattern_store.h"

PatternStore::PatternStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector) :
    Store(move(stmt_vector)) {}

void PatternStore::AddStmtWithPattern(std::string const &stmt, std::string const &lhs, std::string const &rhs) {
  m_stmt_pattern_pairs.insert({stmt, lhs});
  m_stmt_pattern_map[stmt] = {lhs, rhs};
}

void PatternStore::AddWhileWithPattern(std::string const &stmt, std::string const &expr) {
  std::string var;

  for (char const &c : expr) {
    if (isalpha(c)) {
      var += c;
    } else if (!var.empty()) {
      m_while_pattern_pairs.insert({stmt, var});
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
      m_if_pattern_pairs.insert({stmt, var});
      var.clear();
    }
  }
}

std::unordered_set<std::string> PatternStore::GetStmtWithPattern(std::string lhs, std::string rhs) {
  ExpressionTree expr_tree = ExpressionTree();
  std::unordered_set<std::string> result = {};
  lhs.erase(remove(lhs.begin(), lhs.end(), ' '), lhs.end());
  rhs.erase(remove(rhs.begin(), rhs.end(), ' '), rhs.end());
  std::string temp, sub_pattern;

  if (rhs.find('_') != std::string::npos) {
    auto first = rhs.find("_\"");
    auto last = rhs.find("\"_");
    temp = "(" + rhs.substr(first + 2, last - 2) + ")";
  } else {
    temp = "(" + rhs + ")";
  }

  if (temp != "()") {
    expr_tree.build(temp);
    sub_pattern = expr_tree.GetPattern(expr_tree.GetRoot());
  }

  for (auto const&[key, val] : m_stmt_pattern_map) {
    //pattern a(_, _)
    if (lhs == "_" && rhs == "_") {
      result.insert(key);
    }

    //PARTIAL MATCH RHS: pattern a(_, _"x+1"_)
    if (lhs == "_" && rhs != "_" && rhs.find('_') != std::string::npos) {
      if (val.second.find(sub_pattern) != std::string::npos) {
        result.insert(key);
      }
    }

    //EXACT MATCH RHS: pattern a(_, "x+1")
    if (lhs == "_" && rhs != "_" && rhs.find('_') == std::string::npos) {
      if (val.second == rhs) {
        result.insert(key);
      }
    }

    //EXACT MATCH LHS: pattern a("x", _)
    if (lhs != "_" && rhs == "_") {
      if (lhs == val.first) {
        result.insert(key);
      }
    }

    //EXACT MATCH LHS + PARTIAL MATCH RHS: pattern a("x", _"x+1"_)
    if (lhs != "_" && rhs != "_" && rhs.find('_') != std::string::npos) {
      if (lhs == val.first && val.second.find(sub_pattern) != std::string::npos) {
        result.insert(key);
      }
    }

    //EXACT MATCH LHS + EXACT MATCH RHS: pattern a("x", "x+1")
    if (lhs != "_" && rhs != "_" && rhs.find('_') == std::string::npos) {
      if (lhs == val.first && val.second == rhs) {
        result.insert(key);
      }
    }
  }
  return result;
}

std::unordered_set<std::string> PatternStore::GetIfWithPattern(std::string var) {
  std::unordered_set<std::string> result = {};
  std::string sub_pattern = "(" + var + ")";

  for (auto pair : m_if_pattern_pairs) {
    //pattern ifs(_, _, _)
    if (var == "_") {
      result.insert(pair.first);
    }

    //pattern ifs("x", _, _)
    if (var != "_" && pair.second.find(sub_pattern) != std::string::npos) {
      result.insert(pair.first);
    }
  }
  return result;
}

std::unordered_set<std::string> PatternStore::GetWhileWithPattern(std::string var) {
  std::unordered_set<std::string> result = {};
  std::string sub_pattern = "(" + var + ")";

  for (auto pair : m_while_pattern_pairs) {
    //pattern w(_, _)
    if (var == "_") {
      result.insert(pair.first);
    }

    //pattern w("x", _)
    if (var != "_" && pair.second.find(sub_pattern) != std::string::npos) {
      result.insert(pair.first);
    }
  }
  return result;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PatternStore::GetStmtWithPatternSynonym(std::string expr) {
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> result = {};

  //pattern a(v, _)
  if (expr == "_") {
    result = m_stmt_pattern_pairs;
  } else {
    for (auto const&[key, val] : m_stmt_pattern_map) {
      ExpressionTree expr_tree = ExpressionTree();
      std::unordered_set<std::string> result = {};
      expr.erase(remove(expr.begin(), expr.end(), ' '), expr.end());
      std::string temp, sub_pattern;

      //pattern a(v, "x")
      if (expr.find('_') == std::string::npos) {
        expr_tree.build(expr);
        sub_pattern = expr_tree.GetPattern(expr_tree.GetRoot());
        if (val.second.find(sub_pattern)) {
          result.insert({key, val.first});
        }
      }

      //pattern a(v, _"x"_)
      if (expr.find('_') != std::string::npos) {
        if (expr.find('_') != std::string::npos) {
          auto first = expr.find("_\"");
          auto last = expr.find("\"_");
          temp = "(" + expr.substr(first + 2, last - 2) + ")";
        } else {
          temp = "(" + expr + ")";
        }

        expr_tree.build(temp);
        sub_pattern = expr_tree.GetPattern(expr_tree.GetRoot());

        if (val.second.find(sub_pattern)) {
          result.insert({key, val.first});
        }
      }
    }
  }
  return result;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PatternStore::GetIfWithPatternSynonym() {
  return m_if_pattern_pairs;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PatternStore::GetWhileWithPatternSynonym() {
  return m_while_pattern_pairs;
}

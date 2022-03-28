#include "call_graph.h"

CallGraph::CallGraph() : m_names(std::set<std::string>()), m_edges(std::map<std::string, std::set<std::string>>()) {}

std::vector<std::string> CallGraph::TopoSort() {
  std::map<std::string, size_t> indegree;
  for (const std::string &n : m_names) {
    indegree.emplace(std::make_pair(n, back_edges.at(n).size()));
  }

  std::queue<std::string> q;
  for (const auto &it : indegree) {
    if (it.second == 0) {
      q.push(it.first);
    }
  }

  std::vector<std::string> topo_order;
  while (!q.empty()) {
    std::string n = q.front();
    q.pop();
    topo_order.push_back(n);
    for (const std::string &neighbour : m_edges.at(n)) {
      size_t newIndegree = --(indegree.find(neighbour)->second);
      if (newIndegree == 0) {
        q.push(neighbour);
      }
      assert(newIndegree >= 0);
    }
  }

  if (topo_order.size() != m_names.size()) {
    return {};
  }

  return topo_order;
};

bool CallGraph::AddNode(std::string node) {
  if (m_names.count(node)) {
    return false;
  }
  m_names.insert(node);
  m_edges.emplace(node, std::set<std::string>());
  back_edges.emplace(node, std::set<std::string>());
  return true;
}

bool CallGraph::AddEdge(std::string x, std::string y) {
  AddNode(x);
  AddNode(y);
  auto x_out_iter = m_edges.find(x);
  auto y_in_iter = m_edges.find(y);

  if (x_out_iter->second.count(y)) {
    return false;
  }

  x_out_iter->second.insert(y);
  y_in_iter->second.insert(x);

  back_edges.find(y)->second.insert(x);
  return true;
}
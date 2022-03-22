#include "call_graph.h"

std::vector<std::string> CallGraph::TopoSort() {
  std::map<std::string, size_t> indegree;
  for (const std::string &n : names) {
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
    for (const std::string &neighbour : edges.at(n)) {
      size_t newIndegree = --(indegree.find(neighbour)->second);
      if (newIndegree == 0) {
        q.push(neighbour);
      }
      assert(newIndegree >= 0);
    }
  }

  if (topo_order.size() != names.size()) {
    return {};
  }

  return topo_order;

};
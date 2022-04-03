#include "call_graph.h"

namespace source {

CallGraph::CallGraph() : m_names(std::set<std::string>()), fwd_edges(std::map<std::string, std::set<std::string>>()) {}

std::vector<std::string> CallGraph::TopoSort() {
  std::map<std::string, int> in_degree;
  for (const std::string &n : m_names) {
    //Add relationship for number of edges pointing to a node
    in_degree.emplace(std::make_pair(n, bwd_edges.at(n).size()));
  }

  std::queue<std::string> queue;
  for (const auto &it : in_degree) {
    if (it.second == 0) {
      //Push node with highest dependency; this should be handled last
      queue.push(it.first);
    }
  }

  std::vector<std::string> topo_order;
  while (!queue.empty()) {
    //Handle node with lowest dependency
    std::string n = queue.front();
    queue.pop();
    topo_order.push_back(n);
    for (const std::string &neighbour : fwd_edges.at(n)) {
      int new_in_degree = --(in_degree.find(neighbour)->second);
      if (new_in_degree == 0) {
        queue.push(neighbour);
      }
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
  fwd_edges.emplace(node, std::set<std::string>());
  bwd_edges.emplace(node, std::set<std::string>());
  return true;
}

bool CallGraph::AddEdge(std::string src, std::string dst) {
  AddNode(src);
  AddNode(dst);
  auto src_out = fwd_edges.find(src);
  auto dst_in = fwd_edges.find(dst);

  if (src_out->second.count(dst)) {
    return false;
  }

  src_out->second.insert(dst);
  dst_in->second.insert(src);

  bwd_edges.find(dst)->second.insert(src);
  return true;
}

}

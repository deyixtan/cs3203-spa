#ifndef CALL_GRAPH_H
#define CALL_GRAPH_H

#include <vector>
#include <stack>
#include <map>
#include <set>
#include <queue>
#include <string>

namespace source {

class CallGraph {
 private:
  std::set<std::string> m_names;
  std::map<std::string, std::set<std::string>> fwd_edges, bwd_edges;

 public:
  explicit CallGraph();
  /* push_back function is used to add new element at the end of the list container */
  bool AddNode(std::string node);
  bool AddEdge(std::string x, std::string y);
  std::vector<std::string> TopoSort();
};

}

#endif //CALL_GRAPH_H

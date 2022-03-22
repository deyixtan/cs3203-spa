#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_TYPES_CALL_GRAPH_CALL_GRAPH_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_TYPES_CALL_GRAPH_CALL_GRAPH_H_

#include <vector>
#include <stack>
#include <map>
#include <set>
#include <queue>
#include "../types/ast/node_call_statement.h"

class CallGraph {
  std::set<std::string> names;
  std::map<std::string, std::set<std::string>> edges, back_edges;

  public:
    /* push_back function is used to add new element at the end of the list container */
    void add_edge(std::string x, std::string y) {
      edges.at(x).insert(y);
    }
    //void topological(int, std::vector<int>, std::stack<int>& );
    std::vector<std::string> TopoSort();
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_TYPES_CALL_GRAPH_CALL_GRAPH_H_

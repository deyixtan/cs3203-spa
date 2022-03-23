#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_TYPES_CALL_GRAPH_CALL_GRAPH_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_TYPES_CALL_GRAPH_CALL_GRAPH_H_

#include <vector>
#include <stack>
#include <map>
#include <set>
#include <queue>
#include <string>

class CallGraph {
 private:
  std::set<std::string> m_names;
  std::map<std::string, std::set<std::string>> m_edges, back_edges;

  public:
    explicit CallGraph();
    /* push_back function is used to add new element at the end of the list container */
    bool AddNode(std::string node);
    bool AddEdge(std::string x, std::string y);
    bool IsConnected(std::string src, std::string dst);
    bool IsConnectedExcluding(std::string src, std::string dst);
    //void topological(int, std::vector<int>, std::stack<int>& );
    std::vector<std::string> TopoSort();
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_TYPES_CALL_GRAPH_CALL_GRAPH_H_

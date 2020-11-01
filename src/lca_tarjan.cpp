/*
 *  Lowest Common Ancestor(LCA) Offline with Tarjan
 */

#include <iostream>
#include <vector>

/*
 * https://www.shuzhiduo.com/A/8Bz8XpQVzx/
 *
 * lca_tarjan(u) {
 *　　 for each(u,v) {        // u -> v
 *　　     lca_tarjan(v);      // dfs
 *　　     union(u,v);         // set the parent of v = u
 *　　     visit[v] = true     // mark v visited
 *　　  }
 *    // all the node(child) with parent u marked
 *　　  for each(q1, q2) {
 *         if(visit[q1]) { lca_q1_q2 = find(q1) };
 *         if(visit[q2]) { lca_q1_q2 = find(q2) };
 *　　 }
 */

template <typename T> using Tree = std::vector<std::vector<T>>;

template <typename T> void PrintTree(Tree<T> &tree) {
#ifdef DEBUG
  std::cout << "\n-----------tree------------------\n";
  for (int i = 1; i < tree.size(); ++i) {
    for (int j = 1; j < tree[i].size(); ++j) {
      std::cout << tree[i][j] << " ";
    }
    std::cout << std::endl;
  }
#endif
}

/**************************************************
 *   1 -> 2 -> 3 - > 7 -> 8
 *        |    |  \
 *        v    v   v
 *        9    4   5 - > 6
 *
 * {2, 3}, {4, 6}, {5, 6}, {4, 4}, {4, 8, 9}
 * The CLA for above pair is
 * 2, 3, 5, 4, 2
 */
std::vector<int> lca;
std::vector<int> parent;

Tree<int> g_tree;
std::vector<std::vector<int>> g_querys;

// the find operation
int find(int node) {
  if (parent[node] == node) {
    return node;
  }
  parent[node] = find(parent[node]);
  return parent[node];
}

void lca_tarjan(int current) {
  parent[current] = current;

  for (int child = 1; child < g_tree[current].size(); ++child) {
    if (g_tree[current][child]) { // has reachable child
      lca_tarjan(child);
      parent[child] = current; // union operation
    }
  }

#ifdef DEBUG
  for (int i = 1; i < parent.size(); ++i) {
    std::cout << parent[i] << " ";
  }
  std::cout << "\n";
#endif

  // At the point the children of "current" had been visited and
  // their parent had be set
  for (int i = 0; i < g_querys.size(); ++i) {
    std::vector<int> &query = g_querys[i];
    bool merge = false;
    for (int &item : query) {
      if (item == current) {
        merge = true;
      }
    }
    if (merge) {
      lca[i] = lca[i] ? find(lca[i]) : current;
    }
  }
}

// init the lca, parent and tree.
void init(int nums, std::vector<std::vector<int>> &edges) {
  lca.resize(g_querys.size(), 0);
  parent.resize(nums + 1, 0);
  g_tree.resize(nums + 1, std::vector<int>(nums + 1, 0));
  for (int i = 0; i < edges.size(); ++i) {
    int &x = edges[i][0];
    int &y = edges[i][1];
    g_tree[x][y] = 1; // each edge with x -> y
  }

  PrintTree(g_tree);
}

int main() {
  std::vector<std::vector<int>> edges = {{1, 2}, {2, 3}, {2, 9}, {3, 4},
                                         {3, 5}, {3, 7}, {5, 6}, {7, 8}};
  g_querys = {{2, 3}, {4, 6}, {5, 6}, {4, 4}, {4, 8, 9}};

  // init
  init(9, edges);

  // calculate lca with tarjan algorithm
  lca_tarjan(1);

  // print the lca result
  std::cout << "\n-----------lca------------------\n";
  for (int i = 0; i < lca.size(); ++i) {
    std::cout << lca[i] << " ";
  }
  std::cout << "\n\n";
}

// g++ -std=c++11 lca_tarjan.cpp -o lca_tarjan

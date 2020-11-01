/*
 *  Strongly Conntected Component Generation for DAG
 */

#include <iostream>
#include <vector>

/*
 *  Vector-Based stack_ extension for std::stack_
 */
template <typename T> class SStack {
public:
  SStack(bool flag = false) : dump_flag() {}
  ~SStack() = default;

  void push(const T &v) { container_.push_back(v); }
  void pop() { container_.pop_back(); }
  T top() { return container_.back(); }
  bool empty() { return container_.empty(); }
  bool contain(T v) {
    for (T &t : container_) {
      if (t == v) {
        return true;
      }
    }
    return false;
  }
  void dump() {
    if (dump_flag) {
      for (T &t : container_) {
        std::cout << t << " ";
      }
      std::cout << std::endl;
    }
  }

private:
  std::vector<T> container_;
  bool dump_flag = false;
};

/*
 *  Matrix for Graph
 */
template <typename T> using GraphMatrix = std::vector<std::vector<T>>;

template <typename T> void PrintMatrix(GraphMatrix<T> &matrix) {
  for (int i = 0; i < matrix.size(); ++i) {
    for (int j = 0; j < matrix[i].size(); ++j) {
      std::cout << matrix[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

/**************************************************
 *   1 -> 2 -> 3
 *         ^   | ^
 *          \  v   \
 *             4 -> 5 - > 6
 *
 * Builder for generationg SCC for DAG, the above DAG will
 * Generate the following SCC sets
 * { 1 }
 * { 2, 3, 4, 5 }
 * { 6 }
 */
class SCCBuilder {
private:
  int gvn = 1;            // global nodes number for dfn
  std::vector<int> dfn;   // the numbering by dfs algorighm
  std::vector<int> group; // the group number for earch node
  SStack<int> stack_;     // stack_ for nodes
  GraphMatrix<int> dag;   // dag represents with matrix
  GraphMatrix<int> SCCs;  // the SCC sets
  bool dump_flag;

public:
  SCCBuilder(bool flags = true) : dump_flag(flags), stack_(flags) {}
  ~SCCBuilder() = default;

  void SCCGen(int head) {
    std::vector<int> scc;
    stack_.dump();
    while (!stack_.empty()) {
      if (stack_.top() == head) {
        scc.emplace_back(stack_.top());
        stack_.pop();
        break;
      }
      scc.emplace_back(stack_.top());
      stack_.pop();
    }
    stack_.dump();
    if (!scc.empty()) {
      SCCs.emplace_back(scc);
    }
  }
  /* Pseudo Code
    void dfs(int current, int from) {
      dfn[current] = gvn;
      group[current] = gvn;
      ++gvn;
      stack_.push(current);

      for (reachable to : dag) {
        if (!dfn[to]) {
          dfs(to, current);
          if (dfn[to] == group[to]) {
            SCCGen(to);
          }
        }
        if (stack_.contain(to)) {
          group[current] = std::min(group[current], group[to]);
        }
      }
    }*/
  void dfs(int current, int from) {
    dfn[current] = gvn;
    group[current] = gvn;
    ++gvn;
    stack_.push(current);

    if (dump_flag) {
      printf("node-%d[%d : %d]\n", current, dfn[current], group[current]);
    }

    for (int to = 1; to < dag.size(); ++to) {
      if (dag[current][to]) {
        if (dump_flag) {
          printf("%d -> %d\n", current, to);
        }
        if (!dfn[to]) {
          dfs(to, current);
          if (dfn[to] == group[to]) {
            SCCGen(to);
          }
        }
        if (stack_.contain(to)) {
          group[current] = std::min(group[current], group[to]);
          if (dump_flag) {
            printf("found group %d -> %d [%d : %d]\n", current, to,
                   dfn[current], group[current]);
          }
        }
      }
    }
  }

  void SCCScan(int nums, std::vector<std::vector<int>> edges) {
    dfn.resize(nums + 1, 0);
    group.resize(nums + 1, 0);
    dag.resize(nums + 1, std::vector<int>(nums + 1, 0));

    for (int i = 0; i < edges.size(); ++i) {
      std::vector<int> &edge = edges[i];
      dag[edge[0]][edge[1]] = 1;
    }

    for (int i = 1; i <= nums; ++i) {
      if (!dfn[i]) {
        dfs(i, 0);
        if (!stack_.empty()) {
          SCCGen(i);
        }
      }
    }
  }
  void dump() {
    std::cout << "\n-----------------------------\n";
    PrintMatrix(dag);
    std::cout << "\n-----------------------------\n";
    for (int i = 1; i < dfn.size(); ++i) {
      printf("node-%d[%d : %d]\n", i, dfn[i], group[i]);
    }
    std::cout << "\n-----------------------------\n";
    PrintMatrix(SCCs);
  }
};

int main() {
  std::vector<std::vector<int>> edges = {
      {1, 2}, {2, 3}, {3, 4}, {4, 2}, {4, 5}, {5, 3}, {5, 6},
  };

  SCCBuilder builder;
  builder.SCCScan(7, edges);

  builder.dump();
}

// g++ -std=c++11 strongly-connected-component.cpp -o scc
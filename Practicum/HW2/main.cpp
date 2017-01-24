#include <iostream>
#include <list>
#include <algorithm>

#include "llist.cpp"
#include "graph.cpp"

using namespace std;

// 1-----2
// |     |
// |     |
// 3-----4
//
// 1 - 2
// 2 - 1
// 1 - 3
// 3 - 1
// 2 - 4
// 4 - 2
// 3 - 4
// 4 - 3

graph<int> connectedGraph() {
  graph<int> gr;

  for(int i = 1; i < 5; i++) gr.addTop(i);

  gr.addRib(1, 2);
  gr.addRib(2, 1);

  gr.addRib(1, 3);
  gr.addRib(3, 1);

  gr.addRib(2, 4);
  gr.addRib(4, 2);

  gr.addRib(3, 4);
  gr.addRib(4, 3);

  return gr;
}

// 1     2
// |     |
// |     |
// 3     4
graph<int> disconnectedGraph() {
  graph<int> gr;

  for(int i = 1; i < 5; i++) gr.addTop(i);

  gr.addRib(1, 3);
  gr.addRib(3, 1);

  gr.addRib(2, 4);
  gr.addRib(4, 2);

  return gr;
}

void DFS(graph<int> gr, int inf, list<int> *visited) {
  list<int>::iterator findIter = find(visited->begin(), visited->end(), inf);
  if(findIter != visited->end()) return;

  visited->push_back(inf);

  for(elem_link1<int> *vertex = gr.point(inf)->link; vertex; vertex = vertex->link) DFS(gr, vertex->inf, visited);
}

bool isGraphConnected(graph<int> gr) {
  LList<int> vertexes = gr.vertexes();
  list<int> visited;

  vertexes.IterStart();
  elem_link1<int> *vertex = vertexes.Iter();
  DFS(gr, vertex->inf, &visited);

  return visited.size() == vertexes.len();
}

void findComponents(graph<int> gr) {
  LList<int> vertexes = gr.vertexes();
  list<int> visited;

  vertexes.IterStart();

  int components = 1;
  for(elem_link1<int> *vertex = vertexes.Iter(); vertex; vertex = vertexes.Iter()) {
    list<int>::iterator findIter = find(visited.begin(), visited.end(), vertex->inf);

    if(findIter == visited.end()) {
      cout<<"Component "<<components<<": Vertexes ";

      list<int> vertex_visited;
      DFS(gr, vertex->inf, &vertex_visited);

      for(list<int>::const_iterator iterator = vertex_visited.begin(), end = vertex_visited.end(); iterator != end; ++iterator) {
        cout<<*iterator<<", ";
        visited.push_back(*iterator);
      }

      cout<<endl;
      components++;
    }
  }
}

void spanningDFS(graph<int> *gr, int inf, list<int> *visited, list<pair<int, int> > *vertex_pairs) {
  list<int>::iterator findIter = find(visited->begin(), visited->end(), inf);
  if(findIter != visited->end()) return;

  visited->push_back(inf);

  for(elem_link1<int> *vertex = gr->point(inf)->link; vertex; vertex = vertex->link) {
    spanningDFS(gr, vertex->inf, visited, vertex_pairs);
    vertex_pairs->push_back(make_pair(inf, vertex->inf));
  }
}

void spanningTree(graph<int> &gr) {
  if(!isGraphConnected(gr)) return;

  LList<int> vertexes = gr.vertexes();
  list<int> visited;
  list<pair<int, int> > vertex_pairs;

  vertexes.IterStart();
  elem_link1<int> *first_vertex = vertexes.Iter();

  spanningDFS(&gr, first_vertex->inf, &visited, &vertex_pairs);

  for(list<pair<int, int> >::const_iterator iterator = vertex_pairs.begin(), end = vertex_pairs.end(); iterator != end; ++iterator) {
    gr.deleteRib(iterator->first, iterator->second);
  }
}

int main() {
  graph<int> connGraph = connectedGraph();
  graph<int> disconGraph = disconnectedGraph();

  cout<<"Connected graph: "<<isGraphConnected(connGraph)<<endl;
  cout<<"Disconnected graph: "<<isGraphConnected(disconGraph)<<endl;

  cout<<"findComponents g1:"<<endl;
  findComponents(connGraph);

  cout<<"findComponents g2:"<<endl;
  findComponents(disconGraph);

  // After we remove the ribs of the spanning tree, we expect the connected
  // graph to become disconnected.
  spanningTree(connGraph);
  cout<<"Connected graph becomes disconnected: "<<isGraphConnected(connGraph)<<endl;

  return 0;
}

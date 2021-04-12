#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <string>

#include "Graph.h"
#include "Grid.h"


int main() {
	Graph G1(5);
	G1.setName(0, "A"); //index for nodes starts from 0
	G1.setName(1, "B"); //setting names/labels for each node - not necessary
	G1.setName(2, "C");
	G1.setName(3, "D");
	G1.setName(4, "E");
	G1.setEdge("A", "B");
	G1.setEdge("B", "C");
	G1.setEdge("C", "D");
	G1.setEdge("D", "E");
	G1.setEdge("C", "E");
	G1.setEdge("B", "E");
	G1.printPath(G1.DepthFirstSearch("A", "E"));
	auto it = G1.BreadthFirstSearch("A", "D");
	G1.printPath(it);
	if (G1.isConnected()) {
		std::cout << "G1 is connected" << std::endl;
	}
	G1.addNode("F");
	if (G1.isConnected()) {
		std::cout << "G1 is connected again" << std::endl;
	}

	Graph G11(2), G12(2);
	G11.setName(0, "A1");
	G11.setName(1, "B1");
	G12.setName(0, "A2");
	G12.setName(1, "B2");
	G11.setEdge("A1", "B1");
	G12.setEdge("A2", "B2");
	Graph G13 = G11 + G12;
	G13.setEdge("B1", "A2");
	G13.save("test"); //saving graph to file called "test"
	Graph G14(8);
	G14.load("test"); //loading the graph
	G14.printPath(G14.DepthFirstSearch("A1", "B2"));

	Graph G2(5);
	G2.setName(0, "AF");
	G2.setName(1, "AB");
	G2.setName(2, "CO");
	G2.setName(3, "D");
	G2.setName(4, "CA");
	G2.printNames(); //prints the names, sorts them first
	
	DiGraph DG2(5);
	DG2.setName(0, "A");
	DG2.setName(1, "B");
	DG2.setName(2, "C");
	DG2.setName(3, "D");
	DG2.setName(4, "E");
	DG2.setEdge(0, 1);
	DG2.setEdge(1, 3);
	DG2.setEdge(2, 1);
	DG2.setEdge(3, 2);
	DG2.setEdge(2, 4);
	DG2.printPath(DG2.BreadthFirstSearch(0, 4));

	DiGraph DG3(5);
	DG3.setName(0, "A");
	DG3.setName(1, "B");
	DG3.setName(2, "C");
	DG3.setName(3, "D");
	DG3.setName(4, "E");
	DG3.setEdge(0, 1); //can use index instead of names
	DG3.setEdge(1, 3);
	DG3.setEdge(2, 1);
	DG3.setEdge(3, 2);
	DG3.setEdge(2, 4);
	DG3.setEdge(3, 4);
	DG3.printPath(DG3.BreadthFirstSearch(0, 4));
	DG3.showNeighboursByName();
	DG3.removeEdge("D", "E");
	DG3.printPath(DG3.BreadthFirstSearch(0, 4));
	DG3.addNode("F");
	DG3.setEdge("C", "F");
	DG3.printPath(DG3.BreadthFirstSearch("A", "F"));
	DG3.removeNode("C");
	DG3.setEdge("D", "F");
	DG3.printPath(DG3.BreadthFirstSearch("A", "F"));
	if (DG3.Bipartite("A")) {
		std::cout << "G3 is bipartite" << std::endl;
	}

	std::cout << "Number of graphs: " << Graph::returnGraphCount() << std::endl;

	DiGraph DG4 = DG3 + G14;
	std::cout << Graph::returnGraphCount() << std::endl;
	DG4 = DG3 + G14;
	std::cout << Graph::returnGraphCount() << std::endl;

	Graph G3(1);
	Graph G4 = G2 + G3;
	std::cout << Graph::returnGraphCount() << std::endl;
	DG2 = DG2 + G4;
	std::cout << Graph::returnGraphCount() << std::endl; //should print 15
	DG2.printPath(DG2.Dijkstra("A", "E"));

	DiGraph DG5(6);
	DG5.setName(0, "A");
	DG5.setName(1, "B");
	DG5.setName(2, "C");
	DG5.setName(3, "D");
	DG5.setName(4, "E");
	DG5.setEdge("A", "B");
	DG5.setEdge("B", "C");
	DG5.setEdge("C", "D");
	DG5.setEdge("A", "D", 10);
	DG5.save("ftest");
	DG5.load("ftest");
	DG5.printPath(DG5.Dijkstra("A", "D"));
	DG5.printFloyd(DG5.FloydWarshall());

	DiGraph DG6(4);
	DG6.setName(0, "A");
	DG6.setName(1, "B");
	DG6.setName(2, "C");
	DG6.setName(3, "D");
	DG6.setEdge("A", "B", 5);
	DG6.setEdge("B", "C", 3);
	DG6.setEdge("C", "D", 1);
	DG6.setEdge("A", "D", 10);
	DG6.printPath(DG6.Dijkstra("A", "D"));
	DG6.printFloyd(DG6.FloydWarshall());
	
	Grid Grid1(5, 5);
	Grid1.setStartNode(1, 2);
	Grid1.setEndNode(5, 4);
	Grid1.blockNode(2, 2);
	Grid1.blockNode(2, 3);
	Grid1.blockNode(2, 4);
	Grid1.blockNode(2, 5);
	Grid1.blockNode(3, 2);
	Grid1.blockNode(4, 2);
	Grid1.blockNode(4, 4);
	Grid1.setEndNode(4, 5);
	Grid1.DepthFirstSearch();
	Grid1.drawGraph();

	DiGraph DGFord(5);
	DGFord.setName(0, "A");
	DGFord.setName(1, "B");
	DGFord.setName(2, "C");
	DGFord.setName(3, "D");
	DGFord.setName(4, "E");
	DGFord.setEdge("A", "B", -1);
	DGFord.setEdge("A", "C", 4);
	DGFord.setEdge("B", "C", 3);
	DGFord.setEdge("B", "D", 2);
	DGFord.setEdge("B", "E", 2);
	DGFord.setEdge("D", "B", 1);
	DGFord.setEdge("D", "C", 5);
	DGFord.setEdge("E", "D", -3);
	DGFord.printBellmanFord(DGFord.BellmanFord("A"));
	return 0;
}
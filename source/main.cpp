#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <string>

#include "Graph.h"
#include "Grid.h"
#include "GraphInteractive.h"


int main() {
	//interactiveMode();  interactive mode - has everything except the grid graph

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

	G1.setStartNode("A");
	G1.setEndNode("E"); //"goal" node

	G1.DepthFirstSearch();
	G1.drawGraph();

	G1.removeEndNode("E");
	G1.setEndNode("D");

	G1.BreadthFirstSearch();
	G1.drawGraph();


	Graph G11(2), G12(2);
	G11.setName(1, "A1");
	G11.setName(2, "B1");
	G12.setName(1, "A2");
	G12.setName(2, "B2");
	G11.setEdge("A1", "B1");
	G12.setEdge("A2", "B2");


	Graph G13 = G11 + G12;
	G13.setEdge("B1", "A2");
	G13.setStartNode("A1");
	G13.setEndNode("B2");
	G13.DepthFirstSearch();
	G13.drawGraph();


	DiGraph DG11(2), DG12(2);
	DG11.setName(0, "A1");
	DG11.setName(1, "B1");
	DG12.setName(0, "A2");
	DG12.setName(1, "B2");
	DG11.setEdge(0, 1);
	DG12.setEdge(0, 1);

	DiGraph DG13 = DG11 + DG12;

	DG13.setEdge(1, 2);
	DG13.setStartNode(0);
	DG13.setEndNode(3);
	DG13.BreadthFirstSearch();
	DG13.drawGraph();


	Graph G7(5);
	Graph G8(1);
	DiGraph G9(1);
	G7.setName(0, "AF");
	G7.setName(1, "AB");
	G7.setName(2, "CO");
	G7.setName(3, "D");
	G7.setName(4, "CA");
	G7.printNames(); //prints the names, sorts them first
	std::cout << "Number of graphs: " << G1.returnGraphCount() << std::endl;
	G13.save("test"); //saving graph to file called "test"

	DiGraph G14(8);
	G14.load("test"); //loading the graph
	G14.BreadthFirstSearch();
	G14.drawGraph();


	DiGraph G2(5);
	G2.setName(0, "A");
	G2.setName(1, "B");
	G2.setName(2, "C");
	G2.setName(3, "D");
	G2.setName(4, "E");
	G2.setEdge(0, 1);
	G2.setEdge(1, 3);
	G2.setEdge(2, 1);
	G2.setEdge(3, 2);
	G2.setEdge(2, 4);
	G2.setStartNode(0);
	G2.setEndNode(4);
	G2.BreadthFirstSearch();
	G2.drawGraph();

	//std::cout << "1" << std::endl;
	DiGraph G3(5);
	G3.setName(0, "A");
	G3.setName(1, "B");
	G3.setName(2, "C");
	G3.setName(3, "D");
	G3.setName(4, "E");
	G3.setEdge(0, 1); //can use index instead of names
	G3.setEdge(1, 3);
	G3.setEdge(2, 1);
	G3.setEdge(3, 2);
	G3.setEdge(2, 4);
	G3.setEdge(3, 4);
	G3.setStartNode(0);
	G3.setEndNode(4);
	G3.BreadthFirstSearch();
	G3.drawGraph();

	G3.showNeighboursByName();
	//std::cout << "2" << std::endl;
	G3.removeEdge("D", "E");
	G3.BreadthFirstSearch();
	G3.drawGraph();
	//std::cout << "3" << std::endl;
	G3.addNode("F");
	G3.removeEndNode("E");
	G3.setEndNode("F");
	G3.setEdge("C", "F");
	G3.BreadthFirstSearch();
	G3.drawGraph();
	//std::cout << "4" << std::endl;
	G3.removeNode("C");
	G3.BreadthFirstSearch();
	G3.drawGraph();
	G3.setEdge("D", "F");
	G3.BreadthFirstSearch();
	G3.drawGraph();
	//std::cout << "5" << std::endl;
	G3.BreadthFirstSearch();
	G3.drawGraph();
	//std::cout << "6" << std::endl;
	//G3.drawGraph();
	std::cout << "Number of graphs: " << G1.returnGraphCount() << std::endl;

	if (G3.Bipartite()) {
		std::cout << "G3 is bipartite" << std::endl;
	}

	DiGraph G20 = G2 + G14;
	std::cout << Graph::returnGraphCount() << std::endl;
	G20 = G2 + G14;
	std::cout << Graph::returnGraphCount() << std::endl;

	Graph G30 = G7 + G8;
	std::cout << Graph::returnGraphCount() << std::endl;
	G2 = G2 + G30;
	std::cout << Graph::returnGraphCount() << std::endl; //should print 15


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

	Grid Grid2(Grid1);
	Grid2.drawGraph();

	Graph G02(3);
	G02.setName(0, "A");
	G02.setName(1, "B");
	G02.setName(2, "C");
	G02.setEdge("A", "B");
	G02.setEdge("B", "C");
	G02.setStartNode("A");
	//G02.setEndNode("C");
	G02.BreadthFirstSearch();
	G02.drawGraph(); //fails to draw because endnode not set
	return 0;
}
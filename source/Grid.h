#pragma once
#include "GridNode.h"

class Grid {
	std::vector<GridNode> nodeArr;
	int row;
	int column;
	int startNodeID;
	int endNodeID;
public:
	Grid(const int, const int);
	Grid(const Grid&);
	void setStartNode(int, int);
	void setEndNode(int, int);
	void blockNode(int, int);
	void removeStartNode();
	void removeEndNode(int, int);
	void removeBlock(int, int);
	void BreadthFirstSearch();
	void DepthFirstSearch();
	void searchCleanup();
	void drawGraph();
	void printGraph();
};
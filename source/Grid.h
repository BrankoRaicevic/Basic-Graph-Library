#pragma once
#include "GridNode.h"

class Grid {
	std::vector<GridNode> nodeArr;
	int row;
	int column;
	int startNodeID;
	int endNodeID;
	int totalNodes;
	bool searched;
public:
	Grid(const int, const int);
	Grid(const Grid&);
	int setStartNode(int, int);
	int setEndNode(int, int);
	int blockNode(int, int);
	int removeStartNode(int, int);
	int removeEndNode(int, int);
	int removeBlock(int, int);
	int BreadthFirstSearch();
	int DepthFirstSearch();
	void searchCleanup();

	void drawGraph();
	void printGraph();
};
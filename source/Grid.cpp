#include <iostream>
#include <queue>
#include <stack>

#include "Grid.h"

Grid::Grid(const int _row, const int _column) {
	if (_row < 1 || _column < 1) {
		throw std::invalid_argument("Parametars must be positive numbers");
	}
	searched = false;
	startNodeID = -1;
	endNodeID = -1;
	row = _row;
	column = _column;
	totalNodes = _row * _column - 1;
	for (int i = 0; i <= totalNodes; i++) {
		nodeArr.push_back(GridNode(i + 1));
	}
	for (int i = 0; i <= totalNodes; i++) {
		nodeArr[i].findNeighbours(column, row);
	}
}
Grid::Grid(const Grid& g) {
	searched = g.searched;
	startNodeID = g.startNodeID;
	endNodeID = g.endNodeID;
	row = g.row;
	column = g.column;
	totalNodes = g.totalNodes;
	nodeArr = g.nodeArr;
}
int Grid::setStartNode(int _row, int _column) {
	if (_row <= row && _column <= column && _row > 0 && _column > 0) {
		_row--;
		_column--;
		startNodeID = _row * column + _column;
		nodeArr[startNodeID].setBeginNode();
		return 0;
	}
	return 1;
}
int Grid::setEndNode(int _row, int _column) {
	if (_row <= row && _column <= column && _row > 0 && _column > 0) {
		_row--;
		_column--;
		endNodeID = _row * column + _column;
		nodeArr[endNodeID].setEndNode();
		return 0;
	}
	return 1;
}

int Grid::blockNode(int _row, int _column) {
	if (_row <= row && _column <= column && _row > 0 && _column > 0) {
		_row--;
		_column--;
		nodeArr[_row * column + _column].setBlocked();
		return 0;
	}
	return 1;
}

int Grid::removeStartNode(int _row, int _column) {
	if (_row <= row && _column <= column && _row > 0 && _column > 0) {
		_row--;
		_column--;
		nodeArr[_row * column + _column].removeBeginNode();
		startNodeID = -1;
		return 0;
	}
	return 1;
}

int Grid::removeEndNode(int _row, int _column) {
	if (_row <= row && _column <= column && _row > 0 && _column > 0) {
		_row--;
		_column--;
		nodeArr[_row * column + _column].removeEndNode();
		endNodeID = -1;
		return 0;
	}
	return 1;
}

int Grid::removeBlock(int _row, int _column) {
	if (_row <= row && _column <= column && _row > 0 && _column > 0) {
		_row--;
		_column--;
		nodeArr[_row * column + _column].removeBlocked();
		return 0;
	}
	return 1;
}

int Grid::BreadthFirstSearch() {
	std::queue<int> frontier;
	int activeNode;
	bool end = false;
	if (searched == true) {
		searchCleanup();
		searched = false;
	}
	frontier.push(startNodeID);
	while (1) {
		if (frontier.empty()) {
			std::cout << "Impossible to reach end node" << std::endl;
			searched = true;
			return 1;
		}
		activeNode = frontier.front();
		frontier.pop();
		for (std::size_t i = 0; i < nodeArr[activeNode].countNeighbours(); i++) {
			int retID = nodeArr[activeNode].returnNeighbour(i) - 1;
			if (nodeArr[retID].isFree() && !nodeArr[retID].isVisited()) {
				nodeArr[retID].setPrevious(activeNode);
				nodeArr[retID].setVisited();
				if (nodeArr[retID].isEndNode()) {
					endNodeID = retID;
					end = true;
					break;
				}
				frontier.push(retID);
			}
		}
		if (end == true) {
			break;
		}
	}
	searched = true;
	return 0;
}

int Grid::DepthFirstSearch() {
	std::stack<int> frontier;
	int activeNode;
	bool end = false;
	if (searched == true) {
		searchCleanup();
		searched = false;
	}
	frontier.push(startNodeID);
	while (1) {
		if (frontier.empty()) {
			std::cout << "Impossible to reach end node" << std::endl;
			searched = true;
			return 1;
		}
		activeNode = frontier.top();
		frontier.pop();
		for (std::size_t i = 0; i < nodeArr[activeNode].countNeighbours(); i++) {
			int retID = nodeArr[activeNode].returnNeighbour(i) - 1;
			if (nodeArr[retID].isFree() && !nodeArr[retID].isVisited()) {
				nodeArr[retID].setPrevious(activeNode);
				nodeArr[retID].setVisited();
				if (nodeArr[retID].isEndNode()) {
					endNodeID = retID;
					end = true;
					break;
				}
				frontier.push(retID);
			}
		}
		if (end == true) {
			break;
		}
	}
	searched = true;
	return 0;
}

void Grid::searchCleanup() {
	for (int i = 0; i <= totalNodes; i++) {
		nodeArr[i].clear();
	}
}

void Grid::drawGraph() {
	std::vector<int> path;
	int currentNode = endNodeID;
	if (!nodeArr[endNodeID].isVisited()) {
		return;
	}
	while (1) {
		if (nodeArr[currentNode].isBeginNode()) {
			break;
		}
		path.push_back(currentNode);
		currentNode = nodeArr[currentNode].getPrevious() - 1;
	}
	for (auto i = path.size(); i > 0; i--) {
		printGraph();
		nodeArr[path[i - 1]].setSign('*');
	}
	for (auto i = path.size(); i > 0; i--) {
		nodeArr[path[i - 1]].setSign('.');
		if (i == 1) {
			nodeArr[path[i - 1]].setSign('E');
		}
	}
}
void Grid::printGraph() {
	int count = column;
	for (int i = 0; i <= totalNodes; i++) {
		std::cout << nodeArr[i].getSign();
		count--;
		if (count == 0) {
			std::cout << '\n';
			count = column;
		}
	}
	std::cout << std::endl;
}
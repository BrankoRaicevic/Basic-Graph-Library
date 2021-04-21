#include <iostream>
#include <queue>
#include <stack>

#include "Grid.h"

Grid::Grid(const int _row, const int _column) {
	if (_row < 1 || _column < 1) {
		throw std::invalid_argument("Parametars must be positive numbers");
	}
	startNodeID = -1;
	endNodeID = -1;
	row = _row;
	column = _column;
	for (int i = 0; i < (_row * _column); i++) {
		nodeArr.push_back(GridNode());
	}
	for (std::size_t i = 0; i < nodeArr.size(); i++) {
		nodeArr[i].findNeighbours(i + 1, column, row);
	}
}
Grid::Grid(const Grid& g) {
	startNodeID = g.startNodeID;
	endNodeID = g.endNodeID;
	row = g.row;
	column = g.column;
	nodeArr = g.nodeArr;
}
void Grid::setStartNode(int _row, int _column) {
	if (_row <= row && _column <= column && _row > 0 && _column > 0) {
		_row--;
		_column--;
		if (startNodeID >= 0) {
			nodeArr[startNodeID].removeBeginNode();
		}
		startNodeID = _row * column + _column;
		nodeArr[startNodeID].setBeginNode();
		return;
	}
	throw std::out_of_range("Index out of scope");
}
void Grid::setEndNode(int _row, int _column) {
	if (_row <= row && _column <= column && _row > 0 && _column > 0) {
		_row--;
		_column--;
		nodeArr[_row * column + _column].setEndNode();
		return;
	}
	throw std::out_of_range("Index out of scope");
}

void Grid::blockNode(int _row, int _column) {
	if (_row <= row && _column <= column && _row > 0 && _column > 0) {
		_row--;
		_column--;
		nodeArr[_row * column + _column].setBlocked();
		return;
	}
	throw std::out_of_range("Index out of scope");
}

void Grid::removeStartNode() {
	if (startNodeID >= 0) {
		nodeArr[startNodeID].removeBeginNode();
		startNodeID = -1;
	}
}

void Grid::removeEndNode(int _row, int _column) {
	if (_row <= row && _column <= column && _row > 0 && _column > 0) {
		_row--;
		_column--;
		nodeArr[_row * column + _column].removeEndNode();
		return;
	}
	throw std::out_of_range("Index out of scope");
}

void Grid::removeBlock(int _row, int _column) {
	if (_row <= row && _column <= column && _row > 0 && _column > 0) {
		_row--;
		_column--;
		nodeArr[_row * column + _column].removeBlocked();
		return;
	}
	throw std::out_of_range("Index out of scope");
}

void Grid::BreadthFirstSearch() {
	std::queue<int> frontier;
	int activeNode;
	bool end = false;
	if (startNodeID == -1) {
		throw std::logic_error("start node not set");
	}
	searchCleanup();
	bool* visited = new bool[nodeArr.size() + 1];
	for (std::size_t i = 0; i < nodeArr.size(); i++) {
		visited[i] = false;
	}
	frontier.push(startNodeID);
	while (!frontier.empty()) {
		activeNode = frontier.front();
		frontier.pop();
		for (std::size_t i = 0; i < nodeArr[activeNode].countNeighbours(); i++) {
			int retID = nodeArr[activeNode].returnNeighbour(i) - 1;
			if (nodeArr[retID].isFree() && visited[retID] == false) {
				nodeArr[retID].setPrevious(activeNode);
				visited[retID] = true;
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
	delete[] visited;
}

void Grid::DepthFirstSearch() {
	std::stack<int> frontier;
	int activeNode;
	bool end = false;
	if (startNodeID == -1) {
		throw std::logic_error("start node not set");
	}
	searchCleanup();
	bool* visited = new bool[nodeArr.size() + 1];
	for (std::size_t i = 0; i < nodeArr.size(); i++) {
		visited[i] = false;
	}
	frontier.push(startNodeID);
	while (!frontier.empty()) {
		activeNode = frontier.top();
		frontier.pop();
		for (std::size_t i = 0; i < nodeArr[activeNode].countNeighbours(); i++) {
			int retID = nodeArr[activeNode].returnNeighbour(i) - 1;
			if (nodeArr[retID].isFree() && visited[retID] == false) {
				nodeArr[retID].setPrevious(activeNode);
				visited[retID] = true;
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
	delete[] visited;
}

void Grid::searchCleanup() {
	for (std::size_t i = 0; i < nodeArr.size(); i++) {
		nodeArr[i].clear();
	}
}

void Grid::drawGraph() {
	std::vector<int> path;
	int currentNode = endNodeID;
	if ((startNodeID == -1) || (endNodeID == -1)) {
		throw std::logic_error("start or end node not set");
	}
	if (nodeArr[endNodeID].getPrevious() == -1) {
		throw std::logic_error("end node not visited/unreachable");
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
	for (std::size_t i = 0; i < nodeArr.size(); i++) {
		std::cout << nodeArr[i].getSign();
		count--;
		if (count == 0) {
			std::cout << '\n';
			count = column;
		}
	}
	std::cout << std::endl;
}
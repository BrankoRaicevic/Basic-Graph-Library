#pragma once
#include <string>
#include <vector>

class Node {
	int id;
	int previous;
	bool blocked;
	bool beginNode;
	bool endNode;
	bool visited;
	std::string name;
	std::vector<int> outEdge;
	std::vector<int> outWeight;
	friend class Graph;
	friend class DiGraph;
public:
	Node(int = 0);
	void setPrevious(const int);
	int getPrevious() const;
	void setVisited();
	bool isBeginNode() const;
	bool isEndNode() const;
	bool isFree() const;
	bool isVisited() const;
	void clear();
	void showNeighbours();
	std::size_t countNeighbours() const;
	int returnNeighbour(const std::size_t) const;
	int returnNeighbourWeight(const std::size_t) const;
	bool addEdge(int, int = 1);
	void updateNeighbours(const int);
	void nodeRemovalUpdate(const int);
	bool removeEdge(const int);
};
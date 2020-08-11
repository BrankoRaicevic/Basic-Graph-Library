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
	friend class Graph;
	friend class DiGraph;
public:
	Node();
	Node(int);
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
	int addEdge(int);
	void updateNeighbours(const int);
	void nodeRemovalUpdate(const int);
	int removeEdge(const int);
};
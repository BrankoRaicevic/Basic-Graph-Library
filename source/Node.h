#pragma once
#include <string>
#include <vector>

class Node {
	bool blocked;
	std::string name;
	std::vector<int> outEdge;
	std::vector<int> outWeight;
	friend class Graph;
	friend class DiGraph;
public:
	Node();
	bool isFree() const;
	void showNeighbours();
	std::size_t countNeighbours() const;
	int returnNeighbour(const std::size_t) const;
	int returnNeighbourWeight(const std::size_t) const;
	bool addEdge(int, int = 1);
	void updateNeighbours(const int);
	void nodeRemovalUpdate(const int);
	bool removeEdge(const int);
};
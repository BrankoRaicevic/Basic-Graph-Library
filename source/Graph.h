#pragma once
#include "Node.h"
#include <map>
#include <stack>

class Graph {
	std::vector<Node> nodeArr;
	std::map<std::string, int> aliasResolve;
	friend class DiGraph;
	static unsigned int graphCount;
public:
	Graph(const int = 0);
	Graph(const Graph&);
	virtual ~Graph();
	void blockNode(const int);
	void removeBlock(const int);
	virtual bool setEdge(const int, const int, const int = 1);
	virtual bool removeEdge(const int, const int);
	void blockNode(const std::string&);
	void removeBlock(const std::string&);
	virtual bool setEdge(const std::string&, const std::string&, const int = 1);
	virtual bool removeEdge(const std::string&, const std::string&);
	void addNode();
	bool addNode(const std::string&);
	void removeNode(const int);
	void removeNode(const std::string&);
	bool setName(const int, const std::string&);
	std::string getName(const int) const;
	void save(const std::string&);
	void load(const std::string&);
	virtual bool isConnected() final;
	std::vector<std::vector<int>> weightMatrix();
	std::vector<int> BreadthFirstSearch(int, int);
	std::vector<int> DepthFirstSearch(int, int);
	std::vector<int> Dijkstra(int, int);
	std::vector<int> BreadthFirstSearch(const std::string&, const std::string&);
	std::vector<int> DepthFirstSearch(const std::string&, const std::string&);
	std::vector<int> Dijkstra(const std::string&, const std::string&);
	bool Bipartite(const int);
	bool Bipartite(const std::string&);
	std::vector<int> drawPath(std::vector<int>&, const int, const int);
	void printPath(std::vector<int>&);
	void printPath(std::vector<int>&&);
	void showNeighbours();
	void showNeighboursByName();
	void printNames();
	void nameResolve();
	static unsigned int returnGraphCount();
	int getNodeCount() const;
	virtual int graphType() const;
	Graph operator+(Graph x);
};

class DiGraph : public Graph {
public:
	DiGraph(const int);
	DiGraph(const Graph&);
	DiGraph(const DiGraph&);
	bool setEdge(const int, const int, const int = 1);
	bool setEdge(const std::string&, const std::string&, const int = 1);
	bool removeEdge(const int, const int);
	bool removeEdge(const std::string&, const std::string&);
	std::vector<std::vector<int>> FloydWarshall();
	void printFloyd(const std::vector<std::vector<int>>&);
	std::vector<int> BellmanFord(const int);
	std::vector<int> BellmanFord(const std::string&);
	void printBellmanFord(const std::vector<int>&);
	std::vector<int> TopologicalSort();
	void TopologicalSortHelper(int, std::stack<int>&, bool*);
	void printTopologicalSort(const std::vector<int>&);
	int graphType() const;
};
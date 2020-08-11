#pragma once
#include <map>
#include "Node.h"

class Graph {
	std::vector<Node> nodeArr;
	std::map<std::string, int> aliasResolve;
	int startNodeID;
	int endNodeID;
	bool searched;
	friend class DiGraph;
	static unsigned int graphCount;
protected:
	int totalNodes;
public:
	Graph();
	Graph(const int);
	Graph(const Graph&);
	virtual ~Graph();
	int setStartNode(const int);
	int setEndNode(const int);
	int blockNode(const int);
	int removeStartNode(const int);
	int removeEndNode(const int);
	int removeBlock(const int);
	virtual int setEdge(const int, const  int);
	virtual int removeEdge(const int, const  int);
	int setStartNode(const std::string&);
	int setEndNode(const std::string&);
	int blockNode(const std::string&);
	int removeStartNode(const std::string&);
	int removeEndNode(const std::string&);
	int removeBlock(const std::string&);
	virtual int setEdge(const std::string&, const std::string&);
	virtual int removeEdge(const std::string&, const std::string&);
	int addNode();
	int addNode(const std::string&);
	int removeNode(const int);
	int removeNode(const std::string&);
	int BreadthFirstSearch();
	int DepthFirstSearch();
	bool Bipartite();
	int setName(const int, const std::string&);
	std::string getName(const int) const;
	int drawGraph();
	void printGraph(std::vector<int>&);
	int getNodeCount() const;
	void showNeighbours();
	void showNeighboursByName();
	void searchCleanup();
	int save(const std::string&);
	int load(const std::string&);
	void printNames();
	static unsigned int returnGraphCount();
	void nameResolve();
	virtual int graphType() const;
	Graph operator+(Graph x);
};

class DiGraph : public Graph {
public:
	DiGraph(const int);
	DiGraph(const Graph&);
	int setEdge(const int, const int);
	int setEdge(const std::string&, const std::string&);
	int removeEdge(const int, const int);
	int removeEdge(const std::string&, const std::string&);
	int graphType() const;
};
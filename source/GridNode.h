#pragma once
#include <vector>

class GridNode {
	int previous;
	bool visited;
	bool blocked;
	bool beginNode;
	bool endNode;
	char sign;
	std::vector<int> outEdge;
public:
	GridNode();
	void setBeginNode();
	void setEndNode();
	void setBlocked();
	void removeBeginNode();
	void removeEndNode();
	void removeBlocked();
	void setSign(const char recvSign);
	char getSign() const;
	void setPrevious(const int recvID);
	int getPrevious() const;
	void setVisited();
	bool isBeginNode() const;
	bool isEndNode() const;
	bool isFree() const;
	bool isVisited() const;
	void clear();
	void findNeighbours(const int id, const int ver, const int hor);
	void showNeighbours();
	std::size_t countNeighbours() const;
	int returnNeighbour(const std::size_t n) const;
};
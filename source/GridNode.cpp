#include <iostream>
#include "GridNode.h"

GridNode::GridNode() {
	visited = false;
	id = 0;
	previous = 0;
	blocked = false;
	beginNode = false;
	endNode = false;
	sign = '.';
}
GridNode::GridNode(const int _id) {
	visited = false;
	id = _id;
	previous = 0;
	blocked = false;
	beginNode = false;
	endNode = false;
	sign = '.';
}
void GridNode::setBeginNode() {
	beginNode = true;
	sign = 'B';
}
void GridNode::setEndNode() {
	endNode = true;
	sign = 'E';
}
void GridNode::setBlocked() {
	blocked = true;
	sign = 'X';
}
void GridNode::removeBeginNode() {
	beginNode = false;
	sign = '.';
}
void GridNode::removeEndNode() {
	endNode = false;
	sign = '.';
}
void GridNode::removeBlocked() {
	blocked = false;
	sign = '.';
}
void GridNode::setSign(const char recvSign) {
	sign = recvSign;
}
char GridNode::getSign() const {
	return sign;
}
void GridNode::setId(const int recvId) {
	id = recvId;
}
int GridNode::getId() const {
	return id;
}
void GridNode::setPrevious(const int recvID) {
	previous = recvID + 1;
}
int GridNode::getPrevious() const {
	return previous;
}
void GridNode::setVisited() {
	visited = true;
}
bool GridNode::isBeginNode() const {
	return beginNode == true ? true : false;
}
bool GridNode::isEndNode() const {
	return endNode == true ? true : false;
}
bool GridNode::isFree() const {
	return blocked == true ? false : true;
}
bool GridNode::isVisited() const {
	return visited == true ? true : false;
}
void GridNode::clear() {
	visited = false;
	previous = 0;
}
void GridNode::findNeighbours(const int ver, const int hor) {
	//Left
	if ((id - 1) != 0 && (id - 1) % ver != 0) {
		outEdge.push_back(id - 1);
	}
	//Right
	if (id % ver != 0) {
		outEdge.push_back(id + 1);
	}
	//Top
	if (id > ver) {
		outEdge.push_back(id - ver);
	}
	//Bottom
	if (id <= (hor * ver) - ver) {
		outEdge.push_back(id + ver);
	}
}
void GridNode::showNeighbours() {
	for (std::vector<int>::iterator it = outEdge.begin(); it != outEdge.end(); ++it) {
		std::cout << *it << " ";
	}
}


std::size_t GridNode::countNeighbours() const {
	return outEdge.size();
}
int GridNode::returnNeighbour(const std::size_t n) const {
	return outEdge[n];
}
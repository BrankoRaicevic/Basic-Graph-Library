#pragma once
#include <iostream>
#include <vector>
#include "Node.h"

Node::Node(int _id) {
	visited = false;
	id = _id;
	previous = 0;
	blocked = false;
	beginNode = false;
	endNode = false;
	name = "";
}
void Node::setPrevious(const int recvID) {
	previous = recvID;
}
int Node::getPrevious() const {
	return previous;
}
void Node::setVisited() {
	visited = true;
}
bool Node::isBeginNode() const {
	return beginNode;
}
bool Node::isEndNode() const {
	return endNode;
}
bool Node::isFree() const {
	return blocked == true ? false : true;
}
bool Node::isVisited() const {
	return visited;
}
void Node::clear() {
	visited = false;
	previous = 0;
}
void Node::showNeighbours() {
	for (std::vector<int>::iterator it = outEdge.begin(); it != outEdge.end(); ++it) {
		std::cout << *it << " ";
	}
}
std::size_t Node::countNeighbours() const {
	return outEdge.size();
}
int Node::returnNeighbour(const std::size_t n) const {
	return outEdge[n];
}
int Node::returnNeighbourWeight(const std::size_t n) const {
	return outWeight[n];
}
bool Node::addEdge(int a_id, int weigh) {
	for (std::vector<int>::iterator it = outEdge.begin(); it != outEdge.end(); ++it) {
		if (*it == a_id) {
			return false;
		}
	}
	outEdge.push_back(a_id);
	outWeight.push_back(weigh);
	return true;
}
void Node::updateNeighbours(const int offset) {
	for (int& id1 : outEdge) {
		id1 += offset;
	}
}
void Node::nodeRemovalUpdate(const int a_id) {
	for (std::vector<int>::iterator it = outEdge.begin(); it != outEdge.end();) {
		if (*it == a_id) {
			it = outEdge.erase(it);
			outWeight.erase(outWeight.begin() + (it - outEdge.begin()));
		}
		else {
			++it;
		}
	}
	for (int& id1 : outEdge) {
		if (id1 > a_id) {
			id1--;
		}
	}
}
bool Node::removeEdge(const int a_id) {
	for (std::vector<int>::iterator it = outEdge.begin(); it != outEdge.end();) {
		if (*it == a_id) {
			it = outEdge.erase(it);
			outWeight.erase(outWeight.begin() + (it - outEdge.begin()));
			return true;
		}
		else {
			++it;
		}
	}
	return false;
}
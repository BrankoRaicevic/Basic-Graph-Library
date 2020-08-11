#pragma once
#include <iostream>
#include <vector>
#include "Node.h"

Node::Node() {
	visited = false;
	id = 0;
	previous = 0;
	blocked = false;
	beginNode = false;
	endNode = false;
	name = "";
}
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
	return beginNode == true ? true : false;
}
bool Node::isEndNode() const {
	return endNode == true ? true : false;
}
bool Node::isFree() const {
	return blocked == true ? false : true;
}
bool Node::isVisited() const {
	return visited == true ? true : false;
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
int Node::addEdge(int a_id) {
	for (std::vector<int>::iterator it = outEdge.begin(); it != outEdge.end(); ++it) {
		if (*it == a_id) {
			return 1;
		}
	}
	outEdge.push_back(a_id);
	return 0;
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
int Node::removeEdge(const int a_id) {
	for (std::vector<int>::iterator it = outEdge.begin(); it != outEdge.end();) {
		if (*it == a_id) {
			it = outEdge.erase(it);
			return 0;
		}
		else {
			++it;
		}
	}
	return 1;
}
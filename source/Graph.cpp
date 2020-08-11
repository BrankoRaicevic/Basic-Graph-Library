#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <fstream>
#include <algorithm>
#include "Graph.h"

unsigned int Graph::graphCount = 0;

Graph::Graph() {
	searched = false;
	startNodeID = -1;
	endNodeID = -1;
	totalNodes = -1;
	graphCount++;
}
Graph::Graph(const int nodeCount) {
	if (nodeCount < 0) {
		throw std::invalid_argument("Node count cannot be negative");
	}
	searched = false;
	startNodeID = -1;
	endNodeID = -1;
	totalNodes = nodeCount - 1;
	graphCount++;
	for (int i = 0; i <= totalNodes; i++) {
		nodeArr.push_back(Node(i));
	}
}
Graph::Graph(const Graph& g) {
	searched = g.searched;
	startNodeID = g.startNodeID;
	endNodeID = g.endNodeID;
	totalNodes = g.totalNodes;
	nodeArr = g.nodeArr;
}
Graph::~Graph() {
	graphCount--;
}
int Graph::setStartNode(const int _startNode) {
	if ((_startNode >= 0) && (_startNode <= totalNodes)) {
		startNodeID = _startNode;
		nodeArr[startNodeID].beginNode = true;
		return 0;
	}
	return 1;
}
int Graph::setEndNode(const int _endNode) {
	if ((_endNode >= 0) && (_endNode <= totalNodes)) {
		endNodeID = _endNode;
		nodeArr[endNodeID].endNode = true;
		return 0;
	}
	return 1;
}

int Graph::blockNode(const int _blockNode) {
	if ((_blockNode >= 0) && (_blockNode <= totalNodes)) {
		nodeArr[_blockNode].blocked = true;
		return 0;
	}
	return 1;
}

int Graph::removeStartNode(const int _startNode) {
	if ((_startNode >= 0) && (_startNode <= totalNodes)) {
		nodeArr[startNodeID].beginNode = false;
		startNodeID = totalNodes;
		return 0;
	}
	return 1;
}

int Graph::removeEndNode(const int _endNode) {
	if ((_endNode >= 0) && (_endNode <= totalNodes)) {
		nodeArr[endNodeID].endNode = false;
		endNodeID = totalNodes;
		return 0;
	}
	return 1;
}

int Graph::removeBlock(const int _blockNode) {
	if ((_blockNode >= 0) && (_blockNode <= totalNodes)) {
		nodeArr[_blockNode].blocked = false;
		return 0;
	}
	return 1;
}

int Graph::setEdge(const int id1, const int id2) {
	if ((id1 >= 0) && (id1 <= totalNodes) && (id2 >= 0) && (id2 <= totalNodes) && (id2 != id1)) {
		nodeArr[id1].addEdge(id2);
		nodeArr[id2].addEdge(id1);
		return 0;
	}
	return 1;
}

int Graph::removeEdge(const int id1, const int id2) {
	if ((id1 >= 0) && (id1 <= totalNodes) && (id2 >= 0) && (id2 <= totalNodes) && (id2 != id1)) {
		nodeArr[id1].removeEdge(id2);
		nodeArr[id2].removeEdge(id1);
		return 0;
	}
	return 1;
}

int Graph::setStartNode(const std::string& _startNode) {
	std::map<std::string, int>::iterator it = aliasResolve.find(_startNode);
	if (it != aliasResolve.end()) {
		if (startNodeID >= 0) {
			nodeArr[startNodeID].beginNode = false;
		}
		startNodeID = it->second;
		nodeArr[it->second].beginNode = true;
		return 0;
	}
	return 1;
}
int Graph::setEndNode(const std::string& _endNode) {
	std::map<std::string, int>::iterator it = aliasResolve.find(_endNode);
	if (it != aliasResolve.end()) {
		if (endNodeID >= 0) {
			nodeArr[endNodeID].endNode = false;
		}
		endNodeID = it->second;
		nodeArr[it->second].endNode = true;
		return 0;
	}
	return 1;
}

int Graph::blockNode(const std::string& _blockNode) {
	std::map<std::string, int>::iterator it = aliasResolve.find(_blockNode);
	if (it != aliasResolve.end()) {
		nodeArr[it->second].blocked = true;
		return 0;
	}
	return 1;
}

int Graph::removeStartNode(const std::string& _startNode) {
	std::map<std::string, int>::iterator it = aliasResolve.find(_startNode);
	if (it != aliasResolve.end()) {
		nodeArr[it->second].beginNode = false;
		startNodeID = -1;
		return 0;
	}
	return 1;
}

int Graph::removeEndNode(const std::string& _endNode) {
	std::map<std::string, int>::iterator it = aliasResolve.find(_endNode);
	if (it != aliasResolve.end()) {
		nodeArr[it->second].endNode = false;
		endNodeID = -1;
		return 0;
	}
	return 1;
}

int Graph::removeBlock(const std::string& _blockNode) {
	std::map<std::string, int>::iterator it = aliasResolve.find(_blockNode);
	if (it != aliasResolve.end()) {
		nodeArr[it->second].blocked = false;
		return 0;
	}
	return 1;
}

int Graph::setEdge(const std::string& _alias1, const std::string& _alias2) {
	std::map<std::string, int>::iterator it1 = aliasResolve.find(_alias1);
	std::map<std::string, int>::iterator it2 = aliasResolve.find(_alias2);
	int added = 0;
	if ((it1 != aliasResolve.end()) && (it2 != aliasResolve.end())) {
		added += nodeArr[it1->second].addEdge(it2->second);
		added += nodeArr[it2->second].addEdge(it1->second);
		return added == 0 ? 1 : 0;
	}
	return 2;
}

int Graph::removeEdge(const std::string& _alias1, const std::string& _alias2) {
	std::map<std::string, int>::iterator it1 = aliasResolve.find(_alias1);
	std::map<std::string, int>::iterator it2 = aliasResolve.find(_alias2);
	int removed = 0;
	if ((it1 != aliasResolve.end()) && (it2 != aliasResolve.end())) {
		removed += nodeArr[it1->second].removeEdge(it2->second);
		removed += nodeArr[it2->second].removeEdge(it1->second);
		return removed == 0 ? 1 : 0;
	}
	return 2;
}

int Graph::addNode() {
	nodeArr.push_back(Node());
	totalNodes++;
	nodeArr[totalNodes].id = totalNodes;
	return 0;
}

int Graph::addNode(const std::string& _name) {
	std::map<std::string, int>::iterator it = aliasResolve.find(_name);
	if ((it == aliasResolve.end())) {
		addNode();
		return setName(nodeArr[totalNodes].id, _name);
	}
	return 1;
}

int Graph::removeNode(const int id) {
	if ((id >= 0) && (id <= totalNodes)) {
		nodeArr.erase(nodeArr.begin() + id);
		totalNodes--;
		if (id < endNodeID) { endNodeID--; }
		else if (id == endNodeID) { endNodeID = -1; }
		if (id < startNodeID) { startNodeID--; }
		else if (id == startNodeID) { startNodeID = -1; }
		for (int i = id; i <= totalNodes; i++) {
			nodeArr[i].id--;
		}
		for (int i = 0; i <= totalNodes; i++) {
			nodeArr[i].nodeRemovalUpdate(id);
		}
		for (std::map<std::string, int>::iterator it = aliasResolve.begin(); it != aliasResolve.end();) {
			if (it->second == id) {
				it = aliasResolve.erase(it);
				break;
			}
			else {
				++it;
			}
		}
		for (std::map<std::string, int>::iterator it = aliasResolve.begin(); it != aliasResolve.end(); ++it) {
			if (it->second > id) {
				it->second--;
			}
		}
		return 0;
	}
	return 1;
}

int Graph::removeNode(const std::string& _name) {
	std::map<std::string, int>::iterator it = aliasResolve.find(_name);
	if (it != aliasResolve.end()) {
		removeNode(it->second);
		return 0;
	}
	return 1;
}

int Graph::BreadthFirstSearch() {
	std::queue<int> frontier;
	int activeNode;
	bool end = false;
	if (startNodeID == -1) {
		return 2;
	}
	if (searched == true) {
		searchCleanup();
		searched = false;
	}
	frontier.push(startNodeID);
	while (1) {
		if (frontier.empty()) {
			searched = true;
			return 1;
		}
		activeNode = frontier.front();
		frontier.pop();
		for (std::size_t i = 0; i < nodeArr[activeNode].countNeighbours(); i++) {
			int retID = nodeArr[activeNode].returnNeighbour(i);
			if (nodeArr[retID].isFree() && !nodeArr[retID].isVisited()) {
				nodeArr[retID].setPrevious(activeNode);
				nodeArr[retID].setVisited();
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
	searched = true;
	return 0;
}

int Graph::DepthFirstSearch() {
	std::stack<int> frontier;
	int activeNode;
	bool end = false;
	if (startNodeID == -1) {
		return 2;
	}
	if (searched == true) {
		searchCleanup();
		searched = false;
	}
	frontier.push(startNodeID);
	while (1) {
		if (frontier.empty()) {
			searched = true;
			return 1;
		}
		activeNode = frontier.top();
		frontier.pop();
		for (std::size_t i = 0; i < nodeArr[activeNode].countNeighbours(); i++) {
			int retID = nodeArr[activeNode].returnNeighbour(i);
			if (nodeArr[retID].isFree() && !nodeArr[retID].isVisited()) {
				nodeArr[retID].setPrevious(activeNode);
				nodeArr[retID].setVisited();
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
	searched = true;
	return 0;
}

bool Graph::Bipartite() {
	std::queue<int> frontier;
	int activeNode;
	if (startNodeID == -1) {
		return false;
	}
	if (searched == true) {
		searchCleanup();
		searched = false;
	}
	char* color = new char[nodeArr.size() + 1];
	for (std::size_t i = 0; i < nodeArr.size(); i++) {
		color[i] = 0;
	}
	color[startNodeID] = 1;
	frontier.push(startNodeID);
	while (1) {
		if (frontier.empty()) {
			searched = true;
			return true;
		}
		activeNode = frontier.front();
		frontier.pop();
		for (std::size_t i = 0; i < nodeArr[activeNode].countNeighbours(); i++) {
			int retID = nodeArr[activeNode].returnNeighbour(i);
			if ((color[retID] != color[activeNode])) {
				if (color[retID] == 0) {
					color[retID] = color[activeNode] == 1 ? 2 : 1;
				}
				if (!nodeArr[retID].isVisited()) {
					nodeArr[retID].setVisited();
					frontier.push(retID);
				}
			}
			else {
				searched = true;
				return false;
			}
		}
	}
	searched = true;
	return false;
}

int Graph::drawGraph() {
	std::vector<int> path;
	int currentNode = endNodeID;
	if ((startNodeID == -1) || (endNodeID == -1)) {
		return 1;
	}

	if (!nodeArr[currentNode].isVisited()) {
		return 2;
	}

	while (1) {
		if (nodeArr[currentNode].isBeginNode()) {
			path.push_back(currentNode);
			break;
		}
		path.push_back(currentNode);
		currentNode = nodeArr[currentNode].getPrevious();
	}
	printGraph(path);
	return 0;
}
void Graph::printGraph(std::vector<int> &path) {
	for (std::vector<int>::reverse_iterator it = path.rbegin(); it != path.rend(); ++it) {
		if (nodeArr[*it].name != "") {
			std::cout << nodeArr[*it].name;
		}
		else {
			std::cout << *it;
		}
		if ((it + 1) != path.rend()) {
			std::cout << " -> ";
		}
	}
	std::cout << std::endl;
}

void Graph::showNeighbours() {
	for (int i = 0; i <= totalNodes; i++) {
		std::cout << "ID " << i << ": ";
		nodeArr[i].showNeighbours();
		std::cout << std::endl;
	}
}

void Graph::showNeighboursByName() {
	for (int i = 0; i <= totalNodes; i++) {
		if (nodeArr[i].beginNode == true) {
			std::cout << "*" << nodeArr[i].name << " -> ";
		}
		else {
			std::cout << " " << nodeArr[i].name << " -> ";
		}
		for (std::size_t j = 0; j < nodeArr[i].countNeighbours(); j++) {
			int retIndex = nodeArr[i].returnNeighbour(j);
			if (nodeArr[retIndex].name != "") {
				std::cout << nodeArr[retIndex].name << "  ";
			}
			else {
				std::cout << retIndex << "  ";
			}
		}
		std::cout << std::endl;
	}
}

int Graph::setName(const int id, const std::string& _name) {
	if ((id >= 0) && (id <= totalNodes) && (_name != "")) {
		std::map<std::string, int>::iterator it = aliasResolve.find(_name);
		if (it != aliasResolve.end()) {
			return 1;
		}
		aliasResolve.try_emplace(_name, id);
		nodeArr[id].name = _name;
		return 0;
	}
	return 1;
}

std::string Graph::getName(const int id) const {
	return nodeArr[id].name;
}

int Graph::getNodeCount() const {
	return totalNodes + 1;
}

void Graph::searchCleanup() {
	for (int i = 0; i <= totalNodes; i++) {
		nodeArr[i].clear();
	}
}

int Graph::save(const std::string& graphName) {
	std::fstream fp((graphName + ".bin").c_str(), std::ios::out | std::ios::binary);
	if (!fp) {
		std::cout << "Failed to open file" << std::endl;
		return 1;
	}
	char byte = 1;
	auto arrSize = nodeArr.size();
	fp.write((char*)&byte, sizeof(byte));
	fp.write((char*)&startNodeID, sizeof(startNodeID));
	fp.write((char*)&endNodeID, sizeof(endNodeID));
	fp.write((char*)&totalNodes, sizeof(totalNodes));
	fp.write((char*)&arrSize, sizeof(int));
	for (std::size_t i = 0; i < nodeArr.size(); i++) {
		fp.write((char*)&nodeArr[i].id, sizeof(nodeArr[i].id));
		fp.write((char*)&nodeArr[i].blocked, sizeof(nodeArr[i].blocked));
		fp.write((char*)&nodeArr[i].endNode, sizeof(nodeArr[i].endNode));
		fp.write((char*)&nodeArr[i].beginNode, sizeof(nodeArr[i].beginNode));
		auto nameSize = nodeArr[i].name.size();
		fp.write((char*)&nameSize, sizeof(int));
		for (std::size_t j = 0; j < nodeArr[i].name.size(); j++) {
			fp.write((char*)&nodeArr[i].name[j], sizeof(nodeArr[i].name[j]));
		}
		auto edgeSize = nodeArr[i].outEdge.size();
		fp.write((char*)&edgeSize, sizeof(nodeArr[i].outEdge.size()));
		for (std::size_t j = 0; j < nodeArr[i].outEdge.size(); j++) {
			fp.write((char*)&nodeArr[i].outEdge[j], sizeof(nodeArr[i].outEdge[j]));
		}
	}
	fp.close();
	return 0;
}

int Graph::load(const std::string& graphName) {
	std::fstream fp((graphName + ".bin").c_str(), std::ios::in | std::ios::binary);
	if (!fp) {
		return 1;
	}
	char byte;
	auto arrSize = nodeArr.size();
	fp.read((char*)&byte, sizeof(byte));
	if (byte != 1) {
		return 2;
	}
	fp.read((char*)&startNodeID, sizeof(startNodeID));
	fp.read((char*)&endNodeID, sizeof(endNodeID));
	fp.read((char*)&totalNodes, sizeof(totalNodes));
	fp.read((char*)&arrSize, sizeof(int));
	nodeArr.resize(arrSize);
	for (std::size_t i = 0; i < arrSize; i++) {
		fp.read((char*)&nodeArr[i].id, sizeof(nodeArr[i].id));
		fp.read((char*)&nodeArr[i].blocked, sizeof(nodeArr[i].blocked));
		fp.read((char*)&nodeArr[i].endNode, sizeof(nodeArr[i].endNode));
		fp.read((char*)&nodeArr[i].beginNode, sizeof(nodeArr[i].beginNode));
		int nameSize = nodeArr[i].name.size();
		fp.read((char*)&nameSize, sizeof(int));
		nodeArr[i].name.resize(nameSize);
		for (int j = 0; j < nameSize; j++) {
			fp.read((char*)&nodeArr[i].name[j], sizeof(nodeArr[i].name[j]));
		}
		auto edgeSize = nodeArr[i].outEdge.size();
		fp.read((char*)&edgeSize, sizeof(nodeArr[i].outEdge.size()));
		nodeArr[i].outEdge.resize(edgeSize);
		for (std::size_t j = 0; j < edgeSize; j++) {
			fp.read((char*)&nodeArr[i].outEdge[j], sizeof(nodeArr[i].outEdge[j]));
		}
	}
	fp.close();
	for (int i = 0; i <= totalNodes; i++) {
		if (nodeArr[i].name != "") {
			aliasResolve.emplace(nodeArr[i].name, nodeArr[i].id);
		}
	}
	return 0;
}

void Graph::printNames() {
	std::vector<std::string> v;
	for (std::map<std::string, int>::iterator it = aliasResolve.begin(); it != aliasResolve.end(); ++it) {
		v.push_back(it->first);
	}
	std::sort(v.begin(), v.end());
	for (std::size_t i = 0; i < v.size(); i++) {
		std::cout << v[i] << " ";
	}
	std::cout << std::endl;
}

unsigned int Graph::returnGraphCount() {
	return graphCount;
}

void Graph::nameResolve() {
	for (std::map<std::string, int>::iterator it = aliasResolve.begin(); it != aliasResolve.end(); ++it) {
		std::cout << it->first << " " << it->second << std::endl;
	}
}

int Graph::graphType() const {
	return 2;
}

Graph Graph::operator+(Graph g2) {
	Graph g1 = *this;
	Graph* pg1, * pg2;
	if (this->totalNodes >= g2.totalNodes) {
		pg1 = &g1;
		pg2 = &g2;
	}
	else {
		pg1 = &g2;
		pg2 = &g1;
	}
	int offset1 = pg1->totalNodes + 1;
	int offset2 = pg2->totalNodes + 1;
	for (int i = 0; i < offset2; i++) {
		pg1->nodeArr.push_back(pg2->nodeArr[i]);
		pg1->totalNodes += 1;
	}
	for (int i = offset1; i < (pg1->totalNodes + 1); i++) {
		pg1->nodeArr[i].id += (offset1);
		pg1->nodeArr[i].updateNeighbours(offset1);
	}
	bool first = true;
	for (int i = offset1; i <= pg1->totalNodes; i++) {
		if (pg1->nodeArr[i].name != "") {
			std::map<std::string, int>::iterator it = pg1->aliasResolve.find(pg1->nodeArr[i].name);
			if (it != pg1->aliasResolve.end() && first == false) {
				graphCount += 2;
				return Graph(0);
			}
			first = true;
			pg1->aliasResolve.emplace(pg1->nodeArr[i].name, pg1->nodeArr[i].id);
		}
	}
	graphCount += 3;
	return *pg1;
}

DiGraph::DiGraph(const int id) : Graph(id) {}

DiGraph::DiGraph(const Graph& g) {
	searched = g.searched;
	startNodeID = g.startNodeID;
	endNodeID = g.endNodeID;
	totalNodes = g.totalNodes;
	nodeArr = g.nodeArr;
}


int DiGraph::setEdge(const int id1, const int id2) {
	if ((id1 >= 0) && (id1 <= totalNodes) && (id2 >= 0) && (id2 <= totalNodes) && (id2 != id1)) {
		nodeArr[id1].addEdge(id2);
		return 0;
	}
	return 1;
}

int DiGraph::setEdge(const std::string& _alias1, const std::string& _alias2) {
	std::map<std::string, int>::iterator it1 = aliasResolve.find(_alias1);
	std::map<std::string, int>::iterator it2 = aliasResolve.find(_alias2);
	int added = 0;
	if ((it1 != aliasResolve.end()) && (it2 != aliasResolve.end())) {
		added += nodeArr[it1->second].addEdge(it2->second);
		return added ^ 1;
	}
	return 2;
}

int DiGraph::removeEdge(const int id1, const int id2) {
	if ((id1 >= 0) && (id1 <= totalNodes) && (id2 >= 0) && (id2 <= totalNodes) && (id2 != id1)) {
		nodeArr[id1].removeEdge(id2);
		return 0;
	}
	return 1;
}

int DiGraph::removeEdge(const std::string& _alias1, const std::string& _alias2) {
	std::map<std::string, int>::iterator it1 = aliasResolve.find(_alias1);
	std::map<std::string, int>::iterator it2 = aliasResolve.find(_alias2);
	int removed = 0;
	if ((it1 != aliasResolve.end()) && (it2 != aliasResolve.end())) {
		removed += nodeArr[it1->second].removeEdge(it2->second);
		return removed ^ 1;
	}
	return 2;
}

int DiGraph::graphType() const {
	return 1;
}
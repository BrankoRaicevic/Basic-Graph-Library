#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <fstream>
#include <algorithm>
#include "Graph.h"

template<typename T, typename priority_t>
struct PriorityQueue {
	typedef std::pair<priority_t, T> PQElement;
	std::priority_queue<PQElement, std::vector<PQElement>,
		std::greater<PQElement>> elements;

	inline bool empty() const {
		return elements.empty();
	}

	inline void put(T item, priority_t priority) {
		elements.emplace(priority, item);
	}

	T get() {
		T best_item = elements.top().second;
		elements.pop();
		return best_item;
	}

	T get_priority() {
		T best_item = elements.top().first;
		return best_item;
	}
};

unsigned int Graph::graphCount = 0;

Graph::Graph(const int nodeCount) {
	if (nodeCount < 0) {
		throw std::invalid_argument("Node count cannot be negative");
	}
	startNodeID = -1;
	endNodeID = -1;
	graphCount++;
	for (int i = 0; i < nodeCount; i++) {
		nodeArr.push_back(Node(i));
	}
}
Graph::Graph(const Graph& g) {
	startNodeID = g.startNodeID;
	endNodeID = g.endNodeID;
	nodeArr = g.nodeArr;
	aliasResolve = g.aliasResolve;
}
Graph::~Graph() {
	graphCount--;
}
void Graph::setStartNode(const int _startNode) {
	if ((_startNode >= 0) && (_startNode < nodeArr.size())) {
		if (startNodeID >= 0) {
			nodeArr[startNodeID].beginNode = false;
		}
		startNodeID = _startNode;
		nodeArr[startNodeID].beginNode = true;
		return;
	}
	throw std::out_of_range("Index out of scope");
}
void Graph::setEndNode(const int _endNode) {
	if ((_endNode >= 0) && (_endNode < nodeArr.size())) {
		if (endNodeID >= 0) {
			nodeArr[endNodeID].endNode = false;
		}
		endNodeID = _endNode;
		nodeArr[endNodeID].endNode = true;
		return;
	}
	throw std::out_of_range("Index out of scope");
}

void Graph::blockNode(const int _blockNode) {
	if ((_blockNode >= 0) && (_blockNode < nodeArr.size())) {
		nodeArr[_blockNode].blocked = true;
		return;
	}
	throw std::out_of_range("Index out of scope");
}

void Graph::removeStartNode() {
	if (startNodeID >= 0) {
		nodeArr[startNodeID].beginNode = false;
		startNodeID = -1;
	}
}

void Graph::removeEndNode() {
	if (endNodeID >= 0) {
		nodeArr[endNodeID].endNode = false;
		endNodeID = -1;
	}
}

void Graph::removeBlock(const int _blockNode) {
	if ((_blockNode >= 0) && (_blockNode < nodeArr.size())) {
		nodeArr[_blockNode].blocked = false;
		return;
	}
	throw std::out_of_range("Index out of scope");
}

bool Graph::setEdge(const int id1, const int id2, const int weigh) {
	if ((id1 >= 0) && (id1 < nodeArr.size()) && (id2 >= 0) && (id2 < nodeArr.size()) && (id2 != id1)) {
		return nodeArr[id1].addEdge(id2, weigh) && nodeArr[id2].addEdge(id1, weigh);
	}
	throw std::out_of_range("Index out of scope");
}

bool Graph::removeEdge(const int id1, const int id2) {
	if ((id1 >= 0) && (id1 < nodeArr.size()) && (id2 >= 0) && (id2 < nodeArr.size()) && (id2 != id1)) {
		return nodeArr[id1].removeEdge(id2) && nodeArr[id2].removeEdge(id1);
	}
	throw std::out_of_range("Index out of scope");
}

void Graph::setStartNode(const std::string& _startNode) {
	std::map<std::string, int>::iterator it = aliasResolve.find(_startNode);
	if (it != aliasResolve.end()) {
		if (startNodeID >= 0) {
			nodeArr[startNodeID].beginNode = false;
		}
		startNodeID = it->second;
		nodeArr[it->second].beginNode = true;
		return;
	}
	throw std::invalid_argument("Couldn't find name");
}
void Graph::setEndNode(const std::string& _endNode) {
	std::map<std::string, int>::iterator it = aliasResolve.find(_endNode);
	if (it != aliasResolve.end()) {
		if (endNodeID >= 0) {
			nodeArr[endNodeID].endNode = false;
		}
		endNodeID = it->second;
		nodeArr[it->second].endNode = true;
		return;
	}
	throw std::invalid_argument("Couldn't find name");
}

void Graph::blockNode(const std::string& _blockNode) {
	std::map<std::string, int>::iterator it = aliasResolve.find(_blockNode);
	if (it != aliasResolve.end()) {
		nodeArr[it->second].blocked = true;
		return;
	}
	throw std::invalid_argument("Couldn't find name");
}

void Graph::removeBlock(const std::string& _blockNode) {
	std::map<std::string, int>::iterator it = aliasResolve.find(_blockNode);
	if (it != aliasResolve.end()) {
		nodeArr[it->second].blocked = false;
		return;
	}
	throw std::invalid_argument("Couldn't find name");
}

bool Graph::setEdge(const std::string& _alias1, const std::string& _alias2, const int weigh) {
	std::map<std::string, int>::iterator it1 = aliasResolve.find(_alias1);
	std::map<std::string, int>::iterator it2 = aliasResolve.find(_alias2);
	if ((it1 != aliasResolve.end()) && (it2 != aliasResolve.end())) {
		return nodeArr[it1->second].addEdge(it2->second, weigh) && nodeArr[it2->second].addEdge(it1->second, weigh);
	}
	throw std::invalid_argument("Couldn't find name");
}

bool Graph::removeEdge(const std::string& _alias1, const std::string& _alias2) {
	std::map<std::string, int>::iterator it1 = aliasResolve.find(_alias1);
	std::map<std::string, int>::iterator it2 = aliasResolve.find(_alias2);
	if ((it1 != aliasResolve.end()) && (it2 != aliasResolve.end())) {
		return nodeArr[it1->second].removeEdge(it2->second) && nodeArr[it2->second].removeEdge(it1->second);
	}
	throw std::invalid_argument("Couldn't find name");
}

void Graph::addNode() {
	nodeArr.push_back(Node());
	nodeArr[nodeArr.size() - 1].id = nodeArr.size() - 1;
}

bool Graph::addNode(const std::string& _name) {
	std::map<std::string, int>::iterator it = aliasResolve.find(_name);
	if ((it == aliasResolve.end())) {
		addNode();
		return setName(nodeArr[nodeArr.size() - 1].id, _name);
	}
	return false;
}

void Graph::removeNode(const int id) {
	if ((id >= 0) && (id < nodeArr.size())) {
		nodeArr.erase(nodeArr.begin() + id);
		if (id < endNodeID) { endNodeID--; }
		else if (id == endNodeID) { endNodeID = -1; }
		if (id < startNodeID) { startNodeID--; }
		else if (id == startNodeID) { startNodeID = -1; }
		for (std::size_t i = id; i < nodeArr.size(); i++) {
			nodeArr[i].id--;
		}
		for (std::size_t i = 0; i < nodeArr.size(); i++) {
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
		return;
	}
	throw std::out_of_range("Index out of scope");
}

void Graph::removeNode(const std::string& _name) {
	std::map<std::string, int>::iterator it = aliasResolve.find(_name);
	if (it != aliasResolve.end()) {
		removeNode(it->second);
		return;
	}
	throw std::invalid_argument("Invalid name");
}

bool Graph::setName(const int id, const std::string& _name) {
	if ((id >= 0) && (id < nodeArr.size()) && (_name != "")) {
		std::map<std::string, int>::iterator it = aliasResolve.find(_name);
		if (it != aliasResolve.end()) {
			return false;
		}
		aliasResolve.try_emplace(_name, id);
		nodeArr[id].name = _name;
		return true;
	}
	throw std::out_of_range("Index out of scope");
}

std::string Graph::getName(const int id) const {
	return nodeArr[id].name;
}

void Graph::save(const std::string& graphName) {
	std::fstream fp((graphName + ".bin").c_str(), std::ios::out | std::ios::binary);
	if (!fp) {
		throw std::ios_base::failure("Failed to open file");
	}
	char byte = static_cast<char> (graphType());
	auto arrSize = nodeArr.size();
	fp.write((char*)&byte, sizeof(byte));
	fp.write((char*)&startNodeID, sizeof(startNodeID));
	fp.write((char*)&endNodeID, sizeof(endNodeID));
	fp.write((char*)&arrSize, sizeof(arrSize));
	for (std::size_t i = 0; i < nodeArr.size(); i++) {
		fp.write((char*)&nodeArr[i].id, sizeof(nodeArr[i].id));
		fp.write((char*)&nodeArr[i].visited, sizeof(nodeArr[i].visited));
		fp.write((char*)&nodeArr[i].blocked, sizeof(nodeArr[i].blocked));
		fp.write((char*)&nodeArr[i].endNode, sizeof(nodeArr[i].endNode));
		fp.write((char*)&nodeArr[i].beginNode, sizeof(nodeArr[i].beginNode));
		auto nameSize = nodeArr[i].name.size();
		fp.write((char*)&nameSize, sizeof(nameSize));
		for (std::size_t j = 0; j < nodeArr[i].name.size(); j++) {
			fp.write((char*)&nodeArr[i].name[j], sizeof(nodeArr[i].name[j]));
		}
		auto edgeSize = nodeArr[i].outEdge.size();
		fp.write((char*)&edgeSize, sizeof(nodeArr[i].outEdge.size()));
		for (std::size_t j = 0; j < nodeArr[i].outEdge.size(); j++) {
			fp.write((char*)&nodeArr[i].outEdge[j], sizeof(nodeArr[i].outEdge[j]));
		}
		auto weightSize = nodeArr[i].outWeight.size();
		fp.write((char*)&weightSize, sizeof(nodeArr[i].outWeight.size()));
		for (std::size_t j = 0; j < nodeArr[i].outWeight.size(); j++) {
			fp.write((char*)&nodeArr[i].outWeight[j], sizeof(nodeArr[i].outWeight[j]));
		}
	}
	fp.close();
}

void Graph::load(const std::string& graphName) {
	std::fstream fp((graphName + ".bin").c_str(), std::ios::in | std::ios::binary);
	if (!fp) {
		throw std::ios_base::failure("Failed to open file");
	}
	char byte = 0;
	auto arrSize = nodeArr.size();
	fp.read((char*)&byte, sizeof(byte));
	if (byte != graphType()) {
		throw std::ios_base::failure("Trying to load incompatible graph");
	}
	fp.read((char*)&startNodeID, sizeof(startNodeID));
	fp.read((char*)&endNodeID, sizeof(endNodeID));
	fp.read((char*)&arrSize, sizeof(arrSize));
	nodeArr.resize(arrSize);
	for (std::size_t i = 0; i < arrSize; i++) {
		fp.read((char*)&nodeArr[i].id, sizeof(nodeArr[i].id));
		fp.read((char*)&nodeArr[i].visited, sizeof(nodeArr[i].visited));
		fp.read((char*)&nodeArr[i].blocked, sizeof(nodeArr[i].blocked));
		fp.read((char*)&nodeArr[i].endNode, sizeof(nodeArr[i].endNode));
		fp.read((char*)&nodeArr[i].beginNode, sizeof(nodeArr[i].beginNode));
		auto nameSize = nodeArr[i].name.size();
		fp.read((char*)&nameSize, sizeof(nameSize));
		nodeArr[i].name.resize(nameSize);
		for (std::size_t j = 0; j < nameSize; j++) {
			fp.read((char*)&nodeArr[i].name[j], sizeof(nodeArr[i].name[j]));
		}
		auto edgeSize = nodeArr[i].outEdge.size();
		fp.read((char*)&edgeSize, sizeof(nodeArr[i].outEdge.size()));
		nodeArr[i].outEdge.resize(edgeSize);
		for (std::size_t j = 0; j < edgeSize; j++) {
			fp.read((char*)&nodeArr[i].outEdge[j], sizeof(nodeArr[i].outEdge[j]));
		}
		auto weightSize = nodeArr[i].outWeight.size();
		fp.read((char*)&weightSize, sizeof(nodeArr[i].outWeight.size()));
		nodeArr[i].outWeight.resize(weightSize);
		for (std::size_t j = 0; j < weightSize; j++) {
			fp.read((char*)&nodeArr[i].outWeight[j], sizeof(nodeArr[i].outWeight[j]));
		}
	}
	fp.close();
	for (std::size_t i = 0; i < nodeArr.size(); i++) {
		if (nodeArr[i].name != "") {
			aliasResolve.emplace(nodeArr[i].name, nodeArr[i].id);
		}
	}
}

bool Graph::isConnected() {
	if (nodeArr.empty() || (nodeArr.size() == 1)) {
		return true;
	}
	for (std::size_t i = 0; i < nodeArr.size(); ++i) {
		if (nodeArr[i].outEdge.size() == 0) {
			return false;
		}
	}
	return true;
}

void Graph::BreadthFirstSearch() {
	std::queue<int> frontier;
	int activeNode;
	bool end = false;
	if (startNodeID == -1) {
		throw std::logic_error("start node not set");
	}
	searchCleanup();
	frontier.push(startNodeID);
	while (!frontier.empty()) {
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
}

void Graph::DepthFirstSearch() {
	std::stack<int> frontier;
	int activeNode;
	bool end = false;
	if (startNodeID == -1) {
		throw std::logic_error("start node not set");
	}
	searchCleanup();
	frontier.push(startNodeID);
	while (!frontier.empty()) {
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
}

void Graph::Dijkstra() {
	PriorityQueue<int, int> frontier;
	int activeNode;
	bool end = false;
	constexpr int maxWeight = std::numeric_limits<int>::max();
	std::vector<int> minDistance;
	if (startNodeID == -1) {
		throw std::logic_error("start node not set");
	}
	searchCleanup();
	minDistance.clear();
	minDistance.resize(nodeArr.size(), maxWeight);
	minDistance[startNodeID] = 0;
	frontier.put(startNodeID, minDistance[startNodeID]);
	while (!frontier.empty()) {
		int dist = frontier.get_priority();
		activeNode = frontier.get();
		if (dist > minDistance[activeNode]) {
			continue;
		}
		for (std::size_t i = 0; i < nodeArr[activeNode].countNeighbours(); i++) {
			int retID = nodeArr[activeNode].returnNeighbour(i);
			int retWeigh = nodeArr[activeNode].returnNeighbourWeight(i);
			int distThruVertex = dist + retWeigh;
			if (nodeArr[retID].isFree() && (distThruVertex < minDistance[retID])) {
				nodeArr[retID].setPrevious(activeNode);
				nodeArr[retID].setVisited();
				minDistance[retID] = distThruVertex;
				if (nodeArr[activeNode].isEndNode()) {
					endNodeID = retID;
					end = true;
					break;
				}
				frontier.put(retID, minDistance[retID]);
			}
		}
		if (end == true) {
			break;
		}
	}
}

bool Graph::Bipartite() {
	std::queue<int> frontier;
	int activeNode;
	if (startNodeID == -1) {
		throw std::logic_error("start node not set");
	}
	searchCleanup();
	char* color = new char[nodeArr.size() + 1];
	for (std::size_t i = 0; i < nodeArr.size(); i++) {
		color[i] = 0;
	}
	color[startNodeID] = 1;
	frontier.push(startNodeID);
	while (1) {
		if (frontier.empty()) {
			delete[] color;
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
				delete[] color;
				return false;
			}
		}
	}
}

std::vector<int> Graph::BreadthFirstSearch(int startNode, int endNode) {
	if ((startNode >= 0) && (startNode < nodeArr.size()) && (endNode >= 0) && (endNode < nodeArr.size()) && (endNode != startNode)) {
		int tmpSN = startNodeID;
		int tmpEN = endNodeID;
		setStartNode(startNode);
		setEndNode(endNode);
		BreadthFirstSearch();
		auto path = drawPath();
		if (tmpSN < 0) {
			removeStartNode();
		}
		else {
			setStartNode(tmpSN);
		}
		if (tmpEN < 0) {
			removeEndNode();
		}
		else {
			setEndNode(tmpEN);
		}
		return path;
	}
	throw std::out_of_range("Index out of scope");
}

std::vector<int> Graph::DepthFirstSearch(int startNode, int endNode) {
	if ((startNode >= 0) && (startNode < nodeArr.size()) && (endNode >= 0) && (endNode < nodeArr.size()) && (endNode != startNode)) {
		int tmpSN = startNodeID;
		int tmpEN = endNodeID;
		setStartNode(startNode);
		setEndNode(endNode);
		DepthFirstSearch();
		auto path = drawPath();
		if (tmpSN < 0) {
			removeStartNode();
		}
		else {
			setStartNode(tmpSN);
		}
		if (tmpEN < 0) {
			removeEndNode();
		}
		else {
			setEndNode(tmpEN);
		}
		return path;
	}
	throw std::out_of_range("Index out of scope");
}

std::vector<int> Graph::Dijkstra(int startNode, int endNode) {
	if ((startNode >= 0) && (startNode < nodeArr.size()) && (endNode >= 0) && (endNode < nodeArr.size()) && (endNode != startNode)) {
		int tmpSN = startNodeID;
		int tmpEN = endNodeID;
		setStartNode(startNode);
		setEndNode(endNode);
		Dijkstra();
		auto path = drawPath();
		if (tmpSN < 0) {
			removeStartNode();
		}
		else {
			setStartNode(tmpSN);
		}
		if (tmpEN < 0) {
			removeEndNode();
		}
		else {
			setEndNode(tmpEN);
		}
		return path;
	}
	throw std::out_of_range("Index out of scope");
}

std::vector<int> Graph::BreadthFirstSearch(const std::string& _alias1, const std::string& _alias2) {
	std::map<std::string, int>::iterator it1 = aliasResolve.find(_alias1);
	std::map<std::string, int>::iterator it2 = aliasResolve.find(_alias2);
	if ((it1 != aliasResolve.end()) && (it2 != aliasResolve.end())) {
		return BreadthFirstSearch(it1->second, it2->second);
	}
	throw std::invalid_argument("Couldn't find name");
}

std::vector<int> Graph::DepthFirstSearch(const std::string& _alias1, const std::string& _alias2) {
	std::map<std::string, int>::iterator it1 = aliasResolve.find(_alias1);
	std::map<std::string, int>::iterator it2 = aliasResolve.find(_alias2);
	if ((it1 != aliasResolve.end()) && (it2 != aliasResolve.end())) {
		return DepthFirstSearch(it1->second, it2->second);
	}
	throw std::invalid_argument("Couldn't find name");
}

std::vector<int> Graph::Dijkstra(const std::string& _alias1, const std::string& _alias2) {
	std::map<std::string, int>::iterator it1 = aliasResolve.find(_alias1);
	std::map<std::string, int>::iterator it2 = aliasResolve.find(_alias2);
	if ((it1 != aliasResolve.end()) && (it2 != aliasResolve.end())) {
		return Dijkstra(it1->second, it2->second);
	}
	throw std::invalid_argument("Couldn't find name");
}

bool Graph::Bipartite(const int _startNode) {
	if ((_startNode >= 0) && (_startNode < nodeArr.size())) {
		setStartNode(_startNode);
		return Bipartite();
	}
	throw std::out_of_range("Index out of scope");
}

bool Graph::Bipartite(const std::string& _startNode) {
	std::map<std::string, int>::iterator it = aliasResolve.find(_startNode);
	if (it != aliasResolve.end()) {
		setStartNode(it->second);
		return Bipartite();
	}
	throw std::out_of_range("Couldn't find name");
}

std::vector<int> Graph::drawPath() {
	std::vector<int> path;
	int currentNode = endNodeID;
	if ((startNodeID == -1) || (endNodeID == -1)) {
		throw std::logic_error("start or end node not set");
	}

	if (!nodeArr[currentNode].isVisited()) {
		throw std::logic_error("end node not visited/unreachable");
	}

	while (1) {
		if (nodeArr[currentNode].isBeginNode()) {
			path.push_back(currentNode);
			break;
		}
		path.push_back(currentNode);
		currentNode = nodeArr[currentNode].getPrevious();
	}
	int tmp;
	for (std::size_t i = 0; i < path.size() - 1 - i; i++) {
		tmp = path[i];
		path[i] = path[path.size() - 1 - i];
		path[path.size() - 1 - i] = tmp;
	}
	return path;
}

void Graph::printPath(std::vector<int>& path) {
	for (std::vector<int>::iterator it = path.begin(); it != path.end(); ++it) {
		if (nodeArr[*it].name != "") {
			std::cout << nodeArr[*it].name;
		}
		else {
			std::cout << *it;
		}
		if ((it + 1) != path.end()) {
			std::cout << " -> ";
		}
	}
	std::cout << std::endl;
}

void Graph::printPath(std::vector<int> &&path) {
	printPath(path);
}

void Graph::searchCleanup() {
	for (std::size_t i = 0; i < nodeArr.size(); i++) {
		nodeArr[i].clear();
	}
}

void Graph::showNeighbours() {
	for (std::size_t i = 0; i < nodeArr.size(); i++) {
		std::cout << "ID " << i << ": ";
		nodeArr[i].showNeighbours();
		std::cout << std::endl;
	}
}

void Graph::showNeighboursByName() {
	for (std::size_t i = 0; i < nodeArr.size(); i++) {
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

void Graph::nameResolve() {
	for (std::map<std::string, int>::iterator it = aliasResolve.begin(); it != aliasResolve.end(); ++it) {
		std::cout << it->first << " " << it->second << std::endl;
	}
}

unsigned int Graph::returnGraphCount() {
	return graphCount;
}

int Graph::getNodeCount() const {
	return nodeArr.size();
}

int Graph::graphType() const {
	return 2;
}

Graph Graph::operator+(Graph g2) {
	Graph g1 = *this;
	Graph* pg1, * pg2;
	if (this->nodeArr.size() >= g2.nodeArr.size()) {
		pg1 = &g1;
		pg2 = &g2;
	}
	else {
		pg1 = &g2;
		pg2 = &g1;
	}
	auto offset1 = pg1->nodeArr.size();
	auto offset2 = pg2->nodeArr.size();
	for (std::size_t i = 0; i < offset2; i++) {
		pg1->nodeArr.push_back(pg2->nodeArr[i]);
	}
	for (std::size_t i = offset1; i < pg1->nodeArr.size(); i++) {
		pg1->nodeArr[i].id += (offset1);
		pg1->nodeArr[i].updateNeighbours(offset1);
	}
	bool first = true;
	for (std::size_t i = offset1; i < pg1->nodeArr.size(); i++) {
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
	startNodeID = g.startNodeID;
	endNodeID = g.endNodeID;
	nodeArr = g.nodeArr;
	aliasResolve = g.aliasResolve;
}

DiGraph::DiGraph(const DiGraph& g) {
	startNodeID = g.startNodeID;
	endNodeID = g.endNodeID;
	nodeArr = g.nodeArr;
	aliasResolve = g.aliasResolve;
}


bool DiGraph::setEdge(const int id1, const int id2, const int weigh) {
	if ((id1 >= 0) && (id1 < nodeArr.size()) && (id2 >= 0) && (id2 < nodeArr.size()) && (id2 != id1)) {
		return nodeArr[id1].addEdge(id2, weigh);
	}
	throw std::out_of_range("Index out of scope");
}

bool DiGraph::setEdge(const std::string& _alias1, const std::string& _alias2, const int weigh) {
	std::map<std::string, int>::iterator it1 = aliasResolve.find(_alias1);
	std::map<std::string, int>::iterator it2 = aliasResolve.find(_alias2);
	if ((it1 != aliasResolve.end()) && (it2 != aliasResolve.end())) {
		return nodeArr[it1->second].addEdge(it2->second, weigh);
	}
	throw std::invalid_argument("Couldn't find name");
}

bool DiGraph::removeEdge(const int id1, const int id2) {
	if ((id1 >= 0) && (id1 < nodeArr.size()) && (id2 >= 0) && (id2 < nodeArr.size()) && (id2 != id1)) {
		return nodeArr[id1].removeEdge(id2);;
	}
	throw std::out_of_range("Index out of scope");
}

bool DiGraph::removeEdge(const std::string& _alias1, const std::string& _alias2) {
	std::map<std::string, int>::iterator it1 = aliasResolve.find(_alias1);
	std::map<std::string, int>::iterator it2 = aliasResolve.find(_alias2);
	if ((it1 != aliasResolve.end()) && (it2 != aliasResolve.end())) {
		return nodeArr[it1->second].removeEdge(it2->second);
	}
	throw std::invalid_argument("Couldn't find name");
}

std::vector<std::vector<int>> DiGraph::FloydWarshall() {
	std::vector<std::vector<int>> distMatrix(nodeArr.size(), std::vector<int>(nodeArr.size()));
	int maxWeight = std::numeric_limits<int>::max();
	for (std::size_t i = 0; i < distMatrix.size(); ++i) {
		for (std::size_t j = 0; j < distMatrix.size(); ++j) {
			auto it = std::find(nodeArr[i].outEdge.begin(), nodeArr[i].outEdge.end(), nodeArr[j].id);
			if (it != nodeArr[i].outEdge.end()) {
				auto dist = std::distance(nodeArr[i].outEdge.begin(), it);
				distMatrix[i][j] = nodeArr[i].outWeight[dist];
			}
			else if (i == j) {
				distMatrix[i][j] = 0;
			}
			else {
				distMatrix[i][j] = maxWeight;
			}
		}
	}
	for (std::size_t k = 0; k < (nodeArr.size()); k++) {
		for (std::size_t i = 0; i < (nodeArr.size()); i++) {
			for (std::size_t j = 0; j < (nodeArr.size()); j++) {
				if ((distMatrix[i][k] != maxWeight && distMatrix[k][j] != maxWeight) && ((distMatrix[i][k] + distMatrix[k][j]) < distMatrix[i][j])) {
					distMatrix[i][j] = distMatrix[i][k] + distMatrix[k][j];
				}
			}
		}
	}
	return distMatrix;
}

void DiGraph::printFloyd(const std::vector<std::vector<int>>& distMatrix) {
	constexpr int maxWeight = std::numeric_limits<int>::max();
	for (std::size_t i = 0; i < nodeArr.size(); i++) {
		for (std::size_t j = 0; j < nodeArr.size(); j++) {
			if (distMatrix[i][j] == maxWeight) {
				std::cout << "INF" << "     ";
			}
			else {
				std::cout << distMatrix[i][j] << "     ";
			}
		}
		std::cout << std::endl;
	}
}

std::vector<int> DiGraph::BellmanFord(const int source) {
	if (!((source >= 0) && (source < nodeArr.size()))) {
		throw std::out_of_range("Index out of scope");
	}

	std::vector<int> dist(nodeArr.size());
	for (std::size_t i = 0; i < nodeArr.size(); ++i) {
		dist[i] = std::numeric_limits<int>::max();
	}
	dist[source] = 0;

	for (std::size_t k = 1; k <= nodeArr.size() - 1; k++) {
		for (std::size_t i = 0; i < nodeArr.size(); i++) {
			for (std::size_t j = 0; j < nodeArr[i].outEdge.size(); j++) {
				int u = i;
				int v = nodeArr[i].outEdge[j];
				int weight = nodeArr[i].outWeight[j];
				if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
					dist[v] = dist[u] + weight;
				}
			}
		}
	}

	for (std::size_t i = 1; i <= nodeArr.size() - 1; i++) {
		for (std::size_t j = 0; j < nodeArr[i - 1].outEdge.size(); j++) {
			int u = i - 1;
			int v = nodeArr[i - 1].outEdge[j];
			int weight = nodeArr[i - 1].outWeight[j];
			if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
				printf("Graph contains negative weight cycle");
				return std::vector<int>();
			}
		}
	}

	return dist;
}

std::vector<int> DiGraph::BellmanFord(const std::string& source) {
	std::map<std::string, int>::iterator it = aliasResolve.find(source);
	if (it != aliasResolve.end()) {
		return BellmanFord(it->second);
	}
	throw std::invalid_argument("Couldn't find name");
}

void DiGraph::printBellmanFord(const std::vector<int>& dist) {
	std::cout << "Vertex   Distance from Source\n";
	for (std::size_t i = 0; i < dist.size(); ++i) {
		if (dist[i] == std::numeric_limits<int>::max()) {
			std::cout << i << " \t INF" << std::endl;
		}
		else {
			std::cout << i << " \t " << dist[i] << std::endl;
		}
	}
}

int DiGraph::graphType() const {
	return 1;
}
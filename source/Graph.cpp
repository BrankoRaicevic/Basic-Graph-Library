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
	graphCount++;
	for (int i = 0; i < nodeCount; i++) {
		nodeArr.push_back(Node());
	}
}
Graph::Graph(const Graph& g) {
	nodeArr = g.nodeArr;
	aliasResolve = g.aliasResolve;
}
Graph::~Graph() {
	graphCount--;
}

void Graph::blockNode(const int _blockNode) {
	if ((_blockNode >= 0) && (_blockNode < nodeArr.size())) {
		nodeArr[_blockNode].blocked = true;
		return;
	}
	throw std::out_of_range("Index out of scope");
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
}

bool Graph::addNode(const std::string& _name) {
	std::map<std::string, int>::iterator it = aliasResolve.find(_name);
	if ((it == aliasResolve.end())) {
		addNode();
		return setName(nodeArr.size() - 1, _name); //(nodeArr.size() - 1) is the 'id'
	}
	return false;
}

void Graph::removeNode(const int id) {
	if ((id >= 0) && (id < nodeArr.size())) {
		nodeArr.erase(nodeArr.begin() + id);
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
	fp.write((char*)&arrSize, sizeof(arrSize));
	for (std::size_t i = 0; i < nodeArr.size(); i++) {
		fp.write((char*)&nodeArr[i].blocked, sizeof(nodeArr[i].blocked));
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
	fp.read((char*)&arrSize, sizeof(arrSize));
	nodeArr.resize(arrSize);
	for (std::size_t i = 0; i < arrSize; i++) {
		fp.read((char*)&nodeArr[i].blocked, sizeof(nodeArr[i].blocked));
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
			aliasResolve.emplace(nodeArr[i].name, i); //(i) second argument is 'id'
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

std::vector<std::vector<int>> Graph::weightMatrix(){
	std::vector<std::vector<int>> distMatrix(nodeArr.size(), std::vector<int>(nodeArr.size()));

	for (std::size_t i = 0; i < distMatrix.size(); ++i) {
		for (std::size_t j = 0; j < distMatrix.size(); ++j) {
			auto it = std::find(nodeArr[i].outEdge.begin(), nodeArr[i].outEdge.end(), j); //(j) is 'id'
			if (it != nodeArr[i].outEdge.end()) {
				auto dist = std::distance(nodeArr[i].outEdge.begin(), it);
				distMatrix[i][j] = nodeArr[i].outWeight[dist];
			}
			else {
				distMatrix[i][j] = 0;
			}
		}
	}
	return distMatrix;
}

std::vector<int> Graph::BreadthFirstSearch(const int startNodeID, int endNodeID) {
	if (!((startNodeID >= 0) && (startNodeID < nodeArr.size()) && (endNodeID >= 0) && (endNodeID < nodeArr.size()) && (endNodeID != startNodeID))) {
		throw std::out_of_range("Index out of scope");
	}
	std::queue<int> frontier;
	int activeNode;
	bool end = false;
	std::vector<int> whoVisited(nodeArr.size(), -1);
	bool* visited = new bool[nodeArr.size() + 1];
	for (std::size_t i = 0; i < nodeArr.size(); i++) {
		visited[i] = false;
	}
	frontier.push(startNodeID);
	while (!frontier.empty()) {
		activeNode = frontier.front();
		frontier.pop();
		for (std::size_t i = 0; i < nodeArr[activeNode].countNeighbours(); i++) {
			int retID = nodeArr[activeNode].returnNeighbour(i);
			if (nodeArr[retID].isFree() && visited[retID] == false) {
				whoVisited[retID] = activeNode;
				visited[retID] = true;
				if (retID == endNodeID) {
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
	delete[] visited;
	return drawPath(whoVisited, startNodeID, endNodeID);
}

std::vector<int> Graph::DepthFirstSearch(const int startNodeID, int endNodeID) {
	if (!((startNodeID >= 0) && (startNodeID < nodeArr.size()) && (endNodeID >= 0) && (endNodeID < nodeArr.size()) && (endNodeID != startNodeID))) {
		throw std::out_of_range("Index out of scope");
	}
	std::stack<int> frontier;
	int activeNode;
	bool end = false;
	std::vector<int> whoVisited(nodeArr.size(), -1);
	bool* visited = new bool[nodeArr.size() + 1];
	for (std::size_t i = 0; i < nodeArr.size(); i++) {
		visited[i] = false;
	}
	frontier.push(startNodeID);
	while (!frontier.empty()) {
		activeNode = frontier.top();
		frontier.pop();
		for (std::size_t i = 0; i < nodeArr[activeNode].countNeighbours(); i++) {
			int retID = nodeArr[activeNode].returnNeighbour(i);
			if (nodeArr[retID].isFree() && visited[retID] == false) {
				whoVisited[retID] = activeNode;
				visited[retID] = true;
				if (retID == endNodeID) {
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
	delete[] visited;
	return drawPath(whoVisited, startNodeID, endNodeID);
}

std::vector<int> Graph::Dijkstra(const int startNodeID, int endNodeID) {
	if (!((startNodeID >= 0) && (startNodeID < nodeArr.size()) && (endNodeID >= 0) && (endNodeID < nodeArr.size()) && (endNodeID != startNodeID))) {
		throw std::out_of_range("Index out of scope");
	}
	PriorityQueue<int, int> frontier;
	int activeNode;
	bool end = false;
	constexpr int maxWeight = std::numeric_limits<int>::max();
	std::vector<int> minDistance;
	std::vector<int> whoVisited(nodeArr.size(), -1);
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
				whoVisited[retID] = activeNode;
				minDistance[retID] = distThruVertex;
				if (activeNode == endNodeID) {
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
	return drawPath(whoVisited, startNodeID, endNodeID);
}

bool Graph::Bipartite(const int startNodeID) {
	if (!((startNodeID >= 0) && (startNodeID < nodeArr.size()))) {
		throw std::out_of_range("Index out of scope");
	}
	std::queue<int> frontier;
	int activeNode;
	bool* visited = new bool[nodeArr.size() + 1];
	for (std::size_t i = 0; i < nodeArr.size(); i++) {
		visited[i] = false;
	}
	char* color = new char[nodeArr.size() + 1];
	for (std::size_t i = 0; i < nodeArr.size(); i++) {
		color[i] = 0;
	}
	color[startNodeID] = 1;
	frontier.push(startNodeID);
	while (1) {
		if (frontier.empty()) {
			delete[] visited;
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
				if (visited[retID] == false) {
					visited[retID] = true;
					frontier.push(retID);
				}
			}
			else {
				delete[] visited;
				delete[] color;
				return false;
			}
		}
	}
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

bool Graph::Bipartite(const std::string& _startNode) {
	std::map<std::string, int>::iterator it = aliasResolve.find(_startNode);
	if (it != aliasResolve.end()) {
		return Bipartite(it->second);
	}
	throw std::out_of_range("Couldn't find name");
}

std::vector<int> Graph::drawPath(std::vector<int>& whoVisited, const int startNodeID, const int endNodeID) {
	std::vector<int> path;
	int currentNode = endNodeID;

	if (whoVisited[currentNode] == -1) {
		throw std::logic_error("end node not visited/unreachable");
	}

	while (1) {
		if (startNodeID == currentNode) {
			path.push_back(currentNode);
			break;
		}
		path.push_back(currentNode);
		currentNode = whoVisited[currentNode];
	}
	for (std::size_t i = 0; i < path.size() - 1 - i; i++) {
		int tmp = path[i];
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

void Graph::showNeighbours() {
	for (std::size_t i = 0; i < nodeArr.size(); i++) {
		std::cout << "ID " << i << ": ";
		nodeArr[i].showNeighbours();
		std::cout << std::endl;
	}
}

void Graph::showNeighboursByName() {
	for (std::size_t i = 0; i < nodeArr.size(); i++) {
		std::cout << nodeArr[i].name << " -> ";
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
			pg1->aliasResolve.emplace(pg1->nodeArr[i].name, i); //(i) second argument is 'id'
		}
	}
	graphCount += 3;
	return *pg1;
}

DiGraph::DiGraph(const int id) : Graph(id) {}

DiGraph::DiGraph(const Graph& g) {
	nodeArr = g.nodeArr;
	aliasResolve = g.aliasResolve;
}

DiGraph::DiGraph(const DiGraph& g) {
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
	//std::vector<std::vector<int>> distMatrix(nodeArr.size(), std::vector<int>(nodeArr.size()));
	int maxWeight = std::numeric_limits<int>::max();
	//for (std::size_t i = 0; i < distMatrix.size(); ++i) {
	//	for (std::size_t j = 0; j < distMatrix.size(); ++j) {
	//		auto it = std::find(nodeArr[i].outEdge.begin(), nodeArr[i].outEdge.end(), j); //(j) is 'id'
	//		if (it != nodeArr[i].outEdge.end()) {
	//			auto dist = std::distance(nodeArr[i].outEdge.begin(), it);
	//			distMatrix[i][j] = nodeArr[i].outWeight[dist];
	//		}
	//		else if (i == j) {
	//			distMatrix[i][j] = 0;
	//		}
	//		else {
	//			distMatrix[i][j] = maxWeight;
	//		}
	//	}
	//}
	auto distMatrix = weightMatrix();
	for (std::size_t i = 0; i < distMatrix.size(); i++) {
		for (std::size_t j = 0; j < distMatrix.size(); j++) {
			if (distMatrix[i][j] == 0 && i != j) {
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

	constexpr int maxInt = std::numeric_limits<int>::max();
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
				if (dist[u] != maxInt && dist[u] + weight < dist[v]) {
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
			if (dist[u] != maxInt && dist[u] + weight < dist[v]) {
				return std::vector<int>(); //Graph contains negative weight cycle
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

std::vector<int> DiGraph::TopologicalSort() {
	std::stack<int> frontier;

	bool* visited = new bool[nodeArr.size() + 1];
	for (std::size_t i = 0; i < nodeArr.size(); i++) {
		visited[i] = false;
	}
	for (std::size_t i = 0; i < nodeArr.size(); i++) {
		if (visited[i] == false) {
			TopologicalSortHelper(i, frontier, visited);
		}
	}

	std::vector<int> order;
	while (!frontier.empty()) {
		order.push_back(frontier.top());
		frontier.pop();
	}
	delete[] visited;
	return order;
}

void DiGraph::TopologicalSortHelper(int v, std::stack<int>& frontier, bool *visited) {
	visited[v] = true;

	for (auto i : nodeArr[v].outEdge) {
		if (visited[i] == false) {
			TopologicalSortHelper(i, frontier, visited);
		}
	}

	frontier.push(v);
}

void DiGraph::printTopologicalSort(const std::vector<int>& order) {
	for (auto node : order) {
		std::cout << node << " ";
	}
	std::cout << std::endl;
}

int DiGraph::graphType() const {
	return 1;
}
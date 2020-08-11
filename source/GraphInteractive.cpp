#include <iostream>
#include "GraphInteractive.h"


std::string& ltrim(std::string& s, const char* t)
{
	s.erase(0, s.find_first_not_of(t));
	return s;
}

std::string& rtrim(std::string& s, const char* t)
{
	s.erase(s.find_last_not_of(t) + 1);
	return s;
}

std::string& trim(std::string& s, const char* t)
{
	return ltrim(rtrim(s, t), t);
}

int solvePlusEqual(Graph* g1, Graph* g2, Graph* g3) {
	*g1 = *g2 + *g3;
	return 0;
}

int solvePlusEqual(DiGraph* g1, DiGraph* g2, DiGraph* g3) {
	*g1 = *g2 + *g3;
	return 0;
}

bool isNameGood(const std::string& name) {
	if (name.empty()) {
		return false;
	}
	if (name.find(' ') != std::string::npos) {
		return false;
	}
	if (isdigit(name[0])) {
		return false;
	}
	return true;
}


int handleGraph(Graph* graph, const std::string& name) {
	std::string input;
	bool searched = false;
	while (true) {
		std::cout << "\nGraph: " << name << std::endl;
		std::cout << "[1]Add Node [2]Remove Node [3]Add Edge [4]Remove Edge [5]Set Start Node\n";
		std::cout << "[6]BreadthFirstSearch [7]DepthFirstSearch [8]Shortest Path To\n[9]Save [10]Load ";
		std::cout << "[11]Print Names [12]Print Connections [13]Bipartite\n[14]Block [15]Unblock\n[0]Exit" << std::endl;
		std::cout << "$>";
		std::getline(std::cin, input);
		input = trim(input);
		int num = atoi(input.c_str());
		if (num == 0) {
			return 0;
		}
		else if (num == 1) {
			std::cout << "Enter name for new node: ";
			std::getline(std::cin, input);
			input = trim(input);
			if (isNameGood(input)) {
				int retVal = graph->addNode(input);
				if (retVal == 0) {
					std::cout << "Node \"" << input << "\" added" << std::endl;
				}
				else {
					std::cout << "Name already taken" << std::endl;
				}
			}
			else {
				std::cout << "Invalid name" << std::endl;
			}
		}
		else if (num == 2) {
			if (graph->getNodeCount() == 0) {
				std::cout << "No nodes to delete" << std::endl;
			}
			else {
				std::cout << "Enter name of node for removal: ";
				std::getline(std::cin, input);
				input = trim(input);
				if (isNameGood(input)) {
					int retVal = graph->removeNode(input);
					if (retVal == 0) {
						std::cout << "Node \"" << input << "\" removed" << std::endl;
						searched = false;
					}
					else {
						std::cout << "No node with that name" << std::endl;
					}
				}
				else {
					std::cout << "Invalid name" << std::endl;
				}
			}
		}
		else if (num == 3) {
			std::cout << "Enter names of nodes to be connected: ";
			std::string input1, input2;
			std::cin >> input1 >> input2;
			std::cin.ignore();
			input1 = trim(input1);
			input2 = trim(input2);
			if (isNameGood(input1) && isNameGood(input2)) {
				int retVal = graph->setEdge(input1, input2);
				if (retVal == 1) {
					std::cout << "Edge \"" << input1 << "\" - \"" << input2 << "\" added" << std::endl;
					searched = false;
				}
				else if (retVal == 0) {
					std::cout << "Edge already exists" << std::endl;
				}
				else {
					std::cout << "Those nodes don't exist" << std::endl;
				}
			}
			else {
				std::cout << "Invalid name" << std::endl;
			}
		}
		else if (num == 4) {
			std::cout << "Enter names of nodes to be disconnected: ";
			std::string input1, input2;
			std::cin >> input1 >> input2;
			std::cin.ignore();
			input1 = trim(input1);
			input2 = trim(input2);
			if (isNameGood(input1) && isNameGood(input2)) {
				int retVal = graph->removeEdge(input1, input2);
				if (retVal == 1) {
					std::cout << "Edge \"" << input1 << "\" - \"" << input2 << "\" removed" << std::endl;
					searched = false;
				}
				else if (retVal == 0) {
					std::cout << "Edge doesn't exist" << std::endl;
				}
				else {
					std::cout << "Those nodes don't exist" << std::endl;
				}
			}
			else {
				std::cout << "Invalid name" << std::endl;
			}
		}
		else if (num == 5) {
			std::cout << "Enter name of node to be the begin node: ";
			std::getline(std::cin, input);
			input = trim(input);
			if (isNameGood(input)) {
				int retVal = graph->setStartNode(input);
				if (retVal == 0) {
					std::cout << "Node \"" << input << "\" set as start node" << std::endl;
					searched = false;
				}
				else {
					std::cout << "Node \"" << input << "\" doesn't exist" << std::endl;
				}
			}
			else {
				std::cout << "Invalid name" << std::endl;
			}
		}
		else if (num == 6) {
			graph->BreadthFirstSearch();
			std::cout << "Search complete" << std::endl;
			searched = true;
		}
		else if (num == 7) {
			graph->DepthFirstSearch();
			std::cout << "Search complete" << std::endl;
			searched = true;
		}
		else if (num == 8) {
			std::cout << "Enter name of node for shortest path: ";
			std::getline(std::cin, input);
			input = trim(input);
			if (isNameGood(input)) {
				int retVal = graph->setEndNode(input);
				if (retVal == 0) {
					retVal = graph->drawGraph();
					if (retVal == 1) {
						std::cout << "Start node not set" << std::endl;
					}
					else if (retVal == 2) {
						if (searched) {
							std::cout << "Node \"" << input << "\" cannot be reached" << std::endl;
						}
						else {
							std::cout << "Graph must be searched first" << std::endl;
						}
					}
				}
				else {
					std::cout << "Node \"" << input << "\" doesn't exist" << std::endl;
				}
			}
			else {
				std::cout << "Invalid name" << std::endl;
			}
		}
		else if (num == 9) {
			std::cout << "Enter filename to save to: ";
			std::getline(std::cin, input);
			input = trim(input);
			if (input == "abort") {
				std::cout << "Saving to file aborted" << std::endl;
			}
			else {
				if (isNameGood(input)) {
					graph->save(input);
				}
				else {
					std::cout << "Invalid name" << std::endl;
				}
			}
		}
		else if (num == 10) {
			std::cout << "Enter filename to load from: ";
			std::getline(std::cin, input);
			input = trim(input);
			if (input == "abort") {
				std::cout << "Loading from file aborted" << std::endl;
			}
			else {
				if (isNameGood(input)) {
					int retVal = graph->load(input);
					if (retVal == 1) {
						std::cout << "Failed to open file" << std::endl;
					}
					else {
						std::cout << "Graph loaded" << std::endl;
					}
				}
				else {
					std::cout << "Invalid name" << std::endl;
				}
			}
		}
		else if (num == 11) {
			graph->printNames();
		}
		else if (num == 12) {
			graph->showNeighboursByName();
		}
		else if (num == 13) {
			if (graph->Bipartite()) {
				std::cout << "It is bipartite" << std::endl;
			}
			else {
				std::cout << "It isn't bipartite" << std::endl;
			}
		}
		else if (num == 14) {
			std::cout << "Enter name of node to block: ";
			std::getline(std::cin, input);
			input = trim(input);
			if (isNameGood(input)) {
				int retVal = graph->blockNode(input);
				if (retVal == 0) {
					std::cout << "Node \"" << input << "\" is blocked" << std::endl;
					searched = false;
				}
				else {
					std::cout << "Node \"" << input << "\" doesn't exist" << std::endl;
				}
			}
			else {
				std::cout << "Invalid name" << std::endl;
			}
		}
		else if (num == 15) {
			std::cout << "Enter name of node to unblock: ";
			std::getline(std::cin, input);
			input = trim(input);
			if (isNameGood(input)) {
				int retVal = graph->removeBlock(input);
				if (retVal == 0) {
					std::cout << "Node \"" << input << "\" is unblocked" << std::endl;
					searched = false;
				}
				else {
					std::cout << "Node \"" << input << "\" doesn't exist" << std::endl;
				}
			}
			else {
				std::cout << "Invalid name" << std::endl;
			}
		}
		else {
			std::cout << "Invalid number" << std::endl;
		}

	}
	return 0;
}

int interactiveMode() {
	std::vector<Graph*> graphArr;
	std::vector<std::string> names;
	std::string input;
	while (true) {
		std::cout << "Enter input" << std::endl;

		if (graphArr.size()) {
			std::cout << "Graphs: " << graphArr[0]->returnGraphCount() << std::endl;
		}

		for (unsigned int i = 0; i < names.size(); i++) {
			std::cout << '[' << i + 1 << "]: ";
			if (graphArr[i]->graphType() == 2) {
				std::cout << "Graph ";
			}
			else if (graphArr[i]->graphType() == 1) {
				std::cout << "DiGraph ";
			}
			std::cout << names[i] << std::endl;
		}

		std::cout << "$>";
		std::getline(std::cin, input);
		input = trim(input);
		if (input == "exit") {
			return 0;
		}
		if (!input.compare(0, 6, "Graph ") || !input.compare(0, 6, "graph ")) {
			std::string name;
			name = input.substr(6);
			if (isNameGood(name)) {
				Graph* n = new Graph(0);
				graphArr.push_back(n);
				names.push_back(name);
			}
			else {
				std::cout << "Invalid name" << std::endl;
			}
			continue;
		}
		else if (!input.compare(0, 8, "DiGraph ") || !input.compare(0, 8, "digraph ")) {
			std::string name;
			name = input.substr(8);
			if (isNameGood(name)) {
				Graph* n = new DiGraph(0);
				graphArr.push_back(n);
				names.push_back(name);
			}
			else {
				std::cout << "Invalid name" << std::endl;
			}
			continue;
		}
		else {
			std::string reInput;
			for (std::size_t i = 0; i < input.size(); i++) {
				if (input[i] != ' ') {
					reInput += input[i];
				}
			}
			std::size_t foundPlus = reInput.find("+");
			std::size_t foundEqual = reInput.find("=");
			if (foundPlus != std::string::npos && foundEqual != std::string::npos && foundEqual < foundPlus) {
				std::string lvalue, firstop, secondop;
				lvalue = reInput.substr(0, foundEqual);
				firstop = reInput.substr(foundEqual + 1, foundPlus - foundEqual - 1);
				secondop = reInput.substr(foundPlus + 1);
				if (isNameGood(lvalue) && isNameGood(firstop) && isNameGood(secondop)) {
					std::size_t lb = names.size() + 1, fb = names.size() + 1, sb = names.size() + 1;
					for (std::size_t i = 0; i < names.size(); i++) {
						if (names[i] == lvalue) { lb = i; }
						else if (names[i] == firstop) { fb = i; }
						else if (names[i] == secondop) { sb = i; }
						if (lb != names.size() + 1 && fb != names.size() + 1 && sb != names.size() + 1) { break; }
					}
					if (lb != names.size() + 1 && fb != names.size() + 1 && sb != names.size() + 1) {
						if ((graphArr[lb]->graphType() == graphArr[fb]->graphType()) && (graphArr[lb]->graphType() == graphArr[sb]->graphType())) {
							solvePlusEqual(graphArr[lb], graphArr[fb], graphArr[sb]);
							continue;
						}
						else {
							std::cout << "Graphs incompatible" << std::endl;
							continue;
						}
					}
				}
			}
		}
		auto num = static_cast<std::size_t> (std::stoi(input));
		if (num >= 1 && num <= names.size()) {
			handleGraph(graphArr[num - 1], names[num - 1]);
		}
		else {
			std::cout << "Invalid input" << std::endl;
		}
	}
	return 0;
}
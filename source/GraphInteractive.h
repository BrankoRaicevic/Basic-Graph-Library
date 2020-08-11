#pragma once
#include <string>
#include "Graph.h"

std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v");

std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v");

std::string& trim(std::string& s, const char* t = " \t\n\r\f\v");

int solvePlusEqual(Graph* g1, Graph* g2, Graph* g3);

int solvePlusEqual(DiGraph* g1, DiGraph* g2, DiGraph* g3);

bool isNameGood(const std::string& name);

int handleGraph(Graph* graph, const std::string& name);

int interactiveMode();
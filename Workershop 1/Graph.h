#ifndef GRAPH_H
#define GRAPH_H

#include<string>

class Graph
{
public:
	Graph();

	void graphHorizontal(int realNumber[], std::string object[], std::string x_axis, std::string y_axis, int arraySize);
	void graphVertical(int realNumber[], int arraySize, std::string x_axis, std::string y_axis);
	void graphVertical(int realNumber[], std::string object[], int arraySize, std::string x_axis, std::string y_axis);
	void graphVerticalBig(int realNumber[], std::string object[], int arraySize, std::string x_axis, std::string y_axis);

	void graphTable(int realNumber[], std::string object[], std::string tittle, int arraySize,int width);
	void graphTable(int realNumber[], std::string object[], std::string tittle, std::string tittle2, int arraySize, int width);

	void decimalPoint(double num, int decimal);
};
#endif

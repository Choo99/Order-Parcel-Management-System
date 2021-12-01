#include "Graph.h"

#include<string>
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

Graph::Graph(){}

//draw a horizontal graph
void Graph::graphHorizontal(int realNumber[], string object[], string x_axis, string  y_axis, int arraySize)
{
	int blockNumber[100] = {};
	int largest = 0;
	double total = 0;

	//find the largest number and total number in array
	for (int i = 0; i < arraySize; i++)
	{
		if (realNumber[i] > largest)
		{
			largest = realNumber[i];
		}
		total += realNumber[i];
	}


	//calculate number of digit hold by higest number
	string check = to_string(largest);
	int x = check.length();

	//round up the higest number to higest block of y-axis
	int highestBlock = ceil((largest / pow(10, x - 2))) * pow(10, x - 2);
	//calculate value of one single block
	//double block = highestBlock * 0.01;
	double block = highestBlock * 0.00625;

	//convert real number to block number by divide real number with value of single block 
	for (int i = 0; i < arraySize; i++)
	{
		blockNumber[i] = realNumber[i] / block;
	}

	//display of graph
	int k = 0;
	cout << x_axis << endl;
	cout << "  " << char(30) << endl;

	//number of variable in x-axis can control by numberOfElement
	//every 2 blocks will display one variable
	for (int i = 0; i < arraySize * 2 - 1; i++)
	{
		cout << "  ";
		cout << char(179);
		if (i % 2 == 0)
		{
			//display block number in each array
			for (int j = 0; j < blockNumber[k]; j++)
			{
				cout << char(219);
			}
			//display percentage of variable
			cout << " " << object[k] << " (";
			//cout << setprecision(4) << realNumber[k] / total * 100 << "%)";
			decimalPoint(realNumber[k] / total * 100, 2);
			cout << "%)";
			k++;
		}

		cout << endl;
	}

	cout << "  " << char(179) << endl;
	cout << "  " << char(192);
	for (int i = 2; i <= 165; i++)
	{
		if (i % 32 == 1)
		{
			cout << char(197);
		}
		else
		{
			cout << char(196);
		}
	}
	cout << char(16) << " " << y_axis;
	cout << endl;
	//display value of every 20 blocks
	cout << "  0" <<  right << setw(33) << setfill(' ')
		<< highestBlock * 0.2 << setw(32) << setfill(' ')
		<< highestBlock * 0.4 << setw(32) << setfill(' ')
		<< highestBlock * 0.6 << setw(32) << setfill(' ')
		<< highestBlock * 0.8 << setw(32) << setfill(' ')
		<< highestBlock << endl;

}

void Graph::graphVertical(int realNumber[], int arraySize,string x_axis,string y_axis)
{
	int blockNumber[100] = {};
	int largest = 0;
	double total = 0;

	//find the largest number and total number in array
	for (int i = 0; i < arraySize; i++)
	{
		if (realNumber[i] > largest)
		{
			largest = realNumber[i];
		}
		total += realNumber[i];
	}

	//calculate number of digit hold by higest number
	string check = to_string(largest);
	int x = check.length();

	//round up the higest number to higest block of y-axis
	int highestBlock = ceil((largest / pow(10, x - 2))) * pow(10, x - 2);
	//calculate value of one single block
	double block = highestBlock * 0.05;

	//convert real number to block number by divide real number with value of single block 
	for (int i = 0; i < arraySize; i++)
	{
		blockNumber[i] = realNumber[i] / block;
	}

	int block5[5] = { highestBlock * 1,highestBlock * 0.8, highestBlock * 0.6, highestBlock * 0.4, highestBlock * 0.2 };

	string month[12] = { "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec" };

	//display graph
	cout << setw(15) << right << setfill(' ') << x_axis << endl;
	cout << setw(10) << setfill(' ') << char(30) << endl;
	cout << setw(10) << setfill(' ');
	cout << char(179);
	cout << "      ";
	/*for (int j = 0; j < arraySize; j++)
	{
		cout << month[j];
		cout<<"       ";
	}*/
	cout << endl;
	int count = 0;
	for (int i = 20; i > 0; i--)
	{

		if (i % 4 == 0)
		{
			cout << setw(9) << setfill(' ');
			cout << block5[count];
			count++;
		}
		else
		{
			cout << setw(10) << setfill(' ');
		}

		if (i % 4 == 0)
		{
			cout << char(197);
		}
		else
		{
			cout << char(179);
		}

		for (int j = 0; j < arraySize; j++)
		{
			
			if (blockNumber[j] + 1 == i)
			{
				cout << setw(7) << setfill(' ');
				//cout <<setprecision(1) << fixed<< realNumber[j] / total * 100 << "%";
				decimalPoint(realNumber[j] / total * 100, 1);
				cout << "%";
			}
			else if (blockNumber[j] == i)
			{
				cout << "     ";
				cout << char(219) << char(219) << char(219) << char(219) << char(219);
				blockNumber[j] = blockNumber[j] - 1;
			}
			else
			{
				cout << "     ";
				cout << "     ";
			}
		}
		cout << endl;
	}

	cout << setw(10) << char(192);
	for (int i = 2; i <= (arraySize * 5 + arraySize * 5 + 3); i++)
	{
			cout << char(196);
	}
	cout << char(16) << " " << y_axis;
	cout << endl;

	
}

void Graph::graphVertical(int realNumber[], string object[], int arraySize, string x_axis, string y_axis)
{
	int blockNumber[100] = {};
	int largest = 0;
	double total = 0;

	//find the largest number and total number in array
	for (int i = 0; i < arraySize; i++)
	{
		if (realNumber[i] > largest)
		{
			largest = realNumber[i];
		}
		total += realNumber[i];
	}

	//calculate number of digit hold by higest number
	string check = to_string(largest);
	int x = check.length();

	//round up the higest number to higest block of y-axis
	int highestBlock = ceil((largest / pow(10, x - 2))) * pow(10, x - 2);
	//calculate value of one single block
	double block = highestBlock * 0.05;

	//convert real number to block number by divide real number with value of single block 
	for (int i = 0; i < arraySize; i++)
	{
		blockNumber[i] = realNumber[i] / block;
	}

	int block5[5] = { highestBlock * 1,highestBlock * 0.8, highestBlock * 0.6, highestBlock * 0.4, highestBlock * 0.2 };


	//display graph
	cout << setw(15) << right << setfill(' ') << x_axis << endl;
	cout << setw(10) << setfill(' ') << char(30) << endl;
	cout << setw(10) << setfill(' ');
	cout << char(179);
	cout << "      ";
	for (int j = 0; j < arraySize; j++)
	{
		cout << object[j];
		cout<<"       ";
	}
	cout << endl;
	int count = 0;
	for (int i = 20; i > 0; i--)
	{

		if (i % 4 == 0)
		{
			cout << setw(9) << setfill(' ');
			cout << block5[count];
			count++;
		}
		else
		{
			cout << setw(10) << setfill(' ');
		}

		if (i % 4 == 0)
		{
			cout << char(197);
		}
		else
		{
			cout << char(179);
		}

		for (int j = 0; j < arraySize; j++)
		{

			if (blockNumber[j] + 1 == i)
			{
				cout << setw(9) << setfill(' ');
				decimalPoint(realNumber[j] / total * 100, 1);
				cout << "%";
			}
			else if (blockNumber[j] == i)
			{
				cout << "     ";
				cout << char(219) << char(219) << char(219) << char(219) << char(219);
				blockNumber[j] = blockNumber[j] - 1;
			}
			else
			{
				cout << "     ";
				cout << "     ";
			}
		}
		cout << endl;
	}

	cout << setw(10) << char(192);
	for (int i = 2; i <= (arraySize * 5 + arraySize * 5 + 3); i++)
	{
		cout << char(196);
	}
	cout << char(16) << " " << y_axis;
	cout << endl;
}

void Graph::graphVerticalBig(int realNumber[], string object[], int arraySize, string x_axis, string y_axis)
{
	int blockNumber[100] = {};
	int largest = 0;
	double total = 0;

	//find the largest number and total number in array
	for (int i = 0; i < arraySize; i++)
	{
		if (realNumber[i] > largest)
		{
			largest = realNumber[i];
		}
		total += realNumber[i];
	}

	//calculate number of digit hold by higest number
	string check = to_string(largest);
	int x = check.length();

	//round up the higest number to higest block of y-axis
	int highestBlock = ceil((largest / pow(10, x - 2))) * pow(10, x - 2);
	//calculate value of one single block
	double block = highestBlock * 0.05;

	//convert real number to block number by divide real number with value of single block 
	for (int i = 0; i < arraySize; i++)
	{
		blockNumber[i] = realNumber[i] / block;
	}

	int block5[5] = { highestBlock * 1,highestBlock * 0.8, highestBlock * 0.6, highestBlock * 0.4, highestBlock * 0.2 };


	//display graph
	cout << setw(11) << right << setfill(' ') << x_axis << endl;
	cout << setw(10) << setfill(' ') << char(30) << endl;
	cout << setw(10) << setfill(' ');
	cout << char(179);

	cout << endl;
	int count = 0;
	for (int i = 20; i > 0; i--)
	{

		if (i % 4 == 0)
		{
			cout << setw(9) << setfill(' ');
			cout << block5[count];
			count++;
		}
		else
		{
			cout << setw(10) << setfill(' ');
		}

		if (i % 4 == 0)
		{
			cout << char(197);
		}
		else
		{
			cout << char(179);
		}

		for (int j = 0; j < arraySize; j++)
		{
			cout << " ";
			if (blockNumber[j] + 1 == i)
			{
				cout << setw(2) << setfill(' ');
				decimalPoint(realNumber[j] / total * 100, 1);
				cout << "%";
			}
			else if (blockNumber[j] == i)
			{
				cout << char(219) << char(219) << char(219) << char(219) << char(219);
				blockNumber[j] = blockNumber[j] - 1;
			}
			else
			{
				cout << "     ";
			}
		}
		cout << endl;
	}

	cout << setw(10) << char(192);
	for (int i = 2; i <= (arraySize * 5 + arraySize + 3); i++)
	{
		cout << char(196);
	}
	cout << char(16) << " " << y_axis;
	cout << endl;

	cout << setw(11) << setfill(' ');
	for (int j = 0; j < arraySize; j++)
	{
		cout << " ";
		cout << object[j];
	}
	cout << endl;
}

void Graph::graphTable(int realNumber[], string object[], string tittle, int arraySize, int width)
{
	double total = 0;
	//calculate total number in array
	for (int i = 0; i < arraySize; i++)
	{
		total += realNumber[i];
	}

	cout << setw(width) << setfill(' ') << char(218) << setw(30) << setfill(char(196)) << char(194) << setw(30) << setfill(char(196)) << char(191) << endl;
	cout << setw(width) << setfill(' ') << char(179) << setw(29) << left << setfill(' ') << "Total" << char(179) << setw(29) << setfill(' ') << total << char(179) << endl;
	cout << setw(width) << right << setfill(' ') << char(192) << setw(30) << right << setfill(char(196)) << char(193) << setw(30) << setfill(char(196)) << char(217) << endl;
	//display table
	cout <<setw(width)<<setfill(' ')<< char(218) << setw(30) << setfill(char(196)) << char(194)
		 <<setw(15) << setfill(char(196)) << char(194) 
		 << setw(15) << setfill(char(196)) << char(191) << endl;

	cout << setw(width) << setfill(' ') << char(179) << setw(29) << left << setfill(' ') << tittle << char(179)
	     <<setw(14) << setfill(' ') << "Sales(RM)" << char(179) 
		 << setw(14) << setfill(' ') << "Percentage(%)" << char(179) << endl;

	int i = 0;
	while (i < arraySize)
	{
		cout <<right<< setw(width) << setfill(' ') << char(195) << setw(30) << right << setfill(char(196)) << char(197)
			 << setw(15) << setfill(char(196)) << char(197)
			 << setw(15) << setfill(char(196)) << char(180) << endl;

		cout << setw(width) << setfill(' ') << char(179) << setw(29) << left << setfill(' ') << object[i] << char(179)
			 << setw(14) << setfill(' ') << realNumber[i] << char(179)
			 << setw(14) << setfill(' ') << realNumber[i] /total * 100 << char(179) << endl;

		i++;
	}

	cout << setw(width)<<right << setfill(' ') << char(192) << setw(30) << right << setfill(char(196)) << char(193)
		 <<setw(15) << setfill(char(196)) << char(193) 
		 <<setw(15) << setfill(char(196)) << char(217) << endl;

}

void Graph::graphTable(int realNumber[], string object[], string tittle, string tittle2, int arraySize, int width)
{
	double total = 0;
	//calculate total number in array
	for (int i = 0; i < arraySize; i++)
	{
		total += realNumber[i];
	}

	cout << setw(width) << setfill(' ') << char(218) << setw(30) << setfill(char(196)) << char(194) << setw(30) << setfill(char(196)) << char(191) << endl;
	cout << setw(width) << setfill(' ') << char(179) << setw(29) << left << setfill(' ') << "Total" << char(179) << setw(29) << setfill(' ') << total << char(179) << endl;
	cout << setw(width) << right << setfill(' ') << char(192) << setw(30) << right << setfill(char(196)) << char(193) << setw(30) << setfill(char(196)) << char(217) << endl;
	//display table
	cout << setw(width) << setfill(' ') << char(218) << setw(30) << setfill(char(196)) << char(194)
		<< setw(16) << setfill(char(196)) << char(194)
		<< setw(15) << setfill(char(196)) << char(191) << endl;

	cout << setw(width) << setfill(' ') << char(179) << setw(29) << left << setfill(' ') << tittle << char(179)
		<< setw(15) << setfill(' ') << tittle2 << char(179)
		<< setw(14) << setfill(' ') << "Percentage(%)" << char(179) << endl;

	int i = 0;
	while (i < arraySize)
	{
		cout << right << setw(width) << setfill(' ') << char(195) << setw(30) << right << setfill(char(196)) << char(197)
			<< setw(16) << setfill(char(196)) << char(197)
			<< setw(15) << setfill(char(196)) << char(180) << endl;

		cout << setw(width) << setfill(' ') << char(179) << setw(29) << left << setfill(' ') << object[i] << char(179)
			<< setw(15) << setfill(' ') << realNumber[i] << char(179)
			<< setw(14) << setfill(' ') << realNumber[i] / total * 100 << char(179) << endl;

		i++;
	}

	cout << setw(width) << right << setfill(' ') << char(192) << setw(30) << right << setfill(char(196)) << char(193)
		<< setw(16) << setfill(char(196)) << char(193)
		<< setw(15) << setfill(char(196)) << char(217) << endl;

}

void Graph::decimalPoint(double num, int decimal)
{
	cout << (int)num << "." << (int)((num - (int)num) * pow(10, decimal));
}
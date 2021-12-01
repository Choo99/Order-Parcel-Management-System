#include<string>
using namespace std;

#ifndef ORDERDETAIL_H
#define ORDERDETAIL_H

class OrderDetail
{
public:
	int OrderID, ProductID;
	int OriginalPrice, Discount, Quantity, Subtotal;
};
#endif
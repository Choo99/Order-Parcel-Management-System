#include<String>
using namespace std;

#ifndef ORDER_H
#define ORDER_H

class Order
{
public:
	int OrderID = 0;
	string OrderAddress = "",OrderPhoneNo = "", OrderDate = "", OrderStatus = "", OrderPayment = "";
	int TotalPrice = 0;
};
#endif
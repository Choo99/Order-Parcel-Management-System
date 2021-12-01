#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

#include <mysql/jdbc.h>
#include "userInputManager.h"
using namespace sql;

#include "DatabaseConnection.h"
#include "Order.h"
#include "Product.h"
#include "OrderDetail.h"
#include "OrderDetailManager.h"

OrderDetailManager::OrderDetailManager() {};

//insert a new order detail
//product id and order id will pass from order and also the quantity
int OrderDetailManager::insertOrderDetail(Order* order, Product* product, OrderDetail* orderDetail)
{
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("INSERT INTO OrderDetail VALUES(?,?,?,?,?)");

	ps->setUInt(1, order->OrderID);
	ps->setUInt(2, product->ProductID);
	ps->setUInt(3, orderDetail->Quantity);
	ps->setUInt(4, product->ProductSoldPrice);
	ps->setUInt(5, orderDetail->Discount);

	//execute query
	int status = ps->executeUpdate();

	delete ps;

	return status;
}

//view table of order detail(worker view)
void OrderDetailManager::viewOrderDetail()
{
	DatabaseConnection dbConn;
	PreparedStatement* ps;

	//join table OrderDetail,Order and Product
	ps = dbConn.prepareStatement
	("SELECT `order`.OrderID,orderdetail.ProductId,ProductName,OrderDate,OrderAddress,Quantity,OrderStatus FROM OrderDetail INNER JOIN Product"
		" ON OrderDetail.ProductID = Product.ProductID INNER JOIN `order`"
		" ON OrderDetail.OrderID = `order`.OrderID WHERE OrderStatus NOT IN('Completed', 'Cancelled') ORDER BY OrderDetail.OrderID ");

	ResultSet* rs;
	rs = ps->executeQuery();

	if (!rs->rowsCount())
	{
		cout << "No precessing order's detail can be found...\n\n";
		system("pause");
		system("cls");
		return;
	}

	//header of table
	cout << char(218) << setw(10) << setfill(char(196)) << char(194) <<
		setw(10) << setfill(char(196)) << char(194) <<
		setw(30) << setfill(char(196)) << char(194) <<
		setw(11) << setfill(char(196)) << char(194) <<
		setw(50) << setfill(char(196)) << char(194) <<
		setw(9) << setfill(char(196)) << char(194) <<
		setw(13) << setfill(char(196)) << char(191) << endl;

	cout << char(179) << setw(9) << left << setfill(' ') << "OrderID" << char(179) <<
		setw(9) << setfill(' ') << "ProductId" << char(179) <<
		setw(29) << setfill(' ') << "ProductName" << char(179) <<
		setw(10) << setfill(' ') << "OrderDate" << char(179) <<
		setw(49) << setfill(' ') << "OrderAddress" << char(179) <<
		setw(8) << setfill(' ') << "Quantity" << char(179) <<
		setw(12) << setfill(' ') << "OrderStatus" << char(179) << endl;
	
	while (rs->next())
	{
		cout << char(195) << setw(10) << right << setfill(char(196)) << char(197) <<
			setw(10) << setfill(char(196)) << char(197) <<
			setw(30) << setfill(char(196)) << char(197) <<
			setw(11) << setfill(char(196)) << char(197) <<
			setw(50) << setfill(char(196)) << char(197) <<
			setw(9) << setfill(char(196)) << char(197) <<
			setw(13) << setfill(char(196)) << char(180) << endl;

		//retrieve data from database
		cout << char(179) << setw(9) << left << setfill(' ') << rs->getInt(1) << char(179) <<
			setw(9) << setfill(' ') << rs->getInt(2) << char(179) <<
			setw(29) << setfill(' ') << rs->getString(3) << char(179) <<
			setw(10) << setfill(' ') << rs->getString(4) << char(179) <<
			setw(49) << setfill(' ') << rs->getString(5) << char(179) <<
			setw(8) << setfill(' ') << rs->getInt(6) << char(179) <<
			setw(12) << setfill(' ') << rs->getString(7) << char(179) << endl;
	}

	cout << char(192) << setw(10) << right << setfill(char(196)) << char(193) <<
		setw(10) << setfill(char(196)) << char(193) <<
		setw(30) << setfill(char(196)) << char(193) <<
		setw(11) << setfill(char(196)) << char(193) <<
		setw(50) << setfill(char(196)) << char(193) <<
		setw(9) << setfill(char(196)) << char(193) <<
		setw(13) << setfill(char(196)) << char(217) << endl;

	delete rs;
	delete ps;
}

//view table of order detail (manager view)
void OrderDetailManager::viewOrderDetailPrice()
{
	DatabaseConnection dbConn;
	PreparedStatement* ps;

	//join table OrderDetail,Order and Product
	ps = dbConn.prepareStatement("SELECT OrderDetail.OrderID,orderDetail.productID,ProductName,OrderDate,OriginalPrice,Discount,Quantity,(OriginalPrice-Discount)*Quantity, orderStatus FROM OrderDetail"
		" INNER JOIN `order` ON OrderDetail.OrderID = `order`.OrderID"
		" INNER JOIN Product ON OrderDetail.ProductID = Product.ProductID WHERE OrderStatus NOT IN('Cancelled','Completed') ORDER BY OrderDetail.OrderID");
	//view order with total price(manager view)");

	ResultSet* rs;
	rs = ps->executeQuery();

	if (!rs->rowsCount())
	{
		cout << "No processing order's detail can be found...\n\n";
		system("pause");
		system("cls");
		return;
	}

	//header of table
	cout << char(218) << setw(10) << setfill(char(196)) << char(194) <<
		setw(10) << setfill(char(196)) << char(194) <<
		setw(30) << setfill(char(196)) << char(194) <<
		setw(11) << setfill(char(196)) << char(194) <<
		setw(15) << setfill(char(196)) << char(194) <<
		setw(9) << setfill(char(196)) << char(194) <<
		setw(9) << setfill(char(196)) << char(194) <<
		setw(13) << setfill(char(196)) << char(194) <<
		setw(13) << setfill(char(196)) << char(191) << endl;

	cout << char(179) << setw(9) << left << setfill(' ') << "OrderID" << char(179) <<
		setw(9) << setfill(' ') << "ProductID" << char(179) <<
		setw(29) << setfill(' ') << "ProductName" << char(179) <<
		setw(10) << setfill(' ') << "OrderDate" << char(179) <<
		setw(14) << setfill(' ') << "Original Price" << char(179) <<
		setw(8) << setfill(' ') << "Discount" << char(179) <<
		setw(8) << setfill(' ') << "Quantity" << char(179) <<
		setw(12) << setfill(' ') << "Subtotal" << char(179) <<
		setw(12) << setfill(' ') << "OrderStatus" << char(179) << endl;

	while (rs->next())
	{
		cout << char(195) << setw(10) << right << setfill(char(196)) << char(197) <<
			setw(10) << setfill(char(196)) << char(197) <<
			setw(30) << setfill(char(196)) << char(197) <<
			setw(11) << setfill(char(196)) << char(197) <<
			setw(15) << setfill(char(196)) << char(197) <<
			setw(9) << setfill(char(196)) << char(197) <<
			setw(9) << setfill(char(196)) << char(197) <<
			setw(13) << setfill(char(196)) << char(197) <<
			setw(13) << setfill(char(196)) << char(180) << endl;

		//retrieve data from database
		cout << char(179) << setw(9) << left << setfill(' ') << rs->getInt(1) << char(179) <<
			setw(9) << setfill(' ') << rs->getInt(2) << char(179) <<
			setw(29) << setfill(' ') << rs->getString(3) << char(179) <<
			setw(10) << setfill(' ') << rs->getString(4) << char(179) <<
			setw(14) << setfill(' ') << rs->getDouble(5) << char(179) <<
			setw(8) << setfill(' ') << rs->getDouble(6) << char(179) <<
			setw(8) << setfill(' ') << rs->getDouble(7) << char(179) <<
			setw(12) << setfill(' ') << rs->getDouble(8) << char(179) <<
			setw(12) << setfill(' ') << rs->getString(9) << char(179) << endl;
	}

	cout << char(192) << setw(10) << right << setfill(char(196)) << char(193) <<
		setw(10) << setfill(char(196)) << char(193) <<
		setw(30) << setfill(char(196)) << char(193) <<
		setw(11) << setfill(char(196)) << char(193) <<
		setw(15) << setfill(char(196)) << char(193) <<
		setw(9) << setfill(char(196)) << char(193) <<
		setw(9) << setfill(char(196)) << char(193) <<
		setw(13) << setfill(char(196)) << char(193) <<
		setw(13) << setfill(char(196)) << char(217) << endl;

	delete rs;
	delete ps;
}

//update  quantity column 
bool OrderDetailManager::updateQuantity(OrderDetail* orderDetail)
{
	DatabaseConnection dbConn;
	PreparedStatement* ps;
																			//set actual product quantity to (original quantity + Order quantity that change quantity - new quantity of order)
	ps = dbConn.prepareStatement("Update Product SET ProductQuantity = ((SELECT ProductQuantity FROM Product WHERE ProductID = ?) + (SELECT Quantity FROM OrderDetail WHERE OrderID = ? AND ProductID =?) - ?) WHERE ProductID = ?");

	ps->setInt(1, orderDetail->ProductID);
	ps->setInt(2, orderDetail->OrderID);
	ps->setInt(3, orderDetail->ProductID);
	ps->setInt(4, orderDetail->Quantity);
	ps->setInt(5, orderDetail->ProductID);
	ps->executeUpdate();

	ps = dbConn.prepareStatement("Update OrderDetail SET Quantity = ? WHERE OrderID = ? AND ProductID = ?");

	ps->setInt(1, orderDetail->Quantity);
	ps->setInt(2, orderDetail->OrderID);
	ps->setInt(3, orderDetail->ProductID);

	bool status = ps->executeUpdate();
	return status;
}

//update original price column
bool OrderDetailManager::updateOriginalPrice(OrderDetail* orderDetail)
{
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("Update OrderDetail SET OriginalPrice = ? WHERE OrderID = ? AND ProductID = ?");

	ps->setInt(1, orderDetail->OriginalPrice);
	ps->setInt(2, orderDetail->OrderID);
	ps->setInt(3, orderDetail->ProductID);

	bool status = ps->executeUpdate();
	return status;
}

//update discount column
bool OrderDetailManager::updateDiscount(OrderDetail* orderDetail)
{
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("Update OrderDetail SET Discount = ? WHERE OrderID = ? AND ProductID = ?");

	ps->setInt(1, orderDetail->Discount);
	ps->setInt(2, orderDetail->OrderID);
	ps->setInt(3, orderDetail->ProductID);

	bool status = ps->executeUpdate();
	return status;
}

//search order detail with order id
void OrderDetailManager::searchOrderDetailPrice(Order* order)
{
	DatabaseConnection dbConn;
	PreparedStatement* ps;
	ps = dbConn.prepareStatement("SELECT OrderDetail.OrderID,OrderDetail.productID,ProductName,OrderDate,OriginalPrice,Discount,Quantity,(OriginalPrice-Discount)*Quantity AS Subtotal FROM OrderDetail"
		" INNER JOIN `order` ON OrderDetail.OrderID = `order`.OrderID"
		" INNER JOIN Product ON OrderDetail.ProductID = Product.ProductID WHERE orderDetail.OrderID = ?");

	ps->setInt(1, order->OrderID);
	ResultSet* rs = ps->executeQuery();

	cout << char(218) << setw(10) << setfill(char(196)) << char(194) <<
		setw(10) << setfill(char(196)) << char(194) <<
		setw(30) << setfill(char(196)) << char(194) <<
		setw(11) << setfill(char(196)) << char(194) <<
		setw(15) << setfill(char(196)) << char(194) <<
		setw(9) << setfill(char(196)) << char(194) <<
		setw(9) << setfill(char(196)) << char(194) <<
		setw(13) << setfill(char(196)) << char(191) << endl;

	cout << char(179) << setw(9) << left << setfill(' ') << "OrderID" << char(179) <<
		setw(9) << setfill(' ') << "ProductId" << char(179) <<
		setw(29) << setfill(' ') << "ProductName" << char(179) <<
		setw(10) << setfill(' ') << "OrderDate" << char(179) <<
		setw(14) << setfill(' ') << "Original Price" << char(179) <<
		setw(8) << setfill(' ') << "Discount" << char(179) <<
		setw(8) << setfill(' ') << "Quantity" << char(179) <<
		setw(12) << setfill(' ') << "Subtotal" << char(179) << endl;

	while (rs->next())
	{
		//count++;
		cout << char(195) << setw(10) << right << setfill(char(196)) << char(197) <<
			setw(10) << setfill(char(196)) << char(197) <<
			setw(30) << setfill(char(196)) << char(197) <<
			setw(11) << setfill(char(196)) << char(197) <<
			setw(15) << setfill(char(196)) << char(197) <<
			setw(9) << setfill(char(196)) << char(197) <<
			setw(9) << setfill(char(196)) << char(197) <<
			setw(13) << setfill(char(196)) << char(180) << endl;

		//retrieve data from database
		cout << char(179) << setw(9) << left << setfill(' ') << rs->getInt(1) << char(179) <<
			setw(9) << setfill(' ') << rs->getInt(2) << char(179) <<
			setw(29) << setfill(' ') << rs->getString(3) << char(179) <<
			setw(10) << setfill(' ') << rs->getString(4) << char(179) <<
			setw(14) << setfill(' ') << rs->getDouble(5) << char(179) <<
			setw(8) << setfill(' ') << rs->getDouble(6) << char(179) <<
			setw(8) << setfill(' ') << rs->getDouble(7) << char(179) <<
			setw(12) << setfill(' ') << rs->getDouble(8) << char(179) << endl;
	}

	cout << char(192) << setw(10) << right << setfill(char(196)) << char(193) <<
		setw(10) << setfill(char(196)) << char(193) <<
		setw(30) << setfill(char(196)) << char(193) <<
		setw(11) << setfill(char(196)) << char(193) <<
		setw(15) << setfill(char(196)) << char(193) <<
		setw(9) << setfill(char(196)) << char(193) <<
		setw(9) << setfill(char(196)) << char(193) <<
		setw(13) << setfill(char(196)) << char(217) << endl;

	delete ps;
	delete rs;
}
//this function execute the query of searching 
//needed information is passing by parameter
int OrderDetailManager::searchOrderDetailPrice(OrderDetail* orderDetail, string name, string date, string maxDate, int priceRange, int discountRange, int quantityRange, int subtotalRange)
{
	DatabaseConnection dbConn;
	PreparedStatement* ps;
	//these string storing the string version of integer data
	string orderID, productID, quantity, originalPrice, discount, subtotal;

	//convert all int information to string 
	//By default, int member will set to -1 means that integer will not use in this searching 
	//therefore integer member with -1 will not connvert to string and set NULL to the string version
	if (orderDetail->OrderID != -1)
	{
		orderID = to_string(orderDetail->OrderID);
	}
	if (orderDetail->ProductID != -1)
	{
		productID = to_string(orderDetail->ProductID);
	}
	if (orderDetail->OriginalPrice != -1)
	{
		originalPrice = to_string(orderDetail->OriginalPrice);
	}
	if (orderDetail->Discount != -1)
	{
		discount = to_string(orderDetail->Discount);
	}
	if (orderDetail->Quantity != -1)
	{
		quantity = to_string(orderDetail->Quantity);
	}
	if (orderDetail->Subtotal != -1)
	{
		subtotal = to_string(orderDetail->Subtotal);
	}

	//this is a template that can use to search by every column in the table
	//every unuse column will set to null and will not affect the result
	//'OR' after the condition enable the NULL condition occur 
	//This template also enable user to search within a range with the integer member
	ps = dbConn.prepareStatement
	("SELECT OrderDetail.OrderID,OrderDetail.productID,ProductName,OrderDate,OriginalPrice,Discount,Quantity,(OriginalPrice-Discount)*Quantity AS Subtotal FROM OrderDetail"
		" INNER JOIN `order` ON OrderDetail.OrderID = `order`.OrderID"
		" INNER JOIN Product ON OrderDetail.ProductID = Product.ProductID WHERE"
		" OrderStatus NOT IN('Cancelled','Completed') AND"
		" (OrderDetail.OrderID = ? or ? ='') AND"
		" (OrderDetail.ProductID = ? or ? ='') AND"
		" (ProductName LIKE ? or ? LIKE '') AND"
		" (OrderDate BETWEEN ? AND ? OR ? BETWEEN '' AND '') AND"
		" (OriginalPrice BETWEEN ? AND ? + ? OR ? BETWEEN '' AND '') AND"
		" (Quantity BETWEEN  ? AND ? + ? OR ? BETWEEN '' AND '') AND"
		" (Discount BETWEEN ? AND ? + ? OR ? BETWEEN '' AND '') AND"
		" ((OriginalPrice-Discount)*Quantity BETWEEN ? AND ? + ? OR ? BETWEEN '' AND '') ORDER BY OrderDetail.OrderID, OrderDetail.ProductID");
	int i = 1;
	//set condition to orderID
	ps->setString(1, orderID);
	//enable NULL condition
	ps->setString(2, orderID);

	//set condition to productID
	ps->setString(3, productID);
	//enable NULL condition
	ps->setString(4, productID);

	//set condition to name
	ps->setString(5, name);
	//name will automatically search by '%%' and more easier to find the target
	ps->setString(6, name);

	//set condition to date
	ps->setString(7, date);
	//set a maximum date within the range
	//maximum date will same as date when user don't want to search in range
	ps->setString(8, maxDate);
	//enable NULL condition
	ps->setString(9, date);

	//set condition to price
	ps->setString(10, originalPrice);
	//maximum price = base price + price range
	ps->setString(11, originalPrice);
	//price range will set to zero when user don't want to search in range
	ps->setInt(12, priceRange);
	//enable NULL condition
	ps->setString(13, originalPrice);

	// set condition to quantity
	ps->setString(14, quantity);
	//maximum quantity = base quantity + quantity range
	ps->setString(15, quantity);
	//quantity range will set to zero when user don't want to search in range
	ps->setInt(16, quantityRange);
	//enable NULL condition
	ps->setString(17, quantity);

	//set condition to discount 
	ps->setString(18, discount);
	//maximum discount = base discount + discount range
	ps->setString(19, discount);
	//discount range will set to zero when user don't want to search in range
	ps->setInt(20, discountRange);
	//enable NULL conditionn
	ps->setString(21, discount);

	//set subtotal condition
	ps->setString(22, subtotal);
	//maximum subtotal = base subtotal +subtotal range
	ps->setString(23, subtotal);
	//subtotal range will set to zero when user don't wannt to search in range
	ps->setInt(24, subtotalRange);
	//enable NULL condition
	ps->setString(25, subtotal);

	ResultSet* rs = ps->executeQuery();
	if (!rs->rowsCount())
	{
		return 0;
	}
	cout << char(218) << setw(10) << setfill(char(196)) << char(194) <<
		setw(10) << setfill(char(196)) << char(194) <<
		setw(30) << setfill(char(196)) << char(194) <<
		setw(11) << setfill(char(196)) << char(194) <<
		setw(15) << setfill(char(196)) << char(194) <<
		setw(9) << setfill(char(196)) << char(194) <<
		setw(9) << setfill(char(196)) << char(194) <<
		setw(13) << setfill(char(196)) << char(191) << endl;

	cout << char(179) << setw(9) << left << setfill(' ') << "OrderID" << char(179) <<
		setw(9) << setfill(' ') << "ProductId" << char(179) <<
		setw(29) << setfill(' ') << "ProductName" << char(179) <<
		setw(10) << setfill(' ') << "OrderDate" << char(179) <<
		setw(14) << setfill(' ') << "Original Price" << char(179) <<
		setw(8) << setfill(' ') << "Discount" << char(179) <<
		setw(8) << setfill(' ') << "Quantity" << char(179) <<
		setw(12) << setfill(' ') << "Subtotal" << char(179) << endl;

	while (rs->next())
	{
		//count++;
		cout << char(195) << setw(10) << right << setfill(char(196)) << char(197) <<
			setw(10) << setfill(char(196)) << char(197) <<
			setw(30) << setfill(char(196)) << char(197) <<
			setw(11) << setfill(char(196)) << char(197) <<
			setw(15) << setfill(char(196)) << char(197) <<
			setw(9) << setfill(char(196)) << char(197) <<
			setw(9) << setfill(char(196)) << char(197) <<
			setw(13) << setfill(char(196)) << char(180) << endl;

		//retrieve data from database
		cout << char(179) << setw(9) << left << setfill(' ') << rs->getInt(1) << char(179) <<
			setw(9) << setfill(' ') << rs->getInt(2) << char(179) <<
			setw(29) << setfill(' ') << rs->getString(3) << char(179) <<
			setw(10) << setfill(' ') << rs->getString(4) << char(179) <<
			setw(14) << setfill(' ') << rs->getDouble(5) << char(179) <<
			setw(8) << setfill(' ') << rs->getDouble(6) << char(179) <<
			setw(8) << setfill(' ') << rs->getDouble(7) << char(179) <<
			setw(12) << setfill(' ') << rs->getDouble(8) << char(179) << endl;
	}

	cout << char(192) << setw(10) << right << setfill(char(196)) << char(193) <<
		setw(10) << setfill(char(196)) << char(193) <<
		setw(30) << setfill(char(196)) << char(193) <<
		setw(11) << setfill(char(196)) << char(193) <<
		setw(15) << setfill(char(196)) << char(193) <<
		setw(9) << setfill(char(196)) << char(193) <<
		setw(9) << setfill(char(196)) << char(193) <<
		setw(13) << setfill(char(196)) << char(217) << endl;

	//counting row returned by query
	cout << "\n" << rs->rowsCount()<< " row(s) selected\n";

	delete ps;
	delete rs;

	return 1;
}

//This function enable user to choose the condition needed to search
//all choice from user will store in an array
//No repeated element storing in array
void OrderDetailManager::search()
{
	//string for error message
	string error;
	//set searchList to NUL
	int searchList[10] = { 0 };
	UserInputManager userInput;
	string choice;
	string again;
	//flag for checking repeated number
	bool flag = true;
	const int n = 9;

	while (1)
	{
		for (int i = 0; i <= n; i++)
		{
			while (1)
			{
				system("cls");
				//user selects from the list below and put into an array
				cout << "Please choose your needed condition\n";
				cout << "1. Order ID\n";
				cout << "2. Product ID\n";
				cout << "3. Product Name\n";
				cout << "4. Order Date\n";
				cout << "5. Price\n";
				cout << "6. Discount\n";
				cout << "7. Quantity\n";
				cout << "8. Subtotal \n";
				cout << "9. Quit\n";

				cout << "Your choice : ";
				//display a selected list of user
				for (int j = 0; j < n && searchList[j] != NULL; j++)
				{
					cout << searchList[j] << " ";
				}
				cout << endl;
				cout << "Enter :";
				cin >> setw(1) >> searchList[i];
				//if user is not keyin an integer
				if (!cin)
				{
					cout << "Invalid character! Please try again\n\n";
					system("pause");
					cin.clear();
					cin.ignore(INT_MAX, '\n');
					system("cls");
					continue;
				}
				//user input must in the range 1-9
				else if (searchList[i] < 1 || searchList[i] > n)
				{
					searchList[i] = NULL;
					cout << "Invalid number! Please try again\n\n";
					system("pause");
					system("cls");
					continue;
				}
				//checking for repeated element inside the array
				for (int j = 0; j < i; j++)
				{
					//set the flag to true in every loop
					flag = true;
					if (searchList[j] == searchList[i])
					{
						searchList[i] = NULL;
						cout << "Invalid repeated number! Please try again\n\n";
						//once repeated number found, set flag to false then quit
						flag = false;
						break;
					}
				}
				//flag will false when repeated number found
				if (!flag)
				{
					system("pause");
					system("cls");
					continue;
				}
				break;
			}
			if (searchList[i] == n)
			{
				cout << "\n";
				break;
			}
		}

		//display the selected choice from user
		cout << "Your choice:\t";
		for (int j = 0; j <= n && searchList[j] != NULL && searchList[j] != n; j++)
		{

			switch (searchList[j])
			{
			case 1:
				cout << "Order ID";
				break;
			case 2:
				cout << "Product ID";
				break;
			case 3:
				cout << "Product Name";
				break;
			case 4:
				cout << "OrderDate";
				break;
			case 5:
				cout << "Price";
				break;
			case 6:
				cout << "Discount";
				break;
			case 7:
				cout << "Quantity";
				break;
			case 8:
				cout << "Subtotal";
				break;
			default:
				break;
			}
			cout << "\n\t\t";
		}

		string message = "\nConfirm your selection? <Y/N> ";
		cin.ignore(INT_MAX, '\n');
		userInput.getChoice(message, choice, 'Y', 'N');

		if (choice[0] == 'Y')
		{
			system("cls");
			if (searchList[0] == n)
			{
				return;
			}

			searchInput(searchList);

			message = "Do you want to search again? <Y/N> : ";
			userInput.getChoice(message, again);
			if (again[0] == 'Y')
			{
				system("cls");
				for (int i = 0; i < n; i++)
				{
					searchList[i] = 0;
				}
				continue;
			}
		}
		else if (choice[0] == 'N')
		{
			system("cls");
			for (int i = 0; i < n; i++)
			{
				searchList[i] = 0;
			}
			continue;
		}
		system("cls");
		break;
	}
}

//This function asking the user input for the column needed for searching in workerSearch 
void OrderDetailManager::searchInput(int searchList[10])
{
	//set an enum list to improve readability
	enum list { orderID = 1, productID = 2, productName = 3, orderDate = 4, price = 5, discount = 6, quantity = 7, subtotal = 8, quit = 9 };
	string choice;
	//string for error message
	string error;
	string message;
	//checking for the search result
	int check;
	OrderDetail* orderDetail = new OrderDetail;
	UserInputManager user;

	//set all integer member to -1 by default
	orderDetail->OrderID = -1;
	orderDetail->ProductID = -1;
	orderDetail->Quantity = -1;
	orderDetail->OriginalPrice = -1;
	orderDetail->Discount = -1;
	orderDetail->Subtotal = -1;

	//maximum value using in range's search
	int maxPrice = 0;
	int maxDiscount = 0;
	int maxQuantity = 0;
	int maxSubtotal = 0;

	string name = "";
	string date = "";
	string maxDate = "";

	for (int i = 0; searchList[i] != 0; i++)
	{
		//read from the array one by one and user input needed information
		switch (searchList[i])
		{
		case orderID:
			//search for an order id
			user.drawTittle("Order ID", 9, 10);
			message = "Please enter an order ID : ";
			user.getInt(message, orderDetail->OrderID);
			cout << "\n";
			break;
		case productID:
			//search for a product id
			user.drawTittle("Product ID", 11, 10);
			message = "Please enter a product ID : ";
			user.getInt(message, orderDetail->ProductID);
			cout << "\n";
			break;
		case productName:
			//search for a product name
			user.drawTittle("Product Name", 13, 10);
			cout << "Please enter a product name : ";
			getline(cin, name);
			user.toNormalForm(name);
			cout << "\n";
			break;
		case orderDate:
			user.drawTittle("Order Date", 11, 10);
			while (1)
			{
				//search for a date either in range or single value
				message = "Search order date in range? <Y/N> : ";
				user.getChoice(message, choice, 'Y', 'N');
				//reset max value 
				maxDate = "";
				if (choice[0] == 'Y')
				{
					message = "Order Date From <YYYY-MM-DD> : ";
					user.getDate(message, date);
					message = "Order Date To <YYYY-MM-DD> : ";
					user.getDate(message, maxDate);
					if (!user.compareDate(date, maxDate))
					{
						continue;
					}
					cout << "\n";
					break;
				}
				//search for single value
				else if (choice[0] == 'N')
				{
					message = "\nPlease enter an order date <YYYY-MM-DD> : ";
					user.getDate(message, date);
					maxDate = date;
					cout << "\n";
					break;
				}
			}
			break;
		case price:
			//search price in either in range or single value
			user.drawTittle("Price", 6, 10);
			while (1)
			{
				message = "Search price in range? <Y/N> : ";
				user.getChoice(message, choice, 'Y', 'N');
				//reset max value 
				maxPrice = 0;
				if (choice[0] == 'Y')
				{
					message = "Price From : ";
					user.getInt(message, orderDetail->OriginalPrice);
					message = "Price To : ";
					user.getInt(message, maxPrice);
					if (!user.compare(orderDetail->OriginalPrice, maxPrice))
					{
						continue;
					}
					cout << "\n";
					break;
				}
				else if (choice[0] == 'N')
				{
					message = "Please enter a product price : ";
					user.getInt(message, orderDetail->OriginalPrice);
					maxPrice = orderDetail->OriginalPrice;
					cout << "\n";
					break;
				}
			}
			break;
		case discount:
			user.drawTittle("Discount", 9, 10);
			while (1)
			{
				message = "Search discount in range? <Y/N> : ";
				user.getChoice(message, choice, 'Y', 'N');
				//reset max value 
				maxDiscount = 0;
				if (choice[0] == 'Y')
				{
					message = "Discount From : ";
					user.getInt(message, orderDetail->Discount);
					message = "Discount To : ";
					user.getInt(message, maxDiscount);
					if (!user.compare(orderDetail->Discount, maxDiscount))
					{
						continue;
					}
					cout << "\n";
					break;
				}
				else if (choice[0] == 'N')
				{
					message = "Please enter a discount price : ";
					user.getInt(message, orderDetail->Discount);
					maxDiscount = orderDetail->Discount;
					cout << "\n";
					break;
				}
			}
			break;
		case quantity:
			user.drawTittle("Quantity", 9, 10);
			while (1)
			{
				message = "Search quantity in range? <Y/N> : ";
				user.getChoice(message, choice, 'Y', 'N');
				//reset max value 
				maxQuantity = 0;
				if (choice[0] == 'Y')
				{
					message = "Quantity From : ";
					user.getInt(message, orderDetail->Quantity);
					message = "Quantity To : ";
					user.getInt(message, maxQuantity);
					if (!user.compare(orderDetail->Quantity, maxQuantity))
					{
						continue;
					}
					break;
					cout << "\n";
				}
				else if (choice[0] == 'N')
				{
					message = "Please enter a quantity : ";
					user.getInt(message, orderDetail->Quantity);
					maxQuantity = orderDetail->Quantity;
					cout << "\n";
					break;
				}
			}
			break;
		case subtotal:
			user.drawTittle("Subtotal", 9, 10);
			while (1)
			{
				message = "Search subtotal in range? <Y/N> ";
				user.getChoice(message, choice, 'Y', 'N');
				//reset max value 
				maxSubtotal = 0;
				if (choice[0] == 'Y')
				{
					message = "Subtotal From : ";
					user.getInt(message, orderDetail->Subtotal);
					message = "Subtotal To : ";
					user.getInt(message, maxSubtotal);
					if (!user.compare(orderDetail->Subtotal, maxSubtotal))
					{
						continue;
					}
					cout << "\n";
					break;
				}
				else if (choice[0] == 'N')
				{
					message = "Please enter a subtotal : ";
					user.getInt(message, orderDetail->Subtotal);
					maxSubtotal = orderDetail->Subtotal;
					cout << "\n";
				}
				break;
			}
			break;
		case quit:
			break;
		}
	}
	int n1 = 20;
	int n2 = 30;
	//let user view before searching
	cout << "\n";
	//display '-' when not choosen
	if (orderDetail->OrderID == -1)
	{
		user.drawRowHead("Order ID", '-', n1, n2);
	}
	//display user selection 
	else if (orderDetail->OrderID != -1)
	{
		user.drawRowHead("Order ID", orderDetail->OrderID, n1, n2);
	}
	//display '-' when not choosen
	if (orderDetail->ProductID == -1)
	{
		user.drawRowBody("Product ID", '-', n1, n2);
	}
	//display user selection 
	else if (orderDetail->ProductID != -1)
	{
		user.drawRowBody("Product ID", orderDetail->ProductID, n1, n2);
	}
	//display '-' when not choosen
	if (name == "")
	{
		user.drawRowBody("Product Name", '-', n1, n2);
	}
	//display user selection 
	else if (name != "")
	{
		user.drawRowBody("Product Name", name, n1, n2);
	}
	//display '-' when not choosen
	if (date == "")
	{
		user.drawRowBody("Order Date", '-', n1, n2);
	}
	//display in range when user search in range
	else if (date != "" && maxDate != date)
	{
		user.drawRowBody("Order Date", date + " To " + maxDate, n1, n2);
	}
	//display user selection
	else if (maxDate == date)
	{
		user.drawRowBody("Order Date", date, n1, n2);
	}
	//display '-' when not choose
	if (orderDetail->OriginalPrice == -1)
	{
		user.drawRowBody("Price", '-', n1, n2);
	}
	//display in range when user search in range
	else if (orderDetail->OriginalPrice != -1 && maxPrice != orderDetail->OriginalPrice)
	{
		user.drawRowBody("Price", to_string(orderDetail->OriginalPrice) + " - " + to_string(maxPrice), n1, n2);
	}
	//display user selection
	else if (maxPrice == orderDetail->OriginalPrice)
	{
		user.drawRowBody("Price", orderDetail->OriginalPrice, n1, n2);
	}
	//display '-' when not choose
	if (orderDetail->Discount == -1)
	{
		user.drawRowBody("Discount", '-', n1, n2);
	}
	//display in range when user search in range
	else if (orderDetail->Discount != -1 && maxDiscount != orderDetail->Discount)
	{
		user.drawRowBody("Discount", to_string(orderDetail->Discount) + " - " + to_string(maxDiscount), n1, n2);
	}
	//display user selection
	else if (maxDiscount == orderDetail->Discount)
	{
		user.drawRowBody("Discount", orderDetail->Discount, n1, n2);
	}
	//display '-' when not choose
	if (orderDetail->Quantity == -1)
	{
		user.drawRowBody("Quantity", '-', n1, n2);
	}
	//display in range when user search in range
	else if (orderDetail->Quantity != -1 && maxQuantity != orderDetail->Quantity)
	{
		user.drawRowBody("Quantity", to_string(orderDetail->Quantity) + " - " + to_string(maxQuantity), n1, n2);
	}
	//display user selection
	else if (maxQuantity == orderDetail->Quantity)
	{
		user.drawRowBody("Quantity", to_string(orderDetail->Quantity), n1, n2);
	}
	//display '-' when not choose
	if (orderDetail->Subtotal == -1)
	{
		user.drawRowTail("Subtotal", '-', n1, n2);
	}
	//display in range when user search in range
	else if (orderDetail->Subtotal != -1 && maxSubtotal != orderDetail->Subtotal)
	{
		user.drawRowTail("Subtotal", to_string(orderDetail->Subtotal) + " - " + to_string(maxSubtotal), n1, n2);
	}
	//display user selection
	else if (maxSubtotal == orderDetail->Subtotal)
	{
		user.drawRowTail("Subtotal", orderDetail->Subtotal, n1, n2);
	}
	message = "Please comfirm your selection <Y/N> : ";
	user.getChoice(message, choice, 'Y', 'N');
	if (choice[0] == 'N')
	{
		cout << "Your searching has been cancelled...\n\n";
		system("pause");
		system("cls");
		return;
	}
	//check for the result and execute query
	check = searchOrderDetailPrice
	(orderDetail, "%" + name + "%", date, maxDate, maxPrice - orderDetail->OriginalPrice, maxDiscount - orderDetail->Discount, maxQuantity - orderDetail->Quantity, maxSubtotal - orderDetail->Subtotal);
	//display error message when query return zero row
	if (check == 0)
	{
		cout << "\n";
		cout << "Your search did not match any documents.\n\n";
		cout << "Suggestion:\n\n";
		cout << "Try different condition.\n";
		cout << "Try different range.\n";
	}
	return;
}

//This function is use by worker for worker's table
int OrderDetailManager::searchOrderDetail(OrderDetail* orderDetail, Order* order, string name, string maxDate, int quantityRange, string status2)
{
	DatabaseConnection dbConn;
	PreparedStatement* ps;

	string orderID, productID, quantity;

	if (orderDetail->OrderID != -1)
	{
		orderID = to_string(orderDetail->OrderID);
	}
	if (orderDetail->ProductID != -1)
	{
		productID = to_string(orderDetail->ProductID);
	}
	if (orderDetail->Quantity != -1)
	{
		quantity = to_string(orderDetail->Quantity);
	}

	//join table OrderDetail,Order and Product
	ps = dbConn.prepareStatement
	("SELECT `order`.OrderID,orderdetail.ProductId,ProductName,OrderDate,OrderAddress,Quantity,OrderStatus FROM OrderDetail INNER JOIN Product"
		" ON OrderDetail.ProductID = Product.ProductID INNER JOIN `order`"
		" ON OrderDetail.OrderID = `order`.OrderID WHERE"
		" (OrderDetail.OrderID = ? or ? ='') AND"
		" (OrderDetail.ProductID = ? or ? ='') AND"
		" (ProductName LIKE ? OR ? LIKE '') AND"
		" (OrderDate BETWEEN ? AND ? OR ? BETWEEN '' AND '') AND"
		" (OrderAddress LIKE ? OR ? = '') AND"
		" (Quantity BETWEEN  ? AND ? + ? OR ? BETWEEN '' AND '') AND"
		" (OrderStatus NOT IN('Completed','Cancelled') AND (OrderStatus IN(?,?) OR ? = '')) ORDER BY OrderDetail.OrderID ");

	//set condition to orderID
	ps->setString(1, orderID);
	//enable NULL condition
	ps->setString(2, orderID);

	//set condition to productID
	ps->setString(3, productID);
	//enable NULL condition
	ps->setString(4, productID);

	//set condition to name
	ps->setString(5, name);
	//name will automatically search by '%%' and more easier to find the target
	ps->setString(6, name);

	//set condition to date
	ps->setString(7, order->OrderDate);
	//set a maximum date within the range
	//maximum date will same as date when user don't want to search in range
	ps->setString(8, maxDate);
	//enable NULL condition
	ps->setString(9, order->OrderDate);

	//set condition to name
	ps->setString(10, "%" + order->OrderAddress + "%");
	//address will automatically search by '%%' and more easier to find the target
	ps->setString(11, order->OrderAddress);

	// set condition to quantity
	ps->setString(12, quantity);
	//maximum quantity = base quantity + quantity range
	ps->setString(13, quantity);
	//quantity range will set to zero when user don't want to search in range
	ps->setInt(14, quantityRange);
	//enable NULL condition
	ps->setString(15, quantity);

	//set condition to status
	ps->setString(16, order->OrderStatus);
	//set second status
	ps->setString(17, status2);
	//enable NULL condition
	ps->setString(18, order->OrderStatus);

	ResultSet* rs;
	rs = ps->executeQuery();

	if (!rs->rowsCount())
	{
		return 0;
	}

	//header of table
	cout << char(218) << setw(10) << setfill(char(196)) << char(194) <<
		setw(10) << setfill(char(196)) << char(194) <<
		setw(30) << setfill(char(196)) << char(194) <<
		setw(11) << setfill(char(196)) << char(194) <<
		setw(50) << setfill(char(196)) << char(194) <<
		setw(9) << setfill(char(196)) << char(194) <<
		setw(13) << setfill(char(196)) << char(191) << endl;

	cout << char(179) << setw(9) << left << setfill(' ') << "OrderID" << char(179) <<
		setw(9) << setfill(' ') << "ProductID" << char(179) <<
		setw(29) << setfill(' ') << "ProductName" << char(179) <<
		setw(10) << setfill(' ') << "OrderDate" << char(179) <<
		setw(49) << setfill(' ') << "OrderAddress" << char(179) <<
		setw(8) << setfill(' ') << "Quantity" << char(179) <<
		setw(12) << setfill(' ') << "OrderStatus" << char(179) << endl;

	while (rs->next())
	{
		cout << char(195) << setw(10) << right << setfill(char(196)) << char(197) <<
			setw(10) << setfill(char(196)) << char(197) <<
			setw(30) << setfill(char(196)) << char(197) <<
			setw(11) << setfill(char(196)) << char(197) <<
			setw(50) << setfill(char(196)) << char(197) <<
			setw(9) << setfill(char(196)) << char(197) <<
			setw(13) << setfill(char(196)) << char(180) << endl;

		//retrieve data from database
		cout << char(179) << setw(9) << left << setfill(' ') << rs->getInt(1) << char(179) <<
			setw(9) << setfill(' ') << rs->getInt(2) << char(179) <<
			setw(29) << setfill(' ') << rs->getString(3) << char(179) <<
			setw(10) << setfill(' ') << rs->getString(4) << char(179) <<
			setw(49) << setfill(' ') << rs->getString(5) << char(179) <<
			setw(8) << setfill(' ') << rs->getInt(6) << char(179) <<
			setw(12) << setfill(' ') << rs->getString(7) << char(179) << endl;
	}

	cout << char(192) << setw(10) << right << setfill(char(196)) << char(193) <<
		setw(10) << setfill(char(196)) << char(193) <<
		setw(30) << setfill(char(196)) << char(193) <<
		setw(11) << setfill(char(196)) << char(193) <<
		setw(50) << setfill(char(196)) << char(193) <<
		setw(9) << setfill(char(196)) << char(193) <<
		setw(13) << setfill(char(196)) << char(217) << endl;

	//counting row returned by query
	cout << "\n" << rs->rowsCount() << " row(s) selected\n";

	delete rs;
	delete ps;
	return 1;
}

//worker's list of condition 
void OrderDetailManager::workerSearch()
{
	//string for error message
	string error;
	//set searchList to NUL
	int searchList[9] = { 0 };
	string choice, message;
	UserInputManager user;
	bool flag = true;
	string again;

	while (1)
	{
		for (int i = 0; i < 9; i++)
		{
			while (1)
			{
				system("cls");
				//user selects from the list below and put into an array
				cout << "Please choose your needed condition\n";
				cout << "1. Order ID\n";
				cout << "2. Product ID\n";
				cout << "3. Product Name\n";
				cout << "4. Order Date\n";
				cout << "5. Order Address\n";
				cout << "6. Quantity\n";
				cout << "7. Order Status\n";
				cout << "8. Quit\n";

				user.drawBox("Select Quit to end the choose!", 34);
				cout << "Your choice : ";
				//display a selected list of user
				for (int j = 0; j < 9 && searchList[j] != NULL; j++)
				{
					cout << searchList[j] << " ";
				}
				cout << endl;
				cout << "Enter : ";
				cin >> setw(1) >> searchList[i];
				//if user is not keyin an integer
				if (!cin)
				{
					cout << "Invalid character! Please try again\n\n";
					cin.ignore(INT_MAX, '\n');
					system("pause");
					system("cls");
					continue;
				}
				//user input must in the range 1-8
				else if (searchList[i] < 1 || searchList[i] >8)
				{
					searchList[i] = NULL;
					cout << "Invalid number! Please try again\n\n";
					system("pause");
					system("cls");
					continue;
				}
				//checking for repeated element inside the array
				for (int j = 0; j < i; j++)
				{
					//set the flag to true in every loop
					flag = true;
					if (searchList[j] == searchList[i])
					{
						searchList[i] = NULL;
						cout << "Invalid repeated number! Please try again\n\n";
						//once repeated number found, set flag to false then quit
						flag = false;
						break;
					}
				}
				//flag will false when repeated number found
				if (!flag)
				{
					system("pause");
					system("cls");
					continue;
				}
				break;
			}
			//exit point 
			if (searchList[i] == 8)
			{
				cout << endl;
				break;
			}
		}


		//display the selected choice from user
		cout << "Your choice:\t";
		for (int j = 0; j < 8 && searchList[j] != NULL && searchList[j] != 8; j++)
		{

			switch (searchList[j])
			{
			case 1:
				cout << "Order ID";
				break;
			case 2:
				cout << "Product ID";
				break;
			case 3:
				cout << "Product Name";
				break;
			case 4:
				cout << "OrderDate";
				break;
			case 5:
				cout << "Order Address";
				break;
			case 6:
				cout << "Quantity";
				break;
			case 7:
				cout << "Order Status";
				break;
			default:
				break;
			}
			cout << "\n\t\t";
		}
		message = "\nConfirm your selection? <Y/N> ";
		cin.ignore(INT_MAX, '\n');
		user.getChoice(message, choice, 'Y', 'N');
		if (choice[0] == 'Y')
		{
			if (searchList[0] == 8)
			{
				system("cls");
				return;
			}
			workerInput(searchList);
			message = "Do you want to search again? <Y/N> : ";
			user.getChoice(message,again);
			if (again[0] == 'Y')
			{
				for (int i = 0; i < 7; i++)
				{
					searchList[i] = 0;
				}
				continue;
			}
		}
		else if (choice[0] == 'N')
		{
			system("cls");
			for (int i = 0; i < 7; i++)
			{
				searchList[i] = 0;
			}
			continue;
		}
		break;
	}
}

//worker's key in information
void OrderDetailManager::workerInput(int searchList[9])
{
	//set an enum list to improve readability
	enum list { orderID = 1, productID = 2, productName = 3, orderDate = 4, address = 5, quantity = 6, orderStatus = 7, quit = 8 };
	string choice;
	//string for error message
	string error;
	string message;
	//checking for the search result
	int check;
	OrderDetail* orderDetail = new OrderDetail;
	Order* order = new Order;
	UserInputManager user;

	//set all integer member to -1 by default
	orderDetail->OrderID = -1;
	orderDetail->ProductID = -1;
	orderDetail->Quantity = -1;

	//maximum value using in range's search
	int maxQuantity = 0;
	int n1 = 0, n2 = 0;

	string maxDate = "";
	string name = "";
	string status2 = "";

	system("cls");

	for (int i = 0; searchList[i] != 0; i++)
	{
		//read from the array one by one and user input needed information
		switch (searchList[i])
		{
		case orderID:
			//search for order id
			user.drawTittle("Order ID", 9, 10);
			message ="Please enter an order ID : ";
			user.getInt(message, orderDetail->OrderID);
			cout << "\n";
			break;
		case productID:
			//search for product id
			user.drawTittle("Product ID", 11, 10);
			message = "Please enter an product ID : ";
			user.getInt(message, orderDetail->ProductID);
			cout << "\n";
			break;
		case productName:
			//search for product name
			user.drawTittle("Product Name", 13, 10);
			cout << "Please enter a product name : ";
			getline(cin, name);
			cout << "\n";
			break;
		case orderDate:
			user.drawTittle("Order Date", 11, 10);
		while(1)
		{
			//search for a date either in range or single value
			message = "Search order date in range? <Y/N> : ";
			user.getChoice(message, choice, 'Y', 'N');
			//reset max value 
			maxDate = "";
			if (choice[0] == 'Y')
			{
				message = "Order Date From <YYYY-MM-DD> : ";
				user.getDate(message, order->OrderDate);
				message = "Order Date To <YYYY-MM-DD> : ";
				user.getDate(message, maxDate);
				if (!user.compareDate(order->OrderDate, maxDate))
				{
					continue;
				}
				cout << "\n";
				break;
			}
			else if (choice[0] == 'N')
			{
				message = "Please enter an order Date <YYYY-MM-DD> : ";
				user.getDate(message, order->OrderDate);
				maxDate = order->OrderDate;
				cout << "\n";
				break;
			}
		}
			break;
		case address:
			user.drawTittle("Order Address", 14, 10);
			//search address using keyword
			cout << "Please enter an address : ";
			getline(cin, order->OrderAddress);
			cout << "\n";
			break;
		case quantity:
			user.drawTittle("Quantity", 9, 10);
			while (1)
			{
				message = "Search quantity in range? <Y/N> ";
				user.getChoice(message, choice, 'Y', 'N');
				//reset max value 
				maxQuantity = 0;
				if (choice[0] == 'Y')
				{
					message = "Quantity From :";
					user.getInt(message, orderDetail->Quantity);
					message = "Quantity To :";
					user.getInt(message, maxQuantity);
					if (!user.compare(orderDetail->Quantity, maxQuantity))
					{
						continue;
					}
					cout << "\n";
					break;
				}
				else if (choice[0] == 'N')
				{
					message = "Please enter a quantity :";
					user.getInt(message, orderDetail->Quantity);
					maxQuantity = orderDetail->Quantity;
					cout << "\n";
					break;
				}
			}
			break;
		case orderStatus:
			user.drawTittle("Order Status", 13, 10);
			while (1)
			{
				message = "Search order status in range? <Y/N> : ";
				user.getChoice(message, choice, 'Y', 'N');
				status2 = "";
				if (choice[0] == 'Y')
				{
					message = "Status 1 < Order Placed / Pending / Packaging > : ";
					user.getStatus(message, order->OrderStatus, "Order Placed", "Pending", "Packaging");
					message = "Status 2 < Order Placed / Pending / Packaging > : ";
					user.getStatus(message, status2, "Order Placed", "Pending", "Packaging");
					if (order->OrderStatus == status2)
					{
						cout << "Repeated status! Please try again\n\n";
						continue;
					}
					cout << "\n";
					break;
				}
				else if (choice[0] == 'N')
				{
					message = "Please enter a order status < Order Placed / Pending / Packaging > : ";
					user.getStatus(message, order->OrderStatus, "Order Placed", "Pending", "Packaging");
					status2 = order->OrderStatus;
					cout << "\n";
					break;
				}
			}
			break;
		case quit:
			break;			
		}
	}
	n1 = 20;
	n2 = 60;
	//display '-' when not choosen
	if (orderDetail->OrderID == -1)
	{
		user.drawRowHead("Order ID", '-', n1, n2);
	}
	//display user selection 
	else if (orderDetail->OrderID != -1)
	{
		user.drawRowHead("Order ID", orderDetail->OrderID, n1, n2);
	}
	//display '-' when not choosen
	if (orderDetail->ProductID == -1)
	{
		user.drawRowBody("Product ID", '-', n1, n2);
	}
	//display user selection 
	else if (orderDetail->ProductID != -1)
	{
		user.drawRowBody("Product ID", orderDetail->ProductID, n1, n2);
	}
	//display '-' when not choosen
	if (name == "")
	{
		user.drawRowBody("Product Name", '-', n1, n2);
	}
	//display user selection 
	else if (name != "")
	{
		user.drawRowBody("Product Name", name, n1, n2);
	}
	//display '-' when not choosen
	if (order->OrderDate == "")
	{
		user.drawRowBody("Order Date", '-', n1, n2);
	}
	//display in range when user search in range
	else if (order->OrderDate != "" && maxDate != order->OrderDate)
	{
		user.drawRowBody("Order Date", order->OrderDate + " To " + maxDate, n1, n2);
	}
	//display user selection
	else if (maxDate == order->OrderDate)
	{
		user.drawRowBody("Order Date", order->OrderDate, n1, n2);
	}
	//display '-' when not choosen
	if (order->OrderAddress == "")
	{
		user.drawRowBody("Order Address", '-', n1, n2);
	}
	//display user selection
	else if (order->OrderAddress != "")
	{
		user.drawRowBody("Order Address", order->OrderAddress, n1, n2);
	}
	//display '-' when not choosen
	if (orderDetail->Quantity == -1)
	{
		user.drawRowBody("Quantity", '-', n1, n2);
	}
	//display in range when user search in range
	else if (orderDetail->Quantity != -1 && maxQuantity != orderDetail->Quantity)
	{
		user.drawRowBody("Quantity", to_string(orderDetail->Quantity) + " - " + to_string(maxQuantity), n1, n2);
	}
	//display user selection
	else if (maxQuantity == orderDetail->Quantity)
	{
		user.drawRowBody("Quantity", to_string(orderDetail->Quantity), n1, n2);
	}
	if (order->OrderStatus == "")
	{
		user.drawRowTail("Order Status", '-', n1, n2);
	}
	//display in range when user search in range
	else if (order->OrderStatus != "" && status2 != order->OrderStatus)
	{
		user.drawRowTail("Order Status", order->OrderStatus + "/" + status2, n1, n2);
	}
	//display user selection
	else if (status2 == order->OrderStatus)
	{
		user.drawRowTail("Order Status", order->OrderStatus, n1, n2);
	}
	message = "Please confirm your selection <Y/N> : ";
	user.getChoice(message, choice, 'Y', 'N');
	if (choice[0] == 'N')
	{
		cout << "\nYour search has been cancelled...\n\n";
		return;
	}

	//check for the result and execute query
	check = searchOrderDetail
	(orderDetail, order, "%" + name + "%", maxDate, maxQuantity - orderDetail->Quantity, status2);
	//display error message when query return zero row
	if (check == 0)
	{
		cout << "\n";
		cout << "Your search did not match any documents.\n\n";
		cout << "Suggestion:\n\n";
		cout << "Try different condition.\n";
		cout << "Try different range.\n";
	}
	return;
}

void OrderDetailManager::updateMenu()
{
	UserInputManager user;

	int selection = 0;
	string message;

	while (1)
	{
		cout << "Please choose one of the following to update\n";
		cout << "1. Product Quantity\n";
		cout << "2. Product Discount\n";
		cout << "3. Quit\n\n";

		message = "Enter : ";
		user.getInt(message, selection, 1, 3);
		system("cls");
		switch (selection)
		{
		case 1:
			update("Quantity", "Product Quantity", selection);
			break;
		case 2:
			update("Discount", "Product Discount", selection);
			break;
		case 3:
			return;
			break;
		}
	}
}

//asking input for update table
//column for changing column name in query, object for displaying what object is updating, selection for asking user input into right variable
void OrderDetailManager::update(string column, string object,int selection)
{
	OrderDetail* orderDetail = new OrderDetail;
	UserInputManager user;
	DatabaseConnection dbConn;
	PreparedStatement* ps;
	ResultSet* rs;

	string message;
	string choice;
	bool resume = true;
	bool check = true;

	int max = 0;

	while (1)
	{
		viewOrderDetailPrice();
		//searching a target orderDetail before update
		cout << "Select an orderDetail id from table above\n";
		user.drawTittle("Order ID", 9, 10);
		message = "Please enter order ID : ";
		user.getInt(message, orderDetail->OrderID);
		cout << "\n";

		user.drawTittle("Product ID", 11, 8);
		message = "Please enter product ID : ";
		user.getInt(message, orderDetail->ProductID);
		cout << "\n";

		//display targeted orderDetail with object that need in update
		ps = dbConn.prepareStatement("SELECT orderdetail.OrderID,ProductID," + column + " FROM OrderDetail INNER JOIN `Order` ON `Order`.`OrderID` = orderdetail.OrderID"
									 " WHERE orderdetail.OrderID = ? AND ProductID = ? AND OrderStatus NOT IN('Completed','Cancelled')");
		ps->setInt(1, orderDetail->OrderID);
		ps->setInt(2, orderDetail->ProductID);
		rs = ps->executeQuery();

		//display error message when no result found in database
		//user can choose either keep input a correct id or stop updating
		if (!rs->rowsCount())
		{
			message = "Cannot find Order ID " + to_string(orderDetail->OrderID) + " and Product ID " + to_string(orderDetail->ProductID);
			user.drawBox(message, 56 );
			message = "Do you want to continue? <Y/N> : ";
			user.getChoice(message, choice);
			if (choice[0] == 'N')
			{
				resume = false;
				break;
			}		
			system("cls");
			continue;
		}
		break;
	}

	//end this function when user don't want to update after searching a wrong id number
	if (!resume)
	{
		system("cls");
		return;
	}

	//display table
	cout << char(218) << setw(10) << setfill(char(196)) << char(194) << setw(10) << setfill(char(196)) << char(194) << setw(20) << setfill(char(196)) << char(191) << endl;
	cout << char(179) << setw(9) << left << setfill(' ') << "OrderID" << char(179) << setw(9) << setfill(' ') << "ProductId" << char(179) << setw(19) << setfill(' ') << object << char(179) << endl;
	if (rs->next())
	{
		cout << char(195) << setw(10) << right << setfill(char(196)) << char(197) << setw(10) << setfill(char(196)) << char(197) << setw(20) << setfill(char(196)) << char(180) << endl;
		//retrieve data from database
		cout << char(179) << setw(9) << left << setfill(' ') << rs->getInt(1) << char(179) <<
			setw(9) << setfill(' ') << rs->getInt(2) << char(179) <<
			setw(19) << setfill(' ') << rs->getDouble(3) << char(179) << endl;
	}
	cout << char(192) << setw(10) << right << setfill(char(196)) << char(193) << setw(10) << setfill(char(196)) << char(193) << setw(20) << setfill(char(196)) << char(217) << endl;
	cout << "\n";

	//user input value that want to update
	user.drawTittle(object, object.length() + 1, 10);
	message = "Please enter updated " + object + " : ";

	//asking user input different values in different case
	switch (selection)
	{
	case 1:
		ps = dbConn.prepareStatement("SELECT ProductQuantity + (SELECT Quantity FROM OrderDetail WHERE OrderID = ?  AND ProductID = ?) FROM Product WHERE ProductID = ? ");

		ps->setInt(1,orderDetail->OrderID);
		ps->setInt(2, orderDetail->ProductID);
		ps->setInt(3, orderDetail->ProductID);

		rs = ps->executeQuery();
		if (rs->next())
		{
			max = rs->getInt(1);
		}

		user.getInt(message, orderDetail->Quantity, 0, max);
		cout << "\n";
		user.drawTittle("Caution!!!", 11, 25);
		cout << "Updating " << object << " to " << orderDetail->Quantity << " in " << "Order ID " << orderDetail->OrderID << " and Product ID " << orderDetail->ProductID << "\n";
		break;
	case 2:
		user.getInt(message, orderDetail->Discount);
		cout << "\n";
		user.drawTittle("Caution!!!", 11, 25);
		cout << "Updating " << object << " to " << orderDetail->Discount << " in " << "Order ID " << orderDetail->OrderID << " and Product ID " << orderDetail->ProductID << "\n";
		break;
	}
	
	//user confirm message before update
	message = "Please confirm your update <Y/N> : ";
	user.getChoice(message, choice);

	if (choice[0] == 'N')
	{
		cout << "\n";
		cout << "Your update has been cancelled...\n\n";
		system("pause");
		system("cls");
		return;
	}

	//update different column in different case
	switch (selection)
	{
	case 1:
		check = updateQuantity(orderDetail);
		break;
	case 2:
		check = updateDiscount(orderDetail);
		break;
	}

	cout << "\n";
	if (check)
	{
		cout << "Update database successfully!\n\n";

		ps = dbConn.prepareStatement("SELECT OrderID,ProductID," + column + " FROM OrderDetail WHERE OrderID = ? AND ProductID = ?");
		ps->setInt(1, orderDetail->OrderID);
		ps->setInt(2, orderDetail->ProductID);
		rs = ps->executeQuery();

		cout << char(218) << setw(10) << setfill(char(196)) << char(194) << setw(10) << setfill(char(196)) << char(194) << setw(20) << setfill(char(196)) << char(191) << endl;
		cout << char(179) << setw(9) << left << setfill(' ') << "OrderID" << char(179) << setw(9) << setfill(' ') << "ProductId" << char(179) << setw(19) << setfill(' ') << object << char(179) << endl;
		if (rs->next())
		{
			cout << char(195) << setw(10) << right << setfill(char(196)) << char(197) << setw(10) << setfill(char(196)) << char(197) << setw(20) << setfill(char(196)) << char(180) << endl;
			//retrieve data from database
			cout << char(179) << setw(9) << left << setfill(' ') << rs->getInt(1) << char(179) <<
				setw(9) << setfill(' ') << rs->getInt(2) << char(179) <<
				setw(19) << setfill(' ') << rs->getDouble(3) << char(179) << endl;
		}
		cout << char(192) << setw(10) << right << setfill(char(196)) << char(193) << setw(10) << setfill(char(196)) << char(193) << setw(20) << setfill(char(196)) << char(217) << endl;
		cout << "\n";
	}
	else
	{
		cout << "Fail to update database!\n\n";
	}
	system("pause");
	system("cls");
}






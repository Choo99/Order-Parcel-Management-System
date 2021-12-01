#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
#include <mysql/jdbc.h>
#include <conio.h>
#include "sha3.h"
#include <windows.h>
using namespace std;
using namespace sql;

#include "DatabaseConnection.h"
#include "User.h"
#include "UserManager.h"
#include "Staff.h"
#include "StaffManager.h"
#include "ProductType.h"
#include "ProductTypeManager.h"
#include "Supplier.h"
#include "SupplierManager.h"
#include "Product.h"
#include "ProductManager.h"
#include "RestockRequest.h"
#include "RestockRequestManager.h"
#include "Order.h"
#include "OrderManager.h"
#include "OrderDetail.h"
#include "OrderDetailManager.h"
#include "Role.h"
#include "UserInputManager.h"
#include "Graph.h"

void mainMenu(Staff* staff);
void orderMenu(Staff* staff);
void productMenu(Staff* staff);
void restockRequestMenu(Staff* staff);
void staffMenu(Staff* staff);
void supplierMenu(Staff* staff);
void productTypeMenu(Staff* staff);

void financialReport(Staff* staff);
void productGraph(string duration);
void supplierGraph(string duration);
void paymentGraph(string duration);
void productTypeGraph(string duration);
void dateGraph(string duration);
void profitGraph();
void productQuantityGraph(string duration);
void orderQuantityGraph();
void record();


int main()
{
	Staff* staff = new Staff;
	UserInputManager userInputManager;
	User* user = new User();
	UserManager userManager;
	DatabaseConnection dbConn;
	PreparedStatement* ps;
	ResultSet* rs;

	string message;
	string choice;
	string password;

	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ProductManager productManager;
	OrderManager orderManager;
	OrderDetailManager orderDetailManager;
	RestockRequestManager restockRequest;
	
	SetConsoleTextAttribute(hConsole, 11);
	cout << "\n";
	message = " Welcome to Order Parcel Management System";
	userInputManager.drawTittle(message, message.length()+2,2 );
	cout << setfill(' ') << setw(25) << " 1. Login\n";
	cout << setfill(' ') << setw(24) << " 2. Quit\n";
	char ch;

	while ((ch = _getch()) != 49 && (ch = _getch()) != 50);
	if (ch == 50)
	{
		return 0;
	}
	SetConsoleTextAttribute(hConsole, 7);
	system("cls");
	while (1)
	{
		cout << "Please enter your username and password\n";
		
		message = "Username : ";
		userInputManager.getPassword(message, user->UserName);
		message = "Password : ";
		userInputManager.getPassword(message, user->UserPassword);
		ps = dbConn.prepareStatement("SELECT UserPassword,Salt,UserID FROM User WHERE Username = ?");
		ps->setString(1, user->UserName);
		rs = ps->executeQuery();
		if (!rs->rowsCount())
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Your username/password is wrong!\n\n";
			SetConsoleTextAttribute(hConsole, 7);
			message = "Do you want to continue? <Y/N> : ";
			userInputManager.getChoice(message, choice);
			if (choice[0] == 'Y')
			{
				system("cls");
				continue;
			}
			else
			{
				main();
				return 0;
			}
		}
		if (rs->next())
		{
			password = rs->getString(1);
			user->Salt = rs->getString(2);
			user->UserID = rs->getInt(3);
		}
		user->UserPassword = userManager.hash(user->Salt, user->UserPassword);
		if (user->UserPassword == password)
		{
			SetConsoleTextAttribute(hConsole, 2);
			cout << "\nLog In succefully!\n\n";
			SetConsoleTextAttribute(hConsole, 7);
			system("pause");
		}
		else
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Your username/password is wrong!\n";
			SetConsoleTextAttribute(hConsole, 7);
			message = "Do you want to continue? <Y/N> : ";
			userInputManager.getChoice(message, choice);
			if (choice[0] == 'Y')
			{
				system("cls");
				continue;
			}
			else
			{
				main();
				return 0;
			}
		}
		break;
	}

	ps = dbConn.prepareStatement("SELECT StaffId,StaffName,UserId,RoleID FROM Staff WHERE UserID = ?");
	ps->setInt(1, user->UserID);
	rs = ps->executeQuery();
	if (rs->next())
	{
		staff->StaffID = rs->getInt(1);
		staff->StaffName = rs->getString(2);
		staff->UserID = rs->getInt(3);
		staff->RoleID = rs->getInt(4);
	}
	system("cls");
	mainMenu(staff);
	return 0;
}

void mainMenu(Staff* staff)
{
	UserInputManager user;
	UserManager userManager;
	SupplierManager supplierManager;
	ProductTypeManager TypeManager;
	string message;
	int selection;

	enum Role { admin = 9001, manager = 9002, worker = 9003 };

	switch (staff->RoleID)
	{
	case admin:
		while (1)
		{
			enum List { order = 1, worker = 2, product = 3, restock = 4, supplier = 5, productType = 6, account = 7, report = 8, quit = 9 };
			cout << "Welcome back, " << staff->StaffName << "!\n";
			user.drawTittle(" Main menu ", 12, 10);
			cout << "1. Manage Order\n";
			cout << "2. Manage Staff\n";
			cout << "3. Manage Product\n";
			cout << "4. Manage Restock Request\n";
			cout << "5. Manage Supplier\n";
			cout << "6. Manage Product Type\n";
			cout << "7. Manage account setting\n";
			cout << "8. Financial Report\n";
			cout << "9. Exit\n";

			cout << "\nWhat do you want to do?\n";
			message = "Enter : ";
			user.getInt(message, selection, 1, 9);
			system("cls");


			switch (selection)
			{
			case order:
				orderMenu(staff);
				break;
			case worker:
				staffMenu(staff);
				break;
			case product:
				productMenu(staff);
				break;
			case restock:
				restockRequestMenu(staff);
				break;
			case supplier:
				supplierMenu(staff);
				break;
			case productType:
				productTypeMenu(staff);
				break;
			case account:
				userManager.updateMenu(staff);
				break;
			case report:
				financialReport(staff);
				break;
			case quit:
				main();
				return;
				break;
			}
		}
		break;
	case manager:
		while (1)
		{
			enum List { order = 1, worker = 2, product = 3, restock = 4, supplier = 5, productType = 6, account = 7, quit = 8 };
			cout << "Welcome back, " << staff->StaffName << "!\n";
			user.drawTittle(" Main menu ", 12, 10);
			cout << "1. Manage Order\n";
			cout << "2. Manage Staff\n";
			cout << "3. Manage Product\n";
			cout << "4. Manage Restock Request\n";
			cout << "5. Manage Supplier\n";
			cout << "6. Manage Product Type\n";
			cout << "7. Manage account setting\n";
			cout << "8. Exit\n";

			cout << "\nWhat do you want to do?\n";
			message = "Enter : ";
			user.getInt(message, selection, 1, 8);
			system("cls");

			switch (selection)
			{
			case order:
				orderMenu(staff);
				break;
			case worker:
				staffMenu(staff);
				break;
			case product:
				productMenu(staff);
				break;
			case restock:
				restockRequestMenu(staff);
				break;
			case supplier:
				supplierMenu(staff);
				break;
			case productType:
				productTypeMenu(staff);
				break;
			case account:
				userManager.updateMenu(staff);
				break; 
			case quit:
				main();
				return;
				break;
			}
		}
		break;
	case worker:
		while (1)
		{
			enum List { order = 1, worker = 2, product = 3, restock = 4, supplier = 5, productType = 6, account = 7, quit = 8 };
			cout << "Welcome back, " << staff->StaffName << "!\n";
			user.drawTittle(" Main menu ", 12, 10);

			cout << "1. Manage Order\n";
			cout << "2. Manage worker\n";
			cout << "3. Manage Product\n";
			cout << "4. Manage Restock Request\n";
			cout << "5. View Supplier\n";
			cout << "6. View Product Type\n";
			cout << "7. Manage account setting\n";
			cout << "8. Exit\n";

			cout << "\nWhat do you want to do?\n";
			message = "Enter : ";
			user.getInt(message, selection, 1, 8);
			system("cls");
			switch (selection)
			{
			case order:
				orderMenu(staff);
				break;
			case worker:
				staffMenu(staff);
				break;
			case product:
				productMenu(staff);
				break;
			case restock:
				restockRequestMenu(staff);
				break;
			case supplier:
				supplierManager.viewSupplier();
				system("pause");
				system("cls");
				break;
			case productType:
				TypeManager.viewProductType();
				system("pause");
				system("cls");
				break;
			case account:
				userManager.updateMenu(staff);
				break;
			case quit:
				main();
				return;
				break;
			}
		}
		break;
	}
}

void orderMenu(Staff* staff)
{
	UserInputManager user;
	string message;
	string choice;
	int selection;
	OrderManager orderManager;
	ProductManager productManager;
	OrderDetailManager orderDetailManager;

	enum Role { admin = 9001, manager = 9002, worker = 9003 };

	switch(staff->RoleID)
	{
	case admin:
		while (1)
		{
			enum list { displayAllOrder = 1, displayOrder = 2, displayOrderDetail = 3, searchAllOrder = 4, searchOrder = 5, searchOrderDetail = 6, cancelOrder = 7, quit = 8 };
			message = "Order Menu";
			user.drawTittle(message, message.size() + 1, 8);
			cout << "1. Display all orders\n";
			cout << "2. Display processing orders\n";
			cout << "3. Display processing order's detail\n";
			cout << "4. Searching in all orders\n";
			cout << "5. Display specific order\n";
			cout << "6. Display specific order's detail\n";
			cout << "7. Cancel an order record\n";
			cout << "8. Quit\n";

			cout << "\n";
			cout << "Please select one from the table above\n";
			message = "Enter : ";
			user.getInt(message, selection, 1, 8);
			system("cls");
			switch (selection)
			{
			case displayAllOrder:
				orderManager.viewAllOrder();	
				message = "Do you want to search specific order? <Y/N> : ";
				user.getChoice(message, choice);
				if (choice[0] == 'Y')
				{
					orderManager.search();
				}
				system("cls");
			case searchAllOrder:
				orderManager.search();
				break;
			case displayOrder:
				orderManager.viewOrder();
				message = "Do you want to search specific order? <Y/N> : ";
				user.getChoice(message, choice);
				if (choice[0] == 'Y')
				{
					orderManager.workerSearch();
				}
				system("cls");
				break;
			case displayOrderDetail:
				orderDetailManager.viewOrderDetailPrice();
				message = "Do you want to search specific order detail? <Y/N> : ";
				user.getChoice(message, choice);
				if (choice[0] == 'Y')
				{
					orderDetailManager.search();
				}
				system("cls");
				break;
			case searchOrder:
				orderManager.search();
				break;
			case searchOrderDetail:
				orderDetailManager.search();
				break;
			case cancelOrder:
				orderManager.deleteOrder(staff);
				break;
			case quit:
				return;
				break;
			}
			productManager.autoUpdateStatus();
		}
		break;
	case manager:
		while (1)
		{
			enum list { createOrder = 1, displayOrder = 2, displayOrderDetail = 3, searchOrder = 4, searchOrderDetail = 5, updateOrder = 6, updateOrderDetail = 7, quit = 8 };
			message = "Order Menu";
			user.drawTittle(message, message.size() + 1, 8);
			cout << "1. Create new order\n";
			cout << "2. Display processing orders\n";
			cout << "3. Display processing order's detail\n";
			cout << "4. Display specific order\n";
			cout << "5. Display specific order's detail\n";
			cout << "6. Modify order record\n";
			cout << "7. Modify order's detail\n";
			cout << "8. Quit\n";

			cout << "\n";
			cout << "Please select one from the table above\n";
			message = "Enter : ";
			user.getInt(message, selection, 1, 8);
			system("cls");
			switch (selection)
			{
			case createOrder:
				orderManager.insertOrder(staff);
				break;
			case displayOrder:
				orderManager.viewOrder();
				message = "Do you want to search a specific order? <Y/N> : ";
				user.getChoice(message, choice);
				//if only one result get from search offer choice to looking order detail of this order
				if (choice[0] == 'Y')
				{
					orderManager.workerSearch();
				}
				system("cls");
				break;
			case displayOrderDetail:
				orderDetailManager.viewOrderDetailPrice();
				message = "Do you want to search a specific order's detail? <Y/N> : ";
				user.getChoice(message, choice);
				if (choice[0] == 'Y')
				{
					orderDetailManager.search();
				}
				system("cls");
				break;
			case  searchOrder:
				orderManager.search();
				break;
			case searchOrderDetail:
				orderDetailManager.workerSearch();
				break;
			case updateOrder:
				orderManager.updateMenu(staff);
				break;
			case updateOrderDetail:
				orderDetailManager.updateMenu();
				break;
			case quit:
				return;
				break;
			}
			productManager.autoUpdateStatus();
		}
		break;
	case worker:
		while (1)
		{
			enum List { viewOrder = 1, viewOrderDetail = 2, searchOrder = 3, searchOrderDetail = 4, updateOrderStatus = 5, quit = 6 };
			message = "Order Menu";
			user.drawTittle(message, message.size() + 1, 8);
			cout << "1. Display processing orders\n";
			cout << "2. Display processing order's detail\n";
			cout << "3. Display specific order\n";
			cout << "4. Display specific order's detail record\n";
			cout << "5. Modify order's status\n";
			cout << "6. Quit\n";

			cout << "\n";
			cout << "Please select one from the table above\n";
			message = "Enter : ";
			user.getInt(message, selection, 1, 9);
			system("cls");
			switch (selection)
			{
			case viewOrder:
				orderManager.viewOrder();
				message = "Do you want to search a specific order? <Y/N> : ";
				user.getChoice(message, choice);
				if (choice[0] == 'Y')
				{
					orderManager.workerSearch();
				}
				system("cls");
				break;
			case viewOrderDetail:
				orderDetailManager.viewOrderDetail();
				message = "Do you want to search a specific order's detail? <Y/N> : ";
				user.getChoice(message, choice);
				if (choice[0] == 'Y')
				{
					orderDetailManager.workerSearch();
				}
				system("cls");
				break;
			case searchOrder:
				orderManager.workerSearch();
				break;
			case searchOrderDetail:
				orderDetailManager.workerSearch();
				break;
			case updateOrderStatus:
				orderManager.update(staff, "OrderStatus", "Order Status", 3);
				break;
			case quit:
				return;
				break;
			}
			productManager.autoUpdateStatus();
		}
		break;
	}
}

void staffMenu(Staff* staff)
{
	UserInputManager user;
	string message;
	string choice;
	int selection;
	StaffManager staffManager;
	UserManager userManager;

	enum Role { admin = 9001, manager = 9002, worker = 9003 };

	switch (staff->RoleID)
	{
	case admin:
		while (1)
		{
			enum List { insert = 1, view = 2, search = 3, salary = 4, quit = 5 };

			message = "Staff Menu";
			user.drawTittle(message, message.length() + 1, 10);
			cout << "1. Register new staff\n";
			cout << "2. Display all staffs' information\n";
			cout << "3. Display specific staff's information\n";
			cout << "4. Update staff salary\n";
			cout << "5. Quit\n";

			cout << "\n";
			message = "Please select one from the table above : ";
			user.getInt(message, selection, 1, 5);
			system("cls");
			switch (selection)
			{
			case insert:
				userManager.insertUser();
				break;
			case view:
				staffManager.viewAll();
				message = "Do you want to search for specific staff? <Y/N> : ";
				user.getChoice(message, choice);
				if (choice[0] == 'Y')
				{
					staffManager.searchAll();
				}
				system("cls");
				break;
			case search:
				staffManager.searchAll();
				break;
			case salary:
				staffManager.updateSalary();
				break;
			case quit:
				return;
				break;
			}
		}
		break;
	case manager:
		while (1)
		{
			enum List { view = 1, search = 2, quit = 3 };

			message = "Staff Menu";
			user.drawTittle(message, message.length() + 1, 10);
			cout << "1. Display warehouse workers information\n";
			cout << "2. Display specific warehouse workers information\n";
			cout << "3. Quit\n";

			cout << "\n";
			message = "Please select one from the table above : ";
			user.getInt(message, selection, 1, 3);
			system("cls");
			switch (selection)
			{
			case view:
				staffManager.viewWorker();
				message = "Do you want to search specific worker? <Y/N> : ";
				user.getChoice(message, choice);
				if (choice[0] == 'Y')
				{
					staffManager.search();
				}
				system("cls");
				break;
			case search:
				staffManager.search();
				break;
			case quit:
				return;
				break;
			}
		}
		break;
	case worker:
		enum List { view = 1, search = 2, quit = 3 };
		while (1)
		{
			message = "Staff Menu";
			user.drawTittle(message, message.length() + 1, 10);
			cout << "1. Display warehouse workers information\n";
			cout << "2. Display specific warehouse workers information\n";
			cout << "3. Quit\n";

			cout << "\n";
			message = "Please select one from the table above : ";
			user.getInt(message, selection, 1, 3);
			
			switch (selection)
			{
			case view:
				staffManager.viewWorker();
				message = "Do you want to search specific worker? <Y/N> : ";
				user.getChoice(message, choice);
				if (choice[0] == 'Y')
				{
					staffManager.search();
				}
				system("cls");
				break;
			case search:
				staffManager.search();
				break;
			case quit:
				system("cls");
				return;
				break;
			}
		}
		break;
	}

}

void productMenu(Staff* staff)
{
	UserInputManager user;
	string message;
	string choice;
	int selection;
	ProductManager productManager;
	UserManager userManager;

	enum Role { admin = 9001, manager = 9002, worker = 9003 };

	switch (staff->RoleID)
	{
	case admin:
		while (1)
		{
			enum List { insert = 1, view = 2, search = 3, update = 4, Delete = 5, quit = 6 };
			message = "Product Menu";
			user.drawTittle(message, message.length() + 1, 8);
			cout << "1. Create new product\n";
			cout << "2. Display all product in store\n";
			cout << "3. Display specific product\n";
			cout << "4. Modify available product\n";
			cout << "5. Delete a product\n";
			cout << "6. Quit\n";

			cout << "\n";
			cout << "Please select one from the table above\n";
			message = "Enter : ";
			user.getInt(message, selection, 1, 6);
			system("cls");
			switch (selection)
			{
			case insert:
				productManager.insertProduct(staff);
				break;
			case view:
				productManager.viewAllProduct();
				message = "Do you want to search specific product? <Y/N> : ";
				user.getChoice(message, choice);
				if (choice[0] == 'Y')
				{
					productManager.searchDetailPrice();
				}
				system("cls");
				break;
			case search:
				productManager.searchDetailPrice();
				break;
			case update:
				productManager.updateMenu();
				break;
			case Delete:
				productManager.deleteProduct();
				break;
			case quit:
				return;
				break;
			}
			productManager.autoUpdateStatus();
		}
		break;
	case manager:
		while (1)
		{
			enum List { view = 1, search = 2, update = 3, quit = 4 };
			message = "Product Menu";
			user.drawTittle(message, message.length() + 1, 8);
			cout << "1. Display product in store\n";
			cout << "2. Display specific product\n";
			cout << "3. Modify available product\n";
			cout << "4. Quit\n";

			cout << "\n";
			cout << "Please select one from the table above\n";
			message = "Enter : ";
			user.getInt(message, selection, 1, 6);
			system("cls");
			switch (selection)
			{
			case view:
				productManager.viewProductDetailPrice();
				message = "Do you want to search specific product? <Y/N> : ";
				user.getChoice(message, choice);
				if (choice[0] == 'Y')
				{
					productManager.searchDetailPrice();
				}
				system("cls");
				break;
			case search:
				productManager.searchDetailPrice();
				break;
			case update:
				productManager.updateMenu();
				break;
			case quit:
				return;
				break;
			}
			productManager.autoUpdateStatus();
		}
		break;
	case worker:
		while (1)
		{
			enum List { view = 1, search = 2, quit = 3 };
			message = "Product Menu";
			user.drawTittle(message, message.length() + 1, 8);
			cout << "1. Display product in store\n";
			cout << "2. Display specific product\n";
			cout << "3. Quit\n";

			cout << "\n";
			cout << "Please select one from the table above\n";
			message = "Enter : ";
			user.getInt(message, selection, 1, 3);
			system("cls");

			switch (selection)
			{
			case view:
				productManager.viewProductDetail();
				message = "Do you want to search specific product? <Y/N> : ";
				user.getChoice(message, choice);
				if (choice[0] == 'Y')
				{
					productManager.searchDetail();
				}
				system("cls");
				break;
			case search:
				productManager.searchDetail();
				break;
			case quit:
				return;
				break;
			}
			productManager.autoUpdateStatus();
		}	
		break;
	}
}

void restockRequestMenu(Staff* staff)
{
	UserInputManager user;
	string message;
	string choice;
	int selection;
	RestockRequestManager restock;
	UserManager userManager;
	ProductManager productManager;

	enum Role { admin = 9001, manager = 9002, worker = 9003 };

	switch (staff->RoleID)
	{
	case admin:
		while (1)
		{
			enum List { insert = 1, viewCompleted = 2, searchCompleted = 3, view = 4, search = 5, viewProduct = 6, update = 7, quit = 8 };
			message = "Restock Request Menu";
			user.drawTittle(message, message.length() + 1, 8);
			cout << "1. Add a new restock of product\n";
			cout << "2. Display completed restock request\n";
			cout << "3. Searching completed restock request\n";
			cout << "4. Display pending restock request\n";
			cout << "5. Search pending restock requests\n";
			cout << "6. Display nearly sold out or out of stock product\n";
			cout << "7. Approve pending restock requests\n";
			cout << "8. Quit\n";

			cout << "\n";
			cout << "Please select one from the table above\n";
			message = "Enter : ";
			user.getInt(message, selection, 1, 8);
			system("cls");
			switch (selection)
			{
			case insert:
				restock.insertRestockRequest(staff);
				break;
			case viewCompleted:
				restock.viewRestockRequestPrice();
				message = "Do you want to search specific completed restock request? <Y/N> : ";
				user.getChoice(message, choice);
				if (choice[0] == 'Y')
				{
					restock.searchRequestPrice();
				}
				system("cls");
				break;
			case searchCompleted:
				restock.searchRequestPrice();
				break;
			case view:
				restock.viewRestockRequestDetail();
				message = "Do you want to search specific restock request? <Y/N> : ";
				user.getChoice(message, choice);
				if (choice[0] == 'Y')
				{
					restock.searchRequestDetail();
				}
				system("cls");
				break;
			case search:
				restock.searchRequestDetail();
				break;
			case viewProduct:
				productManager.viewSoldOutProduct();
				message = "Do you want to add a restock of product? <Y/N> : ";
				user.getChoice(message, choice);
				system("cls");
				if (choice[0] == 'Y')
				{
					restock.insertRestockRequest(staff);
				}
				break;
			case update:
				restock.approveRequest();
				break;
			case quit:
				return;
				break;
			}
			productManager.autoUpdateStatus();
		}
		break;
	case manager:
		while (1)
		{
			enum List { view = 1, search = 2, update = 3, quit = 4 };
			message = "Restock Request Menu";
			user.drawTittle(message, message.length() + 1, 8);
			cout << "1. Display pending restock requests\n";
			cout << "2. Search pending restock requests\n";
			cout << "3. Approve pending restock requests\n";
			cout << "4. Quit\n";

			cout << "\n";
			cout << "Please select one from the table above\n";
			message = "Enter : ";
			user.getInt(message, selection, 1, 4);
			system("cls");
			switch (selection)
			{
			case view:
				restock.viewRestockRequestDetail();
				message = "Do you want to search specific product? <Y/N> : ";
				user.getChoice(message, choice);
				if (choice[0] == 'Y')
				{
					restock.searchRequestDetail();
				}
				break;
			case search:
				restock.searchRequestDetail();
				break;
			case update:
				restock.approveRequest();
				break;
			case quit:
				return;
				break;
			}
			productManager.autoUpdateStatus();
		}
		break;
	case worker:
		while (1)
		{
			enum List { insert = 1, view = 2, search = 3, viewProduct = 4, update = 5, cancel = 6, quit = 7 };
			message = "Restock Request Menu";
			user.drawTittle(message, message.length() + 1, 8);
			cout << "1. Add a new restock request\n";
			cout << "2. Display pending restock request\n";
			cout << "3. Search pending restock requests\n";
			cout << "4. Display nearly sold out or out of stock product\n";
			cout << "5. Modify your restock request information\n";
			cout << "6. Cancel your restock request\n";
			cout << "7. Quit\n";

			cout << "\n";
			cout << "Please select one from the table above\n";
			message = "Enter : ";
			user.getInt(message, selection, 1, 7);
			system("cls");

			switch (selection)
			{
			case insert:
				restock.insertRestockRequest(staff);
				break;
			case view:
				restock.viewRequest();
				message = "Do you want to search specific restock request? <Y/N> : ";
				user.getChoice(message, choice);
				if (choice[0] == 'Y')
				{
					restock.searchRequest();
				}
				system("cls");
				break;
			case search:
				restock.searchRequest();
				break;
			case viewProduct:
				productManager.viewSoldOutProduct();
				message = "Do you want request a restock? <Y/N> : ";
				user.getChoice(message, choice);
				system("cls");
				if (choice[0] == 'Y')
				{
					restock.insertRestockRequest(staff);
				}
				break;
			case update:
				restock.updateMenu(staff);
				break;
			case cancel:
				restock.deleteRequest(staff);
				break;
			case quit: 
				return;
				break;
			}
		}
		break;
	}

}

void supplierMenu(Staff* staff)
{
	UserInputManager user;
	string message;
	string choice;
	int selection;
	SupplierManager supplierManager;
	UserManager userManager;

	enum List { insert = 1, view = 2, update = 3, quit = 4 };

	while (1)
	{
		message = "Supplier Menu";
		user.drawTittle(message, message.length() + 1, 8);
		cout << "1. Add a new supplier\n";
		cout << "2. Display all supplier\n";
		cout << "3. Modify supplier information\n";
		cout << "4. Quit\n";

		cout << "Please select one from the table above\n";
		message = "Enter : ";
		user.getInt(message, selection, 1, 4);

		switch (selection)
		{
		case insert:
			system("cls");
			supplierManager.insertSupplier();
			break;
		case view:
			system("cls");
			supplierManager.viewSupplier();
			system("pause");
			system("cls");
			break;
		case update:
			system("cls");
			supplierManager.updateMenu();
		case quit:
			system("cls");
			return;
			break;
		}
	}
}

void productTypeMenu(Staff* staff)
{
	UserInputManager user;
	string message;
	string choice;
	int selection;
	ProductTypeManager productType;
	UserManager userManager;

	enum List { insert = 1, view = 2, update = 3, quit = 4 };

	while (1)
	{
		message = "Product Type Menu";
		user.drawTittle(message, message.length() + 1, 8);
		cout << "1. Add a new produuct type\n";
		cout << "2. Display all product type\n";
		cout << "3. Update product type information\n";
		cout << "4. Quit\n";

		cout << "\nPlease select one from the table above\n";
		message = "Enter : ";
		user.getInt(message, selection, 1, 4);

		switch (selection)
		{
		case insert:
			system("cls");
			productType.insertProductType();
			break;
		case view:
			system("cls");
			productType.viewProductType();
			system("pause");
			system("cls");
			break;
		case update:
			system("cls");
			productType.updateMenu();
		case quit:
			system("cls");
			return;
			break;
		}
	}
}

void financialReport(Staff* staff)
{
	bool quit1 = false;
	bool quit2 = false;
	bool quit3 = false;
	bool quit4 = false;
	bool quit5 = false;
	if (staff->RoleID != 9001)
	{
		return;
	}
	char table;
	char time;
	

	while (1)
	{
		cout << "Press corresponding number to view the table\n";
		cout << "1. Product vs Sales & Cost\n";
		cout << "2. Supplier vs Sales & Cost\n";
		cout << "3. Payment method vs Sales\n";
		cout << "4. Product Type vs Sales & Cost\n";
		cout << "5. Sales & Cost last 30 days\n";
		cout << "6. Profit of last 3 months\n";
		cout << "7. Product vs Quantity sold\n";
		cout << "8. Orders completed last 30 days\n";
		cout << "9. All time record\n";
		cout << "0. Quit\n";

		table = _getch();

		switch (table)
		{
		case 49: //1
			system("cls");
			while (1)
			{
				quit1 = false;
				cout << "Press corresponding number to change the duration\n";
				cout << "1. Today\n";
				cout << "2. Last 10 day\n";
				cout << "3. Last month\n";
				cout << "4. Last 3 month\n";
				cout << "5. Last year\n";
				cout << "6. Quit\n";
				time = _getch();
				system("cls");
				switch (time)
				{
				case 49:
					productGraph("0 day");
					break;
				case 50:
					productGraph("10 day");
					break;
				case 51:
					productGraph("30 day");
					break;
				case 52:
					productGraph("3 month");
					break;
				case 53:
					productGraph("1 year");
					break;
				case 54:
					quit1 = true;
					break;
				}
				if (quit1)
				{
					break;
				}
			}
			break;
		case 50: //2
			system("cls");
			while (1)
			{
				quit2 = false;
				cout << "Press corresponding number to change the duration\n";
				cout << "1. Today\n";
				cout << "2. Last 10 day\n";
				cout << "3. Last month\n";
				cout << "4. Last 3 month\n";
				cout << "5. Last year\n";
				cout << "6. Quit\n";
				time = _getch();
				system("cls");
				switch (time)
				{
				case 49:
					supplierGraph("0 day");
					break;
				case 50:
					supplierGraph("10 day");
					break;
				case 51:
					supplierGraph("30 day");
					break;
				case 52:
					supplierGraph("3 month");
					break;
				case 53:
					supplierGraph("1 year");
					break;
				case 54:
					quit2 = true;
					break;
				}
				if (quit2)
				{
					break;
				}
			}
			break;
		case 51: //3
			system("cls");
			while (1)
			{
				quit3 = false;
				cout << "Press corresponding number to change the duration\n";
				cout << "1. Today\n";
				cout << "2. Last 10 day\n";
				cout << "3. Last month\n";
				cout << "4. Last 3 month\n";
				cout << "5. Last year\n";
				cout << "6. Quit\n";
				time = _getch();
				system("cls");
				switch (time)
				{
				case 49:
					paymentGraph("0 day");
					break;
				case 50:
					paymentGraph("10 day");
					break;
				case 51:
					paymentGraph("30 day");
					break;
				case 52:
					paymentGraph("3 month");
					break;
				case 53:
					paymentGraph("1 year");
					break;
				case 54:
					quit3 = true;
					break;
				}
				if (quit3)
				{
					break;
				}
			}
			break;
		case 52: //4
			system("cls");
			while (1)
			{
				quit4 = false;
				cout << "Press corresponding number to change the duration\n";
				cout << "1. Today\n";
				cout << "2. Last 10 day\n";
				cout << "3. Last month\n";
				cout << "4. Last 3 month\n";
				cout << "5. Last year\n";
				cout << "6. Quit\n";
				time = _getch();
				system("cls");
				switch (time)
				{
				case 49:
					productTypeGraph("0 day");
					break;
				case 50:
					productTypeGraph("10 day");
					break;
				case 51:
					productTypeGraph("30 day");
					break;
				case 52:
					productTypeGraph("3 month");
					break;
				case 53:
					productTypeGraph("1 year");
					break;
				case 54:
					quit4 = true;
					break;
				}
				if (quit4)
				{
					break;
				}
			}
			break;
		case 53: //5
			system("cls");
			dateGraph("30 day");
			system("pause");
			system("cls");
			break;
		case 54: //6
			system("cls");
			profitGraph();
			system("pause");
			system("cls");
			break;
		case 55: //7
			system("cls");
			while (1)
			{
				quit5 = false;
				cout << "Press corresponding number to change the duration\n";
				cout << "1. Today\n";
				cout << "2. Last 10 day\n";
				cout << "3. Last month\n";
				cout << "4. Last 3 month\n";
				cout << "5. Last year\n";
				cout << "6. Quit\n";
				time = _getch();
				system("cls");
				switch (time)
				{
				case 49:
					productQuantityGraph("0 day");
					break;
				case 50:
					productQuantityGraph("10 day");
					break;
				case 51:
					productQuantityGraph("30 day");
					break;
				case 52:
					productQuantityGraph("3 month");
					break;
				case 53:
					productQuantityGraph("1 year");
					break;
				case 54:
					quit5 = true;
					break;
				}
				if (quit5)
				{
					break;
				}
			}
			system("cls");
			break;
		case 56: //8
			system("cls");
			orderQuantityGraph();
			system("pause");
			system("cls");
			break;
		case 57: //9
			system("cls");
			record();
			system("pause");
			system("cls");
			break;
		case 48: //0
			system("cls");
			return;
			break;
		}
	}
}

void productGraph(string duration)
{
	Graph graph;
	DatabaseConnection dbConn;
	PreparedStatement* ps;
	ResultSet* rs;
	UserInputManager user;
	string message;
	string day;

	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	string product[100] = { "" };
	int totalSales[100] = { 0 };
	int totalCost[100] = { 0 };
	int i = 0;

	day = duration;
	if (duration == "0 day")
	{
		day = "1 day";
	}

	ps = dbConn.prepareStatement("SELECT ProductID,ProductName, sum(`order`.subtotal) AS 'SALES' FROM "
		"(SELECT orderDate,orderdetail.ProductID,ProductName,OrderStatus,(OriginalPrice-Discount)*Quantity AS Subtotal FROM `Order` "
		"INNER JOIN orderdetail ON `Order`.OrderID = orderdetail.OrderID INNER JOIN Product ON Product.ProductID=orderdetail.ProductID) `Order` "
		"WHERE OrderStatus='Completed' AND(OrderDate >= current_date - interval " + duration + ") GROUP BY ProductID ORDER BY SALES DESC");

	rs = ps->executeQuery();
	if (!rs->rowsCount())
	{
		SetConsoleTextAttribute(hConsole, 4);
		message = "No product sold " + day;
		user.drawTittle(message, message.length() + 1, 100);
		SetConsoleTextAttribute(hConsole, 7);
	}
	else
	{
		while (rs->next() && i < rs->rowsCount())
		{
			product[i] = rs->getString(2);
			totalSales[i] = rs->getInt(3);
			i++;
			if (i == 100)
			{
				break;
			}
		}

		cout << endl << endl << endl << endl << endl;
		message = "Graph Product Sales versus Product in " + day;

		user.drawTittle(message, message.length() + 1, 90);
		SetConsoleTextAttribute(hConsole, 15);
		graph.graphVertical(totalSales, i, "Sales(RM)", "Product");
		SetConsoleTextAttribute(hConsole, 7);
		graph.graphTable(totalSales, product, "Product", i, 80);

		//clear all product[]
		for (int i = 0; i < 100; i++)
		{
			product[i] = "";
		}
	}
	//cost
	ps = dbConn.prepareStatement
	("SELECT ProductID,ProductName,sum(restockrequest.subtotal) AS 'Cost' FROM "
	 "(SELECT Product.ProductID,RequestStatus,RestockDateOrdered,ProductName,Quantity*ProductBuyPrice AS 'Subtotal' "
	 "FROM restockrequest INNER JOIN Product ON restockrequest.ProductID = Product.ProductID )restockrequest "
	 "WHERE RequestStatus='Approved' AND RestockDateOrdered >=(current_Date - interval " + duration + ") GROUP BY ProductID ORDER BY COST DESC");

	cout << endl << endl << endl << endl << endl;

	int j = 0;
	rs = ps->executeQuery();

	if (!rs->rowsCount())
	{
		SetConsoleTextAttribute(hConsole, 4);
		message = "No restock request approved in " + day;
		user.drawTittle(message, message.length() + 1, 92);
		SetConsoleTextAttribute(hConsole, 7);
	}
	else
	{
		while (rs->next() && j < rs->rowsCount())
		{
			product[j] = rs->getString(2);
			totalCost[j] = rs->getInt(3);
			j++;
			if (j == 100)
			{
				break;
			}
		}
		message = "Graph Product Cost versus Product in " + day;
		user.drawTittle(message, message.length() + 1, 90);
		SetConsoleTextAttribute(hConsole, 15);
		graph.graphHorizontal(totalCost, product, "Product", "Cost(RM)", j);
		SetConsoleTextAttribute(hConsole, 7);
		graph.graphTable(totalCost, product, "Product", j, 80);
	}
	delete rs;
	delete ps;
}

void supplierGraph(string duration)
{
	Graph graph;
	DatabaseConnection dbConn;
	PreparedStatement* ps;
	ResultSet* rs;
	UserInputManager user;
	string message;

	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	string supplier[50] = { "" };
	int totalSales[50] = { 0 };
	int totalCost[50] = { 0 };
	
	string day = duration;
	if (duration == "0 day")
	{
		day = "1 day";
	}

	//sales
	int i = 0;
	ps = dbConn.prepareStatement("SELECT SupplierName, sum(`order`.subtotal) AS 'SALES' FROM "
		"(SELECT orderDate, Supplier.SupplierID, SupplierName, OrderStatus, (OriginalPrice - Discount) * Quantity AS Subtotal FROM `Order` "
		"INNER JOIN orderdetail ON `Order`.OrderID = orderdetail.OrderID INNER JOIN Product ON Product.ProductID = orderdetail.ProductID INNER JOIN Supplier ON Product.SupplierID = Supplier.SupplierID) `Order` "
		"WHERE OrderStatus = 'Completed' AND(OrderDate >= current_date - interval " + duration + ") GROUP BY SupplierID ORDER BY SALES DESC");

	rs = ps->executeQuery();
	if (!rs->rowsCount())
	{
		SetConsoleTextAttribute(hConsole, 4);
		message = "No product sold in " + day;
		user.drawTittle(message, message.length() + 1, 95);
		SetConsoleTextAttribute(hConsole, 7);
	}
	else
	{
		while (rs->next() && i <rs->rowsCount())
		{
			supplier[i] = rs->getString(1);
			totalSales[i] = rs->getInt(2);
			i++;
			if (i == 50)
			{
				break;
			}
		}

		cout << endl << endl << endl << endl << endl;
		message = "Graph Supplier versus Total Sales(RM) in " + day;
		user.drawTittle(message, message.length() + 1, 60);
		SetConsoleTextAttribute(hConsole, 15);
		graph.graphHorizontal(totalSales, supplier, "Supplier", "Sales(RM)", i);
		SetConsoleTextAttribute(hConsole, 7);
		graph.graphTable(totalSales, supplier, "Supplier", i, 55);

		//clear all suppler[]
		for (int i = 0; i < 50; i++)
		{
			supplier[i] = "";
		}
	}
	//cost
	int j = 0;
	ps = dbConn.prepareStatement("SELECT SupplierName,sum(restockrequest.subtotal) AS 'Cost' FROM "
		"(SELECT Supplier.SupplierID, SupplierName, RequestStatus, RestockDateOrdered, Quantity * ProductBuyPrice AS 'Subtotal' "
		"FROM restockrequest INNER JOIN Product ON restockrequest.ProductID = Product.ProductID INNER JOIN Supplier ON Product.SupplierID = Supplier.SupplierID)restockrequest "
		"WHERE RequestStatus = 'Approved' AND RestockDateOrdered >= (current_Date - interval " + duration + ") GROUP BY SupplierID ORDER BY COST DESC");

	cout << endl << endl << endl << endl << endl;

	rs = ps->executeQuery();
	if (!rs->rowsCount())
	{
		SetConsoleTextAttribute(hConsole, 4);
		message = "No restock request approved in " + day;
		user.drawTittle(message, message.length() + 1, 92);
		SetConsoleTextAttribute(hConsole, 7);
	}
	else
	{
		while (rs->next() && j < rs->rowsCount())
		{
			supplier[j] = rs->getString(1);
			totalCost[j] = rs->getInt(2);
			if (j == 50)
			{
				break;
			}
			j++;
		}
		message = "Graph Total Cost(RM) versus Supplier in " + duration;
		user.drawTittle(message, message.length() + 1, 15);
		SetConsoleTextAttribute(hConsole, 15);
		graph.graphVertical(totalCost, j, "Cost(RM)", "Supplier");
		SetConsoleTextAttribute(hConsole, 7);
		graph.graphTable(totalCost, supplier, "Supplier", j, 10);
	}
	delete rs;
	delete ps;
}

void paymentGraph(string duration)
{
	Graph graph;
	DatabaseConnection dbConn;
	PreparedStatement* ps;
	ResultSet* rs;
	UserInputManager user;
	string message;

	const int n = 3;

	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	string payment[n] = { "" };
	int totalSales[n] = { 0 };

	string day = duration;
	if (duration == "0 day")
	{
		day = "1 day";
	}

	//sales
	int i = 0;
	ps = dbConn.prepareStatement("SELECT OrderPayment, sum(`order`.subtotal) AS 'SALES' FROM (SELECT orderDate,OrderPayment,OrderStatus,(OriginalPrice-Discount)*Quantity AS Subtotal "
		"FROM `Order` INNER JOIN orderdetail ON `Order`.OrderID = orderdetail.OrderID) `Order` "
		"WHERE OrderStatus='Completed' AND(OrderDate >= current_date - interval " + duration + ") GROUP BY OrderPayment ORDER BY SALES DESC");

	rs = ps->executeQuery();
	if (!rs->rowsCount())
	{
		SetConsoleTextAttribute(hConsole, 4);
		message = "No product sold in " + day;
		user.drawTittle(message, message.length() + 1, 95);
		SetConsoleTextAttribute(hConsole, 7);
	}
	else
	{
		while (rs->next())
		{
			payment[i] = rs->getString(1);
			totalSales[i] = rs->getInt(2);
			i++;
			if (i == n)
			{
				break;
			}
		}
		cout << endl << endl << endl << endl << endl;
		message = "Graph Payment method versus Total Sales(RM) in " + day;
		user.drawTittle(message, message.length() + 1, 60);
		SetConsoleTextAttribute(hConsole, 15);
		graph.graphHorizontal(totalSales, payment, "Payment Method", "Sales(RM)", i);
		SetConsoleTextAttribute(hConsole, 7);
		graph.graphTable(totalSales, payment, "Payment Method", i, 55);
	}
	delete ps;
	delete rs;
}

void productTypeGraph(string duration)
{
	Graph graph;
	DatabaseConnection dbConn;
	PreparedStatement* ps;
	ResultSet* rs;
	UserInputManager user;
	string message;
	string day;

	string productType[50] = { "" };
	int totalSales[50] = { 0 };
	int totalCost[50] = { 0 };

	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	day = duration;
	if (duration == "0 day")
	{
		day = "1 day";
	}
	//sales
	ps = dbConn.prepareStatement("SELECT TypeName, sum(`order`.subtotal) AS 'SALES' FROM "
		"(SELECT orderDate, Product.TypeID, TypeName, OrderStatus, (OriginalPrice - Discount) * Quantity AS Subtotal FROM `Order` "
		"INNER JOIN orderdetail ON `Order`.OrderID = orderdetail.OrderID "
		"INNER JOIN Product ON Product.ProductID = orderdetail.ProductID "
		"INNER JOIN ProductType ON ProductType.TypeID = Product.TypeID) `Order` "
		"WHERE OrderStatus = 'Completed' AND(OrderDate >= current_date - interval " + duration + ") GROUP BY TypeID ORDER BY SALES DESC");

	int i = 0;
	rs = ps->executeQuery();
	if (!rs->rowsCount())
	{
		SetConsoleTextAttribute(hConsole, 4);
		message = "No product sold in " + day;
		user.drawTittle(message, message.length() + 1, 95);
		SetConsoleTextAttribute(hConsole, 7);
	}
	else
	{
		while (rs->next() && i < rs->rowsCount())
		{
			productType[i] = rs->getString(1);
			totalSales[i] = rs->getInt(2);
			i++;
			if (i == 50)
			{
				break;
			}
		}
		//graph
		cout << endl << endl << endl << endl << endl;
		message = "Graph Product Type versus Total Sales(RM) in " + day;
		user.drawTittle(message, message.length() + 1, 15);
		SetConsoleTextAttribute(hConsole, 15);
		graph.graphVertical(totalSales, i, "Sales(RM)", "Product Type");
		SetConsoleTextAttribute(hConsole, 7);
		graph.graphTable(totalSales, productType, "Product Type", i, 10);
		//clear all productType[]
		for (int i = 0; i < 50; i++)
		{
			productType[i] = "";
		}
	}

	cout << endl << endl << endl << endl << endl;
	//cost
	ps = dbConn.prepareStatement("SELECT TypeName,sum(restockrequest.subtotal) AS 'Cost' FROM "
		"(SELECT ProductType.TypeID, TypeName, RequestStatus, RestockDateOrdered, Quantity * ProductBuyPrice AS 'Subtotal' FROM restockrequest "
		"INNER JOIN Product ON restockrequest.ProductID = Product.ProductID "
		"INNER JOIN ProductType ON ProductType.TypeID = Product.TypeID)restockrequest "
		"WHERE RequestStatus = 'Approved' AND RestockDateOrdered >= (current_Date - interval " + duration + ") GROUP BY TypeID ORDER BY COST DESC");

	rs = ps->executeQuery();
	if (!rs->rowsCount())
	{
		SetConsoleTextAttribute(hConsole, 4);
		message = "No restock request approved in " + day;
		user.drawTittle(message, message.length() + 1, 92);
		SetConsoleTextAttribute(hConsole, 7);
	}
	else
	{
		int j = 0;
		while (rs->next() && j < rs->rowsCount())
		{
			productType[j] = rs->getString(1);
			totalCost[j] = rs->getInt(2);
			j++;
			if (j == 50)
			{
				break;
			}
		}
		//graph
		message = "Graph Total Sales(RM) versus Product Type in " + day;
		user.drawTittle(message, message.length() + 1, 60);
		SetConsoleTextAttribute(hConsole, 15);
		graph.graphHorizontal(totalCost, productType, "Product Type", "Sales(RM)", j);
		SetConsoleTextAttribute(hConsole, 7);
		graph.graphTable(totalCost, productType, "Product Type", j, 55);
	}
}

void dateGraph(string duration)
{
	Graph graph;
	DatabaseConnection dbConn;
	PreparedStatement* ps;
	ResultSet* rs;
	UserInputManager user;
	string message;
	string day;

	string date[30] = { "" };
	string wholeDate[30] = { "" };
	int totalSales[30] = { 0 };
	int totalCost[30] = { 0 };

	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	//sales
	ps = dbConn.prepareStatement("SELECT OrderDate, sum(`order`.subtotal) AS 'SALES' FROM "
		"(SELECT orderDate, OrderStatus, (OriginalPrice - Discount) * Quantity AS Subtotal FROM `Order` "
		"INNER JOIN orderdetail ON `Order`.OrderID = orderdetail.OrderID) `Order` "
		"WHERE OrderStatus = 'Completed' AND(OrderDate >= current_date - interval " + duration + ") GROUP BY OrderDate "
		"UNION "
		"SELECT dt, 0 FROM `calendar_table` WHERE dt not in(SELECT OrderDate FROM "
		"(SELECT orderDate, OrderStatus, (OriginalPrice - Discount) * Quantity AS Subtotal FROM `Order` "
		"INNER JOIN orderdetail ON `Order`.OrderID = orderdetail.OrderID) `Order` "
		"WHERE OrderStatus = 'Completed' AND(OrderDate >= current_date - interval " + duration + ") GROUP BY OrderDate) AND dt BETWEEN current_date - interval " + duration + " AND current_date "
		"ORDER BY OrderDate");

	rs = ps->executeQuery();
	int i = 0;
	while (rs->next())
	{
		wholeDate[i] = rs->getString(1);
		date[i] = wholeDate[i].substr(5, 5);
		totalSales[i] = rs->getInt(2);
		i++;
		if (i == 30)
		{
			break;
		}
	}

	//graph
	cout << endl << endl << endl << endl << endl;
	message = "Graph Date versus Total Sales(RM) in " + duration;
	user.drawTittle(message, message.length() + 1, 75);
	SetConsoleTextAttribute(hConsole, 15);
	graph.graphVerticalBig(totalSales, date, i, "Sales(RM)", "Date");
	SetConsoleTextAttribute(hConsole, 7);
	graph.graphTable(totalSales, wholeDate, "Date", i, 68);

	//clear all date[]
	for (int i = 0; i < 30; i++)
	{
		date[i] = "";
		wholeDate[i] = "";
	}

	//cost
	ps = dbConn.prepareStatement("SELECT RestockDateOrdered,sum(restockrequest.subtotal) AS 'Cost' FROM "
		"(SELECT  RequestStatus, RestockDateOrdered, Quantity * ProductBuyPrice AS 'Subtotal' FROM restockrequest "
		"INNER JOIN Product ON restockrequest.ProductID = Product.ProductID)restockrequest "
		"WHERE RequestStatus = 'Approved' AND RestockDateOrdered >= (current_Date - interval " + duration + ") GROUP BY RestockDateOrdered "
		"UNION "
		"SELECT dt, 0 FROM `calendar_table` WHERE dt NOT IN(SELECT RestockDateOrdered FROM "
		"(SELECT  RequestStatus, RestockDateOrdered, Quantity * ProductBuyPrice AS 'Subtotal' FROM restockrequest "
		"INNER JOIN Product ON restockrequest.ProductID = Product.ProductID)restockrequest "
		"WHERE RequestStatus = 'Approved' AND RestockDateOrdered >= (current_Date - interval " + duration + ") GROUP BY ORestockDateOrdered) "
		"AND dt BETWEEN CURRENT_DATE - INTERVAL " + duration + " AND CURRENT_DATE "
		"ORDER BY RestockDateOrdered");

	rs = ps->executeQuery();
	int j = 0;
	while (rs->next())
	{
		wholeDate[j] = rs->getString(1);
		date[j] = wholeDate[j].substr(5, 5);
		totalCost[j] = rs->getInt(2);
		j++;
		if (j == 30)
		{
			break;
		}
	}

	//graph
	cout << endl << endl << endl << endl << endl;
	message = "Graph Date versus Total Cost(RM) in " + duration;
	user.drawTittle(message, message.length() + 1, 75);
	SetConsoleTextAttribute(hConsole, 15);
	graph.graphVerticalBig(totalCost, date, i, "Cost(RM)", "Date");
	SetConsoleTextAttribute(hConsole, 7);
	graph.graphTable(totalCost, wholeDate, "Date", i, 68);
}
 
void profitGraph()
{
	Graph graph;
	DatabaseConnection dbConn;
	PreparedStatement* ps;
	ResultSet* rs;
	UserInputManager user;
	string message;
	string day;

	string date[3] = { "" };
	int grossProfit[3] = { 0 };

	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ps = dbConn.prepareStatement("SELECT  CONCAT(MONTHNAME(OrderDate),' ',YEAR(OrderDate))  , sum(`order`.Profit) AS 'SALES' FROM  "
		"(SELECT orderDate, OrderStatus, ((OriginalPrice - Discount) * Quantity - ProductBuyPrice) AS 'Profit' FROM `Order` "
		"INNER JOIN orderdetail ON `Order`.OrderID = orderdetail.OrderID "
		"INNER JOIN Product ON Product.ProductID = OrderDetail.ProductID) `Order` "
		"WHERE OrderStatus = 'Completed' AND(MONTH(OrderDate) = MONTH(CURRENT_DATE - INTERVAL 3 month)) "
		"UNION "
		"SELECT  CONCAT(MONTHNAME(OrderDate), ' ', YEAR(OrderDate)), sum(`order`.Profit) AS 'SALES' FROM "
		"(SELECT orderDate, OrderStatus, ((OriginalPrice - Discount) * Quantity - ProductBuyPrice) AS 'Profit' FROM `Order` "
		"INNER JOIN orderdetail ON `Order`.OrderID = orderdetail.OrderID "
		"INNER JOIN Product ON Product.ProductID = OrderDetail.ProductID) `Order` "
		"WHERE OrderStatus = 'Completed' AND(MONTH(OrderDate) = MONTH(CURRENT_DATE - INTERVAL 2 month)) "
		"UNION "
		"SELECT  CONCAT(MONTHNAME(OrderDate), ' ', YEAR(OrderDate)), sum(`order`.Profit) AS 'SALES' FROM "
		"(SELECT orderDate, OrderStatus, ((OriginalPrice - Discount) * Quantity - ProductBuyPrice) AS 'Profit' FROM `Order` "
		"INNER JOIN orderdetail ON `Order`.OrderID = orderdetail.OrderID "
		"INNER JOIN Product ON Product.ProductID = OrderDetail.ProductID) `Order` "
		"WHERE OrderStatus = 'Completed' AND(MONTH(OrderDate) = MONTH(CURRENT_DATE - INTERVAL 1 month)) ");

	rs = ps->executeQuery();
	int i = 0;
	while (rs->next())
	{
		date[i] = rs->getString(1);
		grossProfit[i] = rs->getInt(2);
		i++;
	}

	//graph
	cout << endl << endl << endl << endl << endl;
	message = "Graph Date versus Total Sales(RM) in last 3 month" ;
	user.drawTittle(message, message.length() + 1, 75);
	SetConsoleTextAttribute(hConsole, 15);
	graph.graphHorizontal(grossProfit, date ,"Gross Profit(RM)", "Date",i);
	SetConsoleTextAttribute(hConsole, 7);
	graph.graphTable(grossProfit, date, "Date", "Profit", i, 68);
}

void productQuantityGraph(string duration)
{
	Graph graph;
	DatabaseConnection dbConn;
	PreparedStatement* ps;
	ResultSet* rs;
	UserInputManager user;
	string message;
	string day;

	string product[20] = { "" };
	int quantity[20] = { 0 };

	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	day = duration;
	if (duration == "0 day")
	{
		day = "1 day";
	}

	ps = dbConn.prepareStatement("SELECT ProductName ,SUM(Quantity) AS SOLD FROM `Order` INNER JOIN OrderDetail ON `Order`.`OrderID` = OrderDetail.OrderID "
		"INNER JOIN Product ON Product.ProductID = orderdetail.ProductID "
		"WHERE OrderStatus = 'Completed'  AND OrderDate >= CURRENT_DATE - INTERVAL " + duration + " GROUP BY Product.ProductID ORDER BY SOLD DESC");

	int i = 0;
	rs = ps->executeQuery();
	if (!rs->rowsCount())
	{
		SetConsoleTextAttribute(hConsole, 4);
		message = "No product sold in " + day;
		user.drawTittle(message, message.length() + 1, 95);
		SetConsoleTextAttribute(hConsole, 7);
	}
	else
	{
		while (rs->next() && i < rs->rowsCount())
		{
			product[i] = rs->getString(1);
			quantity[i] = rs->getInt(2);
			i++;
			if (i == 20)
			{
				break;
			}
		}
		//graph
		cout << endl << endl << endl << endl << endl;
		message = "Graph Quantity(Unit) versus Product in " + day;
		user.drawTittle(message, message.length() + 1, 75);
		SetConsoleTextAttribute(hConsole, 15);
		graph.graphVertical(quantity, i, "Quantity(Unit)", "Product");
		SetConsoleTextAttribute(hConsole, 7);
		graph.graphTable(quantity, product, "Product", "Quantity(Unit)", i, 68);

		for (int i = 0; i < 20; i++)
		{
			product[i] = "";
			quantity[i] = 0;
		}
	}

	cout << endl << endl << endl << endl << endl;

	ps = dbConn.prepareStatement("SELECT ProductName,SUM(Quantity) AS BUY FROM RestockRequest INNER JOIN Product ON Product.ProductID = RestockRequest.ProductID "
		"WHERE RequestStatus = 'Approved' AND RestockDateOrdered >= CURRENT_DATE - INTERVAL " + duration + " GROUP BY RestockRequest.ProductID ORDER BY BUY DESC");

	int j = 0;
	rs = ps->executeQuery();
	if (!rs->rowsCount())
	{
		SetConsoleTextAttribute(hConsole, 4);
		message = "No restock request approved in " + day;
		user.drawTittle(message, message.length() + 1, 92);
		SetConsoleTextAttribute(hConsole, 7);
	}
	else
	{
		while (rs->next() && j < rs->rowsCount())
		{
			product[j] = rs->getString(1);
			quantity[j] = rs->getInt(2);
			j++;
			if (j == 20)
			{
				break;
			}
		}
		//graph
		message = "Graph Quantity(Unit) versus Product in " + day;
		user.drawTittle(message, message.length() + 1, 75);
		SetConsoleTextAttribute(hConsole, 15);
		graph.graphHorizontal(quantity, product, "Product", "Quantity(Unit)", j);
		SetConsoleTextAttribute(hConsole, 7);
		graph.graphTable(quantity, product, "Product", "Quantity(Unit)", j, 68);
	}
	delete ps;
	delete rs;
}

void orderQuantityGraph()
{
	Graph graph;
	DatabaseConnection dbConn;
	PreparedStatement* ps;
	ResultSet* rs;
	UserInputManager user;
	string message;

	string date[35] = { "" };
	string wholeDate[35] = { "" };
	int quantity[35] = { 0 };

	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ps = dbConn.prepareStatement
	("SELECT OrderDate,COUNT(OrderStatus) FROM `Order` WHERE Orderstatus = 'Completed' AND OrderDate BETWEEN CURRENT_DATE - INTERVAL 1 month AND CURRENT_DATE GROUP BY OrderDate "
	 "UNION "
	 "SELECT dt, 0 FROM `calendar_table` WHERE dt NOT IN(SELECT OrderDate FROM `Order` WHERE Orderstatus = 'Completed' AND OrderDate BETWEEN CURRENT_DATE - INTERVAL 3 month AND CURRENT_DATE GROUP BY OrderDate) "
	 "AND dt BETWEEN CURRENT_DATE - INTERVAL 1 month AND CURRENT_DATE  ORDER BY `OrderDate`");

	rs = ps->executeQuery();

	int i = 0;
	while (rs->next())
	{
		wholeDate[i] = rs->getString(1);
		date[i] = wholeDate[i].substr(5, 5);
		quantity[i] = rs->getInt(2);
		i++;
		if (i == 35)
		{
			break;
		}
	}
	//graph
	cout << endl << endl << endl << endl << endl;
	message = "Graph Date versus Total Order Completed in last 30 days";
	user.drawTittle(message, message.length() + 1, 75);
	SetConsoleTextAttribute(hConsole, 15);
	graph.graphVerticalBig(quantity, date, i, "Order Quantity", "Date");
	SetConsoleTextAttribute(hConsole, 7);
	graph.graphTable(quantity, wholeDate, "Date","Order Completed", i, 68);
}

void record()
{
	Graph graph;
	DatabaseConnection dbConn;
	PreparedStatement* ps;
	ResultSet* rs;
	UserInputManager user;
	string message;
	int n1 = 20;
	int n2 = 40;

	string object[3] = { "Sales","Cost","Profit" };
	int sale[3] = { 0 };

	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	message = "All time record in store";
	user.drawTittle(message, message.length() + 1, 20);

	ps = dbConn.prepareStatement("SELECT sum(`order`.subtotal) AS 'SALES' FROM "
		"(SELECT OrderStatus, (OriginalPrice - Discount) * Quantity AS Subtotal FROM `Order` "
		"INNER JOIN orderdetail ON `Order`.OrderID = orderdetail.OrderID) `Order` "
		"WHERE OrderStatus = 'Completed'");

	rs = ps->executeQuery();
	if (rs->next())
	{
		sale[0] = rs->getInt(1);
		user.drawRowHead("Sales", "RM " + to_string(rs->getInt(1)), n1, n2);
	}

	ps = dbConn.prepareStatement("SELECT sum(restockrequest.subtotal) AS 'Cost' FROM  "
		"(SELECT RequestStatus, RestockDateOrdered, Quantity * ProductBuyPrice AS 'Subtotal' FROM restockrequest "
		"INNER JOIN Product ON Product.ProductID = Product.ProductID)restockrequest "
		"WHERE RequestStatus = 'Approved' ");
	rs = ps->executeQuery();
	if (rs->next())
	{
		sale[1] = rs->getInt(1);
		user.drawRowBody("Cost", "RM " + to_string(rs->getInt(1)), n1, n2);
	}

	ps = dbConn.prepareStatement("SELECT  sum(`order`.Profit) AS 'SALES' FROM "
		"(SELECT OrderStatus, ((OriginalPrice - Discount) * Quantity - ProductBuyPrice) AS 'Profit' FROM `Order` "
		"INNER JOIN orderdetail ON `Order`.OrderID = orderdetail.OrderID "
		"INNER JOIN Product ON Product.ProductID = OrderDetail.ProductID) `Order` "
		"WHERE OrderStatus = 'Completed'");
	rs = ps->executeQuery();
	if (rs->next())
	{
		sale[2] = rs->getInt(1);
		user.drawRowTail("Profit", "RM " + to_string(rs->getInt(1)), n1, n2);
	}
	
	//graph
	SetConsoleTextAttribute(hConsole, 15);
	graph.graphHorizontal(sale, object, "Record", "RM", 3);
	SetConsoleTextAttribute(hConsole, 7);
}




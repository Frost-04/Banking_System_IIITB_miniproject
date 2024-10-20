#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <time.h>

#include "./structures/admin.h"
#include "./structures/customer.h"
#include "./structures/employee.h"
#include "./structures/manager.h"
#include "./structures/transaction.h"
#include "./structures/loan.h"

#include "./functions/customerActions.h"
#include "./functions/employeeActions.h"
#include "./functions/managerActions.h"
#include "./functions/adminActions.h"
#include "./functions/transactionActions.h"
#include "./functions/loanActions.h"
#include "./functions/countChecker.h"
#include "./functions/exitUserAction.h"

int ID;
int portNumber = 8082;

void serverTask(int sd)
{
	int option;
	bool authentication = 0;
	////////////////////////////////////////////////// -----LOGIN PROCESS----- ////////////////////////////////////////////////
	while (1)
	{
		read(sd, &option, sizeof(int));

		if (option == 1)
		{
			struct admin admin1;
			ID = admin1.adminID;
			read(sd, &admin1.password, sizeof(admin1.password));

			authentication = checkAdmin(sd, admin1);
			write(sd, &authentication, sizeof(bool));
		}

		else if (option == 2)
		{
			struct manager manager1;
			ID = manager1.managerID;
			read(sd, &manager1.password, sizeof(manager1.password));

			authentication = checkManager(sd, manager1);
			write(sd, &authentication, sizeof(bool));
		}

		else if (option == 3)
		{
			struct employee employee1;
			ID = employee1.employeeID;
			read(sd, &employee1.password, sizeof(employee1.password));

			authentication = checkEmployee(sd, employee1);
			write(sd, &authentication, sizeof(bool));
		}

		else if (option == 4)
		{
			struct customer customer1;
			ID = customer1.customerID;
			read(sd, &customer1.password, sizeof(customer1.password));

			authentication = checkCustomer(customer1);
			write(sd, &authentication, sizeof(bool));
		}
		else if (option == 5)
		{
			int choice;
			if (choice == 1)
			{
				int cCount = adminCount();
				write(sd, &cCount, sizeof(int));
				if (cCount == 0)
					continue;
				struct admin record;
				for (int i = 0; i < cCount; i++)
				{
					record = viewAllAccountsOfAdmin(i);
					write(sd, &record, sizeof(struct admin));
				}
			}
			else if (choice == 2)
			{
				int cCount = managerCount();
				write(sd, &cCount, sizeof(int));
				if (cCount == 0)
					continue;
				struct manager record;
				for (int i = 0; i < cCount; i++)
				{
					record = viewAllAccountsOfManager(i);
					write(sd, &record, sizeof(struct manager));
				}
			}
			else if (choice == 3)
			{
				int cCount = employeeCount();
				write(sd, &cCount, sizeof(int));
				if (cCount == 0)
					continue;
				struct employee record;
				for (int i = 0; i < cCount; i++)
				{
					record = viewAllAccountsOfEmployee(i);
					write(sd, &record, sizeof(struct employee));
				}
			}
			else if (choice == 4)
			{
				int cCount = customerCount();
				write(sd, &cCount, sizeof(int));
				if (cCount == 0)
					continue;
				struct customer record;
				for (int i = 0; i < cCount; i++)
				{
					record = viewAllAccountsOfCustomer(i);
					write(sd, &record, sizeof(struct customer));
				}
			}
			else if (choice == 5)
			{
				resetLoginStatus();
			}
		}
		else if (option == 6)
		{
			exitUserAction(ID, option);
			exit(0);
		}

		if (authentication)
			break;
	}

	////////////////////////////////////////////////// -----LOGIN PROCESS END----- ////////////////////////////////////////////////
	int input;
	bool result;

	read(sd, &input, sizeof(int));
	printf("\n%d\n", input);

	////////////////////////////////////////////////// -----ADMIN------ ////////////////////////////////////////////////

	if (option == 1)
	{
		switch (input)
		{
		case 1:
			struct manager newManager;
			int newManagerID;

			read(sd, &newManager, sizeof(struct manager));
			newManagerID = addManager(newManager);
			write(sd, &newManagerID, sizeof(int));
			break;

		case 2:
			struct employee newEmployee;
			int newEmployeeID;

			read(sd, &newEmployee, sizeof(struct employee));
			newEmployeeID = addEmployee(newEmployee);
			write(sd, &newEmployeeID, sizeof(int));
			break;

		case 3:
			struct customer newCustomer;
			int newCustomerID;

			read(sd, &newCustomer, sizeof(struct customer));
			newCustomerID = addCustomer(newCustomer);
			write(sd, &newCustomerID, sizeof(int));
			break;

		case 4:
			int modifyFromAdminChoice, namePasswordChoice, idToChange;
			char text[30];
			int activeStatus;
			bool activeStatusBool;

			read(sd, &modifyFromAdminChoice, sizeof(int));
			read(sd, &idToChange, sizeof(int));
			read(sd, &namePasswordChoice, sizeof(int));

			if (namePasswordChoice == 1 || namePasswordChoice == 2)
				read(sd, &text, sizeof(text));
			else if (namePasswordChoice == 3 && modifyFromAdminChoice == 2)
				read(sd, &activeStatus, sizeof(int));
			else
				break;

			activeStatusBool = activeStatus;
			if (modifyFromAdminChoice == 1)
			{
				if (idToChange > employeeCount() || idToChange < 1)
					result = false;
				else
					result = modifyEmployeeAccount(idToChange, namePasswordChoice, text);
			}

			else if (modifyFromAdminChoice == 2)
			{
				if (idToChange > customerCount() || idToChange < 1)
					result = false;
				else
					result = modifyAccount(idToChange, namePasswordChoice, text, activeStatusBool);
			}
			write(sd, &result, sizeof(bool));
			break;

		case 5:
			int viewEmployeeID;
			struct employee snapshotOfEmployeeID;
			read(sd, &viewEmployeeID, sizeof(int));
			result = (viewEmployeeID <= employeeCount() && viewEmployeeID > 0);
			write(sd, &result, sizeof(bool));
			if (!result)
				break;
			snapshotOfEmployeeID = viewEmployeeAccountDetails(viewEmployeeID);
			write(sd, &snapshotOfEmployeeID, sizeof(struct employee));
			break;

		case 6:
			int viewCustomerID;
			struct customer snapshotOfCustomerID;
			read(sd, &viewCustomerID, sizeof(int));
			result = (viewCustomerID <= customerCount() && viewCustomerID > 0);
			write(sd, &result, sizeof(bool));
			if (!result)
				break;
			snapshotOfCustomerID = viewAccountDetails(viewCustomerID);
			write(sd, &snapshotOfCustomerID, sizeof(struct customer));
			break;

		case 7:
			int empIdToPromote, resultOfPromotion;
			read(sd, &empIdToPromote, sizeof(int));
			resultOfPromotion = employeeToManagerPromotion(empIdToPromote);
			write(sd, &resultOfPromotion, sizeof(int));
			break;

		case 8:
			char changePassword[10];
			read(sd, &changePassword, sizeof(changePassword));
			changeAdminPassword(ID, changePassword);
			break;

		case 9:
			printf("Client Logged OUT!\n");
		case 10:
			exitUserAction(ID, option);
			exit(0);

		default:
			break;
		}
	}

	////////////////////////////////////////////////// -----MANAGER----- ////////////////////////////////////////////////
	else if (option == 2)
	{
		switch (input)
		{
		case 1:
			int changeState, customerIdToChangeState;
			read(sd, &customerIdToChangeState, sizeof(int));
			read(sd, &changeState, sizeof(int));

			if (customerIdToChangeState > customerCount() || customerIdToChangeState < 1)
				result = false;
			else
				result = modifyAccount(customerIdToChangeState, 3, "NULL", changeState);
			write(sd, &result, sizeof(bool));
			break;

		case 2:
			result = viewLoan(0);
			int recievedFdStatus;
			write(sd, &result, sizeof(bool));
			read(sd, &recievedFdStatus, sizeof(int));
			if (!result)
				break;
			int assignToEmployee, assignLoanID;
			read(sd, &assignLoanID, sizeof(assignLoanID));
			read(sd, &assignToEmployee, sizeof(assignToEmployee));
			if (assignToEmployee > employeeCount() || assignToEmployee < 1)
				result = false;
			else
				result = assignLoan(assignLoanID, assignToEmployee);

			write(sd, &result, sizeof(bool));

			break;

		case 3:
			break;

		case 4:
			char changePassword[10];
			read(sd, &changePassword, sizeof(changePassword));
			changeManagerPassword(ID, changePassword);
			break;

		case 5:
			printf("Client Logged OUT!\n");
		case 6:
			exitUserAction(ID, option);
			exit(0);
			break;

		default:
			break;
		}
	}
	////////////////////////////////////////////////// -----EMPLOYEE----- ////////////////////////////////////////////////
	else if (option == 3)
	{
		switch (input)
		{
		case 1:
			struct customer newCustomer;
			int newCustomerID;

			read(sd, &newCustomer, sizeof(struct customer));
			newCustomerID = addCustomer(newCustomer);
			write(sd, &newCustomerID, sizeof(int));
			break;

		case 2:
			int changeChoice, customerIdToChange;

			read(sd, &customerIdToChange, sizeof(int));
			result = (customerIdToChange <= customerCount() && customerIdToChange > 0);
			write(sd, &result, sizeof(bool));
			if (!result)
				break;
			read(sd, &changeChoice, sizeof(int));

			if (changeChoice == 1)
			{
				char changeName[30];
				read(sd, &changeName, sizeof(changeName));
				result = modifyAccount(customerIdToChange, changeChoice, changeName, 0);
			}
			else if (changeChoice == 2)
			{
				char changePassword[30];
				read(sd, &changePassword, sizeof(changePassword));
				result = modifyAccount(customerIdToChange, changeChoice, changePassword, 0);
			}
			else if (changeChoice == 3)
			{
				bool changeActiveStatus;
				int i;
				read(sd, &i, sizeof(int));
				changeActiveStatus = i;
				result = modifyAccount(customerIdToChange, changeChoice, "TEMP", changeActiveStatus);
			}
			write(sd, &result, sizeof(bool));
			break;

		case 3:
			int viewCustomerID;
			struct customer snapshotOfCustomerID;
			read(sd, &viewCustomerID, sizeof(int));

			result = (viewCustomerID <= customerCount() && viewCustomerID > 0);
			write(sd, &result, sizeof(bool));
			if (!result)
				break;

			snapshotOfCustomerID = viewAccountDetails(viewCustomerID);
			write(sd, &snapshotOfCustomerID, sizeof(struct customer));
			break;

		case 4:
			char changePassword[10];
			read(sd, &changePassword, sizeof(changePassword));
			changeEmployeePassword(ID, changePassword);
			break;

		case 5:
			result = viewLoan(ID);
			int recievedFdStatus;
			write(sd, &result, sizeof(bool));
			read(sd, &recievedFdStatus, sizeof(int));
			if (!result || recievedFdStatus == -1)
				break;

			int actionLoanID, action;
			read(sd, &actionLoanID, sizeof(int));
			read(sd, &action, sizeof(int));
			struct loan record = actionOnLoan(actionLoanID, action);
			if (record.loanID)
				addTransaction(record.loanee, -1, record.amount, 4);
			else
				addTransaction(record.loanee, -1, record.amount, 5);
			write(sd, &result, sizeof(bool));
			break;

		case 6:
			printf("Client Logged OUT!\n");
		case 7:
			exitUserAction(ID, option);
			exit(0);

		default:
			break;
		}
	}
	////////////////////////////////////////////////// -----CUSTOMER----- ////////////////////////////////////////////////
	else if (option == 4)
	{
		switch (input)
		{
		case 1:
			float currentBalance = viewBalance(ID);
			write(sd, &currentBalance, sizeof(float));
			break;

		case 2:
			float depositAmount;
			read(sd, &depositAmount, sizeof(float));
			result = deposit(ID, depositAmount);
			addTransaction(ID, -1, depositAmount, 1);
			write(sd, &result, sizeof(bool));

			break;

		case 3:
			float withdrawAmount;
			read(sd, &withdrawAmount, sizeof(float));
			result = withdraw(ID, withdrawAmount);
			addTransaction(ID, -1, withdrawAmount, 2);
			write(sd, &result, sizeof(bool));
			break;

		case 4:
			float transferAmount;
			int targetCustomerID;
			bool depositResult;
			read(sd, &transferAmount, sizeof(float));
			read(sd, &targetCustomerID, sizeof(int));

			result = (targetCustomerID <= customerCount() && targetCustomerID > 0);

			result = result && withdraw(ID, transferAmount);
			write(sd, &result, sizeof(bool));
			if (!result)
				break;
			depositResult = deposit(targetCustomerID, transferAmount);
			if (!depositResult)
				deposit(ID, transferAmount);
			write(sd, &depositResult, sizeof(bool));
			if (depositResult)
				addTransaction(ID, targetCustomerID, transferAmount, 3);
			break;

		case 5:
			float loanAmount;
			read(sd, &loanAmount, sizeof(float));
			result = applyLoan(ID, loanAmount);
			write(sd, &result, sizeof(bool));
			break;

		case 6:
			char password[10];
			read(sd, &password, sizeof(password));
			changeCustomerPassword(ID, password);
			break;

		case 7:
			char buffer[1024];
			read(sd, buffer, strlen(buffer));
			addFeedback(buffer);
			break;

		case 8:
			result = viewTransaction(ID);
			write(sd, &result, sizeof(bool));
			break;

		case 9:
			printf("Client Logged OUT!\n");
		case 10:
			exitUserAction(ID, option);
			exit(0);

		default:
			break;
		}
	}
}

////////////////////////////////////////////////// -----MAIN----- ////////////////////////////////////////////////
int main()
{
	struct sockaddr_in server, client;
	int sd, nsd, clientLen;
	bool result;
	sd = socket(AF_INET, SOCK_STREAM, 0);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(portNumber);
	bind(sd, (struct sockaddr *)&server, sizeof(server));
	listen(sd, 5);
	printf("Welcome!! ");
	printf("Waiting for Client to connect\n");
	while (1)
	{
		clientLen = sizeof(client);
		nsd = accept(sd, (struct sockaddr *)&client, &clientLen);

		write(1, "Connected to the client\n", sizeof("Connected to the client\n"));
		if (!fork())
		{
			close(sd);

			serverTask(nsd);
			exit(0);
		}
		else
		{
			close(nsd);
		}
	}
	return 0;
}

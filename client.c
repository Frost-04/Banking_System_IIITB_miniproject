#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>

#include "./structures/admin.h"
#include "./structures/customer.h"
#include "./structures/employee.h"
#include "./structures/manager.h"
#include "./structures/transaction.h"
#include "./structures/loan.h"

#include "./functions/exitUserAction.h"

int portNumber = 8082;

int option;

void chooseOption(int);
void userMenu(int);
void login(int);
void logout();

int sd, ID = -1;

void sigintHandler(int sig_num)
{
	printf("\nTerminating client gracefully...\n");
	if (sd != -1)
	{
		if (ID != -1)
			exitUserAction(ID, option);
		shutdown(sd, SHUT_RDWR);
		close(sd);
		printf("Connection closed properly.\n");
	}
	exit(0);
}

////////////////////////////////////////////////// -----LOGIN FUNCTION----- ////////////////////////////////////////////////

void login(int sd)
{
	int id;
	char password[10];
	bool authentication = 0;
	printf("Enter ID: ");
	scanf("%d", &id);

	printf("\nPassword : ");
	char *pass = getpass("");
	strcpy(password, pass);

	write(sd, &option, sizeof(int));
	write(sd, &id, sizeof(int));
	write(sd, &password, sizeof(password));
	read(sd, &authentication, sizeof(bool));
	if (authentication)
	{
		printf("\nLogged in\n");
		ID = id;
	}
	else
	{
		printf("Already Logged-in or Wrong Credentials\n");
		chooseOption(sd);
	}
}

////////////////////////////////////////////////// -----LOGIN MENU----- ////////////////////////////////////////////////

void chooseOption(int sd)
{
	printf("\n");
	printf("1 : Admin Login.\n");
	printf("2 : Manager Login.\n");
	printf("3 : Employee Login.\n");
	printf("4 : Customer Login.\n");
	printf("5 : View All accounts.\n");
	printf("6 : Exit.\n");
	printf("Choose an option : ");
	scanf("%d", &option);
	printf("\n");

	if (option > 0 && option < 5)
		login(sd);

	else if (option == 5)
	{
		write(sd, &option, sizeof(int));
		int viewChoice;
		int count;
		printf("1.See All Admin Accounts\n");
		printf("2.See All Manager Accounts\n");
		printf("3.See All Employee Accounts\n");
		printf("4.See All Customer Accounts\n");
		printf("5. Reset all logged in status\n");
		printf("Enter your choice: \n");
		scanf("%d", &viewChoice);
		write(sd, &viewChoice, sizeof(int));

		if (viewChoice == 1)
		{
			read(sd, &count, sizeof(int));
			if (count == 0)
				chooseOption(sd);
			struct admin record;
			system("clear");
			printf("Admin Accounts Are:-------------------------\n\n");
			for (int i = 0; i < count; i++)
			{
				read(sd, &record, sizeof(struct admin));
				printf("\nCustomer ID: %d\n", record.adminID);
				printf("\nName: %s\n", record.name);
				printf("\nPassword: %s\n", record.password);
				printf("\nLogin status: %d\n", record.isLoggedIn);
				printf("------------------------------------------\n");
			}
		}
		else if (viewChoice == 2)
		{
			read(sd, &count, sizeof(int));
			if (count == 0)
				chooseOption(sd);
			struct manager record;
			system("clear");
			printf("Manager Accounts Are:-------------------------\n\n");
			for (int i = 0; i < count; i++)
			{
				read(sd, &record, sizeof(struct manager));
				printf("\nManager ID: %d\n", record.managerID);
				printf("\nName: %s\n", record.name);
				printf("\nPassword: %s\n", record.password);
				printf("\nLogin status: %d\n", record.isLoggedIn);
				printf("------------------------------------------\n");
			}
		}
		else if (viewChoice == 3)
		{
			read(sd, &count, sizeof(int));
			if (count == 0)
				chooseOption(sd);
			struct employee record;
			system("clear");
			printf("Employee Accounts Are:-------------------------\n\n");
			for (int i = 0; i < count; i++)
			{
				read(sd, &record, sizeof(struct employee));
				printf("\nEmployee ID: %d\n", record.employeeID);
				printf("\nName: %s\n", record.name);
				printf("\nPassword: %s\n", record.password);
				printf("\nLogin status: %d\n", record.isLoggedIn);
				printf("\nDisabled: %d\n", record.isDisabled);
				printf("------------------------------------------\n");
			}
		}
		else if (viewChoice == 4)
		{
			read(sd, &count, sizeof(int));
			if (count == 0)
				chooseOption(sd);
			struct customer record;
			system("clear");
			printf("Customer Accounts Are:-------------------------\n\n");
			for (int i = 0; i < count; i++)
			{
				read(sd, &record, sizeof(struct customer));
				printf("\nCustomer ID: %d\n", record.customerID);
				printf("\nName: %s\n", record.name);
				printf("\nPassword: %s\n", record.password);
				printf("\nBalance: %f\n", record.balance);
				printf("\nLogin status: %d\n", record.isLoggedIn);
				printf("\nStatus: ");
				if (record.status == true)
					printf("ACTIVE!\n");
				else
					printf("IN-ACTIVE!\n");
				printf("------------------------------------------\n");
			}
		}
		else if (viewChoice == 5)
		{
			printf("Success\n");
		}
		chooseOption(sd);
	}
	else if (option == 6)
		exit(0);
	else
	{
		printf("Retry\n");
		chooseOption(sd);
	}

	return;
}

////////////////////////////////////////////////// -----USER MENU----- ////////////////////////////////////////////////

void userMenu(int sd)
{
	int input;
	bool result;

	////////////////////////////////////////////////// -----ADMIN------ ////////////////////////////////////////////////

	if (option == 1)
	{
		while (1)
		{
			printf("\n----------------------\n");
			printf("1. Add Manager Account\n");
			printf("2. Add Employee Account\n");
			printf("3. Add Customer Account\n");
			printf("4. Modify Customer/Employee Details\n");
			printf("5. View Employee Details\n");
			printf("6. View Customer Details\n");
			printf("7. Promote Employee to Manager\n");
			printf("8. Change Password\n");
			printf("9. Logout\n");
			printf("10. Exit\n");
			printf("Enter your choice: ");
			scanf("%d", &input);
			printf("\n");
			write(sd, &input, sizeof(int));
			switch (input)
			{
			case 1:
				struct manager newManager;
				int newManagerID;
				printf("Please enter the name of the Manager: ");
				scanf("%s", newManager.name);
				printf("Password(max 10 characters) : ");
				char *managerPass = getpass("");
				strcpy(newManager.password, managerPass);

				write(sd, &newManager, sizeof(struct manager));
				read(sd, &newManagerID, sizeof(int));

				printf("Your Manager ID is: %d\n", newManagerID);
				break;

			case 2:
				struct employee newEmployee;
				int newEmployeeID;
				printf("Please enter the name of the Employee: ");
				scanf("%s", newEmployee.name);
				printf("Password(max 10 characters) : ");
				char *employeePass = getpass("");
				strcpy(newEmployee.password, employeePass);

				write(sd, &newEmployee, sizeof(struct employee));
				read(sd, &newEmployeeID, sizeof(int));

				printf("Your Employee ID is: %d\n", newEmployeeID);
				break;

			case 3:
				struct customer newCustomer;
				int newCustomerID;
				printf("Please enter the name of the Customer: ");
				scanf("%s", newCustomer.name);
				printf("Password(max 10 characters) : ");
				char *customerPass = getpass("");
				strcpy(newCustomer.password, customerPass);

				write(sd, &newCustomer, sizeof(struct customer));

				read(sd, &newCustomerID, sizeof(int));

				printf("Your Customer ID is: %d\n", newCustomerID);
				break;

			case 4:
				int modifyFromAdminChoice, namePasswordChoice, idToChange;
				printf("1. Edit Employee Details.\n");
				printf("2. Edit Customer Details.\n");
				printf("Enter Your choice: ");
				scanf("%d", &modifyFromAdminChoice);

				printf("\nEnter ID: ");
				scanf("%d", &idToChange);

				printf("1.Change Name.\n2.Change Password.\n");
				if (modifyFromAdminChoice == 2)
					printf("3.Change Activation Staus.\n");
				scanf("%d", &namePasswordChoice);

				write(sd, &modifyFromAdminChoice, sizeof(int));
				write(sd, &idToChange, sizeof(int));
				write(sd, &namePasswordChoice, sizeof(int));

				if (namePasswordChoice == 1 || namePasswordChoice == 2)
				{
					char text[30];
					if (namePasswordChoice == 1)
						printf("Enter New Name: ");
					else
						printf("Enter New Password: ");
					scanf("%s", text);
					printf("\n");
					write(sd, &text, sizeof(text));
				}
				else if (namePasswordChoice == 3 && modifyFromAdminChoice == 2)
				{
					int activeStatus;
					printf("Enter Changed Status: ");
					scanf("%d", &activeStatus);
					printf("\n");
					write(sd, &activeStatus, sizeof(int));
				}
				else
				{
					printf("Wrong Choice.\n");
					break;
				}
				read(sd, &result, sizeof(bool));
				if (result)
					printf("Successfully Modified the details\n");
				else
					printf("Failed To edit the details!\n");
				break;

			case 5:
				int viewEmployeeID;

				struct employee snapshotOfEmployeeID;
				printf("Enter Employee ID: ");
				scanf("%d", &viewEmployeeID);
				printf("\n");
				write(sd, &viewEmployeeID, sizeof(int));
				read(sd, &result, sizeof(bool));
				if (!result)
				{
					printf("Employee does not exist!\n");
					break;
				}
				read(sd, &snapshotOfEmployeeID, sizeof(struct employee));
				printf("\nCustomer ID: %d\n", snapshotOfEmployeeID.employeeID);
				printf("\nName: %s\n", snapshotOfEmployeeID.name);
				printf("\nPassword: %s\n", snapshotOfEmployeeID.password);
				break;

			case 6:
				int viewCustomerID;
				struct customer snapshotOfCustomerID;
				printf("Enter Customer ID: ");
				scanf("%d", &viewCustomerID);
				printf("\n");
				write(sd, &viewCustomerID, sizeof(int));
				read(sd, &result, sizeof(bool));
				if (!result)
				{
					printf("Customer does not exist!\n");
					break;
				}
				read(sd, &snapshotOfCustomerID, sizeof(struct customer));
				printf("\nCustomer ID: %d\n", snapshotOfCustomerID.customerID);
				printf("\nName: %s\n", snapshotOfCustomerID.name);
				printf("\nPassword: %s\n", snapshotOfCustomerID.password);
				printf("\nBalance: %f\n", snapshotOfCustomerID.balance);
				printf("\nStatus: ");
				if (snapshotOfCustomerID.status == true)
					printf("ACTIVE!\n");
				else
					printf("IN-ACTIVE!\n");
				break;

			case 7:
				int empIdToPromote, resultOfPromotion;
				printf("Enter Employee ID to promote: ");
				scanf("%d", &empIdToPromote);
				printf("\n");
				write(sd, &empIdToPromote, sizeof(int));
				read(sd, &resultOfPromotion, sizeof(int));
				if (resultOfPromotion == -1)
					printf("Failed!\n");
				else
				{
					printf("Successfully Promoted!\n\n");
					printf("***New Manager ID is: %d\n\n", resultOfPromotion);
				}

				break;

			case 8:
				char changePassword[10];
				printf("\nEnter New Password : ");
				char *passAdmin = getpass("");
				strcpy(changePassword, passAdmin);
				write(sd, &changePassword, sizeof(changePassword));
				printf("Press enter to continue. \n");
				getchar();
				getchar();
				printf("Password Changed Successfully\n");
				break;

			case 9:
				logout();
				break;
			case 10:
				exit(0);
				break;
			default:
				printf("Invalid Option!\n");
				break;
			}
		}
	}

	////////////////////////////////////////////////// -----MANAGER----- ////////////////////////////////////////////////

	else if (option == 2)
	{
		while (1)
		{
			printf("\n----------------------\n");
			printf("1. Activate/Deactivate Customer accounts\n");
			printf("2. Assign Loan Applications\n");
			printf("3. Review Customer Feedback\n");
			printf("4. Change Password\n");
			printf("5. Logout\n");
			printf("6. Exit\n");
			printf("Enter your choice: ");
			scanf("%d", &input);
			printf("\n");
			write(sd, &input, sizeof(int));
			switch (input)
			{
			case 1:
				int changeState, customerIdToChangeState;
				printf("Enter Customer ID to Activate/Deactivate: ");
				scanf("%d", &customerIdToChangeState);
				printf("\nEnter New State of the account (0/1): ");
				scanf("%d", &changeState);
				write(sd, &customerIdToChangeState, sizeof(int));
				write(sd, &changeState, sizeof(int));
				read(sd, &result, sizeof(bool));
				if (!result)
				{
					printf("\nFailed to change state!\n");
					break;
				}
				printf("\nSuccessfully changes the state to: ");
				if (changeState)
					printf("ACTIVE!\n");
				else
					printf("IN-ACTIVE!\n");
				break;

			case 2:
				char buffer[256];
				int bytesRead;
				read(sd, &result, sizeof(bool));
				int fd = open("./data/tempLoan.data", O_RDONLY, 0744);
				write(sd, &fd, sizeof(int));
				if (!result) // || fd == -1)
				{
					printf("Failed to retrieve the pending Loan list!");
					close(fd);
					break;
				}
				printf("Pending Loans are:-\n");
				while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0)
				{
					write(STDOUT_FILENO, buffer, bytesRead);
				}

				close(fd);
				remove("./data/tempLoan.data");

				int assignToEmployee, assignLoanID;
				printf("Enter Loan Id to assign: ");
				scanf("%d", &assignLoanID);
				printf("\nEnter Employee ID to assign loan to: ");
				scanf("%d", &assignToEmployee);
				printf("\n");
				write(sd, &assignLoanID, sizeof(assignLoanID));
				write(sd, &assignToEmployee, sizeof(assignToEmployee));

				read(sd, &result, sizeof(bool));
				if (!result)
					printf("Failed to Assign Loan application.\n");
				else
					printf("Loan Application Successfully Assigned to %d\n", assignToEmployee);

				break;

			case 3:
				FILE *file;
				char line[256];
				file= fopen("./data/feedback.data", "r");

				// Read the file line by line
				while (fgets(line, sizeof(line), file) != NULL)
				{
					printf("%s", line); // Print each line
				}

				// Close the file
				fclose(file);
				break;

			case 4:
				char changePassword[10];
				printf("\nEnter New Password : ");
				char *passManager = getpass("");
				strcpy(changePassword, passManager);
				write(sd, &changePassword, sizeof(changePassword));
				printf("Press enter to continue. \n");
				getchar();
				getchar();
				printf("Password Changed Successfully\n");
				break;

			case 5:
				logout();
				break;
			case 6:
				exit(0);
				break;

			default:
				break;
			}
		}
	}

	////////////////////////////////////////////////// -----EMPLOYEE----- ////////////////////////////////////////////////

	else if (option == 3)
	{
		while (1)
		{
			printf("\n----------------------\n");
			printf("1. Add Customer Account\n");
			printf("2. Modify Customer Details\n");
			printf("3. View Customer Account Details\n");
			printf("4. Change Password\n");
			printf("5. Approve/Reject Loans\n");
			printf("6. Logout\n");
			printf("7. Exit\n");
			printf("Enter your choice: ");
			scanf("%d", &input);
			printf("\n");
			write(sd, &input, sizeof(int));
			switch (input)
			{
			case 1:
				struct customer newCustomer;
				int newCustomerID;
				printf("Please enter the name of the Customer: ");
				scanf("%s", newCustomer.name);
				printf("Password(max 10 characters) : ");
				char *pass = getpass("");
				strcpy(newCustomer.password, pass);

				write(sd, &newCustomer, sizeof(struct customer));

				read(sd, &newCustomerID, sizeof(int));

				printf("Your Customer ID is: %d\n", newCustomerID);
				break;

			case 2:
				int changeChoice, customerIdToChange;

				printf("Enter Customer ID: ");
				scanf("%d", &customerIdToChange);
				printf("\n");
				write(sd, &customerIdToChange, sizeof(int));
				read(sd, &result, sizeof(bool));
				if (!result)
				{
					printf("Invalid Customer ID!\n");
					break;
				}

				printf("What do you want to change?\n");
				printf("1. Name\n2. Password \n Enter Your Choice: ");
				scanf("%d", &changeChoice);
				printf("\n");

				write(sd, &changeChoice, sizeof(int));

				printf("\n");
				switch (changeChoice)
				{
				case 1:
					char changeName[30];
					printf("Enter New Name: ");
					scanf("%s", changeName);
					printf("\n");
					write(sd, &changeName, sizeof(changeName));
					break;

				case 2:
					char changePassword[10];
					printf("Enter New Password: ");
					scanf("%s", changeName);
					printf("\n");
					write(sd, &changeName, sizeof(changeName));
					printf("Changed Details!\n");
					break;
				default:
					printf("Wrong Choice!\n");
					continue;
				}
				read(sd, &result, sizeof(bool));
				if (result)
					printf("Success!\n");
				else
					printf("Failed\n");
				break;

			case 3:
				int viewCustomerID;
				struct customer snapshotOfCustomerID;
				printf("Enter Customer ID: ");
				scanf("%d", &viewCustomerID);
				printf("\n");
				write(sd, &viewCustomerID, sizeof(int));
				read(sd, &result, sizeof(bool));
				if (!result)
				{
					printf("Invalid Customer ID!\n");
					break;
				}
				read(sd, &snapshotOfCustomerID, sizeof(struct customer));
				printf("\nCustomer ID: %d\n", snapshotOfCustomerID.customerID);
				printf("\nName: %s\n", snapshotOfCustomerID.name);
				printf("\nPassword: %s\n", snapshotOfCustomerID.password);
				printf("\nBalance: %f\n", snapshotOfCustomerID.balance);
				printf("\nLogin Status %d\n", snapshotOfCustomerID.isLoggedIn);
				printf("\nStatus: ");
				if (snapshotOfCustomerID.status == true)
					printf("ACTIVE!\n");
				else
					printf("IN-ACTIVE!\n");
				break;

			case 4:
				char changePassword[10];
				printf("\nEnter New Password : ");
				char *passEmployee = getpass("");
				strcpy(changePassword, passEmployee);
				write(sd, &changePassword, sizeof(changePassword));
				printf("Press enter to continue. \n");
				getchar();
				getchar();
				printf("Password Changed Successfully\n");
				break;

			case 5:
				char buffer[256];
				int bytesRead;
				read(sd, &result, sizeof(bool));
				int fd = open("./data/tempLoan.data", O_RDONLY, 0744);
				write(sd, &fd, sizeof(int));
				if (!result || fd == -1)
				{
					printf("Failed to retrieve the pending Loan list!");
					close(fd);
					break;
				}
				printf("Pending Loans are:-\n");
				while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0)
				{
					write(STDOUT_FILENO, buffer, bytesRead);
				}

				close(fd);
				remove("./data/tempLoan.data");

				int actionLoanID, action;
				printf("Take Action!\n");
				printf("Enter Loan ID: ");
				scanf("%d", &actionLoanID);
				printf("\nEnter Choice (0:Reject, 1:Approve): ");
				scanf("%d", &action);
				write(sd, &actionLoanID, sizeof(int));
				write(sd, &action, sizeof(int));
				read(sd, &result, sizeof(bool));
				if (result)
					printf("Action Completed Successfully!\n");
				else
					printf("Failed to take action on the Loan.\n");

				break;

			case 7:
				logout();
				break;
			case 8:
				exit(0);
				break;

			default:
				break;
			}
		}
	}

	////////////////////////////////////////////////// -----CUSTOMER----- ////////////////////////////////////////////////

	else if (option == 4)
	{
		while (1)
		{
			printf("\n----------------------\n");
			printf("1. View Balance\n");
			printf("2. Deposit Money\n");
			printf("3. Withdraw Money\n");
			printf("4. Transfer Funds\n");
			printf("5. Apply For Loan\n");
			printf("6. Change Password\n");
			printf("7. Add Feedback\n");
			printf("8. Transaction History\n");
			printf("9. Logout\n");
			printf("10. Exit\n");
			printf("Enter your choice: ");
			scanf("%d", &input);
			printf("\n");

			write(sd, &input, sizeof(int));
			switch (input)
			{
			case 1:
				float currBalance;
				read(sd, &currBalance, sizeof(float));
				printf("Current Balance is: %0.2f\n", currBalance);
				break;

			case 2:
				float depositAmount;
				printf("Enter Amount to be deposited: ");
				scanf("%f", &depositAmount);
				printf("\n");
				write(sd, &depositAmount, sizeof(float));

				read(sd, &result, sizeof(bool));
				if (result == true)
					printf("Deposited\n");
				else
					printf("Failed to Deposit\n");

				break;

			case 3:
				float withdrawAmount;
				printf("Enter Amount to be withdrawn: ");
				scanf("%f", &withdrawAmount);
				printf("\n");
				write(sd, &withdrawAmount, sizeof(float));

				read(sd, &result, sizeof(bool));
				if (result == true)
					printf("Withdrawn\n");
				else
					printf("Failed to Withdraw\n");

				break;

			case 4:
				float transferAmount;
				int targetCustomerID;
				bool depositResult;
				printf("Enter Amount to be Transfer: ");
				scanf("%f", &transferAmount);
				printf("\n");
				printf("Enter Customer ID to tranfer the funds: ");
				scanf("%d", &targetCustomerID);
				printf("\n");

				write(sd, &transferAmount, sizeof(float));
				write(sd, &targetCustomerID, sizeof(int));
				read(sd, &result, sizeof(bool));
				if (!result)
				{
					printf("Either Reciever doesn't exist or In-Sufficient Funds!\n");
					break;
				}
				read(sd, &depositResult, sizeof(bool));
				if (depositResult)
					printf("Funds are Successfully Transferred!\n");
				else
					printf("Reciever's Account is not active!\n");
				break;

			case 5:
				float loanAmount;
				printf("Enter Loan Amount Required: ");
				scanf("%f", &loanAmount);
				printf("\n");
				write(sd, &loanAmount, sizeof(float));
				read(sd, &result, sizeof(bool));
				if (result)
					printf("Applied!\nPlease wait till your loan is further processed.....\n");
				else
					printf("Failed to apply for loan\n");
				break;

			case 6:
				char password[10];
				printf("\nEnter New Password : ");
				char *pass = getpass("");
				strcpy(password, pass);
				write(sd, &password, sizeof(password));
				printf("Press enter to continue. \n");
				getchar();
				getchar();
				printf("Password Changed Successfully\n");
				break;

			case 7:
				char feedbackBuffer[1024];
				printf("Enter Feedback: ");
				getchar();
				scanf("%[^\n]", feedbackBuffer);
				write(sd, feedbackBuffer, strlen(feedbackBuffer));
				printf("Success!");
				break;

			case 8:
				char buffer[256]; // temporary storage
				int bytesRead;
				read(sd, &result, sizeof(bool));
				int fd = open("./data/tempTransaction.data", O_RDONLY, 0744);
				if (!result || fd == -1)
				{
					printf("Failed to retrieve the transactions!");
					close(fd);
					break;
				}
				printf("Transactions are:-\n");
				while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0)
				{
					write(STDOUT_FILENO, buffer, bytesRead);
				}

				close(fd);
				remove("./data/tempTransaction.data");
				break;

			case 9:
				logout();
				break;
			case 10:
				exit(0);
				break;

			default:
				break;

				/*
				  case 5: applyLoan(sd);
						  break;
				  case 6: changePassword(sd);
						  break;
				  case 7: addFeedback(sd);
						  break;
				  case 9: exit(0);
				  */
			}
		}
	}
}
void logout()
{
	char *command = "./client";
	int result = system(command);
	if (result == -1)
	{
		printf("Error Logging Out!!.\n");
	}
	exit(0);
}

int main()
{
	system("clear");
	printf("----------Welcome To Motak Kahindra Bank!----------\n\n");
	struct sockaddr_in server;
	int sd, msgLength;
	char buff[50];
	char result;
	signal(SIGINT, sigintHandler);

	sd = socket(AF_INET, SOCK_STREAM, 0);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(portNumber);

	connect(sd, (struct sockaddr *)&server, sizeof(server));
	chooseOption(sd);
	userMenu(sd);
	printf("Wrong place here");
	close(sd);
	return 0;
}
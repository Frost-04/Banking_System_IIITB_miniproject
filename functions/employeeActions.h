#ifndef EMPLOYEE_ACTIONS
#define EMPLOYEE_ACTIONS

bool checkEmployee(int sd, struct employee currUser)
{
	int i = currUser.employeeID - 1;
	int fd = open("./data/employee.data", O_RDWR, 0744);
	bool result;
	struct employee temp;
	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(struct employee);
	lock.l_len = sizeof(struct employee);
	lock.l_pid = getpid();

	fl1 = fcntl(fd, F_SETLKW, &lock);

	lseek(fd, (i) * sizeof(struct employee), SEEK_SET);
	read(fd, &temp, sizeof(struct employee));
	if (!strcmp(temp.password, currUser.password) && !temp.isLoggedIn && !temp.isDisabled)
	{
        result = true;
        lseek(fd, (i) * sizeof(struct employee), SEEK_SET);
        temp.isLoggedIn=1;
        write(fd, &temp, sizeof(struct employee));
    }
	else
		result = false;

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return result;
}

int getNewCustomer()
{
    int fd = open("./data/customer.data", O_RDWR | O_CREAT, 0744);
    if (fd == -1)
    {
        printf("\nERROR: Could not open file.\n");
        return 0;
    }
    int fl1;
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock);

    // Check if the file is empty
    off_t fileSize = lseek(fd, 0, SEEK_END);
    if (fileSize == 0) {
        close(fd);
        return 1;
    }

    struct customer record;

    lseek(fd, -sizeof(struct customer), SEEK_END);
    read(fd, &record, sizeof(struct customer));     

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    return record.customerID + 1;
}

int addCustomer(struct customer newCustomer)
{
	int fd = open("./data/customer.data", O_RDWR | O_CREAT | O_APPEND, 0777);

    int fl1;
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock);

    struct customer temp, record;
	temp.customerID = getNewCustomer();
	strcpy(temp.name,newCustomer.name);
	strcpy(temp.password, newCustomer.password);
	temp.status = true;
    temp.isLoggedIn=0;
	temp.balance = 0;

	write(fd, &temp, sizeof(struct customer));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
	close(fd);

	return temp.customerID;
}

bool modifyAccount(int customerIdToChange, int changeChoice, char text[], bool changeActiveStatus)
{
    int tempFD=open("./data/customer.data", O_RDONLY);
    int numberOfCustomers=lseek(tempFD,0,SEEK_END)/sizeof(struct customer); //counts number of employees
    close(tempFD);
    if(customerIdToChange>numberOfCustomers)
        return false;
	int i = customerIdToChange - 1;
    int fd = open("./data/customer.data", O_RDWR, 0777);
    int fl1;
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(struct customer);
    lock.l_len = sizeof(struct customer);
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock);

    struct customer currUser;

    lseek(fd, (i) * sizeof(struct customer), SEEK_SET);
    read(fd, &currUser, sizeof(struct customer));

	if(changeChoice==1)
		strcpy(currUser.name, text);
	else if(changeChoice==2)
		strcpy(currUser.password, text);
	else if(changeChoice == 3)
		currUser.status=changeActiveStatus;
    lseek(fd, (-1) * sizeof(struct customer), SEEK_CUR);
    write(fd, &currUser, sizeof(struct customer));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    return true;
}

struct customer viewAccountDetails(int ID)
{
    int i = ID - 1;
    int fd = open("./data/customer.data", O_RDONLY, 0777);
    int fl1;
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(struct customer);
    lock.l_len = sizeof(struct customer);
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock);

    struct customer currUser;

    lseek(fd, (i) * sizeof(struct customer), SEEK_SET);
    read(fd, &currUser, sizeof(struct customer));
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    return currUser;
}

void changeEmployeePassword(int employeeID, char password[10])
{
    int i = employeeID - 1;
    int fd = open("./data/employee.data", O_RDWR, 0777);
    int fl1;
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(struct employee);
    lock.l_len = sizeof(struct employee);
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock);

    struct employee currUser;

    lseek(fd, (i) * sizeof(struct employee), SEEK_SET);
    read(fd, &currUser, sizeof(struct employee));
    strcpy(currUser.password, password);
    lseek(fd, (-1) * sizeof(struct employee), SEEK_CUR);
    write(fd, &currUser, sizeof(struct employee));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    return;
}

struct employee viewAllAccountsOfEmployee(int iter)
{
    struct employee record;
    int fd = open("./data/employee.data", O_RDONLY);
	lseek(fd, iter * sizeof(struct employee), SEEK_SET);
    read(fd, &record, sizeof(record));
	close(fd);
	return record;
}

#endif
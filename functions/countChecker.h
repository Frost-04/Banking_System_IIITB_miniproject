#ifndef COUNT_CHECKER
#define COUNT_CHECKER

int adminCount()
{
	int tempFD = open("./data/admin.data", O_RDONLY);
	struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start =0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int fl1 = fcntl(tempFD, F_SETLKW, &lock);
	
	int numberOfAdmin = lseek(tempFD, 0, SEEK_END) / sizeof(struct admin);

	lock.l_type = F_UNLCK;
	fcntl(tempFD, F_SETLK, &lock);
	close(tempFD);
	return numberOfAdmin;
}

int employeeCount()
{
	int tempFD = open("./data/employee.data", O_RDONLY);
	struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start =0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int fl1 = fcntl(tempFD, F_SETLKW, &lock);

	int numberOfEmployee = lseek(tempFD, 0, SEEK_END) / sizeof(struct employee);
	
	lock.l_type = F_UNLCK;
	fcntl(tempFD, F_SETLK, &lock);
	close(tempFD);
	return numberOfEmployee;
}

int customerCount()
{
	int tempFD = open("./data/customer.data", O_RDONLY);
	struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start =0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int fl1 = fcntl(tempFD, F_SETLKW, &lock);

	int numberOfCustomer = lseek(tempFD, 0, SEEK_END) / sizeof(struct customer);
	
	lock.l_type = F_UNLCK;
	fcntl(tempFD, F_SETLK, &lock);
	close(tempFD);
	return numberOfCustomer;
}

int managerCount()
{
	int tempFD = open("./data/manager.data", O_RDONLY);
	struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start =0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int fl1 = fcntl(tempFD, F_SETLKW, &lock);

	int numberOfManager = lseek(tempFD, 0, SEEK_END) / sizeof(struct manager);
	
	lock.l_type = F_UNLCK;
	fcntl(tempFD, F_SETLK, &lock);
	close(tempFD);
	return numberOfManager;
}

int feedbackCount()
{
	int tempFD = open("./data/manager.data", O_RDONLY);
	struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start =0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int fl1 = fcntl(tempFD, F_SETLKW, &lock);

	int numberOfFeedback = lseek(tempFD, 0, SEEK_END) / sizeof(1024);
	
	lock.l_type = F_UNLCK;
	fcntl(tempFD, F_SETLK, &lock);
	close(tempFD);
	return numberOfFeedback;
}
#endif
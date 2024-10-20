#ifndef CUSTOMER_ACTIONS
#define CUSTOMER_ACTIONS

bool checkCustomer(struct customer currUser)
{
    int i = currUser.customerID - 1;
    int fd = open("./data/customer.data", O_RDWR, 0744);
    bool result;
    struct customer temp;
    int fl1;
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(struct customer);
    lock.l_len = sizeof(struct customer);
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock);

    lseek(fd, (i) * sizeof(struct customer), SEEK_SET);
    read(fd, &temp, sizeof(struct customer));

    if (!strcmp(temp.password, currUser.password) && temp.status && !temp.isLoggedIn)
    {
        result = true;
        lseek(fd, (i) * sizeof(struct customer), SEEK_SET);
        temp.isLoggedIn=1;
        write(fd, &temp, sizeof(struct customer));
    }
    else
        result = false;

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result;
}

bool deposit(int customerID, float amt)
{
    int i = customerID - 1;
    int fd = open("./data/customer.data", O_RDWR, 0777);
    bool result;
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

    if (currUser.status == true && amt > 0)
    {
        currUser.balance += amt;
        lseek(fd, (-1) * sizeof(struct customer), SEEK_CUR);
        write(fd, &currUser, sizeof(struct customer));
        result = true;
    }
    else
        result = false;

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    return result;
}

bool withdraw(int customerID, float amt)
{
    int i = customerID - 1;
    int fd = open("./data/customer.data", O_RDWR, 0777);
    bool result;
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

    if (currUser.status == true && amt > 0 && amt <= currUser.balance)
    {
        currUser.balance -= amt;
        lseek(fd, (-1) * sizeof(struct customer), SEEK_CUR);
        write(fd, &currUser, sizeof(struct customer));
        result = true;
    }
    else
        result = false;

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    return result;
}

void changeCustomerPassword(int customerID, char password[10])
{
    int i = customerID - 1;
    int fd = open("./data/customer.data", O_RDWR, 0777);
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
    strcpy(currUser.password, password);
    lseek(fd, (-1) * sizeof(struct customer), SEEK_CUR);
    write(fd, &currUser, sizeof(struct customer));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    return;
}

float viewBalance(int customerID)
{
    int i = customerID - 1;
    int fd = open("./data/customer.data", O_RDONLY, 0744);

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
    return currUser.balance;
}

struct customer viewAllAccountsOfCustomer(int iter)
{
    struct customer record;
    int fd = open("./data/customer.data", O_RDONLY);
	lseek(fd, iter * sizeof(struct customer), SEEK_SET);
    read(fd, &record, sizeof(record));
	close(fd);
	return record;
}

bool addFeedback(char buffer[1024])
{
    int fd = open("./data/feedback.data", O_RDWR | O_CREAT | O_APPEND, 0744);

    int fl1;
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0 ;
    lock.l_len = 0;
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock);

    write(fd, buffer, strlen(buffer));
    write(fd, "\n----------------------------\n", sizeof"\n----------------------------\n");
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    return true;
}
#endif
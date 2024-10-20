#ifndef LOAN_ACTIONS
#define LOAN_ACTIONS

bool applyLoan(int ID, float amt)
{
    srand(time(NULL));
    bool result;

    struct loan newLoan;
    newLoan.loanee = ID;
    newLoan.loanID = (rand() % 10000) + 1;
    newLoan.assignedTo = -1;
    newLoan.loanStatus = 0;
    newLoan.amount = amt;

    int fd = open("./data/loan.data", O_RDWR | O_CREAT | O_APPEND, 0777);
    int fl1;
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    fl1 = fcntl(fd, F_SETLKW, &lock);

    if (fd == -1 || fl1 == -1)
        return false;

    write(fd, &newLoan, sizeof(struct loan));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    return true;
}

void formatLoanRecord(struct loan loanRecord, int ID)
{
    int fd = open("./data/tempLoan.data", O_RDWR | O_CREAT | O_APPEND, 0777);
    char buffer[256];
    write(fd, "Txn: ", sizeof("Txn: "));
    sprintf(buffer, "%d", loanRecord.loanID);
    write(fd, buffer, strlen(buffer));
    write(fd, "    |    ", sizeof("    |    "));

    write(fd, "Customer ID: ", sizeof("Customer ID: "));
    sprintf(buffer, "%d", loanRecord.loanee);
    write(fd, buffer, strlen(buffer));
    write(fd, "    |    ", sizeof("    |    "));
    write(fd, "Amount: ", sizeof("Amount: "));
    sprintf(buffer, "%.2f", loanRecord.amount);
    write(fd, buffer, strlen(buffer));
    write(fd, " Rupees", sizeof(" Rupees"));
    write(fd, "    |    ", sizeof("    |    "));
    write(fd, "Assigned To Employee: ", sizeof("Assigned To Employee: "));
    sprintf(buffer, "%d", loanRecord.assignedTo);
    write(fd, buffer, strlen(buffer));

    write(fd, "\n-------------------------------------------------------------------------------------------------------------\n", strlen("\n-------------------------------------------------------------------------------------------------------------\n"));
    close(fd);
    return;
}

bool viewLoan(int ID)
{
    struct loan loanRecord;
    int bytesRead;
    int fd = open("./data/loan.data", O_RDONLY, 0744);
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    if (fcntl(fd, F_SETLKW, &lock) == -1 || fd == -1)
        return false;

    while ((bytesRead = read(fd, &loanRecord, sizeof(struct loan))) > 0)
    {
        if (ID == 0 && loanRecord.loanStatus==0)
            formatLoanRecord(loanRecord, ID);
        else if (loanRecord.assignedTo == ID && loanRecord.loanStatus==0)
            formatLoanRecord(loanRecord, ID);
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    return true;
}

bool assignLoan(int assignLoanID, int assignToEmployee)
{
    struct loan record;
    bool found=false;
    int fd = open("./data/loan.data", O_RDWR, 0744);
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    if (fcntl(fd, F_SETLKW, &lock) == -1 || fd == -1)
        return false;

    while (read(fd, &record, sizeof(struct loan)) > 0)
    {
        if (record.loanID == assignLoanID)
        {
            found = true;
            break;
        }
    }
    if(!found)
        return false;
    record.assignedTo = assignToEmployee;

    lseek(fd, (-1) * sizeof(struct loan), SEEK_CUR);
    write(fd, &record, sizeof(struct loan));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    return true;
}

struct loan actionOnLoan(int loanID, int action)
{
    struct loan record;
    int loaneeID;
    bool found=false;
    int fd = open("./data/loan.data", O_RDWR, 0744);
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    if (fcntl(fd, F_SETLKW, &lock) == -1 || fd == -1)
    {   
        record.loanID=0;
        return record;
    }

    while (read(fd, &record, sizeof(struct loan)) > 0)
    {
        if (record.loanID == loanID)
        {
            found = true;
            break;
        }
    }
    if(!found)
    {   record.loanID=0;
        return record;
    }
    record.loanStatus=action+1;
    if(record.loanStatus==2)
        deposit(record.loanee,record.amount);
    else if(record.loanStatus==1)
        record.loanID=0;



    lseek(fd, (-1) * sizeof(struct loan), SEEK_CUR);
    write(fd, &record, sizeof(struct loan));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    return record;

}

#endif
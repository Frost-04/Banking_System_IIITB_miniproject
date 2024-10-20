#ifndef TRANSACTION_ACTIONS
#define TRANSACTION_ACTIONS

void addTransaction(int senderID, int recieverID, float amt, int type)
{
    struct transaction currTransaction;
    srand(time(NULL));
    currTransaction.sender = senderID;
    currTransaction.reciever = recieverID;
    currTransaction.amount = amt;
    currTransaction.type = type;
    currTransaction.transactionID = rand();

    int fd = open("./data/transaction.data", O_RDWR | O_CREAT | O_APPEND, 0777);
    int fl1;
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    fl1 = fcntl(fd, F_SETLKW, &lock);

    write(fd, &currTransaction, sizeof(struct transaction));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
}

void formatTransactionRecord(struct transaction record, int ID)
{
    int fd = open("./data/tempTransaction.data", O_RDWR | O_CREAT | O_APPEND, 0777);
    char buffer[256];
    write(fd, "Txn: \t", sizeof("Txn: \t"));
    sprintf(buffer, "%d", record.transactionID); // Convert float to string
    write(fd, buffer, strlen(buffer));
    write(fd, "  |\t", sizeof("  |\t"));

    if (record.type == 1)
    {
        write(fd, "Deposited  ", sizeof("Deposited  "));
        sprintf(buffer, "%.2f", record.amount); 
        write(fd, buffer, strlen(buffer));
        write(fd, " Rupees", sizeof(" Rupees"));
        write(fd, "\n", strlen("\n"));
    }
    else if (record.type == 2)
    {
        write(fd, "Withdrawn  ", strlen("Withdrawn  "));
        sprintf(buffer, "%.2f", record.amount); 
        write(fd, buffer, strlen(buffer));
        write(fd, " Rupees", strlen(" Rupees"));
        write(fd, "\n", strlen("\n"));
    }
    else if (record.type == 3 && record.sender==ID)
    {
        write(fd, "Transferred  ", strlen("Transferred  "));
        sprintf(buffer, "%.2f", record.amount); 
        write(fd, buffer, strlen(buffer));
        write(fd, " Rupees", strlen(" Rupees"));
        write(fd, "\tto\tCustomer ID: ", strlen("\tto\tCustomer ID: "));
        sprintf(buffer, "%d", record.reciever); 
        write(fd, buffer, strlen(buffer));
        write(fd, "\n", strlen("\n"));
    }
    else if(record.type == 3 && record.reciever==ID)
    {
        write(fd, "Recieved  ", strlen("Recieved  "));
        sprintf(buffer, "%.2f", record.amount); 
        write(fd, buffer, strlen(buffer));
        write(fd, " Rupees", strlen(" Rupees"));
        write(fd, "\tfrom\tCustomer ID: ", strlen("\tfrom\tCustomer ID: "));
        sprintf(buffer, "%d", record.sender); 
        write(fd, buffer, strlen(buffer));
        write(fd, "\n", strlen("\n"));
    }
    else if (record.type == 4)
    {
        write(fd, "Loan Granted of  ", strlen("Loan Granted of  "));
        sprintf(buffer, "%.2f", record.amount); 
        write(fd, buffer, strlen(buffer));
        write(fd, " Rupees", strlen(" Rupees"));
        write(fd, "\n", strlen("\n"));
    }
    else if (record.type == 5)
    {
        write(fd, "Loan DENIED! of  ", strlen("Loan DENIED! of  "));
        sprintf(buffer, "%.2f", record.amount); 
        write(fd, buffer, strlen(buffer));
        write(fd, " Rupees", strlen(" Rupees"));
        write(fd, "\n", strlen("\n"));
    }
    write(fd, "--------------------------------------------\n",strlen("--------------------------------------------\n"));
    close(fd);
    return;
}

bool viewTransaction(int ID)
{
    struct transaction record;
    int bytesRead;
    int fd = open("./data/transaction.data", O_RDONLY, 0744);
    if (fd == -1)
        return false;
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    if (fcntl(fd, F_SETLKW, &lock) == -1)
        return false;

    while ((bytesRead = read(fd, &record, sizeof(struct transaction))) > 0)
    {
        if(record.sender == ID || record.reciever==ID )
            formatTransactionRecord(record,ID);
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    return true;
}

#endif
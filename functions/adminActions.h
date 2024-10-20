#ifndef ADMIN_ACTIONS
#define ADMIN_ACTIONS

bool checkAdmin(int sd, struct admin currUser)
{
    int i = currUser.adminID - 1;
    int fd = open("./data/admin.data", O_RDWR, 0744);
    bool result;
    struct admin temp;

    int fl1;
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(struct admin);
    lock.l_len = sizeof(struct admin);
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock);

    lseek(fd, (i) * sizeof(struct admin), SEEK_SET);
    read(fd, &temp, sizeof(struct admin));
    if (!strcmp(temp.password, currUser.password) && !temp.isLoggedIn)
    {
        result = true;
        lseek(fd, (i) * sizeof(struct admin), SEEK_SET);
        temp.isLoggedIn = 1;
        write(fd, &temp, sizeof(struct admin));
    }
    else
        result = false;

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    return result;
}

int getNewEmployee()
{
    int fd = open("./data/employee.data", O_RDONLY| O_CREAT, 0744);
    if (fd == -1)
    {
        printf("\nERROR: Could not open file.\n");
        return 0;
    }

    // Check if the file is empty
    off_t fileSize = lseek(fd, 0, SEEK_END);
    if (fileSize == 0)
    {
        close(fd);
        return 1;
    }

    struct employee record;

    lseek(fd, -sizeof(struct employee), SEEK_END);
    read(fd, &record, sizeof(struct employee));

    close(fd);
    return record.employeeID + 1;
}

int addEmployee(struct employee newEmployee)
{

    struct employee temp;
    temp.employeeID = getNewEmployee();
    strcpy(temp.name, newEmployee.name);
    strcpy(temp.password, newEmployee.password);
    temp.isLoggedIn = 0;
    temp.isDisabled = 0;

    int fd = open("./data/employee.data", O_RDWR | O_CREAT | O_APPEND, 0744);

    write(fd, &temp, sizeof(struct employee));

    close(fd);
    return temp.employeeID;
}

int getNewManager()
{
    int fd = open("./data/manager.data", O_RDONLY | O_CREAT, 0744);
    if (fd == -1)
    {
        printf("\nERROR: Could not open file.\n");
        return 0;
    }

    // Check if the file is empty
    off_t fileSize = lseek(fd, 0, SEEK_END);
    if (fileSize == 0)
    {
        close(fd);
        return 1;
    }

    struct manager record;

    lseek(fd, -sizeof(struct manager), SEEK_END);
    read(fd, &record, sizeof(struct manager));

    close(fd);
    return record.managerID + 1;
}

int addManager(struct manager newManager)
{
    int fd = open("./data/manager.data", O_RDWR | O_CREAT | O_APPEND, 0744);

    struct manager temp, record;
    temp.managerID = getNewManager();
    strcpy(temp.name, newManager.name);
    strcpy(temp.password, newManager.password);
    temp.isLoggedIn = 0;

    int fl1;
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock);

    if (fl1 == -1 || fd == -1)
    {
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        close(fd);
        return -1;
    }

    write(fd, &temp, sizeof(struct manager));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);

    return temp.managerID;
}

bool modifyEmployeeAccount(int employeeIdToChange, int changeChoice, char text[])
{

    int i = employeeIdToChange - 1;
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

    if (changeChoice == 1)
        strcpy(currUser.name, text);
    else if (changeChoice == 2)
        strcpy(currUser.password, text);
    lseek(fd, (-1) * sizeof(struct employee), SEEK_CUR);
    write(fd, &currUser, sizeof(struct employee));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    return true;
}

struct employee viewEmployeeAccountDetails(int ID)
{
    int i = ID - 1;
    int fd = open("./data/employee.data", O_RDONLY, 0777);
    int fl1;
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(struct employee);
    lock.l_len = sizeof(struct employee);
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock);

    struct employee currUser;

    lseek(fd, (i) * sizeof(struct employee), SEEK_SET);
    read(fd, &currUser, sizeof(struct employee));
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    return currUser;
}

void changeAdminPassword(int adminID, char password[10])
{
    int i = adminID - 1;
    int fd = open("./data/admin.data", O_RDWR, 0777);
    int fl1;
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(struct admin);
    lock.l_len = sizeof(struct admin);
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock);

    struct admin currUser;

    lseek(fd, (i) * sizeof(struct admin), SEEK_SET);
    read(fd, &currUser, sizeof(struct admin));
    strcpy(currUser.password, password);
    lseek(fd, (-1) * sizeof(struct admin), SEEK_CUR);
    write(fd, &currUser, sizeof(struct admin));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    return;
}

struct admin viewAllAccountsOfAdmin(int iter)
{
    struct admin record;
    int fd = open("./data/admin.data", O_RDONLY);
    lseek(fd, iter * sizeof(struct admin), SEEK_SET);
    read(fd, &record, sizeof(record));
    close(fd);
    return record;
}

int employeeToManagerPromotion(int ID)
{
    int bytesread, result;
    int fd = open("./data/employee.data", O_RDWR, 0777);
    int fl1;
    struct employee emp;
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock);
    if (fd == -1 || fl1 == -1)
    {
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        close(fd);
        return -1;
    }
    while (bytesread = read(fd, &emp, sizeof(struct employee)) > 0)
    {
        if (emp.employeeID == ID)
            break;
    }
    if (!emp.employeeID == ID && emp.isDisabled)
    {
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        close(fd);
        return -1;
    }

    struct manager mgr;
    strcpy(mgr.name, emp.name);
    strcpy(mgr.password, emp.password);
    

    result = addManager(mgr);
    if (result==-1)
    {
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        close(fd);
        return -1;
    }
    emp.isDisabled=1;
    lseek(fd, (-1) * sizeof(struct employee), SEEK_CUR);
    write(fd, &emp, sizeof(struct employee));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    return result;
}

void resetLoginStatus()
{
    struct admin record1;
    int fd1 = open("./data/admin.data", O_RDWR);
    int bytesread;
    while(bytesread=read(fd1,&record1,sizeof(struct admin))>0)
    {
        record1.isLoggedIn=0;
        lseek(fd1,-1*sizeof(struct admin), SEEK_CUR);
        write(fd1, &record1, sizeof(struct admin));
    }
    close(fd1);
    struct manager record2;
    int fd2 = open("./data/manager.data", O_RDWR);
    while(bytesread=read(fd2,&record2,sizeof(struct manager))>0)
    {
        record2.isLoggedIn=0;
        lseek(fd2,-1*sizeof(struct manager), SEEK_CUR);
        write(fd2, &record2, sizeof(struct manager));
    }
    close(fd2);

    struct employee record3;
    int fd3 = open("./data/employee.data", O_RDWR);
    while(bytesread=read(fd3,&record3,sizeof(struct employee))>0)
    {
        record3.isLoggedIn=0;
        lseek(fd3,-1*sizeof(struct employee), SEEK_CUR);
        write(fd3, &record3, sizeof(struct employee));
    }
    close(fd3);

    struct customer record4;
    int fd4 = open("./data/customer.data", O_RDWR);
    while(bytesread=read(fd4,&record4,sizeof(struct customer))>0)
    {
        record4.isLoggedIn=0;
        lseek(fd4,-1*sizeof(struct customer), SEEK_CUR);
        write(fd4, &record4, sizeof(struct customer));
    }
    close(fd4);
}

#endif
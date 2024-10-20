#ifndef EXIT_ACTION
#define EXIT_ACTION

void exitUserAction(int ID, int type)
{
    if (type == 1)
    {
        int fd = open("./data/admin.data", O_RDWR);
        struct admin record;
        lseek(fd, (ID - 1) * sizeof(struct admin), SEEK_SET);
        read(fd, &record, sizeof(struct admin));
        record.isLoggedIn = 0;
        lseek(fd, (ID - 1) * sizeof(struct admin), SEEK_SET);
        write(fd, &record, sizeof(struct admin));
        close(fd);
    }
    else if (type == 2)
    {
        int fd = open("./data/manager.data", O_RDWR);
        struct manager record;
        lseek(fd, (ID - 1) * sizeof(struct manager), SEEK_SET);
        read(fd, &record, sizeof(struct manager));
        record.isLoggedIn = 0;
        lseek(fd, (ID - 1) * sizeof(struct manager), SEEK_SET);
        write(fd, &record, sizeof(struct manager));
        close(fd);
    }
    else if (type == 3)
    {
        int fd = open("./data/employee.data", O_RDWR);
        struct employee record;
        lseek(fd, (ID - 1) * sizeof(struct employee), SEEK_SET);
        read(fd, &record, sizeof(struct employee));
        record.isLoggedIn = 0;
        lseek(fd, (ID - 1) * sizeof(struct employee), SEEK_SET);
        write(fd, &record, sizeof(struct employee));
        close(fd);
    }
    else if (type == 4)
    {
        int fd = open("./data/customer.data", O_RDWR);
        struct customer record;
        lseek(fd, (ID - 1) * sizeof(struct customer), SEEK_SET);
        read(fd, &record, sizeof(struct customer));
        record.isLoggedIn = 0;
        lseek(fd, (ID - 1) * sizeof(struct customer), SEEK_SET);
        write(fd, &record, sizeof(struct customer));
        close(fd);
    }
}
#endif
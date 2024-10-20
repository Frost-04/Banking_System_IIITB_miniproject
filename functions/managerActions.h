#ifndef MANAGER_ACTIONS
#define MANAGER_ACTIONS


bool checkManager(int sd, struct manager currUser)
{
	int i = currUser.managerID - 1;
	int fd = open("./data/manager.data", O_RDWR, 0744);
	bool result;
	struct manager temp;

	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(struct manager);
	lock.l_len = sizeof(struct manager);
	lock.l_pid = getpid();

	fl1 = fcntl(fd, F_SETLKW, &lock);

	lseek(fd, (i) * sizeof(struct manager), SEEK_SET);
	read(fd, &temp, sizeof(struct manager));
	if (!strcmp(temp.password, currUser.password) && !temp.isLoggedIn)
	{
		result = true;
		lseek(fd, (i) * sizeof(struct manager), SEEK_SET);
        temp.isLoggedIn=1;
        write(fd, &temp, sizeof(struct manager));
	}
	else
		result = false;

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return result;
}

void changeManagerPassword(int managerID, char password[10])
{
    int i = managerID - 1;
    int fd = open("./data/manager.data", O_RDWR, 0777);
    int fl1;
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(struct manager);
    lock.l_len = sizeof(struct manager);
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock);

    struct manager currUser;

    lseek(fd, (i) * sizeof(struct manager), SEEK_SET);
    read(fd, &currUser, sizeof(struct manager));
    strcpy(currUser.password, password);
    lseek(fd, (-1) * sizeof(struct manager), SEEK_CUR);
    write(fd, &currUser, sizeof(struct manager));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    return;
}

struct manager viewAllAccountsOfManager(int iter)
{
    struct manager record;
    int fd = open("./data/manager.data", O_RDONLY);
	lseek(fd, iter * sizeof(struct manager), SEEK_SET);
    read(fd, &record, sizeof(record));
	close(fd);
	return record;
}



#endif
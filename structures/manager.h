#ifndef MANAGER_STRUCT
#define MANAGER_STRUCT

struct manager{
        int managerID;
        char name[30];
        char password[10];
        bool isLoggedIn;
};

#endif
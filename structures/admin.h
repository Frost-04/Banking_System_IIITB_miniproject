#ifndef ADMIN_STRUCT
#define ADMIN_STRUCT

struct admin
{
        int adminID;
        char name[30];
        char password[10];
        bool isLoggedIn;
};

#endif
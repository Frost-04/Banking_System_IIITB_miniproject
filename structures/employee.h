#ifndef EMPLOYEE_STRUCT
#define EMPLOYEE_STRUCT

struct employee{
        int employeeID;
        char name[30];
        char password[10];
        bool isLoggedIn;
        bool isDisabled;
};

#endif
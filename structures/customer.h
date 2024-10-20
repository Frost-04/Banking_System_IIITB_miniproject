#ifndef CUSTOMER_STRUCT
#define CUSTOMER_STRUCT

struct customer{
        int customerID;
        char name[30];
        char password[10];
        int account_no;
        float balance;
        bool status;
        bool isLoggedIn;
};

#endif
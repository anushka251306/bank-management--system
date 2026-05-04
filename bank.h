#ifndef BANK_H
#define BANK_H

/* -----------------------------------------------------------------------
 * bank.h  –  Structures and function prototypes for the bank management
 *             system.  All persistent data is stored in "accounts.dat"
 *             using binary file I/O.
 * ----------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_FILE     "accounts.dat"
#define NAME_LEN      50
#define ADDR_LEN      100
#define PHONE_LEN     15

/* ------------------------------------------------------------------
 * Account structure
 * ------------------------------------------------------------------ */
typedef struct {
    int    account_no;
    char   name[NAME_LEN];
    char   address[ADDR_LEN];
    char   phone[PHONE_LEN];
    double balance;
} Account;

/* ------------------------------------------------------------------
 * Transaction log structure  (appended to "transactions.log")
 * ------------------------------------------------------------------ */
typedef struct {
    int    account_no;
    char   type[10];   /* "DEPOSIT", "WITHDRAW", "CREATE", "DELETE" */
    double amount;
    double balance_after;
} Transaction;

/* ------------------------------------------------------------------
 * Function prototypes
 * ------------------------------------------------------------------ */
void   create_account(void);
void   deposit(void);
void   withdraw(void);
void   check_balance(void);
void   display_account(void);
void   display_all_accounts(void);
void   delete_account(void);
void   modify_account(void);
void   show_menu(void);

/* helpers */
int    account_exists(int account_no, Account *out);
void   write_transaction(int account_no, const char *type,
                         double amount, double balance_after);

#endif /* BANK_H */

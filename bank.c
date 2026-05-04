/* =========================================================================
 * bank.c  –  Bank Management System
 *
 * Demonstrates:
 *   • Structures  (Account, Transaction)
 *   • File handling  (binary read/write with fread/fwrite, text append for log)
 *
 * Compile:  gcc -Wall -o bank bank.c
 * Run:      ./bank
 * ========================================================================= */

#include "bank.h"

/* -------------------------------------------------------------------------
 * Helper: check whether an account with the given number exists.
 * If 'out' is not NULL and the account is found, it is copied there.
 * Returns 1 if found, 0 otherwise.
 * ------------------------------------------------------------------------- */
int account_exists(int account_no, Account *out)
{
    FILE    *fp;
    Account  acc;

    fp = fopen(DATA_FILE, "rb");
    if (fp == NULL)
        return 0;

    while (fread(&acc, sizeof(Account), 1, fp) == 1) {
        if (acc.account_no == account_no) {
            if (out != NULL)
                *out = acc;
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

/* -------------------------------------------------------------------------
 * Helper: append one transaction record to the log file.
 * ------------------------------------------------------------------------- */
void write_transaction(int account_no, const char *type,
                       double amount, double balance_after)
{
    FILE        *fp;
    Transaction  tx;

    tx.account_no    = account_no;
    tx.amount        = amount;
    tx.balance_after = balance_after;
    strncpy(tx.type, type, sizeof(tx.type) - 1);
    tx.type[sizeof(tx.type) - 1] = '\0';

    fp = fopen("transactions.log", "ab");
    if (fp == NULL)
        return;
    fwrite(&tx, sizeof(Transaction), 1, fp);
    fclose(fp);
}

/* -------------------------------------------------------------------------
 * Create a new account
 * ------------------------------------------------------------------------- */
void create_account(void)
{
    FILE    *fp;
    Account  acc;
    int      acc_no;

    printf("\n--- Create New Account ---\n");
    printf("Enter account number: ");
    if (scanf("%d", &acc_no) != 1) { while (getchar() != '\n'); return; }

    if (account_exists(acc_no, NULL)) {
        printf("Account %d already exists.\n", acc_no);
        return;
    }

    acc.account_no = acc_no;

    /* Flush newline left by scanf */
    while (getchar() != '\n');

    printf("Enter name         : ");
    if (fgets(acc.name, NAME_LEN, stdin) == NULL) return;
    acc.name[strcspn(acc.name, "\n")] = '\0';

    printf("Enter address      : ");
    if (fgets(acc.address, ADDR_LEN, stdin) == NULL) return;
    acc.address[strcspn(acc.address, "\n")] = '\0';

    printf("Enter phone        : ");
    if (fgets(acc.phone, PHONE_LEN, stdin) == NULL) return;
    acc.phone[strcspn(acc.phone, "\n")] = '\0';

    printf("Enter initial deposit: ");
    if (scanf("%lf", &acc.balance) != 1) { while (getchar() != '\n'); return; }

    fp = fopen(DATA_FILE, "ab");
    if (fp == NULL) {
        perror("Cannot open data file");
        return;
    }
    fwrite(&acc, sizeof(Account), 1, fp);
    fclose(fp);

    write_transaction(acc.account_no, "CREATE", acc.balance, acc.balance);
    printf("Account %d created successfully.\n", acc.account_no);
}

/* -------------------------------------------------------------------------
 * Deposit money into an account
 * ------------------------------------------------------------------------- */
void deposit(void)
{
    FILE    *fp, *tmp;
    Account  acc;
    int      acc_no, found = 0;
    double   amount;

    printf("\n--- Deposit ---\n");
    printf("Enter account number: ");
    if (scanf("%d", &acc_no) != 1) { while (getchar() != '\n'); return; }

    printf("Enter deposit amount: ");
    if (scanf("%lf", &amount) != 1) { while (getchar() != '\n'); return; }
    if (amount <= 0) { printf("Amount must be positive.\n"); return; }

    fp  = fopen(DATA_FILE, "rb");
    tmp = fopen("accounts.tmp", "wb");
    if (fp == NULL || tmp == NULL) {
        perror("File error");
        if (fp)  fclose(fp);
        if (tmp) fclose(tmp);
        return;
    }

    while (fread(&acc, sizeof(Account), 1, fp) == 1) {
        if (acc.account_no == acc_no) {
            acc.balance += amount;
            found = 1;
            write_transaction(acc_no, "DEPOSIT", amount, acc.balance);
        }
        fwrite(&acc, sizeof(Account), 1, tmp);
    }
    fclose(fp);
    fclose(tmp);

    if (!found) {
        printf("Account %d not found.\n", acc_no);
        remove("accounts.tmp");
        return;
    }

    remove(DATA_FILE);
    rename("accounts.tmp", DATA_FILE);
    printf("Deposited %.2f. New balance: see account details.\n", amount);
}

/* -------------------------------------------------------------------------
 * Withdraw money from an account
 * ------------------------------------------------------------------------- */
void withdraw(void)
{
    FILE    *fp, *tmp;
    Account  acc;
    int      acc_no, found = 0;
    double   amount;

    printf("\n--- Withdraw ---\n");
    printf("Enter account number: ");
    if (scanf("%d", &acc_no) != 1) { while (getchar() != '\n'); return; }

    printf("Enter withdrawal amount: ");
    if (scanf("%lf", &amount) != 1) { while (getchar() != '\n'); return; }
    if (amount <= 0) { printf("Amount must be positive.\n"); return; }

    fp  = fopen(DATA_FILE, "rb");
    tmp = fopen("accounts.tmp", "wb");
    if (fp == NULL || tmp == NULL) {
        perror("File error");
        if (fp)  fclose(fp);
        if (tmp) fclose(tmp);
        return;
    }

    while (fread(&acc, sizeof(Account), 1, fp) == 1) {
        if (acc.account_no == acc_no) {
            if (acc.balance < amount) {
                printf("Insufficient balance (%.2f available).\n", acc.balance);
                fclose(fp);
                fclose(tmp);
                remove("accounts.tmp");
                return;
            }
            acc.balance -= amount;
            found = 1;
            write_transaction(acc_no, "WITHDRAW", amount, acc.balance);
        }
        fwrite(&acc, sizeof(Account), 1, tmp);
    }
    fclose(fp);
    fclose(tmp);

    if (!found) {
        printf("Account %d not found.\n", acc_no);
        remove("accounts.tmp");
        return;
    }

    remove(DATA_FILE);
    rename("accounts.tmp", DATA_FILE);
    printf("Withdrew %.2f successfully.\n", amount);
}

/* -------------------------------------------------------------------------
 * Check balance for an account
 * ------------------------------------------------------------------------- */
void check_balance(void)
{
    Account acc;
    int     acc_no;

    printf("\n--- Check Balance ---\n");
    printf("Enter account number: ");
    if (scanf("%d", &acc_no) != 1) { while (getchar() != '\n'); return; }

    if (!account_exists(acc_no, &acc)) {
        printf("Account %d not found.\n", acc_no);
        return;
    }
    printf("Account: %d | Name: %s | Balance: %.2f\n",
           acc.account_no, acc.name, acc.balance);
}

/* -------------------------------------------------------------------------
 * Display full details for one account
 * ------------------------------------------------------------------------- */
void display_account(void)
{
    Account acc;
    int     acc_no;

    printf("\n--- Account Details ---\n");
    printf("Enter account number: ");
    if (scanf("%d", &acc_no) != 1) { while (getchar() != '\n'); return; }

    if (!account_exists(acc_no, &acc)) {
        printf("Account %d not found.\n", acc_no);
        return;
    }

    printf("\n  Account No : %d\n",  acc.account_no);
    printf("  Name       : %s\n",   acc.name);
    printf("  Address    : %s\n",   acc.address);
    printf("  Phone      : %s\n",   acc.phone);
    printf("  Balance    : %.2f\n", acc.balance);
}

/* -------------------------------------------------------------------------
 * Display all accounts
 * ------------------------------------------------------------------------- */
void display_all_accounts(void)
{
    FILE    *fp;
    Account  acc;
    int      count = 0;

    printf("\n--- All Accounts ---\n");
    fp = fopen(DATA_FILE, "rb");
    if (fp == NULL) {
        printf("No accounts found.\n");
        return;
    }

    printf("%-12s %-20s %-15s %12s\n",
           "Account No", "Name", "Phone", "Balance");
    printf("------------------------------------------------------------\n");

    while (fread(&acc, sizeof(Account), 1, fp) == 1) {
        printf("%-12d %-20s %-15s %12.2f\n",
               acc.account_no, acc.name, acc.phone, acc.balance);
        count++;
    }
    fclose(fp);

    if (count == 0)
        printf("No accounts found.\n");
    else
        printf("------------------------------------------------------------\n");
    printf("Total accounts: %d\n", count);
}

/* -------------------------------------------------------------------------
 * Delete an account
 * ------------------------------------------------------------------------- */
void delete_account(void)
{
    FILE    *fp, *tmp;
    Account  acc;
    int      acc_no, found = 0;

    printf("\n--- Delete Account ---\n");
    printf("Enter account number to delete: ");
    if (scanf("%d", &acc_no) != 1) { while (getchar() != '\n'); return; }

    fp  = fopen(DATA_FILE, "rb");
    tmp = fopen("accounts.tmp", "wb");
    if (fp == NULL || tmp == NULL) {
        perror("File error");
        if (fp)  fclose(fp);
        if (tmp) fclose(tmp);
        return;
    }

    while (fread(&acc, sizeof(Account), 1, fp) == 1) {
        if (acc.account_no == acc_no) {
            found = 1;
            write_transaction(acc_no, "DELETE", acc.balance, 0.0);
            /* Do not copy this record to tmp – it is being deleted */
        } else {
            fwrite(&acc, sizeof(Account), 1, tmp);
        }
    }
    fclose(fp);
    fclose(tmp);

    if (!found) {
        printf("Account %d not found.\n", acc_no);
        remove("accounts.tmp");
        return;
    }

    remove(DATA_FILE);
    rename("accounts.tmp", DATA_FILE);
    printf("Account %d deleted successfully.\n", acc_no);
}

/* -------------------------------------------------------------------------
 * Modify an account's name, address, or phone
 * ------------------------------------------------------------------------- */
void modify_account(void)
{
    FILE    *fp, *tmp;
    Account  acc;
    int      acc_no, found = 0;

    printf("\n--- Modify Account ---\n");
    printf("Enter account number to modify: ");
    if (scanf("%d", &acc_no) != 1) { while (getchar() != '\n'); return; }

    fp  = fopen(DATA_FILE, "rb");
    tmp = fopen("accounts.tmp", "wb");
    if (fp == NULL || tmp == NULL) {
        perror("File error");
        if (fp)  fclose(fp);
        if (tmp) fclose(tmp);
        return;
    }

    while (fread(&acc, sizeof(Account), 1, fp) == 1) {
        if (acc.account_no == acc_no) {
            char buf[NAME_LEN];
            char abuf[ADDR_LEN];
            char pbuf[PHONE_LEN];

            found = 1;
            while (getchar() != '\n');  /* flush */

            printf("Current name: %s\nNew name (Enter to keep): ", acc.name);
            if (fgets(buf, NAME_LEN, stdin) != NULL) {
                buf[strcspn(buf, "\n")] = '\0';
                if (buf[0] != '\0') {
                    strncpy(acc.name, buf, NAME_LEN - 1);
                    acc.name[NAME_LEN - 1] = '\0';
                }
            }

            printf("Current address: %s\nNew address (Enter to keep): ",
                   acc.address);
            if (fgets(abuf, ADDR_LEN, stdin) != NULL) {
                abuf[strcspn(abuf, "\n")] = '\0';
                if (abuf[0] != '\0') {
                    strncpy(acc.address, abuf, ADDR_LEN - 1);
                    acc.address[ADDR_LEN - 1] = '\0';
                }
            }

            printf("Current phone: %s\nNew phone (Enter to keep): ",
                   acc.phone);
            if (fgets(pbuf, PHONE_LEN, stdin) != NULL) {
                pbuf[strcspn(pbuf, "\n")] = '\0';
                if (pbuf[0] != '\0') {
                    strncpy(acc.phone, pbuf, PHONE_LEN - 1);
                    acc.phone[PHONE_LEN - 1] = '\0';
                }
            }
        }
        fwrite(&acc, sizeof(Account), 1, tmp);
    }
    fclose(fp);
    fclose(tmp);

    if (!found) {
        printf("Account %d not found.\n", acc_no);
        remove("accounts.tmp");
        return;
    }

    remove(DATA_FILE);
    rename("accounts.tmp", DATA_FILE);
    printf("Account %d updated successfully.\n", acc_no);
}

/* -------------------------------------------------------------------------
 * Print the menu
 * ------------------------------------------------------------------------- */
void show_menu(void)
{
    printf("\n========================================\n");
    printf("       BANK MANAGEMENT SYSTEM           \n");
    printf("========================================\n");
    printf("  1. Create Account\n");
    printf("  2. Deposit\n");
    printf("  3. Withdraw\n");
    printf("  4. Check Balance\n");
    printf("  5. Display Account Details\n");
    printf("  6. Display All Accounts\n");
    printf("  7. Modify Account\n");
    printf("  8. Delete Account\n");
    printf("  0. Exit\n");
    printf("========================================\n");
    printf("Enter choice: ");
}

/* -------------------------------------------------------------------------
 * main
 * ------------------------------------------------------------------------- */
int main(void)
{
    int choice;

    while (1) {
        show_menu();
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1: create_account();    break;
            case 2: deposit();           break;
            case 3: withdraw();          break;
            case 4: check_balance();     break;
            case 5: display_account();   break;
            case 6: display_all_accounts(); break;
            case 7: modify_account();    break;
            case 8: delete_account();    break;
            case 0:
                printf("Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

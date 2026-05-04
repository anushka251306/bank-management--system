# Bank Management System

A console-based **Bank Management System** written in C that practically demonstrates **structures** and **file handling**.

## Features

| Operation | Description |
|-----------|-------------|
| Create Account | Add a new account (account number, name, address, phone, initial balance) |
| Deposit | Add money to an existing account |
| Withdraw | Deduct money (with insufficient-funds check) |
| Check Balance | View the current balance of an account |
| Display Account | Print all details for a single account |
| Display All Accounts | List every account in the system |
| Modify Account | Update name, address, or phone number |
| Delete Account | Remove an account permanently |

## Key Concepts

### Structures
Two `struct` types are defined in `bank.h`:

```c
typedef struct {
    int    account_no;
    char   name[50];
    char   address[100];
    char   phone[15];
    double balance;
} Account;

typedef struct {
    int    account_no;
    char   type[10];    /* "CREATE", "DEPOSIT", "WITHDRAW", "DELETE" */
    double amount;
    double balance_after;
} Transaction;
```

### File Handling
* **`accounts.dat`** – binary file; each `Account` record is written with `fwrite` and read back with `fread`.
* **`transactions.log`** – binary append-only log; every operation records a `Transaction` entry.
* Updates are done safely using a temporary file (`accounts.tmp`) that replaces the original on success.

## Build & Run

```bash
# Build
make

# Run
./bank

# Clean build artifacts and data files
make clean
```

> **Requirements:** GCC (or any C99-compatible compiler) and `make`.

## Files

```
bank.h          – struct definitions and function prototypes
bank.c          – full implementation + main()
Makefile        – build rules
accounts.dat    – created at runtime; stores account records (binary)
transactions.log– created at runtime; stores transaction history (binary)
```

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
char account{
    char name[50];
    float balance;
    int accno;
};
void showMenu(){
    printf("------Bank Management System------\n");
    printf("1.Create Account\n");
    printf("2.Deposit Money\n");
    printf("3.Withdraw Money\n");
    printf("4.Check Balance\n");
    printf("5.Exit");
    printf("Enter choice:\n");
}
int main(){
    printf("\nBank System Initialized\n");
}

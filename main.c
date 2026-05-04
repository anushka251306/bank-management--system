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
void createAccount(){
    FILE *fp;
    fp=fopen("account.dat","ab");
    struct account a;
    if(fp==NULL){
        printf("file cannot be opened\n");
        return 1;
    }
    printf("Enter the account number\n");
    scanf("%d", & a.accno);
    printf("Enter the name\n");
    scanf("%s",a.name);
    balance=0.0;//new accout opens with zero balance
    fwrite(& account,sizeof(struct account),1,fp);
    printf("Account Created Successfully!!\n");
}
int main(){
    while (1){
  showMenu();
  scanf("%d",& choice);
  switch(choice){
if(choice==1){
    createAccount();
}
else if(choice==2){
    break;
}
else{
    printf("\n invalid choice!\n");
}
default:
                printf("Invalid choice! Please enter 1 or 2.\n");
    }
}
return 0;
}

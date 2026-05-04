#include<stdio.h>
#include<string.h>
#include<stdlib.h>
struct  account{
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
    struct account a;
    fp=fopen("account.dat","ab");
    if(fp==NULL){
        printf("file cannot be opened\n");
    }
    printf("Enter the account number\n");
    scanf("%d", & a.accno);
    printf("Enter the name\n");
    scanf("%s",a.name);
    a.balance=0.0;//new accout opens with zero balance
    fwrite(& a,sizeof(a),1,fp);
    printf("Account Created Successfully!!\n");
    fclose(fp);
}
void checkBalance(){
    FILE *fp;
    struct account a;
fp=fopen("account.dat","rb");
int acc_search,found=0;
printf("Enter the account number you want to search\n");
scanf("%d",&acc_search);
while(fread(&a,sizeof(a),1,fp)!=0){
    if(a.accno==acc_search){
        found=1;
        printf("\n------account found-----\n");
        printf("name:  %s\n",a.name);
        printf("account no: %d",a.accno);
        printf("balance : %.2f\n",a.balance);
        break;
    }
}
if(found==0){
    printf("\n Error: The %d account no does not exist\n",acc_search);
}
fclose(fp);
}
void WithdrawMoney(){
    struct account a;
    FILE *fp;
    fp=fopen("account.dat","rb+");
    if(fp==NULL){
        printf("\n No account found\n");
    }
    int acc_search,found=0;
    float amount;
    printf("Enter the account number\n");
    scanf("%d",& acc_search);
    while(fread(&a,sizeof(a),1,fp)!=0){
        if(a.accno==acc_search){
            found=1;
            printf("Current balance:%.2f\n",a.balance);
printf("Enter the amount you want to withdraw\n");
scanf("%f",& amount);
if(amount>a.balance){
    printf("Error: Insufficient Balance Transaction cancelled\n");
}
else{
    a.balance-=amount;
    fseek(fp,-sizeof(a),SEEK_CUR);
    fwrite(&a,sizeof(a),1,fp);
    printf("withdrawl Successful!! New balance:%.2f\n",a.balance);
}
break;
        }
    }
    fclose(fp);
}
void depositMoney(){
    struct account a;
    float amount;
    int found=0,acc_search;
    FILE *fp;
    fp=fopen("account.dat","rb+");
    if (fp==NULL){
        printf("\n No account found. Please create a new account first.\n");
    }
    printf("Enter the account number for deposit\n");
    scanf("%d",& acc_search);
    while(fread(&a,sizeof(a),1,fp)){
        if(acc_search==a.accno){
            found=1;
            printf("Current balance is %.2f\n",a.balance);
            printf("Enter the amount you want to deposit\n");
            scanf("%f",& amount);
            a.balance+=amount;
            fseek(fp, -sizeof(a), SEEK_CUR);
            fwrite(&a, sizeof(a), 1, fp);
            printf("Deposit Successful! New balance: Rs. %.2f\n", a.balance);
            break;
        }
    }
    if (found==0) {
        printf("Error: Account number %d not found.\n", acc_search);
    }

    fclose(fp);
}
int main(){
    struct account a;
    int choice;
    while (1){
  showMenu();
  scanf("%d",& choice);
  switch(choice){
case 1:
    createAccount();
break;
case 2:
depositMoney();
    break;
case 3:
WithdrawMoney();
break;
case 4:
checkBalance();
break;
case 5:
exit(0);
default:
                printf("Invalid choice! Please enter 1 or 2.\n");
    }}
return 0;
}

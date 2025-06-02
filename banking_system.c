#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct account {
    int acc_num;
    char name[50];
    char password[20];
    float balance;
};

int getacc_num() {
    FILE *fp = fopen("accounts.dat", "rb");
    struct account acc;
    int lastacc_num = 1000;
    if (fp == NULL)
        return lastacc_num + 1;
    while (fread(&acc, sizeof(acc), 1, fp)) {
        lastacc_num = acc.acc_num;
    }
    fclose(fp);
    return lastacc_num + 1;
}

void createaccount() {
    struct account acc;
    FILE *fp = fopen("accounts.dat", "ab");
    if (fp == NULL) {
        printf("Error in opening file\n");
        return;
    }
    acc.acc_num = getacc_num();
    printf("\n=========================== CREATE YOUR ACCOUNT ================================\n");
    printf("Enter your name            : ");
    scanf(" %[^\n]", acc.name);
    printf("Enter your new password    : ");
    scanf(" %[^\n]", acc.password);
    printf("Enter your initial deposit : ");
    scanf("%f", &acc.balance);
    fwrite(&acc, sizeof(acc), 1, fp);
    fclose(fp);
    printf("\n===================== ACCOUNT CREATED SUCCESSFULLY ============================\n");
    printf("YOUR ACCOUNT NUMBER IS : %d\n", acc.acc_num);
}

void login() {
    FILE *fp = fopen("accounts.dat", "rb+");
    struct account acc;
    int acc_num, found = 0, choice;
    char pass[20];
    float amount;

    if (fp == NULL) {
        printf("\n============================ LOGIN FAILED ============================\n");
        printf("No accounts exist yet.\n");
        return;
    }

    printf("\n=============================== LOGIN =========================================\n");
    printf("Enter your account number : ");
    scanf("%d", &acc_num);
    getchar();
    printf("Enter your password       : ");
    scanf(" %[^\n]", pass);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc_num == acc.acc_num && strcmp(pass, acc.password) == 0) {
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("\n========================= INVALID CREDENTIALS =========================\n");
        fclose(fp);
        return;
    }

    printf("\n============================ LOGIN SUCCESSFUL ================================\n");
    printf("WELCOME %s\n", acc.name);

    long pos = ftell(fp) - sizeof(acc);

    while (1) {
        printf("\n============================== ACCOUNT MENU ===================================\n");
        printf("1. Check Balance\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Logout\n");
        printf("===============================================================================\n");
        printf("Enter your choice : ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Your current bank balance is ₹%.2f\n", acc.balance);
                break;
            case 2:
                printf("Enter the amount you want to deposit: ");
                scanf("%f", &amount);
                if (amount > 0) {
                    acc.balance += amount;
                    fseek(fp, pos, SEEK_SET);
                    fwrite(&acc, sizeof(acc), 1, fp);
                    fflush(fp);
                    printf("\n============================= DEPOSIT SUCCESSFUL ==============================\n");
                    printf("Your New Balance is ₹%.2f\n", acc.balance);
                } else {
                    printf("Invalid Amount\n");
                }
                break;
            case 3:
                printf("Enter the amount you want to withdraw: ");
                scanf("%f", &amount);
                if (amount > 0 && acc.balance >= amount) {
                    acc.balance -= amount;
                    fseek(fp, pos, SEEK_SET);
                    fwrite(&acc, sizeof(acc), 1, fp);
                    fflush(fp);
                    printf("\n=========================== WITHDRAWAL SUCCESSFUL =============================\n");
                    printf("Your New Balance is ₹%.2f\n", acc.balance);
                } else {
                    printf("Invalid Amount or Insufficient Balance\n");
                }
                break;
            case 4:
                printf("\n============================== LOGGING OUT ====================================\n");
                fclose(fp);
                return;
            default:
                printf("INVALID CHOICE! TRY AGAIN\n");
        }
    }
}

int main() {
    int choice;
    while (1) {
        printf("\n========================== WELCOME TO BANKING SYSTEM ==========================\n");
        printf("1. Create a new account\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("===============================================================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                createaccount();
                break;
            case 2:
                login();
                break;
            case 3:
                printf("\n============================ THANK YOU FOR USING ==============================\n");
                exit(0);
            default:
                printf("Invalid choice\n");
        }
    }
    return 0;
}
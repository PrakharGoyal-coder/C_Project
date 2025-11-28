#include <stdio.h>
#include<stdlib.h>
#include<string.h>

struct account 
{
    char name[50];
    int pin;
    float balance;
    char bankName[50];
    char branch[50];
    char type[20];
};

void saveToFile(struct account acc);
struct account loadFromFile();

void checkBalance(struct account acc);
struct account depositMoney(struct account acc);
struct account withdrawMoney(struct account acc);
struct account changePIN(struct account acc);
void addHistory(char *entry);
void showHistory();
struct account changeName(struct account acc);
void resetAccount();


int main()
{
    struct account user; 
    user=loadFromFile();   // to load saved account (or create new)
   
    int enteredPin,choice;
    int attempts=0;

    printf("\n- Welcome to State Bank ATM -\n");

    while (1) 
    {
        printf("Enter your 4 digit pin: ");
        scanf("%d",&enteredPin);

        if (enteredPin==user.pin) 
        {
            printf("Welcome, %s\n",user.name);
            printf("Login successful.\n");
            printf("Bank:%s\n",user.bankName);
            printf("Branch:%s\n",user.branch);
            printf("Account Type:%s\n",user.type);

            // low balance warning
            if (user.balance < 1000)
                printf("\n WARNING:Your balance is very low.Please deposit soon.\n");
            break;
        } 
        else 
        {
            attempts++;
            printf("Wrong pin entered...Attempts left: %d\n",3-attempts);
        }
        if (attempts==3) {
            printf("You tried wrong pin as too many times.\n");
            printf("Account locked for safety.\n");
            return 0;
        }
    }

    // Menu
    do {
        printf("\n-- MENU --\n");
        printf("1.Check Balance\n");
        printf("2.Deposit Money\n");
        printf("3.Withdraw Money\n");
        printf("4.Change PIN\n");
        printf("5.Exit\n");
        printf("6.Transaction History\n");
        printf("7.Change Account Holder Name\n");
        printf("8.Reset Account\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                checkBalance(user);
                break;

            case 2:
                user = depositMoney(user);
                saveToFile(user);
                break;

            case 3:
                user = withdrawMoney(user);
                saveToFile(user);
                break;

            case 4:
                user = changePIN(user);
                saveToFile(user);
                break;

            case 5:
                printf("\nThank you for using the ATM.\n");
                break;
            case 6:
                    showHistory();
                     break;
            case 7:
                    user=changeName(user);
                    saveToFile(user);
                    break; 

            case 8:  resetAccount();
                    break;


            default:
                printf("Invalid option. Try again.\n");
        }

    } while (choice != 5);

    return 0;
}


// Save account to file
void saveToFile(struct account acc)
{
    FILE *fp = fopen("account.txt", "w");
    if (fp == NULL) {
        printf("Error saving data!\n");
        return;
    }

    fprintf(fp, "%s\n%d\n%.2f\n%s\n%s\n%s\n",acc.name,acc.pin,acc.balance,acc.bankName,acc.branch,acc.type);
    fclose(fp);
}

// Load account from file (or create default)
struct account loadFromFile()
{
    struct account acc;
    FILE *fp = fopen("account.txt", "r");

    if (fp == NULL) 
    {
        strcpy(acc.name,"Default User");
        acc.pin=1234;
        acc.balance=10000.0;

        strcpy(acc.bankName,"State Bamk of India");
        strcpy(acc.branch,"Dehradun Main branch");
        strcpy(acc.type,"savings");
        saveToFile(acc);
        return acc;
    }
    fgets(acc.name,sizeof(acc.name),fp);
    acc.name[strcspn(acc.name,"\n")]='\0';

    fscanf(fp, "%d %f", &acc.pin, &acc.balance);
    fgetc(fp);
    fgets(acc.bankName, sizeof(acc.bankName), fp);
    acc.bankName[strcspn(acc.bankName,"\n")]='\0';
    fgets(acc.branch, sizeof(acc.branch), fp);
    acc.branch[strcspn(acc.branch,"\n")]='\0';
    fgets(acc.type, sizeof(acc.type), fp);
    acc.type[strcspn(acc.type,"\n")]='\0';

    fclose(fp);
    return acc;
}



// NORMAL ATM FUNCTIONS
void checkBalance(struct account acc)
{
    printf("\nYour balance: Rs %.2f\n", acc.balance);
    if (acc.balance < 1000)
                printf("\n WARNING:Your balance is very low.Please deposit soon.\n");
            
}

struct account depositMoney(struct account acc)
{
    float amt;
    printf("Enter amount to deposit: ");
    scanf("%f", &amt);

    if (amt <= 0)
    {
        printf("Invalid amount.\n");
    } 
    else 
    {
        acc.balance += amt;
        printf("Deposited Rs %.2f\n", amt);   
        while (getchar() != '\n');
        char ch;
        printf("Do you want a receipt?(y/n):");
        scanf(" %c",&ch);  
        if(ch=='y'|| ch=='Y')
        {
            printf("\n----Receipt-----\n");
            printf("Transaction :deposited\n");
            printf("Amount: Rs %.2f\n", amt);
            printf("New balance:Rs %.2f\n",acc.balance);
    
        }
        char txt[200];
      sprintf(txt, "Deposited: Rs %.2f | Balance: Rs %.2f", amt, acc.balance);

      addHistory(txt);

    }
    return acc;
}

struct account withdrawMoney(struct account acc)
{
    float amt;
    printf("Enter amount to withdraw: ");
    scanf("%f", &amt);
    while (getchar() != '\n');
    if (amt<=0) 
    {
        printf("Invalid amount.\n");
    } 
    else if(amt>acc.balance) 
    {
        printf("Not enough balance.\n");
    } 
    else
     {
        acc.balance-=amt;
        printf("Please collect Rs %.2f\n", amt);
         char ch;
        printf("Do you want a receipt?(y/n):");
        scanf(" %c",&ch);  
        if(ch=='y'|| ch=='Y')
        {
            printf("\n----Receipt-----\n");
            printf("Transaction :withdrawal\n");
            printf("Amount:Rs %.2f\n",amt);
            printf("Remaining Balance: Rs %.2f\n",acc.balance); 
    
        }
        char txt[200];
        sprintf(txt, "Withdrawn: Rs %.2f | Balance: Rs %.2f", amt, acc.balance);
        addHistory(txt);

    }
   
    return acc;
}

struct account changePIN(struct account acc)
{
    int newPin, again;
    printf("Enter new PIN: ");
    scanf("%d", &newPin);

    printf("Re-enter new PIN: ");
    scanf("%d", &again);

    if (newPin == again && newPin >= 1000 && newPin <= 9999) {
        acc.pin = newPin;
        printf("PIN changed successfully.\n");
    } else {
        printf("PIN not changed. Either mismatch or not 4 digits.\n");
    }


    return acc;
}
    void addHistory(char *entry)
   {
    FILE *fp = fopen("history.txt", "a");
    if (fp == NULL) {
        printf("Error writing to history.\n");
        return;
    }
    fprintf(fp, "%s\n", entry);
    fclose(fp);
    }
void showHistory()
{
    FILE *fp = fopen("history.txt", "r");
    char line[200];

    if (fp == NULL) {
        printf("\nNo transactions recorded yet.\n");
        return;
    }

    printf("\n--- MINI TRANSACTION HISTORY ---\n");
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s", line);
    }
    fclose(fp);
}

struct account changeName(struct account acc)
{
    printf("Enter new Account Holder name:");
    getchar();
    fgets(acc.name,sizeof(acc.name),stdin);
    acc.name[strcspn(acc.name,"\n")]='\0';
    printf("Acc Holder name updated successfully\n");
    return acc;
}
void resetAccount() 
{
    FILE *fp=fopen("account.txt","w");
    fprintf(fp,"Default User\n1234\n10000.00\n");
    fprintf(fp, "State Bank of India\n");
    fprintf(fp, "Dehradun Main Branch\n");
    fprintf(fp, "Savings\n");
    fclose(fp);
    printf("Account reset successfully.\n");
}

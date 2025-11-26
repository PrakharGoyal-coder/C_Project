#include <stdio.h>
#include<string.h>

struct account 
{
    char name[50];
    int pin;
    float balance;
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

    fprintf(fp, "%s\n%d\n%.2f\n",acc.name,acc.pin,acc.balance);
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
        saveToFile(acc);
        return acc;
    }
    fgets(acc.name,sizeof(acc.name),fp);
    acc.name[strcspn(acc.name,"\n")]='\0';

    fscanf(fp, "%d %f", &acc.pin, &acc.balance);
    fclose(fp);
    return acc;
}



// NORMAL ATM FUNCTIONS
void checkBalance(struct account acc)
{
    printf("\nYour balance: Rs %.2f\n", acc.balance);
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
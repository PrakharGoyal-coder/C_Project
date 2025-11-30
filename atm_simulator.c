#include <stdio.h>
#include<stdlib.h>
#include<string.h>

struct account 
{
    char name[50];   //to store acc holder name
    int pin;          //to store the pin
    float balance;    //to store account balance
    char bankName[50];   //to store bank name
    char branch[50];    //to store branch
    char type[20];       //to store the account type
};


void checkBalance(struct account acc);
struct account depositMoney(struct account acc);
struct account withdrawMoney(struct account acc);
struct account changePIN(struct account acc);
void addHistory(char *entry);
void showHistory();
struct account changeName(struct account acc);
void saveToFile(struct account acc);
struct account loadFromFile();
struct account resetAccount(struct account user);

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
        if (scanf("%d", &enteredPin) != 1) 
        {
        printf("Invalid input! Numbers only.\n");
         while (getchar()!='\n'); // to clear the buffer
        continue;
        }


        if (enteredPin==user.pin) 
        {   
            //showing user and acc information
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
        if(attempts==3) //exiting if more than 3 wrong pins entered
        {
            printf("You tried wrong pin as too many times.\n");
            printf("Account locked for safety.\n");
            return 0;
        }
    }

    // printing Menu and using switch case to execute the menu choices
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
        while(getchar()!='\n');   // to clear the buffer

        switch (choice) 
        {
            case 1:
                checkBalance(user);
                break;

            case 2:
                user=depositMoney(user);
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

            case 8:  user=resetAccount(user);
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
    if (fp == NULL) 
    {
        printf("Error saving data!\n");
        return;
    }
    //printing information to file account.txt
    fprintf(fp, "%s\n%d\n%.2f\n%s\n%s\n%s\n",acc.name,acc.pin,acc.balance,acc.bankName,acc.branch,acc.type);
    fclose(fp);  //closing file
}

// Load account from file (or create default)
struct account loadFromFile()
{
    struct account acc;
    FILE *fp = fopen("account.txt", "r");

    if (fp==NULL) //if file is empty then storing the default values
    {
        strcpy(acc.name,"Default User");
        acc.pin=1234;
        acc.balance=10000.0;
        strcpy(acc.bankName,"State Bank of India");
        strcpy(acc.branch,"Dehradun Main Branch");
        strcpy(acc.type,"savings");
        saveToFile(acc);
        return acc;
    }
    //if file is not empty Then storing the stored data in structure vriables
    fgets(acc.name,sizeof(acc.name),fp); //reading name
    acc.name[strcspn(acc.name,"\n")]='\0';

    fscanf(fp,"%d",&acc.pin);
    fscanf(fp,"%f",&acc.balance);
    fgetc(fp);
    fgets(acc.bankName, sizeof(acc.bankName), fp);
    acc.bankName[strcspn(acc.bankName,"\n")]='\0';
    fgets(acc.branch, sizeof(acc.branch), fp);
    acc.branch[strcspn(acc.branch,"\n")]='\0';
    fgets(acc.type,sizeof(acc.type), fp);
    acc.type[strcspn(acc.type,"\n")]='\0';

    fclose(fp);   //closing the file
    return acc;
}



// NORMAL ATM FUNCTIONS
void checkBalance(struct account acc)
{
    printf("\nYour balance: Rs %.2f\n", acc.balance);//printing acc balance
    if (acc.balance < 1000)
               { printf("\n WARNING:Your balance is very low.Please deposit soon.\n");
               }
}

struct account depositMoney(struct account acc)
{
    float amt;
    printf("Enter amount to deposit: ");  //taking amount to be deposited
    scanf("%f",&amt);

    if(amt<=0)  //if amt less than or equal to 0
    {
        printf("Invalid amount.\n");
    } 
    else 
    {
        acc.balance+=amt;    //updating the balance
        printf("Deposited Rs %.2f\n", amt);   
        while (getchar() !='\n');  //to clear the buffer 
        char ch;
        printf("Do you want a receipt?(y/n):"); //asking for receipt printing
        scanf(" %c",&ch);  
        if(ch=='y'|| ch=='Y')
        {
            printf("\n----Receipt-----\n");
            printf("Transaction :deposited\n");
            printf("Amount: Rs %.2f\n", amt);
            printf("New balance:Rs %.2f\n",acc.balance);
    
        }
         //adding transaction to history
        addHistory("Deposited:");
        FILE *fp =fopen("history.txt", "a");    //opening file in append mode to add transaction info
        fprintf(fp, " Rs %.2f | Balance: Rs %.2f\n", amt, acc.balance);
        fclose(fp);   //closing the file

    }
    return acc;
}

struct account withdrawMoney(struct account acc)
{
    float amt;
    printf("Enter amount to withdraw: ");
    scanf("%f", &amt);   //inputting withdrawl amount
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
        acc.balance-=amt;    //updating the balance
        printf("Please collect Rs %.2f\n", amt);
         char ch;
        printf("Do you want a receipt?(y/n):");  //asking for receipt
        scanf(" %c",&ch);  
        if(ch=='y'|| ch=='Y')
        {
            printf("\n----Receipt-----\n");
            printf("Transaction :withdrawal\n");
            printf("Amount:Rs %.2f\n",amt);
            printf("Remaining Balance: Rs %.2f\n",acc.balance); 
    
        }
        //adding transaction to history
        addHistory("Withdrawn:");
        FILE *fp= fopen("history.txt", "a");  //opening file in append mode to add transaction info
        fprintf(fp, " Rs %.2f | Balance: Rs %.2f\n", amt, acc.balance);
        fclose(fp);  //closing the file

    }
   
    return acc;
}

struct account changePIN(struct account acc)
{
    int newPin,again;
    printf("Enter new PIN: ");
    scanf("%d", &newPin);
    while(getchar()!='\n');   // to clear buffer

    printf("Re-enter new PIN: ");   //for re-confirming the pin
    scanf("%d", &again);
    while(getchar()!='\n');   // to clear buffer

    if (newPin==again && newPin>=1000 && newPin<= 9999) {
        acc.pin=newPin;
        printf("PIN changed successfully.\n");
    }
    else 
    {   // if the entered new pins doesnot match
        printf("PIN not changed. Either mismatch or not 4 digits.\n");
    }

    return acc;
}
void addHistory(char *entry)
{
    FILE *fp = fopen("history.txt", "a");
    if (fp ==NULL) //if file does not exist 
    {
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
    int empty=1; //to check if history.txt is empty or not

    if (fp==NULL) //if file does not exist
    {
        printf("\nNo transactions recorded yet.\n");
        return;
    }

    printf("\n--- MINI TRANSACTION HISTORY ---\n");
    while (fgets(line,sizeof(line),fp) != NULL) 
    {    empty=0;
        printf("%s", line);
    }
    if(empty)  //if file exist but empty
    {
        printf("No transactions recorded yet.\n");
    }
    fclose(fp);
}
struct account changeName(struct account acc)
{
    char temp[50];
    
    printf("Enter new Account Holder name: ");

   
    // Read the name safely using fgets()
    if (fgets(temp, sizeof(temp), stdin) != NULL) {
        // Remove trailing newline character left by fgets()
        temp[strcspn(temp, "\n")] = '\0'; 
        
        if (strlen(temp) == 0) { // Check for empty input (e.g., if user just pressed Enter)
            printf("No name entered. Name unchanged.\n");
            return acc;
        }
        
        // Update account name
        strcpy(acc.name, temp); 
        printf("Account Holder name updated successfully: %s\n", acc.name);
    } else {
        printf("Error reading name.\n");
    }

    return acc;
}


struct account resetAccount(struct account user) 
{    //reseting the account
    FILE *fp=fopen("account.txt","w");   //first emptying and then writing
    if(fp==NULL) 
    {
        printf("Error resetting account!\n");
        return user;
    }
    fprintf(fp,"Default User\n1234\n10000.00\n");
    fprintf(fp,"State Bank of India\n");
    fprintf(fp,"Dehradun Main Branch\n");
    fprintf(fp,"Savings\n");
    fclose(fp);
    // Reset History File
    FILE *history=fopen("history.txt", "w");  // opening in write mode clears the file
    fclose(history);
    printf("Transaction history cleared.\n");
    printf("Account reset successfully.\n");
    // Reload default values into memory
    user = loadFromFile();
    return user;
}

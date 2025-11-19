#include <stdio.h>

struct account 
{
    int pin;
    float balance;
};


void checkBalance(struct account acc);
struct account depositMoney(struct account acc);
struct account withdrawMoney(struct account acc);
struct account changePIN(struct account acc);

int main()
{
    struct account user;
    user.pin = 1234;
    user.balance = 10000.0;
    int enteredPin, choice;
    int attempts = 0;

    printf("\n- Welcome to State Bank ATM -\n");

    while (1) {
        printf("Enter your 4 digit pin: ");
        scanf("%d", &enteredPin);

        if (enteredPin==user.pin) 
        {
            printf("Login successful.\n");
            break;
        } else
        {
            attempts++;
            printf("Wrong pin. Attempts left: %d\n",3-attempts);
        }

        if (attempts==3)
        {
            printf("\nYou tried wrong pin too many times.\n");
            printf("Account locked for safety.\n");
            return 0;
        }
    }

    // menu for user
    do {
        printf("\n--MENU --\n");
        printf("1.Check Balance\n");
        printf("2.Deposit Money\n");
        printf("3.Withdraw Money\n");
        printf("4.Change PIN\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) 
        {
            case 1:checkBalance(user); break;

            case 2:  user=depositMoney(user); break;

            case 3:  user=withdrawMoney(user); break;

            case 4:  user=changePIN(user); break;

            case 5: printf("\nThanks for using the ATM.\n");break;

            default: printf("Invalid option. Try again.\n");
        }

    } while (choice!=5);
    return 0;
}

void checkBalance(struct account acc)
{
    printf("\nYour balance: Rs %.2f\n", acc.balance);
}

struct account depositMoney(struct account acc)
{
    float amt;
    printf("Enter amount to deposit: ");
    scanf("%f", &amt);

    if (amt <= 0) {
        printf("Invalid amount.\n");
    } else {
        acc.balance+=amt;
        printf("Deposited Rs %.2f\n", amt);
    }

    return acc;
}

struct account withdrawMoney(struct account acc)
{
    float amt;
    printf("Enter amount to withdraw: ");
    scanf("%f", &amt);

    if (amt<=0)
    {
        printf("Invalid amount.\n");
    } else if (amt > acc.balance) {
        printf("Not enough balance.\n");
    } else {
        acc.balance -= amt;
        printf("Please collect Rs %.2f\n", amt);
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

    if (newPin == again && newPin > 999 && newPin <= 9999) {
        acc.pin = newPin;
        printf("PIN changed.\n");
    } else {
        printf("PIN not changed. Either mismatch or not 4 digit.\n");
    }

    return acc;
}

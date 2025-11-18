#include <stdio.h>
#include <string.h>

void check_Balance(float balance);
float deposit_Money(float balance);
float withdraw_Money(float balance);
int change_PIN(int pin);

int main() 
{
    int pin = 1234,enteredPin,choice;
    float balance=10000.0; // Last balance
    int attempts=0;
    int max_attempts=3;

    printf("Welcome to State Bank ATM\n");

    // PIN verification
    while (1)
    {
        printf("Enter your 4-digit PIN: \n");
        scanf("%d",&enteredPin);

        if(enteredPin==pin) 
        {
            printf("Login successful");
            break;
        }
        else 
        {
            attempts++;
            printf("Incorrect PIN. Attempts left:%d\n",max_attempts-attempts);
        }

        if(attempts>=max_attempts) 
        {
            printf("Too many wrong attempts!Your account has been locked.\n");
            return 0;
        }
    }

    
    do {
        printf("ATM Menu\n");
        printf("1.Check Balance\n");
        printf("2.Deposit Money\n");
        printf("3.Withdraw Money\n");
        printf("4.Change PIN\n");
        printf("5.Mini Statement\n");  ////LEFT TO DO
        printf("6.Exit\n");
        
        printf("Enter your choice: ");
        scanf("%d",&choice);

        switch(choice) 
        {
            case 1:    check_Balance(balance);
                        break;

            case 2:    balance=deposit_Money(balance);
                        break;

            case 3:     balance=withdraw_Money(balance);
                         break;

            case 4:      pin=change_PIN(pin);
                         break;

            case 6:     printf("\nThank you for using our ATM.....\n");
                         break;

            default:
                printf("Invalid choice...Please try again.\n");
        }

    } while(choice!=6);

    return 0;
}

void check_Balance(float balance)
{
    printf("Your current balance is: Rs.%.2f\n",balance);
}

float deposit_Money(float balance) 
{
    float deposit;
    printf("Enter amount to deposit: Rs.");
    scanf("%f",&deposit);

    if (deposit<=0) 
    {
        printf("Invalid amount.\n");
    } 
    else 
    {
         balance+=deposit;
        printf("Rs.%.2f deposited successfully\n", deposit);
    }
    return balance;
}

float withdraw_Money(float balance)
 {   
    printf("\n");
    float withdraw;
    printf("Enter amount to withdraw: Rs");
    scanf("%f", &withdraw);

    if(withdraw<=0) 
    {
        printf("Invalid amount.\n");
    } 
    else if(withdraw>balance) 
    {
        printf("Insufficient balance.\n");
    } 
    else 
    {
        balance=balance-withdraw;
        printf("Please collect your cash: Rs.%.2f\n", withdraw);
    }

    return balance;
}

int change_PIN(int pin) 
{
    int newPin, confirmPin;

    printf("\nEnter new 4-digit PIN: ");
    scanf("%d", &newPin);
    printf("Confirm new PIN: ");
    scanf("%d", &confirmPin);

    if(newPin==confirmPin && newPin>=1000 && newPin<=9999) 
    {
        pin=newPin;
        printf("PIN changed successfully\n");
    }
     else 
    {
        printf("PINs do not match or invalid format.\n");
    }
    return pin;
}

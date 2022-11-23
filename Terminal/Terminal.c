#include <stdio.h>
#include "Terminal.h"
#include "string.h"

ST_terminalData_t newTerminal;
ST_cardData_t newUser;
EN_terminalError_t getTransactionDate(ST_terminalData_t* termData) {
	printf("Please Enter Transaction Date\n");
	gets(termData->transactionDate);
	EN_terminalError_t TransactionDateValidity = TERMINAL_OK;
	if (termData->transactionDate[0] == '\0' || termData->transactionDate[2] != '/' || termData->transactionDate[5] != '/' || strlen(termData->transactionDate) != 10)
		TransactionDateValidity = WRONG_DATE;

	return TransactionDateValidity;
}
EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData) {
	EN_terminalError_t CardExpirationValidity = CARD_OK;
	char* terminalDataDate_ptr[3] = {0,0,0};
	char* cardDataDate_ptr[2] = {0,0};
	ST_cardData_t cardDataToInt = *cardData;
	ST_terminalData_t termDataToInt = *termData;
	terminalDataDate_ptr[0] = strtok(termDataToInt.transactionDate, "/");
	terminalDataDate_ptr[1] = strtok(NULL, "/");
	terminalDataDate_ptr[2] = strtok(NULL, "/");
	cardDataDate_ptr[0] = strtok(cardDataToInt.cardExpirationDate,"/");
	cardDataDate_ptr[1] = strtok(NULL, "/");
	if (atoi(cardDataDate_ptr[1]) < (atoi(terminalDataDate_ptr[2])-2000))
		CardExpirationValidity = EXPIRED_CARD;
	else if (atoi(cardDataDate_ptr[1]) == (atoi(terminalDataDate_ptr[2])-2000) && atoi(cardDataDate_ptr[0]) < atoi(terminalDataDate_ptr[1]))
		CardExpirationValidity = EXPIRED_CARD;

	return CardExpirationValidity;
}
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {
	printf("Please Enter The Transaction Amount\n");
	scanf("%f",&(newTerminal.transAmount));
	EN_terminalError_t TransactionAmountValidity = CARD_OK;
	if (termData->transAmount <= 0) {
		TransactionAmountValidity = INVALID_AMOUNT;
		}
	return TransactionAmountValidity;
}
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData) {
	EN_terminalError_t BelowMaxAmountValidity = CARD_OK;
		if (termData->transAmount > termData->maxTransAmount)
			BelowMaxAmountValidity = EXCEED_MAX_AMOUNT;
		return BelowMaxAmountValidity;
}
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float maxAmount) {
	EN_terminalError_t MaxAmountValidity = CARD_OK;
	termData->maxTransAmount = maxAmount;
	if (maxAmount <= 0.0) {
		MaxAmountValidity = INVALID_MAX_AMOUNT;
	}
	return MaxAmountValidity;
}	
void getTransactionDateTest(void) {
	printf("Tester Name: Mohamed Mamdouh\n");
	printf("Function Name : getTransactionDate\n");
	printf("Test Case 1 :Inserting A NULL transaction date\nInput Data :%s\nExpected Result :1\nActual Result :%d\n",newTerminal.transactionDate,getTransactionDate(&newTerminal));
	printf("Test Case 2 :Inserting A transaction date with the wrong format\nInput Data :%s\nExpected Result :1\nActual Result :%d\n", newTerminal.transactionDate, getTransactionDate(&newTerminal));
	printf("Test Case 3 :Inserting A transaction date with not equal to 10 characters\nInput Data :%s\nExpected Result :1\nActual Result :%d\n", newTerminal.transactionDate, getTransactionDate(&newTerminal));
	printf("Test Case 4 :Inserting A transaction date suitable character length\nInput Data :%s\nExpected Result :0\nActual Result :%d\n", newTerminal.transactionDate, getTransactionDate(&newTerminal));

}
void isCardExpriedTest(void) {
	printf("Tester Name: Mohamed Mamdouh\n");
	printf("Function Name : isCardExpiredTest\n");
	strcpy(newTerminal.transactionDate, "10/22");
	strcpy(newUser.cardExpirationDate, "05/21");
	printf("Test Case 1 :Inserting An expiry date earlier in both month and year than in transaction date\nInput Data :%s %s\nExpected Result :2\nActual Result :%d\n",newTerminal.transactionDate, newUser.cardExpirationDate, isCardExpired(&newUser, &newTerminal));
	strcpy(newTerminal.transactionDate, "10/22");
	strcpy(newUser.cardExpirationDate, "11/21");
	printf("Test Case 2 :Inserting A expiry date earlier in year only but later in month than in transaction date\nInput Data :%s %s\nExpected Result :2\nActual Result :%d\n", newTerminal.transactionDate, newUser.cardExpirationDate, isCardExpired(&newUser, &newTerminal));
	strcpy(newTerminal.transactionDate, "10/22");
	strcpy(newUser.cardExpirationDate, "05/25");
	printf("Test Case 3 :Inserting A suitable expiry date and transaction date\nInput Data :%s %s\nExpected Result :0\nActual Result :%d\n", newTerminal.transactionDate, newUser.cardExpirationDate, isCardExpired(&newUser, &newTerminal));
}
void getTransactionAmountTest(void) {
	printf("Tester Name: Mohamed Mamdouh\n");
	printf("Function Name : getTransactionAmount\n");
	printf("Test Case 1 :Inserting A negative amount for transaction\nInput Data :%f\nExpected Result :4\nActual Result :%d\n",newTerminal.transAmount,getTransactionAmount(&newTerminal));
	printf("Test Case 2 :Inserting A zero amount for transaction\nInput Data :%f\nExpected Result :4\nActual Result :%d\n",newTerminal.transAmount,getTransactionAmount(&newTerminal));
	printf("Test Case 3 :Inserting A Positive amount for transaction\nInput Data :%f\nExpected Result :0\nActual Result :%d\n",newTerminal.transAmount,getTransactionAmount(&newTerminal));
}
void isBelowMaxAmountTest(void) {
	printf("Tester Name: Mohamed Mamdouh\n");
	printf("Function Name : isBelowMaxAmount\n");
	newTerminal.maxTransAmount = 10000.0;
	newTerminal.transAmount = 50000.0;
	printf("Test Case 1 :Inserting An amount that exceeds the max Transaction Amount\nInput Data :50000.0\nExpected Result :5\nActual Result :%d\n", isBelowMaxAmount(&newTerminal));
	newTerminal.transAmount = 10000.0;
	printf("Test Case 2 :Inserting An amount that is equal to the max Transaction Amount\nInput Data :10000.0\nExpected Result :0\nActual Result :%d\n", isBelowMaxAmount(&newTerminal));
	newTerminal.transAmount = 5000.0;
	printf("Test Case 3 :Inserting An amount that is less the max Transaction Amount\nInput Data :5000.0\nExpected Result :0\nActual Result :%d\n", isBelowMaxAmount(&newTerminal));
}
void setMaxAmountTest(void) {
	printf("Tester Name: Mohamed Mamdouh\n");
	printf("Function Name : setMaxAmount\n");
	newTerminal.maxTransAmount = -5000.0;
	printf("Test Case 1 :Inserting A negative amount for max Transaction\nInput Data :-5000.0\nExpected Result :6\nActual Result :%d\n", setMaxAmount(&newTerminal, newTerminal.maxTransAmount));
	newTerminal.maxTransAmount = 0.0;
	printf("Test Case 3 :Inserting A zero amount for max Transaction\nInput Data :0.0\nExpected Result :6\nActual Result :%d\n", setMaxAmount(&newTerminal, newTerminal.maxTransAmount));
	newTerminal.maxTransAmount = 5000.0;
	printf("Test Case 3 :Inserting A Positive amount for max Transaction\nInput Data :5000.0\nExpected Result :0\nActual Result :%d\n", setMaxAmount(&newTerminal, newTerminal.maxTransAmount));
}

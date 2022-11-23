#include <stdio.h>
#include "Server.h"
#include "String.h"
ST_accountsDB_t accountsDB[255]={
	{2000.0,RUNNING,"78956423698756489"},
	{3000.0,BLOCKED,"87965369854789423"},
	{4000.0,RUNNING,"78546321895647792"},
	{5000.0,RUNNING,"36598464156484169"},
	{9000.0,RUNNING,"49529585299252999"},
	{8000.0,BLOCKED,"91981926059292529"},
	{4500.0,BLOCKED,"96385274196385271"},
	{1000.0,RUNNING,"78945561237894561"},
	{1500.0,BLOCKED,"14725836914725836"},
	{5450.0,BLOCKED,"95175385236987412"},
	{6500.0,BLOCKED,"98769873692587895"},
	{9999.0,RUNNING,"15995135775365445"},
	{7000.0,RUNNING,"78945698536982566"},
};
ST_transaction_t TransactionDB[255] = {0};
ST_accountsDB_t* AccountInDatabase=NULL;
ST_transaction_t newTransaction;
EN_transState_t recieveTransactionData(ST_transaction_t* transData) {
	EN_transState_t TransactionDataValidity = APPROVED;
	int accountIndex;
	for (accountIndex = 0; accountIndex < 255; accountIndex++) {
		if (!(strcmp(transData->cardHolderData.primaryAccountNumber, accountsDB[accountIndex].primaryAccountNumber))) {
			TransactionDataValidity = APPROVED;
			break;
		}
		else {
			TransactionDataValidity = FRAUD_CARD;
		}

	}
	if (accountsDB[accountIndex].balance - transData->terminalData.transAmount < 0 && TransactionDataValidity != FRAUD_CARD) {
		TransactionDataValidity = DECLINED_INSUFFECIENT_FUND;
	}
	else if (accountsDB[accountIndex].state == BLOCKED && TransactionDataValidity != FRAUD_CARD) {
		TransactionDataValidity = DECLINED_STOLEN_CARD;
	}
	else if (TransactionDB[254].transactionSequenceNumber != 0 && TransactionDataValidity != FRAUD_CARD) {
		TransactionDataValidity = INTERNAL_SERVER_ERROR;
	}
	if (TransactionDataValidity==APPROVED) {
		accountsDB[accountIndex].balance -= transData->terminalData.transAmount;
	}
	return TransactionDataValidity;
}
EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence) {
	EN_serverError_t AccountValidity = SERVER_OK;
	int accountIndex;
	for (accountIndex = 0; accountIndex < 255; accountIndex++) {
		if (!(strcmp(cardData->primaryAccountNumber, accountsDB[accountIndex].primaryAccountNumber))) {
			AccountValidity = SERVER_OK;
			*accountRefrence = accountsDB[accountIndex];
			break;
		}
		else {
			AccountValidity = ACCOUNT_NOT_FOUND;
		}

	}
	return AccountValidity;
}
EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence) {
	EN_serverError_t BlockedAccountValidity = SERVER_OK;
		if (accountRefrence->state == BLOCKED) {
		BlockedAccountValidity = BLOCKED_ACCOUNT;
	}
		return BlockedAccountValidity;
}
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence) {
	EN_serverError_t AmountAvailableValidity = SERVER_OK;
	if (accountRefrence->balance - termData->transAmount < 0) {
		AmountAvailableValidity = LOW_BALANCE;
	}
	return AmountAvailableValidity;
}
EN_serverError_t saveTransaction(ST_transaction_t* transData) {
	EN_serverError_t saveTransactionValidity = SERVER_OK;
	int emptyDatabaseIndex = 0;
	for (emptyDatabaseIndex = 0; emptyDatabaseIndex < 255; emptyDatabaseIndex++) {
		if (TransactionDB[emptyDatabaseIndex].transactionSequenceNumber == 0) {
			break;
		}
	}
	TransactionDB[emptyDatabaseIndex].cardHolderData = transData->cardHolderData;
	TransactionDB[emptyDatabaseIndex].terminalData = transData->terminalData;
	TransactionDB[emptyDatabaseIndex].transState = transData->transState;
	if (emptyDatabaseIndex != 0) {
		TransactionDB[emptyDatabaseIndex].transactionSequenceNumber = TransactionDB[emptyDatabaseIndex - 1].transactionSequenceNumber + 1;
	}
	else {
		TransactionDB[emptyDatabaseIndex].transactionSequenceNumber = 1;
	}
	listSavedTransactions();
	return saveTransactionValidity;
}

void listSavedTransactions(void) {
	for (int i = 0; TransactionDB[i].transactionSequenceNumber != 0; i++) {
		printf("#########################\n");
		printf("Transaction Sequence Number : %d\n", TransactionDB[i].transactionSequenceNumber);
		printf("Transaction Date : %s\n", TransactionDB[i].terminalData.transactionDate);
		printf("Transaction Amount : %f\n", TransactionDB[i].terminalData.transAmount);
		printf("Transaction State : %d\n", TransactionDB[i].transState);
		printf("Terminal Max Amount : %f\n", TransactionDB[i].terminalData.maxTransAmount);
		printf("Cardholder Name :%s\n", TransactionDB[i].cardHolderData.cardHolderName);
		printf("PAN: %s\n", TransactionDB[i].cardHolderData.primaryAccountNumber);
		printf("Card Expiration Date : %s\n", TransactionDB[i].cardHolderData.cardExpirationDate);
		printf("#########################\n");
	}
}
void recieveTransactionDataTest(void) {
	printf("Tester Name: Mohamed Mamdouh\n");
	printf("Function Name : recieveTransactionData\n");
	strcpy(newTransaction.cardHolderData.primaryAccountNumber,"88888888888888888");
	printf("Test Case 1 :Inserting A non existing PAN number\nInput Data :8888888888888888888\nExpected Result :3\nActual Result :%d\n",recieveTransactionData(&newTransaction));
	strcpy(newTransaction.cardHolderData.primaryAccountNumber, "96385274196385271");
	newTransaction.terminalData.transAmount = 10000.0;
	printf("Test Case 2 :Inserting A transaction amount higher than the actual balance\nInput Data :10000.0\nExpected Result :1\nActual Result :%d\n", recieveTransactionData(&newTransaction));
	newTransaction.terminalData.transAmount = 1000.0;
	strcpy(newTransaction.cardHolderData.primaryAccountNumber, "98769873692587895");
	printf("Test Case 3 :Inserting A BLOCKED PAN number\nInput Data :98769873692587895\nExpected Result :2\nActual Result :%d\n", recieveTransactionData(&newTransaction));
	strcpy(newTransaction.cardHolderData.primaryAccountNumber, "15995135775365445");
	printf("Test Case 4 :Inserting A valid set of information\nInput Data :96385274196385271\nExpected Result :0\nActual Result :%d\n", recieveTransactionData(&newTransaction));
}
void isValidAccountTest(void) {
	printf("Tester Name: Mohamed Mamdouh\n");
	printf("Function Name : isValidAccount\n");
	strcpy(newTransaction.cardHolderData.primaryAccountNumber, "88888888888888888");
	printf("Test Case 1 :Inserting A BLOCKED PAN number\nInput Data :88888888888888888\nExpected Result :3\nActual Result :%d\n", isValidAccount(&(newTransaction.cardHolderData), &AccountInDatabase));
	strcpy(newTransaction.cardHolderData.primaryAccountNumber, "15995135775365445");
	printf("Test Case 2 :Inserting A Valid PAN number\nInput Data :15995135775365445\nExpected Result :0\nActual Result :%d\n", isValidAccount(&(newTransaction.cardHolderData), &AccountInDatabase));
}
void isBlockedAccountTest(void) {
	printf("Tester Name: Mohamed Mamdouh\n");
	printf("Function Name : isBlockedAccount\n");
	AccountInDatabase = &accountsDB[1];
	printf("Test Case 1 :Inserting A BLOCKED PAN number\nInput Data :87965369854789423\nExpected Result :5\nActual Result :%d\n", isBlockedAccount(AccountInDatabase));
	AccountInDatabase = &accountsDB[0];
	printf("Test Case 2 :Inserting A RUNNING PAN number\nInput Data :78956423698756489\nExpected Result :0\nActual Result :%d\n", isBlockedAccount(AccountInDatabase));
}
void isAmountAvailableTest(void) {
	printf("Tester Name: Mohamed Mamdouh\n");
	printf("Function Name : isAmountAvailable\n");
	AccountInDatabase = &accountsDB[0];
	newTransaction.terminalData.transAmount = 10000.0;
	printf("Test Case 1 :Inserting A Transaction amount higher than actual balance\nInput Data :87965369854789423\nExpected Result :4\nActual Result :%d\n", isAmountAvailable(&(newTransaction.terminalData),AccountInDatabase));
	AccountInDatabase = &accountsDB[0];
	newTransaction.terminalData.transAmount = 1000.0;
	printf("Test Case 2 :Inserting A Transaction amount lower than the actual balance\nInput Data :78956423698756489\nExpected Result :0\nActual Result :%d\n", isAmountAvailable(&(newTransaction.terminalData), AccountInDatabase));
}
void saveTransactionTest(void) {
	printf("Tester Name: Mohamed Mamdouh\n");
	printf("Function Name : saveTransaction\n");
	strcpy(newTransaction.cardHolderData.cardExpirationDate,"10/25");
	strcpy(newTransaction.cardHolderData.cardHolderName,"Mohamed Mamdouh Mohamed");
	strcpy(newTransaction.cardHolderData.primaryAccountNumber,"78956423698756489");
	newTransaction.terminalData.maxTransAmount=20000.0;
	strcpy(newTransaction.terminalData.transactionDate, "25/10/2022");
	newTransaction.terminalData.transAmount = 1000.0;
	newTransaction.transState = APPROVED;
	printf("Test Case 1 :Inserting A VALID USER TRANSACTION\nInput Data : newTransaction\nExpected Result :0\nActual Result :%d\n", saveTransaction(&newTransaction));
	newTransaction.transState = BLOCKED;
	printf("Test Case 2 :Inserting A BLOCKED USER TRANSACTION\nInput Data : newTransaction\nExpected Result :0\nActual Result :%d\n", saveTransaction(&newTransaction));
}
void listSavedTransactionsTest(void) {
	printf("Tester Name: Mohamed Mamdouh\n");
	printf("Function Name : listSavedTransaction\n");
	strcpy(newTransaction.cardHolderData.cardExpirationDate, "10/25");
	strcpy(newTransaction.cardHolderData.cardHolderName, "Mohamed Mamdouh Mohamed");
	strcpy(newTransaction.cardHolderData.primaryAccountNumber, "78956423698756489");
	newTransaction.terminalData.maxTransAmount = 20000.0;
	strcpy(newTransaction.terminalData.transactionDate, "25/10/2022");
	newTransaction.terminalData.transAmount = 1000.0;
	newTransaction.transState = APPROVED;
	saveTransaction(&newTransaction);
	printf("Test Case 1 :Inserting A VALID USER TRANSACTION\nInput Data : newTransaction\nExpected Result :\nActual Result :\n");
	listSavedTransactions();
	newTransaction.transState = BLOCKED;
	saveTransaction(&newTransaction);
	printf("Test Case 2 :Inserting A BLOCKED USER TRANSACTION\nInput Data : newTransaction\nExpected Result :\nActual Result :\n");
	listSavedTransactions();

}

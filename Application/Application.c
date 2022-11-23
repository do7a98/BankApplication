#include <stdio.h>
#include <stdlib.h>
#include "Card.h"
#include "Terminal.h"
#include "Server.h"
#include "Application.h"
ST_cardData_t newUser;
ST_terminalData_t newTerminal;
ST_transaction_t newTransaction;
ST_accountsDB_t AccountInDB;
void appStart() {
	setMaxAmount(&newTerminal,5000.0);
	getCardHolderName(&newUser);
	getCardExpiryDate(&newUser);
	getCardPAN(&newUser);
	getTransactionDate(&newTerminal);
	if (isCardExpired(&newUser, &newTerminal) == EXPIRED_CARD) {
		printf("Card is Expired !\n");
		newTransaction.transState = EXPIRED_CARD;
	}
	else {
		getTransactionAmount(&newTerminal);
		if (isBelowMaxAmount(&newTerminal) == EXCEED_MAX_AMOUNT) {
			printf("Transaction amount is exceeding the max limit\n");
			newTransaction.transState = EXCEED_MAX_AMOUNT;
		}
		else {
			if (isValidAccount(&newUser, &AccountInDB) == ACCOUNT_NOT_FOUND) {
				printf("Account Not Found\n");
				newTransaction.transState = ACCOUNT_NOT_FOUND;
			}
			else {
				if (isAmountAvailable(&newTerminal, &AccountInDB) == LOW_BALANCE) {
					printf("Declined Insuffecient Fund\n");
					newTransaction.transState = LOW_BALANCE;
				}
				else
				{
					if (isBlockedAccount(&AccountInDB) == BLOCKED_ACCOUNT) {
						printf("Declined Stolen Card\ns");
						newTransaction.transState = DECLINED_STOLEN_CARD;
					}
					else {
						newTransaction.transState = APPROVED;
					}
				}
			}
		}
	}
	newTransaction.cardHolderData = newUser;
	newTransaction.terminalData = newTerminal;
	recieveTransactionData(&newTransaction);
	saveTransaction(&newTransaction);
}
int main() {
	appStart();
}

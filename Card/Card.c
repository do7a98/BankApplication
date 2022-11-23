#include <stdio.h>
#include <string.h>
#include "Card.h"
ST_cardData_t newUser;
EN_cardError_t getCardHolderName(ST_cardData_t* cardData) {
	printf("Please Enter The Card Holder Name\n");
	gets(cardData->cardHolderName);
	EN_cardError_t cardHolderNameValidity = CARD_OK;
	if (strlen(cardData->cardHolderName) > 24 || strlen(cardData->cardHolderName) < 20 || cardData->cardHolderName[0] == '\0')
		cardHolderNameValidity = WRONG_NAME;
	return cardHolderNameValidity;
}
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {
	printf("Please Enter The Card Expiry Date\n");
	gets(cardData->cardExpirationDate);
	EN_cardError_t cardExpiryDateValidity = CARD_OK;
	if (strlen(cardData->cardExpirationDate) != 5 || cardData->cardExpirationDate[2] != '/')
		cardExpiryDateValidity = WRONG_EXP_DATE;
	return cardExpiryDateValidity;
}
EN_cardError_t getCardPAN(ST_cardData_t* cardData) {
	printf("Please Enter The Card PAN\n");
	gets(cardData->primaryAccountNumber);
	EN_cardError_t cardPANValidity = CARD_OK;
	if (strlen(cardData->primaryAccountNumber) < 16 || strlen(cardData->primaryAccountNumber) > 19)
		cardPANValidity = WRONG_PAN;
	return cardPANValidity;
}
void getCardHolderNameTest(void) {
	printf("Tester Name: Mohamed Mamdouh\n");
	printf("Function Name : getCardHolderName\n");
	printf("Test Case 1 :Inserting A NULL User name\nInput Data :%s\nExpected Result : 1\nActual Result : %d\n",newUser.cardHolderName, getCardHolderName(&newUser));
	printf("Test Case 2 :Inserting A user name with less than 20 characters\nInput Data :%s\nExpected Result :1\nActual Result :%d\n", newUser.cardHolderName, getCardHolderName(&newUser));
	printf("Test Case 3 :Inserting A user name with more than 24 characters\nInput Data :%s\nExpected Result :1\nActual Result :%d\n", newUser.cardHolderName, getCardHolderName(&newUser));
	printf("Test Case 4 :Inserting A user name with suitable character length\nInput Data :%s\nExpected Result :0\nActual Result :%d\n", newUser.cardHolderName, getCardHolderName(&newUser));
}

void getCardExpiryDateTest(void) {
	printf("Tester Name: Mohamed Mamdouh\n");
	printf("Function Name : getCardExpiryDate\n");
	printf("Test Case 1 :Inserting A NULL date\nInput Data :%s\nExpected Result :2\nActual Result :%d\n",newUser.cardExpirationDate,getCardExpiryDate(&newUser));
	printf("Test Case 2 :Inserting A date with wrong format\nInput Data :%s\nExpected Result :2\nActual Result :%d\n",newUser.cardExpirationDate, getCardExpiryDate(&newUser));
	printf("Test Case 3 :Inserting A suitable date\nInput Data :%s\nExpected Result :0\nActual Result :%d\n", newUser.cardExpirationDate, getCardExpiryDate(&newUser));
}
void getCardPANTest(void) {
	printf("Tester Name: Mohamed Mamdouh\n");
	printf("Function Name : getCardExpiryDate\n");
	printf("Test Case 1 :Inserting A card PAN less than 16 characters\nInput Data :%s\nExpected Result :3\nActual Result :%d\n",newUser.primaryAccountNumber,getCardPAN(&newUser));
	printf("Test Case 2 :Inserting A card PAN more than 19 characters\nInput Data :%s\nExpected Result :3\nActual Result :%d\n",newUser.primaryAccountNumber,getCardPAN(&newUser));
	printf("Test Case 3 :Inserting A card PAN with a suitable number of characters\nInput Data :%s\nExpected Result :0\nActual Result :%d\n",newUser.primaryAccountNumber,getCardPAN(&newUser));
}

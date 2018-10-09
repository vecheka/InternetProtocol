/**
 * @author Vecheka Chhourn
 * @version 04/05/2018
 **/
 
/*
 * Logic: First, I read the address separately into four codes, then I converted each codes to binary forms. 
 * While doing the binary conversion, I also calculated the decimal value of the whole address by have a global 
 * variable to keep track of the binary position. From there, I re-used the same logic over again for all other 
 * addresses. 
 */ 
 
/*
 * I have tested my programm on Ubuntu 16.04 Desktop, and on an online C compiler for assurance. 
 * I have also learned how to use pointer, classes, and was able to apply the knowledge to this assignment.
 * 
*/
#include <stdio.h>
#include <math.h>

/* Default Bits .*/
int BITS = 8;

/* Binary index position */
unsigned int binaryPosition = 31;
/* Decimal value.*/
unsigned int decimalValue = 0;
/* Position Counter in Binary Form.*/
int positionCounter = 0;

/* functions declarations.*/
int convertToBinary(int number);
void doConversionOne();
void doConversionTwo();
void verifyCodes(int*, int*, int*, int*);
void getSubnetCodes(int*, int*, int*, int*, int);
void resetValue();
void printInfo(int, int, int, int);
int calculateSubnet(int remainder);
int calculateOneComplement(int number);
int getRoutingMaskCounts(int number);
int isValid(int code1, int code2, int code3, int code4, int routingNumber);

int main(void) {
	char userInput;
	do {
		
		int typeOfConversion;
		// read user's input
		printf("What type of conversion do you want?\n");
		printf("Enter 1 or IP-address and subnet, 2 for the host and network prefix: ");
		scanf("%d", &typeOfConversion);
		while (typeOfConversion != 1 && typeOfConversion != 2
	           || ((typeOfConversion == 1 || typeOfConversion == 2) && getchar() != '\n')) {
		   while (getchar() != '\n') {
		    continue;
		  }
		  printf("Please either enter 1 or 2: ");
		  scanf("%d", &typeOfConversion);
		}
		// do first type of conversion
		
		if(typeOfConversion == 1) {
			doConversionOne();
		} else if(typeOfConversion == 2) {
			doConversionTwo();	
		}
		
		
		printf("\n\n");
		printf("Enter r to repeat, q to quit: ");
		scanf(" %c", &userInput);
		
		while (userInput != 'r' && userInput != 'q'
		           || ((userInput == 'r' || userInput == 'q') && getchar() != '\n')) {
  		   while (getchar() != '\n') {
  		    continue;
  		  }
  		  printf("Please either enter r or q: ");
  		  scanf(" %c", &userInput);
  		}
		
		printf("\n-----------------------------------\n");
	} while (userInput != 'q');
	
	return 0;	
}


/*
 * Finding decimal value, and binary form of IP-address 
 * , subnet, network prefix, and host.
 *
 */
void doConversionOne() {
	int ipCode1, ipCode2, ipCode3, ipCode4, routingMask;
	char dotSlash;
	
	printf("\nEnter the message address: ");
	scanf("%d  %c %d  %c %d  %c %d  %c %d", &ipCode1, &dotSlash, &ipCode2, &dotSlash, &ipCode3, &dotSlash, &ipCode4
		  , &dotSlash, &routingMask);
	
	// while codes are not valid, invoke users to enter the value again
	while (!isValid(ipCode1, ipCode2, ipCode3, ipCode4, routingMask)) {
		printf("\nWrong address, try again: ");
		scanf("%d  %c %d  %c %d  %c %d  %c %d", &ipCode1, &dotSlash, &ipCode2, &dotSlash, &ipCode3, &dotSlash, &ipCode4
		  , &dotSlash, &routingMask);

	}	
	
	printf("\n");

	// finding IP-address' decimal and binary form
	printf("The IP-address is ");
	printInfo(ipCode1, ipCode2, ipCode3, ipCode4);
  // re-initialize the position back to 31, and decimal value to 0
	resetValue();
	printf("\n");
  
  
	// finding subnet mask' decimal and binary form
	int subnetCode1, subnetCode2, subnetCode3, subnetCode4;
	getSubnetCodes(&subnetCode1, &subnetCode2, &subnetCode3, &subnetCode4, routingMask);

	printf("The subnet mask is ");
	printInfo(subnetCode1, subnetCode2, subnetCode3, subnetCode4);
	printf("The subnet mask in dot decimal is %d.%d.%d.%d\n", subnetCode1, subnetCode2, subnetCode3, subnetCode4);
	resetValue();
	printf("\n");
	
	
	
	// calculating network prefix
	int networkCode1, networkCode2, networkCode3, networkCode4;
	networkCode1 = ipCode1 & subnetCode1;
	networkCode2 = ipCode2 & subnetCode2;
	networkCode3 = ipCode3 & subnetCode3;
	networkCode4 = ipCode4 & subnetCode4;
	
	printf("The newtwork prefix is ");
	printInfo(networkCode1, networkCode2, networkCode3, networkCode4);
	printf("The network prefix in dot decimal is %d.%d.%d.%d",networkCode1, networkCode2, networkCode3, networkCode4);
	resetValue();
   
 	printf("\n");

 
	// finding subnet's one's complement
	int subnetComp1, subnetComp2, subnetComp3, subnetComp4;
	subnetComp1 = calculateOneComplement(subnetCode1);
	subnetComp2 = calculateOneComplement(subnetCode2);
	subnetComp3 = calculateOneComplement(subnetCode3);
	subnetComp4 = calculateOneComplement(subnetCode4);

	printf("\n");
	
	// finding host part's decimal and binary form
	int hostCode1, hostCode2, hostCode3, hostCode4;
	hostCode1 = subnetComp1 & ipCode1;
	hostCode2 = subnetComp2 & ipCode2;
	hostCode3 = subnetComp3 & ipCode3;
	hostCode4 = subnetComp4 & ipCode4;
	printf("The host id is ");
	printInfo(hostCode1, hostCode2, hostCode3, hostCode4);
	printf("The host id in dot decimal is %d.%d.%d.%d",hostCode1, hostCode2, hostCode3, hostCode4);
	resetValue();
}

/*
 * Print the information of IP-Address, Subnet Mask, Host ID, and Network prefix.
 * @parm code1 first code in the address
 * @parm code2 second code in the address
 * @parm code3 third code in the address
 * @parm code4 fourth code in the address
 */
void printInfo( int code1, int code2, int code3, int code4) {
  int binaryCode1, binaryCode2, binaryCode3, binaryCode4;
  binaryCode1 = convertToBinary(code1);
	binaryCode2 = convertToBinary(code2);
	binaryCode3 = convertToBinary(code3);
	binaryCode4 = convertToBinary(code4);
	printf("%u in decimal and\n", decimalValue);
	printf("%.8d %.8d %.8d %.8d in binary\n", binaryCode1, binaryCode2, binaryCode3, binaryCode4);

}

/*
 * Get subnet codes by counting the routing masks.
 * @param *subnetCode1 pointer to the first subnet code
 * @param *subnetCode2 pointer to the second subnet code
 * @param *subnetCode3 pointer to the third subnet code
 * @param *subnetCode4 pointer to the fourth subnet code
 * @param routingMask number of 1s in binary form of the subnet mask
 */
void getSubnetCodes(int *subnetCode1, int *subnetCode2, int *subnetCode3, int *subnetCode4, int routingMask) {
  
  int bits = routingMask / BITS;
	if (routingMask % BITS == 0) {

		if (bits == 1){
			*subnetCode1 = calculateSubnet(BITS);
			*subnetCode2 = 0;
			*subnetCode3 = 0;
			*subnetCode4 = 0;
		} else if (bits == 2) {
			*subnetCode1 = calculateSubnet(BITS);
			*subnetCode2 = calculateSubnet(BITS);
			*subnetCode3 = 0;
			*subnetCode4 = 0;
		} else if (bits == 3) {
			*subnetCode1 = calculateSubnet(BITS);
			*subnetCode2 = calculateSubnet(BITS);
			*subnetCode3 = calculateSubnet(BITS);;
			*subnetCode4 = 0;
		} else if (bits == 4) {
			*subnetCode1 = calculateSubnet(BITS);
			*subnetCode2 = calculateSubnet(BITS);
			*subnetCode3 = calculateSubnet(BITS);
			*subnetCode4 = calculateSubnet(BITS);
		}

	} else {
		int remainder = routingMask % BITS;
		if (bits == 1){
			*subnetCode1 = calculateSubnet(BITS);
			*subnetCode2 = calculateSubnet(remainder);
			*subnetCode3 = 0;
			*subnetCode4 = 0;
		} else if (bits == 2) {
			*subnetCode1 = calculateSubnet(BITS);
			*subnetCode2 = calculateSubnet(BITS);
			*subnetCode3 = calculateSubnet(remainder);
			*subnetCode4 = 0;
		} else if (bits == 3) {
			*subnetCode1 = calculateSubnet(BITS);
			*subnetCode2 = calculateSubnet(BITS);
			*subnetCode3 = calculateSubnet(BITS);;
			*subnetCode4 = calculateSubnet(remainder);
		} 
	}
}

/*
 * Reset binaryPosition to 31, and decimalValue to 0.
 */
void resetValue() {
  binaryPosition = 31;
  decimalValue = 0;
}

/*
 * Verify host id and network prefix's address for validity. 
 * @param *code1 pointer to the first code in the address
 * @param *code2 pointer to the second code in the address
 * @param *code3 pointer to the third code in the address
 * @param *code4 pointer to the fourth code in the address
 */
void verifyCodes(int *code1, int *code2, int *code3, int *code4) {
  int temp1 = *code1, temp2 = *code2, temp3 = *code3, temp4 = *code4;
  char dot;
  while (!isValid(temp1, temp2, temp3, temp4, 0)) {
    printf("Wrong address, try again: ");
    scanf("%d  %c %d  %c %d  %c %d", &temp1, &dot, &temp2, &dot, &temp3, &dot, &temp4);
    *code1 = temp1;
    *code2 = temp2;
    *code3 = temp3;
    *code4 = temp4;
  }
  
}


/*
 * Finding IP-Address, and Subnet's
 */
void doConversionTwo() {
	int hostCode1, hostCode2, hostCode3, hostCode4;
	int networkCode1, networkCode2, networkCode3, networkCode4;
	char dot;
	printf("Enter the host: ");
	scanf("%d  %c %d  %c %d  %c %d", &hostCode1, &dot, &hostCode2, &dot,
		  &hostCode3, &dot, &hostCode4);
	
	verifyCodes(&hostCode1, &hostCode2, &hostCode3, &hostCode4);
	
	printf("Enter the network prefix: ");
	scanf("%d  %c %d  %c %d  %c %d", &networkCode1, &dot, &networkCode2, &dot,
		  &networkCode3, &dot, &networkCode4);
	verifyCodes(&networkCode1, &networkCode2, &networkCode3, &networkCode4);
	
	int ipCode1, ipCode2, ipCode3, ipCode4;
	ipCode1 = hostCode1 | networkCode1;
	ipCode2 = hostCode2 | networkCode2;
	ipCode3 = hostCode3 | networkCode3;
	ipCode4 = hostCode4 | networkCode4;
	
	// get subnet mask counts
	int subnetCounter = getRoutingMaskCounts(networkCode1) + getRoutingMaskCounts(networkCode2) +
		getRoutingMaskCounts(networkCode3) + getRoutingMaskCounts(networkCode4);
	
	// re initialize count positions to zero
	positionCounter = 0;
	printf("\nThe message is: %d.%d.%d.%d/%d", ipCode1, ipCode2, ipCode3, ipCode4
		   , subnetCounter);
	
}


/*
 * Count subnet mask's routing masks to determine leading 1's.
 * @param: number to convert to binary form
 */
int getRoutingMaskCounts(int number) {
	int k;
	int countOnes = 0;
	int i;
	for (i = 7; i >=0; i--) {
		k = number >> i;
		positionCounter++;
		if ((k & 1)) {
			countOnes += positionCounter;
			positionCounter = 0;
		} 
	
	}
	return countOnes;
}

/*
 * Determine if the address is valid.
 * Pre-conditons:
 * 	1. Codes are between 0-255
 * 	2. Routing number is between 0-32
 * @param: code1 first code in the address
 * @param: code2 second code in the address
 * @param: code3 third code in the address
 * @param: code4 fourth code in the address
 * @param: routingNumber number of 1s in the binary form of subnet mask
 * @return true if it is a valid address
 */
int isValid(int code1, int code2, int code3, int code4, int routingNumber) {
	return  ((code1 >= 0 && code1 <= 255) 
	   && (code2 >= 0 && code2 <= 255)
	   && (code3 >= 0 && code3 <= 255)
	   && (code4 >= 0 && code4 <= 255)
	   && (routingNumber >= 0 && routingNumber <= 32));
}




/*
 * Finding each subnet mask's decimal value.
 * @param: remainder to find decimal value.
 * @return: subnet mask binary form
 */
int calculateSubnet(int remainder) {
	int totalValue = 0;
	int i;
	for (i = 7; i >= BITS - remainder; i--) {
		totalValue += pow(2, i);	
	}
	
	return totalValue;
}

/*
 * Finding subnet's one's complement.
 * @param: number to covert
 * @return: value of one's complement in decimal
 */
int calculateOneComplement(int number){
	int totalValue = 0;
	int k, i;
	for (i = 7; i >=0; i--) {
		k = number >> i;
		
		
		if (!(k & 1)) {
			totalValue += pow(2, i);
		} 
	
	}
	return totalValue;
	
}

/* 
 * convert a number to binary number. 
 * @param: number to covert
 * @return: binary number
 */
int convertToBinary(int number) {

	
	int k, i;
	for (i = 7; i >=0; i--) {
		k = number >> i;
		
		// calculating the 32 bits decimal value of the address
		if (k & 1) {
			decimalValue += pow(2, binaryPosition);
		} 
		
		binaryPosition--;
	
	}
	
	
	int binaryNumber = 0, base = 1; 
	
	while (number > 0){
		int remainder = number % 2;
		binaryNumber = binaryNumber + remainder * base;
		base *= 10;
		number  = number / 2;
		
	} 
	
	
	return binaryNumber;
	
}
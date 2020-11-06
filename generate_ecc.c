#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h> 
#include <string.h>
#include <stdlib.h>

void generate_ecc();
void calculate_parity(long long int* data_array, int power, int size, int index);
int detect_parity(long long int* data_array, int power, int size, int index);
void to_binary(int n);

void generate_ecc()
{
	char data_entry[65];
	long long int original_data[64]; 
	long long int user_num;
	int i, temp, r, end_of_array, count = 0;
	
	// get user data as either decimal or binary
	while (1) {
		printf("Please enter an entry data. Would you like to enter data in binary or decimal? ");
		
		fgets(data_entry, sizeof(data_entry), stdin);
		for (i = 0; i < strlen(data_entry); i++)
			data_entry[i] = tolower(data_entry[i]);
		
		if (strncmp(data_entry, "decimal", 7) == 0){
			// get decimal data
			printf("Enter a positive decimal number: ");
			fgets(data_entry, sizeof(data_entry), stdin);

			// convert string to long long int
			user_num = strtoll(data_entry, NULL, 10);
			// create copy of number
			temp = user_num;
			
			// convert decimal input to binary and insert into array
			// and calculate total number of bits
			i = 0;
			while (user_num) {
				r = user_num % 2;
				original_data[i] = r;
				user_num /= 2;
				i++;
				count++;
			}

			// mark the end of array and assign original value
			// back to user_num
			end_of_array = i;
			user_num = temp;

			break;
		}

		else if (strncmp(data_entry, "binary", 6) == 0){

			while (1) {

				// get binary data
				printf("Enter a binary number: ");
				fgets(data_entry, sizeof(data_entry), stdin);

				// verify entry consists of 1's and 0's only
				for (i = 0; i < strlen(data_entry) - 1; i++) {
					if (data_entry[i] != '0' && data_entry[i] != '1') {
						printf("Please enter only 1's and 0's for binary data entry.\n");
						memset(data_entry, 0, sizeof(data_entry));
						break;
					}
					// count number of digits
					count++;		
				}
				if (i == strlen(data_entry) - 1)
					break;
			}	

			// convert string to long long int
			user_num = strtoll(data_entry, NULL, 10);
			printf("%lld\n", user_num);
			// create copy of number
			temp = user_num;

			for (i = 0; i < count; i++) {
				original_data[i] = user_num % 10;
				user_num /= 10;
			}

			// mark the end of array and assign original value
			// back to user_num
			end_of_array = i;
			user_num = temp;

			break;
		}

		else 
			printf("Invalid entry, please enter \"decimal\" or \"binary\".\n\n");
	}


	// calculate number of parity bits required for ECC using 
	// the Hamming formula
	int n = 2;
	int power, dataBits;
	while (1) {
		power = pow(2, n);
		dataBits = power - n - 1;
		if (dataBits < count) {
			n++;
		}
		else
			break;
	}

	// calculate size for ECC array that will include parity bits
	int total = count + n; 
	long long int ECC_Array[total];
	
	// walk through the ECC array assigning positions for parity bits
	// and giving them a value of 0 and then assigning all other positions
	// with the user data from the original_data array
	n = 0;
	for (i = 0; i < total; i++) {
		power = (pow(2,n)) - 1;
		if (i == power) {
			ECC_Array[i] = 0;
			n++;
		}
		else {
			int r = temp % 2;
			temp /= 2;
			ECC_Array[i] = r;
		}
	}

	// walk backwards through original data array (without the 
	// parity bits included) and print each element
	printf("Error Correction Code for data entry ");
	for (i = end_of_array - 1; i >= 0; i--) {
		printf("%lld", original_data[i]);
	}

	// walk backwards through new array with empty parity bit positions, 
	// find the positions of parity bits and then call function 
	// to calculate them
	n = 0;
	for (i = 0; i < total; i++) {
		power = (pow(2,n)) - 1;
		if (i == power) {
			calculate_parity(ECC_Array, power + 1, total, i);
			n++;
		}
	}

	// walk backwards through ECC array with parity bits included 
	// and print each element
	printf(": ");
	for (i = total - 1; i >= 0; i--) {
		printf("%lld", ECC_Array[i]);
	}
	printf(".\n\n");
}

void calculate_parity(long long int* data_array, int power, int size, int index)
{
	int sum = 0;
	int originalIndex = index;
	int count;

	while (1) {

		// if at any point the index equals the size of the 
		// array, exit the loop

		// walk through data_array starting at index of parity bit and 
		// add subsequent bits stopping when count equals the power
		// of the parity position
		for (count = 0; count < power; count++) {
			if (index == size) {
				break;
			}
			else {
				sum += data_array[index];
				index++;
			}
		}

		// skip n bits, n being the power of the parity position
		for (count = 0; count < power; count++) {
			if (index == size){
				break;
			}
			else {
				index++;
			}
		}

		if (index == size)
			break;
	}

	// calculate parity bit and insert value into the passed array
	if (sum % 2 == 1)
		data_array[originalIndex] = 1;
	else
		data_array[originalIndex] = 0;
}

int detect_parity(long long int* data_array, int power, int size, int index)
 {
	int sum = 0;
	int count;
	int ret;

	while (1) {
		// if at any point the index equals the size of the 
		// array, exit the loop

		// walk through data_array starting at index of parity bit and 
		// add subsequent bits stopping when count equals the power
		// of the parity position
		for (count = 0; count < power; count++) {
			if (index == size) {
				break;
			}
			else {
				sum += data_array[index];
				index++;
			}
		}

		// skip n bits, n being the power of the parity position
		for (count = 0; count < power; count++) {
			if (index == size) {
				break;
			}
			else {
				index++;
			}
		}

		if (index == size)
			break;
	}

	if (sum % 2 == 0) {
		ret = 0;
	}
	else {
		ret = 1;
	}

	return ret;
}

void to_binary(int n)
{
	int r = n % 2;
	if (n >= 2) {
		to_binary(n / 2);
	}
	putchar(r == 0 ? '0' : '1');
}

int main(void)
{
	generate_ecc();
	return 0;
}



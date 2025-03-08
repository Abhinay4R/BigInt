#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

//FOR THE LOOKUP FILE TO HANDLE AND STORE THE MULTIPLICATION RESULTS OVER TIME 
//AND MAKE THE CODE EXTREMELY EFFICIENT
#define LOOKUP_FILE "dynamic_lookup.txt"

//FOR DECIMAL NUMBER IMPLEMENTATION THESE MACROS ARE REQUIRED 
#define MAX_LENGTH 100

//FOR HEXADECIMAL NUMBER IMPLEMENTATION THESE ARE REQUIRED
#define HEX_SIZE 64
#define MAX_HEX_RESULT_SIZE 128
#define HEX_LOOKUP_SIZE 256
#define MAX_BINARY_LENGTH 1024

// Global lookup table for 2-digit hex multiplication 
// and hence making this faster than the regular N^2 algorithm
int hex_multiply_lookup[HEX_LOOKUP_SIZE][HEX_LOOKUP_SIZE];

// Initialize the lookup table with -1
void initialize_lookup_table() {
    for (int i = 0; i < HEX_LOOKUP_SIZE; i++) {
        for (int j = 0; j < HEX_LOOKUP_SIZE; j++) {
            hex_multiply_lookup[i][j] = -1;
        }
    }
}
// FOLLOWING FUCTIONS I WROTE FOR DECIMAL NUMBER IMPLEMENTATION
int is_the_input_character_a_digit(char c) {
    return (c >= '0' && c <= '9');
}

void pad_zeroes(char *num1, char *num2) {
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    int is_neg1 = (num1[0] == '-');
    int is_neg2 = (num2[0] == '-');

    char *ptr1 = is_neg1 ? num1 + 1 : num1;
    char *ptr2 = is_neg2 ? num2 + 1 : num2;

    int len1_adj = strlen(ptr1);
    int len2_adj = strlen(ptr2);
    int max_len = (len1_adj > len2_adj) ? len1_adj : len2_adj;

    char padded1[max_len + 2], padded2[max_len + 2];

    if (is_neg1) padded1[0] = '-';
    if (is_neg2) padded2[0] = '-';

    memset(padded1 + is_neg1, '0', max_len - len1_adj);
    memset(padded2 + is_neg2, '0', max_len - len2_adj);

    strcpy(padded1 + max_len - len1_adj + is_neg1, ptr1);
    strcpy(padded2 + max_len - len2_adj + is_neg2, ptr2);

    strcpy(num1, padded1);
    strcpy(num2, padded2);
}
// New function to pad numbers with zeros
void pad_numbers_to_equal_length(char *number1, char *number2, char *padded1, char *padded2) {
    int len1 = strlen(number1);
    int len2 = strlen(number2);
    int max_length = (len1 > len2) ? len1 : len2;
    
    // Initialize padded strings with zeros
    memset(padded1, '0', MAX_LENGTH);
    memset(padded2, '0', MAX_LENGTH);
    padded1[max_length] = '\0';
    padded2[max_length] = '\0';
    
    // Copy numbers to padded strings, aligned to the right
    strcpy(padded1 + (max_length - len1), number1);
    strcpy(padded2 + (max_length - len2), number2);
}

bool is_the_given_number_valid_input(const char *number) {
    if (number == NULL || strlen(number) == 0) {
        return false;
    }

    int length = strlen(number);

    // Check the first character
    if (number[0] != '-' && !is_the_input_character_a_digit(number[0])) {
        return false;
    }

    // Check the rest of the characters
    for (int i = 1; i < length; i++) {
        if (!is_the_input_character_a_digit(number[i])) {
            return false;
        }
    }

    return true;
}

void take_two_valid_numbers_as_input(char *number1, char *number2) {
    bool is_valid = false;

    while (!is_valid) {
        printf("Enter the first number: ");
        scanf("%s", number1);

        if (is_the_given_number_valid_input(number1)) {
            is_valid = true;
        } else {
            printf("Invalid input. Please re-enter a valid number.\n");
        }
    }

    is_valid = false; // Reset the flag for the second number

    while (!is_valid) {
        printf("Enter the second number: ");
        scanf("%s", number2);

        if (is_the_given_number_valid_input(number2)) {
            is_valid = true;
        } else {
            printf("Invalid input. Please enter a valid number.\n");
        }
    }
}

void take_the_10s_compliment_of_this_decimal_string_number(char *num, char *result) {
    int length = strlen(num);
    int carry = 1;

    for (int i = length - 1; i >= 0; i--) {
        if (num[i] < '0' || num[i] > '9') {
            printf("Invalid input\n");
            return;
        }

        int digit = '9' - num[i] + carry;
        if (digit == 10) {
            digit = 0;
            carry = 1;
        } else {
            carry = 0;
        }

        result[i] = digit + '0';
    }

    result[length] = '\0';
}

// Modified adding function to handle the padded strings properly
void adding_very_large_numbers_from_strings(char *number1, char *number2, char *result) {
    int length = strlen(number1);  // Both numbers should now have the same length
    int carry = 0;
    
    // Clear the result array
    memset(result, 0, MAX_LENGTH);
    
    // Perform addition from right to left
    for (int i = length - 1; i >= 0; i--) {
        int digit1 = number1[i] - '0';
        int digit2 = number2[i] - '0';
        int sum = digit1 + digit2 + carry;
        
        result[i] = (sum % 10) + '0';
        carry = sum / 10;
    }
    
    // Handle final carry
    if (carry) {
        memmove(result + 1, result, length + 1);
        result[0] = carry + '0';
    }
    
    // Remove leading zeros
    int start = 0;
    while (result[start] == '0' && start < strlen(result) - 1) {
        start++;
    }
    if (start > 0) {
        memmove(result, result + start, strlen(result) - start + 1);
    }
}


void subtracting_very_large_numbers_from_strings(char *number1, char *number2, char *result) {
    char complement[MAX_LENGTH];
    char intermediate_result[MAX_LENGTH];
    
    memset(result, 0, MAX_LENGTH);
    memset(complement, 0, MAX_LENGTH);
    memset(intermediate_result, 0, MAX_LENGTH);

    int is_negative = 0;
    if (strcmp(number1, number2) < 0) {
        is_negative = 1;
        char temp[MAX_LENGTH];
        strcpy(temp, number1);
        strcpy(number1, number2);
        strcpy(number2, temp);
    }
    
    take_the_10s_compliment_of_this_decimal_string_number(number2, complement);
    adding_very_large_numbers_from_strings(number1, complement, intermediate_result);

    int start_index = 0;
    while (intermediate_result[start_index] == '0') {
        start_index++;
    }

    if (start_index == strlen(intermediate_result)) {
        strcpy(result, "0");
    } else {
        strcpy(result, intermediate_result + start_index);
    }

    if (is_negative) {
        memmove(result + 1, result, strlen(result) + 1);
        result[0] = '-';
    }
}

void subtract_one_from_first_digit(char *number, bool n1, bool n2, bool s) {
    // Conditions (n1, n2, s): (1,1,0), (1,0,1), (0,1,1), (0,0,0)
    if (!((n1 && n2 && !s) || (n1 && !n2 && s) || (!n1 && n2 && s) || (!n1 && !n2 && !s))) {
        return; // If conditions don't match, do nothing
    }

    int is_negative = (number[0] == '-');
    int start_index = is_negative ? 1 : 0; // Start after the '-' for negative numbers

    if (number[start_index] > '0') { // Ensure we don't go below 0 
        number[start_index]--; // Subtract 1 
    }
}


// Modified wrapper function
void wrapper_function_to_handle_all_types_of_input_numbers(char *number1, char *number2, char op, char *result) {
    int is_number1_negative = (number1[0] == '-');
    int is_number2_negative = (number2[0] == '-');
    
    bool isn1p = (bool) !(is_number1_negative);
    bool isn2p = (bool) !(is_number2_negative);
    bool operation_done = false;
    if (op == '+') {
        operation_done = true;
    }
    
    // Skip past the negative signs if present
    char *num1_ptr = (is_number1_negative) ? number1 + 1 : number1;
    char *num2_ptr = (is_number2_negative) ? number2 + 1 : number2;
    
    // Create padded versions of the numbers
    char padded1[MAX_LENGTH], padded2[MAX_LENGTH];
    pad_numbers_to_equal_length(num1_ptr, num2_ptr, padded1, padded2);

    if (op == '+') {
        if (is_number1_negative == is_number2_negative) {
            adding_very_large_numbers_from_strings(padded1, padded2, result);
            if (is_number1_negative) {
                memmove(result + 1, result, strlen(result) + 1);
                result[0] = '-';
            }
        } else {
            if (is_number1_negative) {
                subtracting_very_large_numbers_from_strings(padded2, padded1, result);
            } else {
                subtracting_very_large_numbers_from_strings(padded1, padded2, result);
            }
        }
    } else if (op == '-') {
        if (is_number1_negative != is_number2_negative) {
            adding_very_large_numbers_from_strings(padded1, padded2, result);
            if (is_number1_negative) {
                memmove(result + 1, result, strlen(result) + 1);
                result[0] = '-';
            }
        } else {
            if (is_number1_negative) {
                subtracting_very_large_numbers_from_strings(padded2, padded1, result);
            } else {
                subtracting_very_large_numbers_from_strings(padded1, padded2, result);
            }
        }
    } else {
        printf("Invalid operation. Please use '+' or '-'.\n");
        return;
    }
    subtract_one_from_first_digit(result, isn1p, isn2p, operation_done);
}
void adding_very_large_numbers_from_strings_for_multiplication_call(char *number1, char *number2, char *result) {
    int length1 = strlen(number1);
    int length2 = strlen(number2);
    int max_length = (length1 > length2) ? length1 : length2;
    int carry = 0;
    
    // making sure that this result is neat 
    memset(result, 0, MAX_LENGTH);
    
    // Adjusting the first digit for handling special cases
    if (number1[0] == '0' || number2[0] == '0') {
        if (number1[0] == '0') {
            strcpy(result, number2);
            return;
        }
        if (number2[0] == '0') {
            strcpy(result, number1);
            return;
        }
    }
    
    // Normal addition logic
    for (int i = 0; i < max_length; i++) {
        int digit1 = (i < length1) ? (number1[length1 - 1 - i] - '0') : 0;
        int digit2 = (i < length2) ? (number2[length2 - 1 - i] - '0') : 0;
        
        int sum = digit1 + digit2 + carry;
        result[max_length - 1 - i] = (sum % 10) + '0';
        carry = sum / 10;
    }
    
    // Handle final carry
    if (carry) {
        memmove(result + 1, result, max_length + 1);
        result[0] = carry + '0';
    } else {
        // Remove leading zeros
        int start = 0;
        while (result[start] == '0' && start < max_length - 1) {
            start++;
        }
        if (start > 0) {
            memmove(result, result + start, max_length - start + 1);
        }
    }
}

// Function to multiply two very large numbers represented as strings
char* multiply_signed_decimal_numbers_input_in_form_of_strings(const char* a, const char* b) {
    if (strcmp(a, "0") == 0 || strcmp(b, "0") == 0) {
        return strdup("0");
    }

    bool negative = false;
    const char *num1 = a, *num2 = b;

    if (a[0] == '-') {
        negative = !negative;
        num1 = a + 1;
    }

    if (b[0] == '-') {
        negative = !negative;
        num2 = b + 1;
    }

    int len1 = strlen(num1), len2 = strlen(num2);
    char* result = strdup("0"); // Initialize result as "0"

    for (int i = len2 - 1; i >= 0; --i) {
        int digit1 = num2[i] - '0';
        int carry = 0;

        char temp_result[MAX_LENGTH + 2];
        memset(temp_result, 0, sizeof(temp_result));
        int temp_len = 0;

        // Append zeros for current position
        for (int j = 0; j < len2 - 1 - i; ++j) {
            temp_result[temp_len++] = '0';
        }

        for (int j = len1 - 1; j >= 0; --j) {
            int digit2 = num1[j] - '0';
            int product = digit1 * digit2 + carry;
            temp_result[temp_len++] = (product % 10) + '0';
            carry = product / 10;
        }

        if (carry) {
            temp_result[temp_len++] = carry + '0';
        }
        temp_result[temp_len] = '\0';

        // Reverse temp_result
        for (int k = 0; k < temp_len / 2; ++k) {
            char temp = temp_result[k];
            temp_result[k] = temp_result[temp_len - 1 - k];
            temp_result[temp_len - 1 - k] = temp;
        }

        // Add temp_result to the current result
        char new_result[MAX_LENGTH + 2];
        adding_very_large_numbers_from_strings_for_multiplication_call(result, temp_result, new_result);

        free(result);
        result = strdup(new_result);
    }

    if (negative) {
        char* temp = (char*)malloc(strlen(result) + 2);
        sprintf(temp, "-%s", result);
        free(result);
        result = temp;
    }

    return result;
}

// THESE FUNCTIONS I HAVE WRITTEN TO IMPLEMENT TO HEXADECIMAL 
// MULTIPLICATION USING RECURSION AND A LOOKUP TABLE OF SIZE 256x256 
// Function to check if a character is a valid hexadecimal digit (lowercase only)
bool is_the_input_hex_character_valid(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
}

// Function to validate a hexadecimal string
bool checking_validity_of_the_input_hexadecimal_number(const char *hex) {
    for (int i = 0; i < HEX_SIZE; i++) {
        if (!is_the_input_hex_character_valid(hex[i])) {
            return false;
        }
    }
    return hex[HEX_SIZE] == '\0';
}

// function to add hexadecimal number with leading zeros 
//to make it exactly match for the recursive algorithm
void add_extra_zeroes_infront_to_make_exact_size(char *hex) {
    int length = strlen(hex);
    if (length < HEX_SIZE) {
        char changed[HEX_SIZE + 1] = {0}; // Create a new array for the changed number
        int needed_change_length = HEX_SIZE - length;

        // Add leading zeros
        for (int i = 0; i < needed_change_length; i++) {
            changed[i] = '0';
        }

        // Copy the original number after the leading zeros
        strcpy(&changed[needed_change_length], hex);

        // Update the original string with the changed value
        strcpy(hex, changed);
    }
}

// Helper function to add two hexadecimal digits with a carry-in
// this takes Cin , D1 = DIGIT1 , D2 = DIGIT2 and returns S1 = SUM OF THESE NUMBERS
// and also returns Carryout
void add_hex_digits(char digit1, char digit2, char carry_in, char *sum, char *carry_out) {
    int value1 = (digit1 >= 'a') ? (digit1 - 'a' + 10) : (digit1 - '0');
    int value2 = (digit2 >= 'a') ? (digit2 - 'a' + 10) : (digit2 - '0');
    int carry = (carry_in >= 'a') ? (carry_in - 'a' + 10) : (carry_in - '0');

    int total = value1 + value2 + carry;
    *sum = (total % 16 < 10) ? (total % 16 + '0') : (total % 16 - 10 + 'a');
    *carry_out = (total / 16 < 10) ? (total / 16 + '0') : (total / 16 - 10 + 'a');
}

void adding_hexadecimal_numbers(const char *hex1, const char *hex2, char *result) {
    char carry = '0';
    int length1 = strlen(hex1);
    int length2 = strlen(hex2);
    int max_length = (length1 > length2) ? length1 : length2;

    char temp_result[MAX_HEX_RESULT_SIZE] = {0};
    int result_index = max_length;

    temp_result[result_index--] = '\0';

    for (int i = 0; i < max_length; i++) {
        char digit1 = (i < length1) ? hex1[length1 - 1 - i] : '0';
        char digit2 = (i < length2) ? hex2[length2 - 1 - i] : '0';

        char sum, carry_out;
        add_hex_digits(digit1, digit2, carry, &sum, &carry_out);

        temp_result[result_index--] = sum;
        carry = carry_out;
    }

    if (carry != '0') {
        temp_result[result_index--] = carry;
    }

    strcpy(result, &temp_result[result_index + 1]);
}

//  function to append zeros to the end of a hexadecimal string 
// to make the appropriate multiplication with 16 
// adding zero at end is equivalent to multiply with 16 in hexadecimal numbers
void append_zeros(char *hex, int count) {
    int length = strlen(hex);
    for (int i = 0; i < count; i++) {
        hex[length + i] = '0';
    }
    hex[length + count] = '\0';
}

// Convert a 2-digit hex string to decimal
// to make sure that we check the approprite location in the lookup table 
int hex_to_decimal(const char *hex) {
    int value = 0;
    for (int i = 0; hex[i] != '\0'; i++) {
        value = value * 16 + (hex[i] >= 'a' ? (hex[i] - 'a' + 10) : (hex[i] - '0'));
    }
    return value;
}

// recursive multiply_hex_numbers function that breaks the whole multiplication into parts
// and also to use lookup table to gain efficiency 
void multiply_hex_numbers(const char *hex1, const char *hex2, char *result) {
    // Base case: if anyone number is 0 
    if (strcmp(hex1, "0") == 0 || strcmp(hex2, "0") == 0) {
        strcpy(result, "0");
        return;
    }

    // Base case: if both numbers are single digits
    // this is done if those values have not been calculated yet and stored in the lookup table
    if (strlen(hex1) == 1 && strlen(hex2) == 1) 
    {
        int value1 = (hex1[0] >= 'a') ? (hex1[0] - 'a' + 10) : (hex1[0] - '0');
        int value2 = (hex2[0] >= 'a') ? (hex2[0] - 'a' + 10) : (hex2[0] - '0');
        int product = value1 * value2;
        sprintf(result, "%x", product);
        return;
    }

    if (strlen(hex1) == 2 && strlen(hex2) == 2) 
    {
     int index1 = hex_to_decimal(hex1);
     int index2 = hex_to_decimal(hex2);

     // Check lookup table
     if (hex_multiply_lookup[index1][index2] != -1) 
     {
     sprintf(result, "%x", hex_multiply_lookup[index1][index2]);
     return;
     }

     // Check file for the result
     FILE *file = fopen(LOOKUP_FILE, "r");
     if (file != NULL) {
        int file_index1, file_index2, product;
        while (fscanf(file, "%x %x %x", &file_index1, &file_index2, &product) == 3) {
            if (file_index1 == index1 && file_index2 == index2) {
                fclose(file);
                hex_multiply_lookup[index1][index2] = product;
                sprintf(result, "%x", product);
                return;
            }
        }
        fclose(file);
    }

    // If not found, compute and store in lookup table and file
    int product = hex_to_decimal(hex1) * hex_to_decimal(hex2);
    hex_multiply_lookup[index1][index2] = product;

    // Append the result to the file
    file = fopen(LOOKUP_FILE, "a");
    if (file != NULL) {
        fprintf(file, "%x %x %x\n", index1, index2, product);
        fclose(file);
    }

    sprintf(result, "%x", product);
    return;
}

    // Split the numbers into two halves
    int length1 = strlen(hex1);
    int length2 = strlen(hex2);
    int half_length1 = length1 / 2;
    int half_length2 = length2 / 2;

    char high1[HEX_SIZE + 1] = {0}, low1[HEX_SIZE + 1] = {0};
    char high2[HEX_SIZE + 1] = {0}, low2[HEX_SIZE + 1] = {0};
    strncpy(high1, hex1, length1 - half_length1);
    high1[length1 - half_length1] = '\0';
    strcpy(low1, &hex1[length1 - half_length1]);
    strncpy(high2, hex2, length2 - half_length2);
    high2[length2 - half_length2] = '\0';
    strcpy(low2, &hex2[length2 - half_length2]);

    char z0[MAX_HEX_RESULT_SIZE] = {0}, z1[MAX_HEX_RESULT_SIZE] = {0}, z2[MAX_HEX_RESULT_SIZE] = {0};
    char temp1[MAX_HEX_RESULT_SIZE] = {0}, temp2[MAX_HEX_RESULT_SIZE] = {0};

    // Recursive calls
    multiply_hex_numbers(low1, low2, z0);
    multiply_hex_numbers(low1, high2, temp1);
    multiply_hex_numbers(high1, low2, temp2);
    adding_hexadecimal_numbers(temp1, temp2, z1);
    multiply_hex_numbers(high1, high2, z2);

    // Combine the results
    append_zeros(z2, 2 * half_length1);
    append_zeros(z1, half_length1);

    adding_hexadecimal_numbers(z2, z1, temp1);
    adding_hexadecimal_numbers(temp1, z0, result);
}
// this below function is there to convert the final result 
// into more readable code by removing the unneccesary zeroes infront of this result
void format_multiplication_result(const char *result, char *formatted_result) {
    int length = strlen(result);
    int start_index = 0;

    // Find the first non-zero character
    while (start_index < length && result[start_index] == '0') {
        start_index++;
    }

    // If the entire result is zero
    if (start_index == length) {
        strcpy(formatted_result, "0");
        return;
    }

    // Copy the non-zero part
    strcpy(formatted_result, &result[start_index]);
}



bool is_valid_binary_number(const char *binary) {
    if (binary == NULL || strlen(binary) == 0) {
        return false;
    }

    for (int i = 0; i < strlen(binary); i++) {
        if (binary[i] != '0' && binary[i] != '1') {
            return false;
        }
    }

    return true;
}

void get_binary_numbers(char *binary1, char *binary2) {
    bool is_valid1 = false, is_valid2 = false;

    while (!is_valid1 || !is_valid2) {
        if (!is_valid1) {
            printf("Enter the first binary number: ");
            scanf("%s", binary1);

            if (is_valid_binary_number(binary1)) {
                is_valid1 = true;
            } else {
                printf("Invalid binary input. Please try again.\n");
            }
        }

        if (!is_valid2) {
            printf("Enter the second binary number: ");
            scanf("%s", binary2);

            if (is_valid_binary_number(binary2)) {
                is_valid2 = true;
            } else {
                printf("Invalid binary input. Please try again.\n");
            }
        }
    }
}

void binary_to_hexadecimal(const char *binary, char *hexadecimal) {
    int binary_length = strlen(binary);
    int hex_length = (binary_length + 3) / 4;
    int index = 0;

    memset(hexadecimal, 0, HEX_SIZE);

    for (int i = 0; i < hex_length; i++) {
        int start = binary_length - 4 * (i + 1);
        int end = binary_length - 4 * i;
        if (start < 0) start = 0;

        char group[5] = {0};
        strncpy(group, binary + start, end - start);
        
        int decimal_value = strtol(group, NULL, 2); // Convert binary group to decimal
        hexadecimal[hex_length - i - 1] = (decimal_value < 10) ? ('0' + decimal_value) : ('a' + decimal_value - 10);
    }
}

void hexadecimal_to_binary(const char *hexadecimal, char *binary) {
    int hex_length = strlen(hexadecimal);
    memset(binary, 0, MAX_HEX_RESULT_SIZE * 4);

    for (int i = 0; i < hex_length; i++) {
        int decimal_value = (hexadecimal[i] >= 'a') ? (hexadecimal[i] - 'a' + 10) : (hexadecimal[i] - '0');

        char group[5] = {0};
        for (int j = 0; j < 4; j++) {
            group[3 - j] = (decimal_value & (1 << j)) ? '1' : '0';
        }
        strcat(binary, group);
    }

    // Remove leading zeros
    int start_index = 0;
    while (binary[start_index] == '0' && start_index < strlen(binary) - 1) start_index++;

    if (start_index > 0) memmove(binary, binary + start_index, strlen(binary) - start_index + 1);
}
void get_valid_hexadecimal_number(char *hex, int number_index) {
    bool is_valid = false;

    while (!is_valid) {
        printf("\n Please enter a positive hexa deciamal input.(characters range {0-9,a-f}): ");
        // Read user input
        scanf("%64s", hex);
        add_extra_zeroes_infront_to_make_exact_size(hex);
        // Validate input
        if (checking_validity_of_the_input_hexadecimal_number(hex)) {
            is_valid = true;
        } else {
            printf("Invalid hexadecimal input. Please try again with lowercase hexadecimal digits (0-9, a-f).\n");
        }
    }
}

int main() {
    // Initialize the lookup table from the file
    FILE *file = fopen(LOOKUP_FILE, "r");
    if (file != NULL) {
        int index1, index2, product;
        while (fscanf(file, "%x %x %x", &index1, &index2, &product) == 3) {
            hex_multiply_lookup[index1][index2] = product;
        }
        fclose(file);
    } else {
        // If the file doesn't exist, initialize the lookup table with -1
        initialize_lookup_table();
    }

    char number1[MAX_LENGTH], number2[MAX_LENGTH], result[MAX_LENGTH], op;
    char choice;
    int sub_choice ;

    do {
        printf("\n----- MENU -----\n");
        printf("1. Add two numbers\n");
        printf("2. Subtract two numbers\n");
        printf("3. Multiply two numbers\n");
        printf("4. Exit\n");
        printf("Enter your choice (1-4): ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                printf("PLEASE INPUT THE STRINGS WITH SAME NUMBER OF LENGTH   \n") ;
                printf("If any of the numbers is zero, please input wth '-' sign \n") ;
                take_two_valid_numbers_as_input(number1, number2);
                wrapper_function_to_handle_all_types_of_input_numbers(number1, number2, '+', result);
                printf("Sum of the two numbers: %s\n\n", result);
                break;

            case '2':
                printf("PLEASE INPUT THE STRINGS WITH SAME NUMBER OF LENGTH \n")   ;
                printf("If any of the numbers is zero, please input without sign\n") ;
                take_two_valid_numbers_as_input(number1, number2);
                wrapper_function_to_handle_all_types_of_input_numbers(number1, number2, '-', result);
                printf("Difference of the two numbers: %s\n\n", result);
                break;

            case '3':
                do {

                    printf("\nThis program supports three types of multiplications:\n");
                    printf("1. Multiply Two  decimal numbers \n");
                    printf("2. Multiply Two positive hexadecimal numbers\n");
                    printf("3. Multiply Two binary numbers\n");
                    printf("4. Exit this section...\n");
                    printf("Please enter your choice (1 to 4 only supported): ");
                    scanf("%d", &sub_choice);
                    switch (sub_choice) {
                        case 1: {
                            char* product;
                            take_two_valid_numbers_as_input(number1, number2);
                            product = multiply_signed_decimal_numbers_input_in_form_of_strings(number1, number2);
                            printf("Product of the two numbers: %s\n\n", product);
                            free(product);
                            break;
                        }
                        case 2: {
                            char hex1[HEX_SIZE + 1], hex2[HEX_SIZE + 1];
                            char result[MAX_HEX_RESULT_SIZE] = {0};
                            char formatted_result[MAX_HEX_RESULT_SIZE] = {0};

                            printf("\n\n--- Hexadecimal Multiplication ---\n");
                            printf("Instructions:\n");
                            printf("1. This function accepts only positive hexadecimal numbers (no signs allowed).\n");
                            printf("2. Valid characters for hexadecimal input are: {0,1,2,3,4,5,6,7,8,9,a,b,c,d,e,f}.\n");
                            printf("3. Uppercase letters are not supported; please use lowercase only.\n");
                            printf("4. Each number should be up to 64 characters long.\n\n");

                            get_valid_hexadecimal_number(hex1, 1);
                            get_valid_hexadecimal_number(hex2, 2);

                            add_extra_zeroes_infront_to_make_exact_size(hex1);
                            add_extra_zeroes_infront_to_make_exact_size(hex2);

                            multiply_hex_numbers(hex1, hex2, result);
                            format_multiplication_result(result, formatted_result);
                            printf("\nProduct of the hexadecimal numbers: %s\n\n", formatted_result);

                            break;
                        }

                        case 3: {
                            char binary1[MAX_BINARY_LENGTH], binary2[MAX_BINARY_LENGTH];
                            char hex1[HEX_SIZE + 1], hex2[HEX_SIZE + 1];
                            char result_hex[MAX_HEX_RESULT_SIZE], result_binary[MAX_HEX_RESULT_SIZE * 4];

                            get_binary_numbers(binary1, binary2);
                            binary_to_hexadecimal(binary1, hex1);
                            binary_to_hexadecimal(binary2, hex2);

                            add_extra_zeroes_infront_to_make_exact_size(hex1);
                            add_extra_zeroes_infront_to_make_exact_size(hex2);
                            multiply_hex_numbers(hex1, hex2, result_hex);
                            hexadecimal_to_binary(result_hex, result_binary);

                            printf("Product of the binary numbers: %s\n", result_binary);
                            break;
                        }
                        case 4:
                            printf("Exiting multiplication...\n");
                            break;
                        default:
                            printf("Invalid input. Please try again...\n");
                            break;
                    }
                } while (sub_choice != 4);
                break;

            case '4':
                printf("Exiting program\n");
                break;

            default:
                printf("Invalid input. Try Again...\n");
        }
    } while (choice != '4');

    return 0;
}
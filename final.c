//convert unicode u+ into binary
// how many bytes: count from the right how many bit are till the first one and then you will know how many bytes you need

// we are coming out with a new hex that has been reformatted to include the byte specifications
//code point is u+ the hex is the utf 8

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

char* hex_bin_converter(const char* hex) {
    int binaryLength = 0;
    while (hex[binaryLength] != '\0') {
        binaryLength++;
    }
    binaryLength *= 8;

    char *binaryString = (char *) malloc(binaryLength + 1); // +1 for the null terminator

    if (binaryString == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    int index = 0;
    for (int i = 0; hex[i] != '\0'; i++) {
        for (int j = 7; j >= 0; j--) {
            binaryString[index++] = ((hex[i] >> j) & 1) + '0';
        }
        //binaryString[index++] = ' ';
    }
    binaryString[index] = '\0';

    return binaryString;
}


char* binary_to_hex(const char binary[]) {
    int bin_len = 0;
    int index = 0;
    int currentHexDigit = 0;
    int bitPosition = 0;

    //get the bin len
    while (binary[bin_len] != '\0') {
        bin_len++;
    }

    //set aside mem for the hex string and null term
    char *hexString = (char *) malloc(((bin_len + 3) / 4) + 1);
    while (bitPosition < bin_len) {
        currentHexDigit = 0;
        for (int i = 0; i < 4; i++) {
            currentHexDigit = (currentHexDigit << 1) | (binary[bitPosition++] - '0');
        }

        if (currentHexDigit < 10) {
            hexString[index++] = currentHexDigit + '0';
        } else {
            hexString[index++] = currentHexDigit - 10 + 'A';
        }
    }
    hexString[index] = '\0';
    return hexString;
}




//Validates that the input binary is a valid UTF8 encoded binary. if valid returns 1 .
//Valid if: 1 byte- 0???????
//          2 byte- 110????? 10??????
//          3 byte- 1110???? 10?????? 10??????
//          4 byte- 11110??? 10?????? 10?????? 10??????
int my_utf8_check(char *string) {
    char* bin_string = hex_bin_converter(string);
    int index = 0;

    //if the string ends in the middle of expected bit - not valid
    if (bin_string[index] == '\0' || bin_string[index + 1] == '\0' || bin_string[index + 2] == '\0' || bin_string[index + 3] == '\0' ||
            bin_string[index + 4] == '\0' || bin_string[index + 5] == '\0' || bin_string[index + 6] == '\0' || bin_string[index + 7] == '\0') {
        printf("Not valid UTF-8: invalid amount of bits\n");
        return 0;
    }

    //In order to check if the code is valid UTF8 it must follow the set patters according to the amount of bits
    //checking each of the possible bit combos and if the string always follows one of the four bit options then its valid
    while (bin_string[index] != '\0') {
        //1 byte
        if (bin_string[index] == '0'){
            index += 8;

        }
            // 2 bytes
        else if (bin_string[index] == '1' && bin_string[index + 1] == '1' && bin_string[index + 2] == '0'
                 && bin_string[index + 8] == '1' && bin_string[index + 9] == '0') {
            index += 16;

        }
            // 3 bytes
        else if (bin_string[index] == '1' && bin_string[index + 1] == '1' && bin_string[index + 2] == '1' && bin_string[index + 3] == '0'
                 && bin_string[index + 8] == '1' && bin_string[index + 9] == '0'
                 && bin_string[index + 16] == '1' && bin_string[index + 17] == '0') {
            index += 24;

        }
            //4 bytes
        else if (bin_string[index] == '1' && bin_string[index + 1] == '1' && bin_string[index + 2] == '1' && bin_string[index + 3] == '1' &&
                bin_string[index + 4] == '0'
                 && bin_string[index + 8] == '1' && bin_string[index + 9] == '0'
                 && bin_string[index + 16] == '1' && bin_string[index + 17] == '0'
                 && bin_string[index + 24] == '1' && bin_string[index + 25] == '0') {
            index += 32;

        }

        else {
            printf("\nNot a valid UTF-8: invalid bit sequence\n");
            return 0;
        }
    }

    return 1;
}

// Returns the number of characters in the string.
int my_utf8_strlen(unsigned char *string) {
    char *bin_string = hex_bin_converter((unsigned char*)string);
    int len = 0;
    int index = 0;

    if (my_utf8_check((char *)string)) {
        while (bin_string[index] != '\0') {

            if (bin_string[index] == '0') {
                index += 8;
                len += 1;
                printf("1 byte letter\n");\

            }
                // 2 bytes
            else if (bin_string[index] == '1' && bin_string[index + 1] == '1' && bin_string[index + 2] == '0'
                     && bin_string[index + 8] == '1' && bin_string[index + 9] == '0') {
                index += 16;
                len += 1;
                printf("2 byte letter\n");


            }
                // 3 bytes
            else if (bin_string[index] == '1' && bin_string[index + 1] == '1' && bin_string[index + 2] == '1' &&
                    bin_string[index + 3] == '0'
                     && bin_string[index + 8] == '1' && bin_string[index + 9] == '0'
                     && bin_string[index + 16] == '1' && bin_string[index + 17] == '0') {
                index += 24;
                len += 1;
                printf("3 byte letter\n");

            }
                // 4 bytes
            else if (bin_string[index] == '1' && bin_string[index + 1] == '1' && bin_string[index + 2] == '1' &&
                    bin_string[index + 3] == '1' &&
                     bin_string[index + 4] == '0'
                     && bin_string[index + 8] == '1' && bin_string[index + 9] == '0'
                     && bin_string[index + 16] == '1' && bin_string[index + 17] == '0'
                     && bin_string[index + 24] == '1' && bin_string[index + 25] == '0') {
                index += 32;
                len += 1;
                printf("4 byte letter\n");

            }
            else{
                break;
            }
        }

        // Print the length of the UTF-8 string
        //printf("%d\n", len);
        return len;
    }

    // Handle the case where the UTF-8 string is invalid
    printf("error in strlen");
    return -1;
}


//Encoding a UTF8 binary, taking as input an ASCII binary, with UTF8 characters encoded using the “U+” notation, and returns a UTF8 encoded binary
int my_utf8_encode(char *input, char *output) {
    int index = 0;
    while (input[index] != '\0') {
        if (input[index] == '\\' && input[index + 1] == 'u') {
            index += 2;
            int start = index;

            while (input[index] != '\\' && input[index + 1] != 'u' && input[index] != '\0') {
                index++;
            }

            int length = index - start;

            char *substring = (char *) malloc(length + 1);
            for (int i = 0; i < length; i++) {
                substring[i] = input[start + i];
            }
            substring[length] = '\0';

            int intValue;

            if (input[index] <= 0x007F) {
                //1 byte
                output[0] = output[0] + (char)(intValue & 0xFF);
                output[1] = '\0';
            } else if (input[index] >= 0x0080 && input[index] <= 0x07FF) {
                //2byes
                output[0] = output[0] + (char)(0xC0 | (intValue >> 6));
                output[1] = output[1] + (char)(0x80 | (intValue & 0x3F));
                output[2] = '\0';
            } else if (input[index] >= 0x0800 && input[index] <= 0xFFFF) {
                //3bytes
                output[0] = output[0] + (char)(0xE0 | (intValue >> 12));
                output[1] = output[1] + (char)(0x80 | ((intValue >> 6) & 0x3F));
                output[2] = output[2] + (char)(0x80 | (intValue & 0x3F));
                output[3] = '\0';
            } else if (input[index] >= 0x10000 && input[index] <= 0x10FFFF) {
                //4byes
                output[0] = output[0] + (char)(0xF0 | (intValue >> 18));
                output[1] = output[1] + (char)(0x80 | ((intValue >> 12) & 0x3F));
                output[2] = output[2] + (char)(0x80 | ((intValue >> 6) & 0x3F));
                output[3] = output[3] + (char)(0x80 | (intValue & 0x3F));
                output[4] = '\0';
            }

            index++;
        }
    }
}


//check which byte patter in is and then chop of the added stuff at teh begiing of each byte
// convert back to hex
//Takes a UTF8 encoded binary, and returns a binary, with ASCII representation where possible, and UTF8 character representation for non-ASCII characters.
int my_utf8_decode(char *input, char *output) {
    char* binary = hex_bin_converter(input);
    int index = 0;

    while (binary[index] != '\0') {
        //1 byte
        if (binary[index] == '0'){
            return

            index += 8;

        }
            // 2 bytes
        else if (binary[index] == '1' && binary[index + 1] == '1' && binary[index + 2] == '0'
                 && binary[index + 8] == '1' && binary[index + 9] == '0') {

            index += 16;

        }
            // 3 bytes
        else if (binary[index] == '1' && binary[index + 1] == '1' && binary[index + 2] == '1' && binary[index + 3] == '0'
                 && binary[index + 8] == '1' && binary[index + 9] == '0'
                 && binary[index + 16] == '1' && binary[index + 17] == '0') {

            index += 24;

        }
            //4 bytes
        else if (binary[index] == '1' && binary[index + 1] == '1' && binary[index + 2] == '1' && binary[index + 3] == '1' &&
                 binary[index + 4] == '0'
                 && binary[index + 8] == '1' && binary[index + 9] == '0'
                 && binary[index + 16] == '1' && binary[index + 17] == '0'
                 && binary[index + 24] == '1' && binary[index + 25] == '0') {

            index += 32;

        }

    }
    return 0;
}
//Returns the UTF8 encoded character at the location specified.
//If the input string is improperly encoded, this function should return NULL to indicate an error.
char *my_utf8_charat(unsigned char *string, int index) {
    char *bin_string = hex_bin_converter((unsigned char *) string);
    int char_index = 0;
    int bin_index = 0;
    int byte_amount = 0;

    if (my_utf8_check((char *) string)) {
        while (bin_string[bin_index] != '\0') {
            if (bin_string[bin_index] == '0') {
                bin_index += 8;
                char_index += 1;
                byte_amount = 1;
                if (char_index == index) {
                    break;
                }
            }
                // 2 bytes
            else if (bin_string[bin_index] == '1' && bin_string[bin_index + 1] == '1' &&
                     bin_string[bin_index + 2] == '0'
                     && bin_string[bin_index + 8] == '1' && bin_string[bin_index + 9] == '0') {
                bin_index += 16;
                char_index += 1;
                byte_amount = 2;
                if (char_index == index) {
                    break;
                }
            }
                // 3 bytes
            else if (bin_string[bin_index] == '1' && bin_string[bin_index + 1] == '1' &&
                     bin_string[bin_index + 2] == '1' &&
                     bin_string[bin_index + 3] == '0'
                     && bin_string[bin_index + 8] == '1' && bin_string[bin_index + 9] == '0'
                     && bin_string[bin_index + 16] == '1' && bin_string[bin_index + 17] == '0') {
                bin_index += 24;
                char_index += 1;
                byte_amount = 3;
                if (char_index == index) {
                    break;
                }

            }
                // 4 bytes
            else if (bin_string[bin_index] == '1' && bin_string[bin_index + 1] == '1' &&
                     bin_string[bin_index + 2] == '1' &&
                     bin_string[bin_index + 3] == '1' &&
                     bin_string[bin_index + 4] == '0'
                     && bin_string[bin_index + 8] == '1' && bin_string[bin_index + 9] == '0'
                     && bin_string[bin_index + 16] == '1' && bin_string[bin_index + 17] == '0'
                     && bin_string[bin_index + 24] == '1' && bin_string[bin_index + 25] == '0') {
                bin_index += 32;
                char_index += 1;
                byte_amount = 4;
                if (char_index == index) {
                    break;
                }
            }
        }

        int length = 2 * byte_amount;
        int start_index = bin_index - (8 * byte_amount);
        int res_index = 0;
        //allocate memory for the UTF-8 character and null
        char *result = (char *) malloc(length + 1);

        for (int i = start_index; i < bin_index; i++) {
             ;
            result[res_index++] = bin_string[i];
        }

        result[length] = '\0';

        return binary_to_hex(result);


    }
    printf("Invalid UTF-8 string\n");
    return NULL;
}

int main () {
    char utf8[] = {0xD7, 0x90, 0xD7, 0xA8, 0xD7, 0x99, 0xD7, 0x94, 0xE0, 0xA4, 0xB9, };


    //Test cconverter
    char *binary = hex_bin_converter(utf8);
    printf("\nHex to Binary Converter\nhex input: %s\nbinary output: %s\n", utf8, binary);

    //Test check function
    printf("\nCheck UTF-8 Function:\n");
    if (my_utf8_check(utf8)) {
        printf("Valid UTF-8 string\n");
        // Your code for a valid UTF-8 string
    } else {
        printf("Invalid UTF-8 string\n");
        // Your code for an invalid UTF-8 string
    }

    //Test str length
    printf("\nStrlen Function:\n");
    int strlen = my_utf8_strlen(utf8);
    if (strlen != -1) {
        printf("String length: %d\n", strlen);
    } else {
        printf("Error in strlen\n");
    }

    //Test charat
    printf("\ncharat function:\n");
    int index = 2;
    char *charc = my_utf8_charat(utf8, index);
    printf("index: %d\nchar: %s", index, charc);

    //Test encode
//    char* output;
//    my_utf8_encode(newstr, output);
//    printf("Encode function:\ninput:%s\noutput:%s\n", newstr, output );
//
//    //Test decode
//    my_utf8_decode(newstr, output);
//    printf("decode function:\ninput:%s\noutput:%s\n", newstr, output );
//
}
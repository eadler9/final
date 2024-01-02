//convert unicode u+ into binary
// how many bytes: count from the right how many bit are till the first one and then you will know how many bytes you need

// we are coming out with a new hex that has been reformatted to include the byte specifications
//code point is u+ the hex is the utf 8

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

char* hex_bin_converter(const char* hex) {
    int index = 0;
    int result_index = 0;
    char *result = (char *) malloc(100);

    // if in the /u form skip the /u and convert from hex to binary
    while (hex[index]) {
        if (hex[index] == 'u' || hex[index] == '\\') {
            index++;
        }
        switch (hex[index]) {
            case '0':
                result[result_index++] = '0';
                result[result_index++] = '0';
                result[result_index++] = '0';
                result[result_index++] = '0';
                break;
            case '1':
                result[result_index++] = '0';
                result[result_index++] = '0';
                result[result_index++] = '0';
                result[result_index++] = '1';
                break;
            case '2':
                result[result_index++] = '0';
                result[result_index++] = '0';
                result[result_index++] = '1';
                result[result_index++] = '0';
                break;
            case '3':
                result[result_index++] = '0';
                result[result_index++] = '0';
                result[result_index++] = '1';
                result[result_index++] = '1';
                break;
            case '4':
                result[result_index++] = '0';
                result[result_index++] = '1';
                result[result_index++] = '0';
                result[result_index++] = '0';
                break;
            case '5':
                result[result_index++] = '0';
                result[result_index++] = '1';
                result[result_index++] = '0';
                result[result_index++] = '1';
                break;
            case '6':
                result[result_index++] = '0';
                result[result_index++] = '1';
                result[result_index++] = '1';
                result[result_index++] = '0';
                break;
            case '7':
                result[result_index++] = '0';
                result[result_index++] = '1';
                result[result_index++] = '1';
                result[result_index++] = '1';
                break;
            case '8':
                result[result_index++] = '1';
                result[result_index++] = '0';
                result[result_index++] = '0';
                result[result_index++] = '0';
                break;
            case '9':
                result[result_index++] = '1';
                result[result_index++] = '0';
                result[result_index++] = '0';
                result[result_index++] = '1';
                break;
            case 'A':
                result[result_index++] = '1';
                result[result_index++] = '0';
                result[result_index++] = '1';
                result[result_index++] = '0';
                break;
            case 'B':
                result[result_index++] = '1';
                result[result_index++] = '0';
                result[result_index++] = '1';
                result[result_index++] = '1';
                break;
            case 'C':
                result[result_index++] = '1';
                result[result_index++] = '1';
                result[result_index++] = '0';
                result[result_index++] = '0';
                break;
            case 'D':
                result[result_index++] = '1';
                result[result_index++] = '1';
                result[result_index++] = '0';
                result[result_index++] = '1';
                break;
            case 'E':
                result[result_index++] = '1';
                result[result_index++] = '1';
                result[result_index++] = '1';
                result[result_index++] = '0';
                break;
            case 'F':
                result[result_index++] = '1';
                result[result_index++] = '1';
                result[result_index++] = '1';
                result[result_index++] = '1';
                break;
        }


        index++;
    }

    result[result_index] = '\0';
    return result;
}

char* bin_to_hex_converter(const char* binary) {
    int index = 0;
    int result_index = 0;
    char* result = (char*)malloc(100); // Adjust the size based on your needs

    while (binary[index]) {
        char chunk[5] = { binary[index], binary[index + 1], binary[index + 2], binary[index + 3], '\0' };
        unsigned int decimal = strtol(chunk, NULL, 2);

        if (decimal < 10) {
            result[result_index++] = '0' + decimal;
        }
        else {
            result[result_index++] = 'A' + (decimal - 10);
        }

        printf("Processing chunk: %s (Decimal: %u)\n", chunk, decimal);
        printf("Intermediate result: %.*s\n", result_index, result);
        index += 4;
    }

    result[result_index] = '\0';
    return result;
}

//Validates that the input binary is a valid UTF8 encoded binary. if valid returns 1 .
//Valid if: 1 byte- 0???????
//          2 byte- 110????? 10??????
//          3 byte- 1110???? 10?????? 10??????
//          4 byte- 11110??? 10?????? 10?????? 10??????
int my_utf8_check(char *string) {
    char bin_string[];
    bin_string = hex_bin_converter(string);
    int index = 0;

    //if the string ends in the middle of expected bit - not valid
    if (string[index] == '\0' || string[index + 1] == '\0' || string[index + 2] == '\0' || string[index + 3] == '\0' ||
        string[index + 4] == '\0' || string[index + 5] == '\0' || string[index + 6] == '\0' || string[index + 7] == '\0') {
        printf("Not valid UTF-8: invalid amount of bits\n");
        return 0;
    }

    //In order to check if the code is valid UTF8 it must follow the set patters according to the amount of bits
    //checking each of the possible bit combos and if the string always follows one of the four bit options then its valid
    while (string[index] != '\0') {
        //1 byte
        if (string[index] == '0'){
            index += 8;

        }
            // 2 bytes
        else if (string[index] == '1' && string[index + 1] == '1' && string[index + 2] == '0'
                 && string[index + 8] == '1' && string[index + 9] == '0') {
            index += 16;

        }
            // 3 bytes
        else if (string[index] == '1' && string[index + 1] == '1' && string[index + 2] == '1' && string[index + 3] == '0'
                 && string[index + 8] == '1' && string[index + 9] == '0'
                 && string[index + 16] == '1' && string[index + 17] == '0') {
            index += 24;

        }
            //4 bytes
        else if (string[index] == '1' && string[index + 1] == '1' && string[index + 2] == '1' && string[index + 3] == '1' &&
                 string[index + 4] == '0'
                 && string[index + 8] == '1' && string[index + 9] == '0'
                 && string[index + 16] == '1' && string[index + 17] == '0'
                 && string[index + 24] == '1' && string[index + 25] == '0') {
            index += 32;

        }

        else {
            printf("\nNot a valid UTF-8: invalid bit sequence\n");
            return 0;
        }
    }

    printf("\nValid UTF-8 string yay!\n");
    return 1;
}

// Returns the number of characters in the string.
int my_utf8_strlen(unsigned char *string) {
    char *bin_string = hex_bin_converter((char*)string);
    int len = 0;
    int index = 0;

    if (my_utf8_check((char *)string)) {
        while (*bin_string != '\0') {

            if (bin_string[index] == '0') {
                index += 8;
                len += 1;

            }
                // 2 bytes
            else if (bin_string[index] == '1' && bin_string[index + 1] == '1' && bin_string[index + 2] == '0'
                     && bin_string[index + 8] == '1' && bin_string[index + 9] == '0') {
                index += 16;
                len += 1;

            }
                // 3 bytes
            else if (bin_string[index] == '1' && bin_string[index + 1] == '1' && bin_string[index + 2] == '1' &&
                    bin_string[index + 3] == '0'
                     && bin_string[index + 8] == '1' && bin_string[index + 9] == '0'
                     && bin_string[index + 16] == '1' && bin_string[index + 17] == '0') {
                index += 24;
                len += 1;

            }
                // 4 bytes
            else if (bin_string[index] == '1' && bin_string[index + 1] == '1' && bin_string[index + 2] == '1' &&
                    bin_string[index + 3] == '1' &&
                     string[index + 4] == '0'
                     && bin_string[index + 8] == '1' && bin_string[index + 9] == '0'
                     && bin_string[index + 16] == '1' && bin_string[index + 17] == '0'
                     && bin_string[index + 24] == '1' && bin_string[index + 25] == '0') {
                index += 32;
                len += 1;

            }

            index++;
        }

        // Print the length of the UTF-8 string
        printf("The length of the UTF-8 string is %d\n", len);
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

int main (){
    //char* letters = "hello world";
    //char* originalCP = "\\u05D0\\u05DB\\u05D2"; //alef, bais, gimmel
    //char* newstr = "\\u0418\\u0939\\u20AC"; //D0 98 E0 A4 B9 E2 82 AC
    char utf8[] = "C2A324";
    char input[] = "F2";
    //int valid = my_utf8_check(binary);

    //check converter
    char binary[];
    binary = hex_bin_converter(utf8);
    printf("\nhexBinConverter:\nhex input:%s\nbinary output:%s\n", utf8, binary);

    //Test check function
    printf("Check function: %d\n", my_utf8_check(utf8));

    //Test str length
    int strlen = my_utf8_strlen(utf8);
    printf("Str length:\ninput:%s\noutput:%d\n", utf8, strlen);

    //Test encode
    char* output;
    my_utf8_encode(newstr, output);
    printf("Encode function:\ninput:%s\noutput:%s\n", newstr, output );

    //Test decode
    my_utf8_decode(newstr, output);
    printf("decode function:\ninput:%s\noutput:%s\n", newstr, output );



}

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

int my_utf8_strcmp(unsigned char *string1, unsigned char *string2);

char* utf8_hex_bin_converter(const char* hex) {
    int binaryLength = 0;
    while (hex[binaryLength] != 0x0) {
        binaryLength++;
    }
    binaryLength *= 8;

    char *binaryString = (char *) malloc(binaryLength + 1); // +1 for the null terminator


    int index = 0;
    for (int i = 0; hex[i] != 0x0; i++) {
        for (int j = 7; j >= 0; j--) {
            binaryString[index++] = ((hex[i] >> j) & 1) + '0';
        }
        //binaryString[index++] = ' ';
    }
    binaryString[index] = '\0';

    return binaryString;
}

int test_hextobin(unsigned char *hex, unsigned char *expected){
    char *actual = utf8_hex_bin_converter(hex);
    printf("%s, String: %s, Expected bin: %s, Actual bin: %s\n", (my_utf8_strcmp(expected, actual) == 0 ? "PASSED" : "FAILED"),
          hex, expected, actual);
}

void testall_hextobin() {
    unsigned char str1[] = {0xD7, 0x90, 0xD7, 0xA8, 0xD7, 0x99, 0xD7, 0x94, 0xE0, 0xA4, 0xB9, 0xF0, 0x9F, 0x98,
                            0x82,0x0};
    unsigned char str2[] = {0xD7, 0x90, 0xD7, 0x92, 0x0};
    unsigned char str3[] = {0xD7, 0x9, 0xD7, 0x92, 0x0};
    unsigned char *str4 = "h";

    test_hextobin(str1, "110101111001000011010111101010001101011110011001110101111001010011100000101001001011100111110000100111111001100010000010");
    test_hextobin(str2,  "11010111100100001101011110010010");
    test_hextobin(str4, "01101000");

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
    hexString[index] = 0x0;
    return hexString;
}

int* byte_checker(unsigned char *string) {
    char *bin_string = utf8_hex_bin_converter(string);
    int index = 0;
    int bytes = 0;
    int arr_index = 0;

    int *result = (int *) malloc(10 * sizeof(int));

    while (bin_string[index] != '\0') {

            //1 byte
            if (bin_string[index] == '0') {
                index += 8;
                bytes = 1;
                result[arr_index++] = bytes;


            }
                // 2 bytes
            else if (bin_string[index] == '1' && bin_string[index + 1] == '1' && bin_string[index + 2] == '0'
                     && bin_string[index + 8] == '1' && bin_string[index + 9] == '0') {
                index += 16;
                bytes = 2;
                result[arr_index++] = bytes;

            }
                // 3 bytes
            else if (bin_string[index] == '1' && bin_string[index + 1] == '1' && bin_string[index + 2] == '1' &&
                     bin_string[index + 3] == '0'
                     && bin_string[index + 8] == '1' && bin_string[index + 9] == '0'
                     && bin_string[index + 16] == '1' && bin_string[index + 17] == '0') {
                index += 24;
                bytes = 3;
                result[arr_index++] = bytes;
            }
                //4 bytes
            else if (bin_string[index] == '1' && bin_string[index + 1] == '1' && bin_string[index + 2] == '1' &&
                     bin_string[index + 3] == '1' &&
                     bin_string[index + 4] == '0'
                     && bin_string[index + 8] == '1' && bin_string[index + 9] == '0'
                     && bin_string[index + 16] == '1' && bin_string[index + 17] == '0'
                     && bin_string[index + 24] == '1' && bin_string[index + 25] == '0') {
                index += 32;
                bytes = 4;
                result[arr_index++] = bytes;
            }
            result[arr_index] = -1;
        }

    free(bin_string);
    return result;
}

int test_byte_checker(unsigned char *string, unsigned char *expected){
    int *actual = byte_checker(string);
    printf("%s, String: %s, Expected: %s Actual: ", (my_utf8_strcmp(expected, actual) == 0 ? "PASSED" : "FAILED"), string, expected);
    for (int i = 0; actual[i] != -1; i++) {
        printf("%d ", actual[i]);
    }
    printf("\n");

}

void testall_byte_checker() {
    unsigned char str1[] = {0xD7, 0x90, 0xD7, 0xA8, 0xD7, 0x99, 0xD7, 0x94, 0xE0, 0xA4, 0xB9, 0xF0, 0x9F, 0x98,
                            0x82,0x0};
    unsigned char str2[] = {0xD7, 0x90, 0xD7, 0x92, 0x0};
    unsigned char *str4 = "hello";

    test_byte_checker(str1, (int[]) {2, 2, 2, 2, 3, 4, -1});
    test_byte_checker(str2, (int[]) {2, -1});
    test_byte_checker(str4, (int[]) {1, -1});

}


//Validates that the input binary is a valid UTF8 encoded binary. if valid returns 1 .
//Valid if: 1 byte- 0???????
//          2 byte- 110????? 10??????
//          3 byte- 1110???? 10?????? 10??????
//          4 byte- 11110??? 10?????? 10?????? 10??????
int my_utf8_check(char *string) {
    char* bin_string = utf8_hex_bin_converter(string);
    int index = 0;

    //if the string ends in the middle of expected bit - not valid
    if (bin_string[index] == '\0' || bin_string[index + 1] == '\0' || bin_string[index + 2] == '\0' || bin_string[index + 3] == '\0' ||
            bin_string[index + 4] == '\0' || bin_string[index + 5] == '\0' || bin_string[index + 6] == '\0' || bin_string[index + 7] == '\0') {
        return 0;
    }

    //In order to check if the code is valid UTF8 it must follow the set patters according to the amount of bits
    //checking each of the possible bit combos and if the string always follows one of the four bit options then its valid
    while (bin_string[index] != '\0') {
        //1 byte
        if (bin_string[index] == '0'){
            index += 8;
            return 0;
        }
            // 2 bytes
        else if (bin_string[index] == '1' && bin_string[index + 1] == '1' && bin_string[index + 2] == '0'
                 && bin_string[index + 8] == '1' && bin_string[index + 9] == '0') {
            index += 16;
            return 0;
        }
            // 3 bytes
        else if (bin_string[index] == '1' && bin_string[index + 1] == '1' && bin_string[index + 2] == '1' && bin_string[index + 3] == '0'
                 && bin_string[index + 8] == '1' && bin_string[index + 9] == '0'
                 && bin_string[index + 16] == '1' && bin_string[index + 17] == '0') {
            index += 24;
            return 0;
        }
            //4 bytes
        else if (bin_string[index] == '1' && bin_string[index + 1] == '1' && bin_string[index + 2] == '1' && bin_string[index + 3] == '1' &&
                bin_string[index + 4] == '0'
                 && bin_string[index + 8] == '1' && bin_string[index + 9] == '0'
                 && bin_string[index + 16] == '1' && bin_string[index + 17] == '0'
                 && bin_string[index + 24] == '1' && bin_string[index + 25] == '0') {
            index += 32;
            return 0;
        }

        else {
            return 1;
        }
    }


}

int test_check(unsigned char *string, int expected){
    int actual = my_utf8_check(string);
    printf("%s, String: %s, Expected: %d, Actual: %d, Valid? %s\n", (expected == actual ? "PASSED" : "FAILED"),
           string, expected, actual, (actual == 0 ? "Valid UTF-8!" : "Invalid UTF-8"));
}

void testall_check() {
    unsigned char str1[] = {0xD7, 0x90, 0xD7, 0xA8, 0xD7, 0x99, 0xD7, 0x94, 0xE0, 0xA4, 0xB9, 0xF0, 0x9F, 0x98,
                            0x82,0x0};
    unsigned char str2[] = {0xD7, 0x90, 0xD7, 0x92, 0x0};
    unsigned char str3[] = {0xD7, 0x9, 0x0};
    unsigned char *str4 = "hello";

    test_check(str1,0);
    test_check(str2,0);
    test_check(str3,1);
    test_check(str4,0);

}

// Returns the number of characters in the string.
int my_utf8_strlen(unsigned char *string) {
    char *bin_string = utf8_hex_bin_converter((unsigned char*)string);
    int len = 0;
    int index = 0;
    if (my_utf8_check((char *)string) == 0) {
        while (bin_string[index] != '\0') {
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
                     bin_string[index + 4] == '0'
                     && bin_string[index + 8] == '1' && bin_string[index + 9] == '0'
                     && bin_string[index + 16] == '1' && bin_string[index + 17] == '0'
                     && bin_string[index + 24] == '1' && bin_string[index + 25] == '0') {
                index += 32;
                len += 1;
            }
            else{
                break;
            }
        }
        // Print the length of the UTF-8 string
        //printf("%d\n", len);
        return len;
    }
    // Handle the case where the UTF-8 string is invali
    return -1;
}
int test_strlen(unsigned char *string, int expected){
    int actual = my_utf8_strlen(string);
    if (actual == -1){
        printf("ERROR:INVALID UTF-8, String: %s\n", string);
    }
    else {
        printf("%s, String: %s, Expected: %d, Actual: %d\n", (expected == actual ? "PASSED" : "FAILED"),
               string, expected, actual);
    }
}

void testall_strlen() {
    unsigned char str1[] = {0xD7, 0x90, 0xD7, 0xA8, 0xD7, 0x99, 0xD7, 0x94, 0xE0, 0xA4, 0xB9, 0xF0, 0x9F, 0x98,
                            0x82,0x0};
    unsigned char str2[] = {0xD7, 0x90, 0xD7, 0x92, 0x0};
    unsigned char str3[] = {0xD7, 0x9, 0x0}; //invlaid utf8
    unsigned char *str4 = "hello";

    test_strlen(str1,6);
    test_strlen(str2,2);
    test_strlen(str3,-1);
    test_strlen(str4,5);


}

//Returns whether the two strings are the same (similar result set to strcmp() )
int my_utf8_strcmp(unsigned char *string1, unsigned char *string2) {
    int index = 0;
    int result = 0;
    char *stringA = utf8_hex_bin_converter((unsigned char *) string1);
    char *stringB = utf8_hex_bin_converter((unsigned char *) string2);
    while (stringA[index] != 0x0) {
        if (stringA[index] == stringB[index]) {
            result = 0;
        } else {
            result = 1;
            break;
        }
        index++;
    }
    return result;

}

    int test_strcmp(unsigned char *string1, unsigned char *string2, int expected){
        int actual = my_utf8_strcmp(string1, string2);
        printf("%s, String 1: %s, String 2: %s, Expected: %d, Actual: %d, Match?: %s\n", (expected == actual ? "PASSED" : "FAILED"),
        string1, string2, expected, actual, (actual == 0 ? "Matching Strings!" : "Non-matching Strings"));
    }

void testall_strcmp() {
    unsigned char str1[] = {0xD7, 0x90, 0xD7, 0x93, 0x0};
    unsigned char str2[] = {0xD7, 0x90, 0xD7, 0x92, 0x0};
    unsigned char str3[] = {0xD7, 0x90, 0xD7, 0x93, 0x0};
    unsigned char str4[] = {0xD7, 0x90, 0xD7, 0xA8, 0xD7, 0x99, 0xD7, 0x94, 0xE0, 0xA4, 0xB9, 0xF0, 0x9F, 0x98,
                            0x82,0x0};
    unsigned char str5[] = {0xD7, 0x90, 0xD7, 0xA8, 0xD7, 0x99, 0xD7, 0x94, 0xE0, 0xA4, 0xB9, 0xF0, 0x9F, 0x98,
                            0x82,0x0};
    test_strcmp(str1,str2,1);
    test_strcmp(str1,str3,0);
    test_strcmp(str2,str4,1);
    test_strcmp(str4,str5,0);

}

//Returns the UTF8 encoded character at the location specified.
//If the input string is improperly encoded, this function should return NULL to indicate an error.
char *my_utf8_charat(unsigned char *string, int index) {
    char *bin_string = utf8_hex_bin_converter(string);
    int *byte_counter = byte_checker(string);
    int str_index = 0;
    int char_index = 0;

    if (my_utf8_check(string) != 1) {
        while (string[str_index] != 0x0) {
            if (byte_counter[char_index] == -1) {
                break; // end of the byte checker
            }
            if (index == 0) {
                // Extract the UTF-8 character
                int res_index;
                char *result = (char *) malloc((byte_counter[char_index] + 1) * sizeof(char));
                for (res_index = 0; res_index < byte_counter[char_index]; res_index++) {
                    result[res_index] = string[str_index + res_index];
                }
                result[res_index] = '\0'; // Null-terminate the string
                return result;


            }

            str_index += byte_counter[char_index];
            char_index++;
            index--;
        }

    }
    return NULL;
}



int test_charat(unsigned char *string, int index, unsigned char *expected) {
    char *actual = my_utf8_charat(string, index);
    if (actual != NULL) {
        printf("%s, String: %s, Index: %d, Expected Index: %s, Actual: %s\n",
               (my_utf8_strcmp(expected, actual) == 0 ? "PASSED" : "FAILED"), string, index, expected, actual);

    }
    else {
        printf("ERROR:INVALID UTF-8, String: %s\n", string);
    }
}

void testall_charat() {
    unsigned char str1[] = {0xD7, 0x90, 0xD7, 0xA8, 0xD7, 0x99, 0xD7, 0x94, 0xE0, 0xA4, 0xB9, 0xF0, 0x9F, 0x98,
                            0x82,0x0};
    unsigned char str2[] = {0xD7, 0x90, 0xD7, 0x92, 0x0};
    unsigned char str3[] = {0xD7, 0x9,  0x0};
    unsigned char *str4 = "hello";

    test_charat(str1, 0,"\xD7\x90");
    test_charat(str1, 5, "\xF0\x9F\x98\x82");

    test_charat(str4,1, "\x65");
    //test_charat(str3, 0, NULL);
}


//my own function that concatanates two strings
char *my_strcat(unsigned char *string1, unsigned char *string2) {
    char *result;
    int index1 = 0;
    int index2 = 0;
    int res_index = 0;
    while (string1[index1] != '\0') {
        result[res_index++] = string1[index1++];
    }
    while (string2[index2] != '\0') {
        result[res_index++] = string2[index2++];
    }
    return result;
}


//check which byte patter in is and then chop of the added stuff at teh begiing of each byte
// convert back to hex
//returns a string, with ASCII representation where possible, and UTF8 character representation for non-ASCII characters.
int my_utf8_decode(char *input, char *output) {
    int utf8_index = 0;
    int codepoint_index = 0;

    
    // Null-terminate the output string
    output[codepoint_index] = '\0';
}



//Encoding a UTF8 binary, taking as input an ASCII binary, with UTF8 characters encoded using the “U+” notation, and returns a UTF8 encoded binary
int my_utf8_encode(char *input, char *output){
    char *bin =  utf8_hex_bin_converter(input);
    if (input > 0 && (int) input <= 0x7F0){
        //1byte
        output[0] = 0;
        for(int i = 1; i <= 8; i++){
            output[i] = bin[i-1];
        }


    }
    else if ((int) input > 0x7F && (int) input <= 0x07FF){
        //2byte
        output[0] = 1;
        output[1] = 1;
        output[2] = 0;
        for(int i = 3; i <= 7; i++){
            output[i] = bin[i-3];
        }
        output[8] = 1;
        output[9] = 0;
        for(int i = 10; i <= 15; i++) {
            output[i] = bin[i - 5];
        }
        output[16] = 1;
        output[17] = 0;
        for(int i = 18; i <= 23; i++) {
            output[i] = bin[i - 7];
        }

    }
    else if ((int) input > 0x800 && (int) input <= 0xFFFF){
        //3byte
    }
    else if ((int) input > 0x10000 && (int) input <= 0x10FFFF){
        //4byte
    }

}



int main() {
    unsigned char utf8[] = {0xD7, 0x90, 0xD7, 0xA8, 0xD7, 0x99, 0xD7, 0x94, 0xE0, 0xA4, 0xB9, 0xF0, 0x9F, 0x98,
                            0x82, 0x0};
    unsigned char alefemoji[] = {0xD7, 0x90, 0x45, 0x0};
    unsigned char codepoint[] = {0x5C, 0x75, 0x30, 0x35, 0x44, 0x30, 0x5C, 0x75, 0x45, 0x0};
    unsigned char *ucode = "\u0939\u0024\u00A3\n";
    unsigned char uccode[] = {0x2F, 0x75, 0x30, 0x39, 0x33, 0x39, 0x2F, 0x75, 0x30, 0x30,0x32, 0x34, 0x2F, 0x75, 0x41, 0x33, 0x0};
    printf("%s", ucode);
    printf("%s", uccode);

    //Test hex to bin converter
    char *binary = utf8_hex_bin_converter(alefemoji);
    printf("\nHex to Binary Converter\nhex input: %s\nbinary output: %s\n", alefemoji, binary);

    //test bin to hex
    char *hex = binary_to_hex(binary);
    printf("\nBinary to hex Converter\nbinary input: %s\nbinary output: %s\n", binary, hex);

    //byte checker
    int *result = byte_checker(utf8);
    printf("\nByte checker\n");
    for (int i = 0; result[i] != -1; i++) {
        printf("Character %d: %d bytes\n", i + 1, result[i]);
    }

    //test byte checker
    printf("\nByte checker\n");
    testall_byte_checker();

    //Test str length
    printf("\nStrlen Function:\n");
    testall_strlen();

    //test UTF 8 hex to bin
    printf("\nUTF-8 String (hex) to binary Function:\n");
    testall_hextobin();

    //Test check function
    printf("\nCheck UTF-8 Function:\n");
    testall_check();

    //test charat
    printf("\ncharat function:\n");
    testall_charat();

    //test strcmp
    printf("\nstrcmp function:\n");
    testall_strcmp();


    //Test encode
    printf("\nEncoder:\n");
    char output[40];
    my_utf8_encode(codepoint, output);


    //Test decode
    printf("\nDecoder:\nInput: %s\nOutput: ", alefemoji);
    //if (my_utf8_decode(alefemoji, output) == 0) {
    printf("%s", output);
}
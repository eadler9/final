
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

int my_utf8_strcmp();
int my_utf8_check();

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
    unsigned char *str3 = "Elana";
    unsigned char str4[] = {0xF0, 0x9F, 0x98, 0x90,0x45, 0x0};
    unsigned char str6[] = {0xF0, 0x9F, 0x98, 0x90,0xF0, 0x9F, 0x98, 0x95,0xF0, 0x9F, 0x98, 0x97, 0x0};
    unsigned char str7[] = {0x0};

    test_hextobin(str1, "110101111001000011010111101010001101011110011001110101111001010011100000101001001011100111110000100111111001100010000010");
    test_hextobin(str2,  "11010111100100001101011110010010");
    test_hextobin(str3, "0100010101101100011000010110111001100001");
    test_hextobin(str4, "1111000010011111100110001001000001000101");
    test_hextobin(str6, "111100001001111110011000100100001111000010011111100110001001010111110000100111111001100010010111");
    test_hextobin(str7, "");

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
           string, expected, actual, (actual == 0 ? "Valid UTF-8!" : "ERROR: INVALID UTF-8"));
}

void testall_check() {
    unsigned char str1[] = { 0xD7, 0xA8, 0xE0, 0xA4, 0xB9, 0xF0, 0x9F, 0x98,
                            0x82,0x0};
    unsigned char str2[] = {0xD7, 0x90, 0xD7, 0x92, 0x0};
    unsigned char str3[] = {0xD7, 0x9, 0x0};
    unsigned char *str4 = "hello";
    unsigned char str5[] = {0xCF, 0xA3, 0x45, 0x4C, 0x41, 0x4E,0x41,0xF0, 0x9F, 0x98,
                            0x90, 0x0};
    unsigned char str6[] = {0xF0, 0x9F, 0x98, 0x90,0xF0, 0x9F, 0x98, 0x95,0xF0, 0x9F, 0x98, 0x97, 0x0};
    unsigned char str7[] = {0x0};
    unsigned char str8[] = {0xCF, 0x3, 0x45, 0x4C, 0x41, 0x4E,0x41,0xF0, 0x9F, 0x98,
                            0x90, 0x0};
    test_check(str1,0);
    test_check(str2,0);
    test_check(str3,1);
    test_check(str4,0);
    test_check(str5,0);
    test_check(str6,0);
    test_check(str7,0);
    test_check(str8,1);



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
    // Handle the case where the UTF-8 string is invaidג
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
    unsigned char str5[] = {0xCF, 0xA3, 0x45, 0x4C, 0x41, 0x4E,0x41,0xF0, 0x9F, 0x98,
                            0x90, 0x0};
    unsigned char str6[] = {0xC6, 0xA9, 0xC6, 0xBD, 0x41, 0x4E,0xF0, 0x9F, 0x98,
                            0x90,0x41, 0x0};
    unsigned char str7[] = {0xF0, 0x9F, 0x98, 0x90,0xF0, 0x9F, 0x98, 0x95,0xF0, 0x9F, 0x98, 0x97, 0x0};
    unsigned char *str8 = "ה";
    unsigned char *str9 = "";

    test_strlen(str1,6);
    test_strlen(str2,2);
    test_strlen(str3,-1);
    test_strlen(str4,5);
    test_strlen(str5,7);
    test_strlen(str6,6);
    test_strlen(str7,3);
    test_strlen(str8,1);
    test_strlen(str9,0);




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

    if (my_utf8_check((char *)string) == 0) {
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
    if (actual == NULL) {
        printf("ERROR: INVALID INDEX OUT OF RANGE, string:%s, index:%d\n", string, index);
    }
    else {
        printf("%s, String: %s, Index: %d, Expected Index: %s, Actual: %s\n",
               (my_utf8_strcmp(expected, actual) == 0 ? "PASSED" : "FAILED"), string, index, expected, actual);
    }
}

void testall_charat() {
    unsigned char str1[] = {0xD7, 0x90, 0xD7, 0xA8, 0xD7, 0x99, 0xD7, 0x94, 0xE0, 0xA4, 0xB9, 0xF0, 0x9F, 0x98,
                            0x82,0x0};
    unsigned char str2[] = {0xD7, 0x90, 0xD7, 0x92, 0x0};
    unsigned char str3[] = {0xD7, 0x9,  0x0};
    unsigned char *str4 = "hello";
    unsigned char str5[] = {0xCF, 0xA3, 0x45, 0x4C, 0x41, 0x4E,0x41,0xF0, 0x9F, 0x98,
                            0x90, 0x0};
    unsigned char str6[] = {0xF0, 0x9F, 0x98, 0x90,0xF0, 0x9F, 0x98, 0x95,0xF0, 0x9F, 0x98, 0x97, 0x0};
    unsigned char str7[] = {0x0};


    test_charat(str1, 0,"\xD7\x90");
    test_charat(str1, 5, "\xF0\x9F\x98\x82");
    test_charat(str4,1, "\x65");
    test_charat(str7,2, "");
    test_charat(str5,10, "");
    test_charat(str6,2, "\x1F\x61\x17");

    //test_charat(str3, 0, NULL);
}


//check which byte patter in is and then chop of the added stuff at teh begging of each byte
// convert back to hex
//returns a string, with ASCII representation where possible, and UTF8 character representation for non-ASCII characters.
int my_utf8_decode(char *input, char *output) {
    int *byte_arr = byte_checker(input);
    int index = 0;
    int arr_index = 0;
    int decoded_index = 0;

    if(my_utf8_check(input) == 0) {
        while (input[index] != '\0') {
            int bytes = byte_arr[arr_index];
            int codepoint = 0;
            if (bytes == 1) {
                codepoint = input[index];
                //sprintf(output + decoded_index, "\\u%s", codepoint);
            } else {
                //anding the first byte with a bit mask that is the takes of the first x bits (ex: 0001111) to mask off however many extra utf8 bits
                codepoint = input[index] & (0xFF >> (bytes + 1));
                //for each additional byte (already did the first byte)
                for (int i = 1; i < bytes; i++) {
                    //first shift codepoint do add room for the next bits
                    // then we and the new byte with to just get the last 6 bits (first 2 are 10-conitue bits which we dont need)
                    codepoint = (codepoint << 6) | (input[index + i] & 0x3F);
                }
            }
            // now add in the \u's and make sure 4-5 places
            sprintf(output + decoded_index, "\\u%04X", codepoint);

            // get next utf8 and amount of bytes
            index += bytes;
            arr_index++;
            decoded_index += my_utf8_strlen(output + decoded_index);

        }
        free(byte_arr);
        return 0;
    }
    return 1;
}

int test_decode(unsigned char *input, unsigned char *output, unsigned char *expected) {
    int valid = my_utf8_decode(input, output);
    //returns 1 if check function says its not valid utf8
    if (valid != 1) {
        printf("%s, input: %s, output: %s, Expected output: %s\n",
               (my_utf8_strcmp(expected, output) == 0 ? "PASSED" : "FAILED"), input, output, expected);

    }
    else {
        printf("ERROR:INVALID UTF-8\n");
    }
}

void testall_decode() {
    unsigned char str1[] = {0xD7, 0x90, 0xD7, 0xA8, 0xD7, 0x99, 0xD7, 0x94, 0xE0, 0xA4, 0xB9, 0xF0, 0x9F, 0x98,
                            0x82,0x0};
    unsigned char str2[] = {0xD7, 0x90, 0xD7, 0x92, 0x0};
    unsigned char str3[] = {0xD7, 0x9,  0x0};
    unsigned char *str4 = "hello";
    char output[40] ;

    test_decode(str1, output,"\\u05D0\\u05E8\\u05D9\\u05D4\\u0939\\u1F602");
    test_decode(str2, output, "\\u05D0\\u05D2");
    test_decode(str4,output, "hello");

}

int unicode_to_int(const char *input) {
    int result = 0;
    // skip \u
    input += 2;
    while (*input != '\0') {
        int digit;
        if (*input >= '0' && *input <= '9') {
            digit = *input - '0';
        } else if (*input >= 'A' && *input <= 'F') {
            digit = *input - 'A' + 10;
        } else {
            return -1; // invalid
        }
        result = result * 16 + digit;
        input++;
    }
    return result;
}

//Encoding a UTF8 binary, taking as input an ASCII binary, with UTF8 characters encoded using the “U+” notation, and returns a UTF8 encoded binary
int my_utf8_encode(char *input, char *output){

    int unicode_val = unicode_to_int(input);

    if (unicode_val > 0 && unicode_val <= 0x7F){
        //1byte
        printf("1 byte\n");
        output[0] = (char)(unicode_val & 0x7F);
        output[1] = '\0';
    }
    else if (unicode_val> 0x7F && unicode_val <= 0x07FF){
        //2byte
        printf("2 byte\n");
        output[0] = (char)(((unicode_val >> 6) & 0x1F) | 0xC0);
        output[1] = (char)((unicode_val & 0x3F) | 0x80);
        output[2] = '\0';
    }
    else if (unicode_val > 0x800 && unicode_val <= 0xFFFF) {
        //3byte
        printf("3 byte\n");
        output[0] = (char)(((unicode_val >> 12) & 0x0F) | 0xE0);
        output[1] = (char)(((unicode_val >> 6) & 0x3F) | 0x80);
        output[2] = (char)((unicode_val & 0x3F) | 0x80);
        output[3] = '\0';
    }

    else if (unicode_val > 0x10000 && unicode_val<= 0x10FFFF){
        //4byte
        printf("4 byte\n");
        output[0] = (char)(((unicode_val >> 18) & 0x07) | 0xF0);
        output[1] = (char)(((unicode_val >> 12) & 0x3F) | 0x80);
        output[2] = (char)(((unicode_val >> 6) & 0x3F) | 0x80);
        output[3] = (char)((unicode_val & 0x3F) | 0x80);
        output[4] = '\0';
    }
    return 0;
}



int main() {
    unsigned char utf8[] = {0xD7, 0x90, 0xD7, 0xA8, 0xD7, 0x99, 0xD7, 0x94, 0xE0, 0xA4, 0xB9, 0xF0, 0x9F, 0x98,
                            0x82, 0x0};
    unsigned char alefemoji[] = {0xD7, 0x90, 0x45, 0x0};
    unsigned char codepoint[] = {0x5C, 0x75, 0x30, 0x35, 0x44, 0x30, 0x5C, 0x75, 0x45, 0x0};
    unsigned char *ucode = "\\u0939\\u0024\\u00A3\n";
    unsigned char *unicodeHex3 = "\\u0939";

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

    //test decode
    printf("\ndecode function:\n");
    testall_decode();


    //Test encode
    printf("\nEncoder:\n");
    char output[40];
    my_utf8_encode(unicodeHex3, output);
    printf("Encoded: %s\n", output);

}
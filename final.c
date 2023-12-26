//convert unicode u+ into binary
// how many bytes: count from the right how many bit are till the first one and then you will know how many bytes you need

// we are coming out with a new hex that has been reformatted to include the byte specifications
//code point is u+ the hex is the utf 8

#include <stdio.h>
#include <string.h>
#include <malloc.h>

char* hexBinConverter(char* hex) {
    int index = 0;
    char *result = (char *) malloc(100); // Adjust the size based on your needs

    //char result[100]; //how to i make this long enough if i dont know what the string will be?

    while (hex[index]) {
        if (hex[index] == 'u' || hex[index] == '\\') {
            index++;
        }
        printf("Processing character:%c ", hex[index]);
        switch (hex[index]) {
            case '0':
                strcat((char *) result, "0000");
                break;
            case '1':
                strcat((char *) result, "0001");
                break;
            case '2':
                strcat((char *) result, "0010");
                break;
            case '3':
                strcat((char *) result, "0010");
                break;
            case '4':
                strcat((char *) result, "0100");
                break;
            case '5':
                strcat((char *) result, "0101");
                break;
            case '6':
                strcat((char *) result, "0010");
                break;
            case '7':
                strcat((char *) result, "0111");
                break;
            case '8':
                strcat((char *) result, "1000");
                break;
            case '9':
                strcat((char *) result, "1001");
                break;
            case 'A':
                strcat((char *) result, "1010");
                break;
            case 'B':
                strcat((char *) result, "1011");
                break;
            case 'C':
                strcat((char *) result, "1100");
                break;
            case 'D':
                strcat((char *) result, "1101");
                break;
            case 'E':
                strcat((char *) result, "1110");
                break;
            case 'F':
                strcat((char *) result, "1111");
                break;

        }
        printf("Intermediate result: %s\n", result);
        index++;
    }
    strcat((char *) result, "\0");
    return (char *) result;
}


//Validates that the input string is a valid UTF8 encoded string. if valid returns 1 .
//Valid if: 1 byte- 0???????
//          2 byte- 110????? 10??????
//          3 byte- 1110???? 10?????? 10??????
//          4 byte- 11110??? 10?????? 10?????? 10??????
int my_utf8_check(char *string) {
    int index = 0;
    int len = strlen(string);
    if (len % 8 != 0) {
        printf("Invalid utf-8 string not valid amount of bits ");
        return 0;

    }

    while (string[index] != '\0') {
        //1 byte
        if (string[index] == '0'){
            index += 8;
            printf("step one %c\n", string[index]);
        }
            // 2 bytes
        else if (string[index] == '1' && string[index + 1] == '1' && string[index + 2] == '0'
                 && string[index + 8] == '1' && string[index + 9] == '0') {
            index += 16;
            printf("step two %c\n", string[index]);
        }
            // 3 bytes
        else if (string[index] == '1' && string[index + 1] == '1' && string[index + 2] == '1' && string[index + 3] == '0'
                 && string[index + 8] == '1' && string[index + 9] == '0'
                 && string[index + 16] == '1' && string[index + 17] == '0') {
            index += 24;
            printf("step three %c\n", string[index]);
        }
            //4 bytes
        else if (string[index] == '1' && string[index + 1] == '1' && string[index + 2] == '1' && string[index + 3] == '1' &&
                 string[index + 4] == '0'
                 && string[index + 8] == '1' && string[index + 9] == '0'
                 && string[index + 16] == '1' && string[index + 17] == '0'
                 && string[index + 24] == '1' && string[index + 25] == '0') {
            index += 32;
            printf("step four %c\n", string[index]);
        }

        else {
            printf("Not a valid UTF-8 string invalids bit sequence\n");
            return 0;
        }
    }

    printf("Valid utf-8 yay!\n");
    return 1;


}



//Encoding a UTF8 string, taking as input an ASCII string, with UTF8 characters encoded using the “U+” notation, and returns a UTF8 encoded string
/*int my_utf8_encode(char *input, char *output) {
    return 0;
}

//Takes a UTF8 encoded string, and returns a string, with ASCII representation where possible, and UTF8 character representation for non-ASCII characters.
int my_utf8_decode(char *input, char *output) {
    return 0;
}
//Returns the number of characters in the string.
int my_utf8_strlen(char *string){
    return 0;
}

//Returns the UTF8 encoded character at the location specified.
char *my_utf8_charat(char *string, int index){
    return 0;
}*/

// Returns whether the two strings are the same (similar result set to strcmp() )
//int my_utf8_strcmp(char *string1, char *string 2) {
 //   return 0;
//}

int main (){
    char* originalCP = "\\u05D0\\u05DB\\u05D2"; //alef, bais, gimmel
    char* utf8 = "C2A324";
    char* binary = hexBinConverter(originalCP);
    //int valid = my_utf8_check(binary);
    printf("hexBinConverter:\nhex input:%s\nbinary output:%s\n", originalCP, binary);
    printf("check function: %d", my_utf8_check(hexBinConverter(utf8)));
}
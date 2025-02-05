#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int setup_buff(char *, char *, int);
int count_words(char *, int, int);
void reverse_string(char *, int);
void print_words(char *, int);


int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    char *src = user_str, *dest = buff;
    int count = 0;
    int space_flag = 0;  

    while (*src && count < len) {
        if (*src != ' ' && *src != '\t') {
            if (space_flag && count > 0) {
                *dest++ = ' ';
                count++;
            }
            *dest++ = *src;
            count++;
            space_flag = 0;
        } else {
            space_flag = 1; 
        }
        src++;
    }
    
    if (count >= len) { 
        return -1;  
    } 
    
    while (count < len) {
        *dest++ = '.';
        count++;
    }
    return count;
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    //YOU MUST IMPLEMENT
    int count = 0;
    int in_word = 0;
    char *ptr = buff;

    while (ptr < buff + str_len) {
        if (*ptr != ' ' && *ptr != '.') {
            if (!in_word) {
                count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
        ptr++;
    }

    return count;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS
void reverse_string(char *buff, int len) {
    char *left = buff, *right = buff + len - 1;
    while (left < right) {
        char temp = *left;
        *left++ = *right;
        *right-- = temp;
    }
}

void print_words(char *buff, int len) {
    printf("Word Print\n----------\n");
    char *ptr = buff;
    int word_num = 1;

    while (ptr < buff + len) {
        while (*ptr == '.' || *ptr == ' ') ptr++; 

        if (*ptr == '\0') break; 
        char *word_start = ptr;
        int word_length = 0;

        while (*ptr != ' ' && *ptr != '.' && ptr < buff + len) {
            word_length++;
            ptr++;
        }

        printf("%d. ", word_num++);
        fwrite(word_start, 1, word_length, stdout);
        printf(" (%d)\n", word_length);
    }
}

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    // The check is to make sure that 2 arguments were provided or then that a valid option was provided, if so the if-statement does not execute. It's safe because it only checks for argv[1] after checking count of arguments.
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    // The if-statement checks if the user provided a string, as argv[0] is the program name, argv[1] is the option, and argv[2] is the string. If the user did not provide a string, then the if-statement executes showing usage and exiting the program.
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc &
    //handle error if malloc fails by exiting with a 
    // return code of 99
    buff = (char *)malloc(BUFFER_SZ);  
    if (buff == NULL) {  
        printf("Error with memory allocation\n");  
        exit(99);  
    }
    
    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len); 
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        // the case statement options
        case 'r':
            reverse_string(buff, user_str_len);
            printf("Reversed String: %.*s\n", user_str_len, buff);
            break;

        case 'w':
            print_words(buff, BUFFER_SZ);
            break;
        
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//starter take both the buffer as well as the length. Why
//do you think providing both the pointer and the length
//is a good practice, after all we know from main() that 
//the buff variable will have exactly 50 bytes?
//  
// Providing both pointer (buff) and length (BUFFER_SZ) ensures safer memory access, prevents buffer overflows, and makes functions more reusable for different buffer sizes.

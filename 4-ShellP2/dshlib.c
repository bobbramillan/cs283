#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"


/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int exec_local_cmd_loop()
{
    char *cmd_buff;
    int rc = 0;
    cmd_buff_t cmd;

    // TODO IMPLEMENT MAIN LOOP
    while (1) {
        printf("%s", SH_PROMPT);

        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff
        char *start = cmd_buff;
        while (*start == SPACE_CHAR) start++;

        char *end = start + strlen(start) - 1;
        while (end > start && *end == SPACE_CHAR) *end-- = '\0';

        if (strlen(start) == 0) {
            printf("%s", CMD_WARN_NO_CMD);
            continue;
        }

        cmd._cmd_buffer = strdup(start);
        cmd.argc = 0;

        bool in_quotes = false;
        char *token = cmd._cmd_buffer;
        char *arg_start = NULL;

        for (int i = 0; cmd._cmd_buffer[i] != '\0'; i++) {
            if (cmd._cmd_buffer[i] == '"' && (i == 0 || cmd._cmd_buffer[i - 1] != '\\')) {
                in_quotes = !in_quotes;
                continue;
            }

            if ((cmd._cmd_buffer[i] == SPACE_CHAR && !in_quotes) || cmd._cmd_buffer[i + 1] == '\0') {
                if (cmd._cmd_buffer[i] == SPACE_CHAR) cmd._cmd_buffer[i] = '\0';
                if (arg_start != NULL) {
                    if (cmd.argc < CMD_ARGV_MAX - 1) {
                        cmd.argv[cmd.argc++] = arg_start;
                    }
                    arg_start = NULL;
                }
            } else if (arg_start == NULL) {
                arg_start = &cmd._cmd_buffer[i];
            }
        }

        
        // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
        if (cmd.argc > 0) {
            if (strcmp(cmd.argv[0], EXIT_CMD) == 0) {
                free(cmd._cmd_buffer);
                return OK_EXIT;
            }
 
        // the cd command should chdir to the provided directory; if no directory is provided, do nothing
        if (strcmp(cmd.argv[0], "cd") == 0) {
            if (cmd.argc == 1) {
                free(cmd._cmd_buffer);
                continue;
            } else if (cmd.argc == 2) {
                if (chdir(cmd.argv[1]) != 0) {
                    printf("error: could not change directory to %s\n", cmd.argv[1]);
                    rc = ERR_EXEC_CMD;
                }
                    free(cmd._cmd_buffer);
                    continue;
                } else {
                    printf("error: too many arguments for cd\n");
                    rc = ERR_CMD_ARGS_BAD;
                    free(cmd._cmd_buffer);
                    continue;
                }
            }
        }

        // TODO IMPLEMENT if not built-in command, fork/exec as an external command
        // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"
        pid_t pid = fork();
        if (pid < 0) {
            printf("error: fork failed\n");
            rc = ERR_MEMORY;
        } else if (pid == 0) {
            execvp(cmd.argv[0], cmd.argv);
            printf("%s", CMD_ERR_EXECUTE);
            exit(ERR_EXEC_CMD);
        } else {
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                rc = WEXITSTATUS(status);
            }
        }

        free(cmd._cmd_buffer);
    }

    return OK;
}
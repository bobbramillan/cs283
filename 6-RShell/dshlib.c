#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

/**** 
 **** FOR REMOTE SHELL USE YOUR SOLUTION FROM SHELL PART 3 HERE
 **** THE MAIN FUNCTION CALLS THIS ONE AS ITS ENTRY POINT TO
 **** EXECUTE THE SHELL LOCALLY
 ****
 */

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
int exec_local_cmd_loop() {
    char cmd_buff[SH_CMD_MAX];
    command_list_t cmd_list;
    int rc;

    while (1) {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }

        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        char *start = cmd_buff;
        while (*start == SPACE_CHAR) start++;
        char *end = start + strlen(start) - 1;
        while (end > start && *end == SPACE_CHAR) *end-- = '\0';

        if (strlen(start) == 0) {
            continue;
        }

        rc = build_cmd_list(start, &cmd_list);
        if (rc < 0) {
            return rc;
        }

        if (cmd_list.num == 1) {
            rc = exec_cmd(&cmd_list.commands[0]);
        } else {
            rc = execute_pipeline(&cmd_list);
        }

        if (rc < 0) {
            return rc;
        }
    }

    return OK;
}

int build_cmd_list(char *cmd_line, command_list_t *clist) {
    char *commands[CMD_MAX];
    int cmd_count = 0;

    char *token = strtok(cmd_line, PIPE_STRING);
    while (token) {
        while (*token == SPACE_CHAR) token++;

        char *end = token + strlen(token) - 1;
        while (end > token && *end == SPACE_CHAR) *end-- = '\0';

        if (cmd_count >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }

        if (strlen(token) == 0) {
            return ERR_CMD_ARGS_BAD;
        }

        commands[cmd_count++] = token;
        token = strtok(NULL, PIPE_STRING);
    }

    clist->num = cmd_count;

    for (int i = 0; i < cmd_count; i++) {
        clist->commands[i].argc = 0;

        char *arg = strtok(commands[i], " ");
        while (arg && clist->commands[i].argc < CMD_ARGV_MAX - 1) {
            clist->commands[i].argv[clist->commands[i].argc++] = arg;
            arg = strtok(NULL, " ");
        }

        clist->commands[i].argv[clist->commands[i].argc] = NULL;
    }

    return OK;
}

int execute_pipeline(command_list_t *clist) {
    int num_commands = clist->num;
    int pipes[CMD_MAX - 1][2];
    pid_t pids[CMD_MAX];

    for (int i = 0; i < num_commands - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            return ERR_MEMORY;
        }
    }

    for (int i = 0; i < num_commands; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            return ERR_MEMORY;
        }

        if (pids[i] == 0) {
            if (i > 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }
            if (i < num_commands - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            for (int j = 0; j < num_commands - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            exit(ERR_EXEC_CMD);
        }
    }

    for (int i = 0; i < num_commands - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    for (int i = 0; i < num_commands; i++) {
        waitpid(pids[i], NULL, 0);
    }

    return OK;
}

int exec_cmd(cmd_buff_t *cmd) {
    pid_t pid = fork();
    if (pid < 0) {
        return ERR_MEMORY;
    }

    if (pid == 0) {
        execvp(cmd->argv[0], cmd->argv);
        exit(ERR_EXEC_CMD);
    }

    int status;
    waitpid(pid, &status, 0);

    return WIFEXITED(status) ? WEXITSTATUS(status) : ERR_EXEC_CMD;
}
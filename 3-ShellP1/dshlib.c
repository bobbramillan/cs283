#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    char *commands[CMD_MAX];
    int cmd_count = 0;

    char *token = strtok(cmd_line, PIPE_STRING);
    while (token) {
        if (cmd_count >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }

        while (*token == SPACE_CHAR)
            token++;

        char *end = token + strlen(token) - 1;
        while (end > token && *end == SPACE_CHAR)
            *end-- = '\0';

        if (strlen(token) == 0) {
            return WARN_NO_CMDS;
        }

        commands[cmd_count++] = token;
        token = strtok(NULL, PIPE_STRING);
    }

    clist->num = cmd_count;
    for (int i = 0; i < cmd_count; i++) {
        char *exe_part = strtok(commands[i], " ");
        if (!exe_part || strlen(exe_part) >= EXE_MAX) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }
        strcpy(clist->commands[i].exe, exe_part);

        char *arg_part = strtok(NULL, "");
        if (arg_part && strlen(arg_part) >= ARG_MAX) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }
        if (arg_part) {
            strcpy(clist->commands[i].args, arg_part);
        }
    }

    return OK;
}
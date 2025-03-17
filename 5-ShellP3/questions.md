1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

waitpid() is called for each child process created by fork() and without doing this child processes would exhaust system resources by becoming zombie processes and accumulating.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

The pipe must be closed to prevent any descriptor leaks and leaving them open causes processes to wait for input that isnt there.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

cd is built-in because unlike an external command it would change the dir of a child process but not the shell if we didn't.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

We could modify with malloc to allow unlimited pipe commands, but this may cause memory leaks if not careful.

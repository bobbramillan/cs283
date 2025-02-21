1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**: fork creates a new process so the shell process isn't replaced and continues running independently

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**: If fork() fails my system returns 1, and returns ERR_MEMORY

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**: execvp() looks for command in directories inside the PATH env variable

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**: It ensures the shell waits for the child process to finish before continuing preventing any wastage of system resources from zombie processes.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**: It provides the exit code of a child process letting us know if a command failed or succeeded

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**: It handles quoted strings as a one argument. For example, echo "hello world" passes as hello world without separating due to spaces.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**: White space trimming was improved and I modified my logic to better handle quoted arguments. I didn't have any unexpected challenges.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Signals notify a process about an event asynchronously rather than using pipes like IPC which require direct communication and can transfer large amounts of data.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGKILL: forcefully terminates a process and cannot be blocked, SIGTERM: can be politely terminated meaning blocked and ignored, SIGINT: is used to interrupt a process but can be ignored

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  SIGSTOP is received, the process is paused immediately and cannot be caught or ignored like SIGINT due to being handled directly by the kernel.

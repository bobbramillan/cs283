1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

The remote client determines this when End of File (0x04) is received from the server. We use recv() to keep looping until the EOF is reached.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

It should do this by checking to see for EOF or a null terminator, if we dont look for this some commands could get combined or cut.

3. Describe the general differences between stateful and stateless protocols.

Stateful is to remember info from past connections, while stateless do not do this. Stateless just moves to the next connection with no past data.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

It has faster for data transfers and has less overhead, we use it for real-time applications like streaming.

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

Sockets is whats provided by the OS as an API and works well with both TCP and UDP.
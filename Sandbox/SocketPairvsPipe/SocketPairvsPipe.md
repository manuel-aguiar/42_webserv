
In this example, we use sockets (namely a socketpair) to allow communication between two processes.

It can be seen as somewhat similar to two uni-directional pipes tied together, two separate buffers for read and write within the socket
while the kernel abstracts away the fact that the same fd is passed to the function, being the kernel responsible to read from and
write to the correct buffer, internally.
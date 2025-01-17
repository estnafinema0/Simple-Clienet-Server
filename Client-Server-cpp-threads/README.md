# README

## Overview
This project demonstrates a basic TCP client and server in C++. The server waits for incoming connections on a chosen port, and the client connects to the same port using the server’s hostname or IP address. After connection both programs can exchange messages. If either side sends the keyword `"revoir"`, the connection closes and both programs exit cleanly.

An important detail is the use of the `SO_REUSEADDR` option on the server’s socket. It allows the server to immediately reuse the same port after shutting down, avoiding the usual wait caused by the **TIME_WAIT** state in TCP. Without `SO_REUSEADDR`, you would often see an `Address already in use` error when trying to restart the server on the same port too quickly.

## Features

1. **Simple TCP Communication**: Send and receive messages between a client and server.
2. **Graceful shutdown**: Typing `"revoir"` on either side closes the connection.
3. **Port reuse**: The code uses `setsockopt` with `SO_REUSEADDR` to allow reusing the same port shortly after the program closes (you can always just use another free port to avoid conflicts or TIME_WAIT issues).

## Compilation
Compile the **server** and **client** separately with the following commands:

```bash
gcc -Wall server.c -o server
gcc -Wall client.c -o client
```

Now you have two executables: `server` and `client`.

## How to Use It

### 1. Start the Server
Start first terminal and start the server on your favorite port number from 1024 to 49151 (let’s say 5000):
```bash
./server 5000
```
The server will wait for a client to connect.

### 2. Start the Client
In a second terminal (or on another machine), start the client by specifying the server hostname (IP) and the same port:
```bash
./client 127.0.0.1 5000
```
Use `127.0.0.1` if you are runnimg on the local machine.
Replace `127.0.0.1` with the actual IP address or hostname of your server if you are running it on a remote machine.

### 3. Exchange Messages
- After the client connects, you can type messages in each terminal.  
- The keyword `"revoir"` (typed by server) will terminate the connection and exit.

## TIME_WAIT and Port Reuse
In TCP, after a connection is closed, the socket often remains in the **TIME_WAIT** state for a short period. During this time, the OS won`t let you to immediately reuse the same address and port to avoid possible conflicts.

By using:
```c
int option = 1;
setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
```
we allow OS re-bind to that port even if it is still in **TIME_WAIT**. 

If you prefer not to use `SO_REUSEADDR` or if you rigorously do not want any port conflicts, you can simply choose **another port** (ex., `5001`) and run the code again.

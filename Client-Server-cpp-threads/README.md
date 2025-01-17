# README

## Welcome to the OOP Network Echo Server
Yes, it's yet another TCP client-server project. Now in C++. But this time, everything is built around an object-oriented design, complete with classes, threads, and mutexes for synchronization.

## Features
1. **Object-Oriented Paradigm**  
   We've reorganized the code into C++ classes that handle low-level networking details:
   - **`Net::Socket`**: Encapsulates socket operations like `bind`, `connect`, `listen`, `accept`, as well as reading/writing data.
   - **`Net::InetSocketAddress`**: A simple wrapper for `sockaddr_in`, making it easier to manage IP addresses and ports.

2. **Multi-Client Support via Threads**  
   The server can now spawn a separate thread for each incoming client connection (using the C++ `<thread>` library), so multiple clients can connect at the same time.

3. **Mutex-based Synchronization**  
   A global `std::mutex` ensures that when multiple clients send messages concurrently, their responses are handled in the order the server receives them. Essentially, the server won't interleave its replies from different threads in a messy way.

4. **Graceful Client-Server Shutdown**  
   By typing the keyword `revoir` during your session, you can politely end the connection from either side.

5. **Port Reuse**  
   Like before, you can reuse ports by setting the `SO_REUSEADDR` option if you want to quickly restart the server without those pesky “port already in use” errors.

## Compilation
We provide a `makefile` for convenience. Simply open a terminal in the project folder and run:

```bash
make
```

This will produce two executables, **`server`** and **`client`**. If you prefer, you can also compile them individually:

```bash
make server
make client
```

To clean up old binaries:

```bash
make clean
```

## How to Use It

### 1. Start the Server
In one terminal, run:
```bash
./server <port>
```
Replace `<port>` with a number between 1024 and 49151 (for example, `5000`). The server will listen on that port and await connections from clients.

For example, 
```bash
./server 5000
```

### 2. Start the Client
Open another terminal (or use another machine, if you sure about your friend) and run:
```bash
./client <hostname> <port>
```
- `<hostname>` is the server address (e.g., `127.0.0.1` for localhost).
- `<port>` must be the same port you used for the server.

For example, 
```bash
./client 127.0.0.1 5000
```

### Step 3: Exchange Messages
Ok! Time to the fun part:  
- Type your message on the client side and press Enter.  
- The server will receive it, and then (from the server’s terminal) you can type a reply back to the client.  

If don't have enough partners to try this cool thing - split the terminal and run the second client with the same port and ip address.
Because of the mutexes - there are will be no conflicts, while answering to the multiple clients  in the server terminal.

This exchange continues until someone decides to end the conversation.

### Example Session

**On the Server side:**  
```
$ ./server 5000
Client, id - 1, pid - 12345: Hello from the client!
MEServer, id - 1, pid - 12345:
```
*(You type your response here, say “Wow! You've coped with it!” and press Enter.)*

**On the Client side:**  
```
$ ./client 127.0.0.1 5000
MEClient:
Hello from the client!
Server:
Wow! You've coped with it!
```

### Step 4: Exit When Done
Type `revoir` on the server side to gracefully close the connection for that particular client. You can keep the server running to accept new connections from other clients if you wish.

Make your clients happy with you!
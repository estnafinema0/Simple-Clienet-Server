# README

## Welcome to the OOP Network Echo Server (Fork Version)
Yes, it's another TCP client-server project in C++. Like before, everything is built around an object-oriented design, with classes to neatly handle networking tasks. The difference here is that **each client connection is handled by a separate process** (using `fork`), rather than by threads.  

**Important Note**: In this version, the server's terminal is a shared resource that is *not* protected. So if multiple clients connect at once, the server terminal may interleave inputs and outputs in unexpected ways.  
*(P.S. For a version with safer concurrency via threads and mutexes, see `Client-Server-cpp-threads`.)*

## Features
1. **Object-Oriented Paradigm**  
   - **`Net::Socket`**: A class encapsulating all socket operations (bind, connect, listen, accept, read, write).  
   - **`Net::InetSocketAddress`**: A convenient wrapper for `sockaddr_in`, handling IP addresses and ports with ease.

2. **Multi-Client Support via `fork`**  
   The server spawns a separate process for each new client connection, so multiple clients can be served at the same time.

3. **Graceful Client-Server Shutdown**  
   As with the previous projects, typing `revoir` during your session politely closes the connection on either side.

4. **Port Reuse**  
   If you need to quickly restart the server on the same port, the `SO_REUSEADDR` option is set, allowing for faster reuse without waiting for the port to become free again.

## Compilation
We provide a `makefile`. To compile everything at once, open your terminal in the project folder and run:
```bash
make
```
This produces two executables: **`server`** and **`client`**. You can also compile them separately:
```bash
make server
make client
```
To remove old binaries and object files:
```bash
make clean
```

## How to Use It

### 1. Start the Server
In one terminal, run:
```bash
./server <port>
```
Pick any free port between 1024 and 49151, for example:
```bash
./server 5000
```
The server will wait for client connections on this port.

### 2. Start the Client
Open another terminal (or use another machine, if you like) and run:
```bash
./client <hostname> <port>
```
- `<hostname>` can be `127.0.0.1` if you are on the same machine, or any valid server IP/hostname.
- `<port>` must match the port given to the server.

For example:
```bash
./client 127.0.0.1 5000
```

### Step 3: Exchange Messages
- Type your message on the client side and press Enter.  
- The server receives it and can respond by typing into its terminal.

Since each client runs in its own server process, multiple people can connect at once. However, **there is no synchronization mechanism in this fork-based version**—so the server's terminal might mix up the input and output if several clients are active.

### Example Session

**On the Server side:**  
```
$ ./server 5000
Client, id - 1, pid - 12345: Hello from the client!
MEServer, id - 1, pid - 12345:
```
*(Type your response, e.g., “Greetings from the server!” and press Enter.)*

**On the Client side:**  
```
$ ./client 127.0.0.1 5000
MEClient:
Hello from the client!
Server:
Greetings from the server!
```

### Step 4: Exit When Done
Type `revoir` on the server side (or client side) to politely end that connection. You can keep the server running to accept more clients.

P.S.
If you need safer concurrency with shared resources (like the server terminal), be sure to check out our thread-based version in `Client-Server-cpp-threads`.
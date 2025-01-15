# README

## Welcome to the classy network calculator
Ok, another TCP client-server project in C. 

## Features
This masterpiece allows you to:

1. **Simple TCP Communication**: Send and receive messages between a client and server.
2. **Graceful shutdown**: Type the magic number (6) and your server will politely disconnect you.
3. **Perform basic math operations**: Addition, subtraction, multiplication, division, and exponentiation (all over TCP)!
4. **Handle Errors**: Like division by zero.
5. **Port reuse**: The code uses `setsockopt` with `SO_REUSEADDR` to allow reusing the same port shortly after the program closes (you can always just use another free port to avoid conflicts or TIME_WAIT issues).
6. **Concurrency**: Handles one client at a time, mayby later could more processes at one time.

## Compilation
Compile the **server** and **client** separately with the following commands:

```bash
gcc -Wall -g server.c -o server
gcc -Wall -g client.c -o client
```

Now you have two executables: `server` and `client`.

## How to Use It

### 1. Start the Server
Start first terminal and start the server on your favorite port number from 1024 to 49151 (let’s say 5000):

```bash
./server 5000
```
The server will wait for a client to connect.

###  2. Start the Client
In a second terminal (or on another machine), start the client by specifying the server hostname (IP) and the same port:

```bash
./client 127.0.0.1 5000
```

Use `127.0.0.1` if you are runnimg on the local machine.
Replace `127.0.0.1` with the actual IP address or hostname of your server if you are running it on a remote machine.

### Step 3: Do math
- Enter the first operand.
- Enter the second operand.
- Choose an operation by entering a number between 1 and 5:
  1. Addition
  2. Subtraction
  3. Multiplication
  4. Division
  5. Exponentiation
  6. Exit 

### Example Session:
```bash
SERVER. Enter 1st operand:
CLIENT. 5
SERVER. Enter 2nd operand:
CLIENT. 3
SERVER. Select operator.
1 - Addition
2 - Subtraction
3 - Multiplication
4 - Division
5 - Pow
6 - Exit
Enter your choice's number:
CLIENT. 1
SERVER. Answer: 8.00

--- New operation ---
```

### Step 4: exit when the client is done
Choose option 6 to gracefully terminate the connection (or just close the terminal).


Happy calculating!


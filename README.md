# High Throughput Transaction Processing System

A banking system built using C socket programming and a client-server architecture.  
The project is designed to handle multiple clients at the same time and perform basic banking operations like deposits, withdrawals, balance checks, and money transfers.

## Features

- Client-server communication using TCP sockets
- Multi-client handling using threads/processes
- Account authentication system
- Deposit and withdrawal operations
- Balance inquiry support
- Fund transfer between accounts
- Synchronization using mutexes to avoid race conditions
- Concurrent transaction processing

## Requirements

- Linux / macOS / WSL
- GCC compiler
- pthread library

## Compilation

Clone the repository:

```bash
git clone https://github.com/Frost-04/High_Throughput_Transaction_Processing_System.git
cd High_Throughput_Transaction_Processing_System
```

Compile the server and client:

```bash
gcc server.c -o server -lpthread
gcc client.c -o client
```

## Running the Project

Start the server:

```bash
./server 8080
```

Run the client:

```bash
./client 127.0.0.1 8080
```

## How It Works

The server listens on a specified port for incoming client connections.  
Whenever a client connects, the server creates a separate thread/process to handle that client independently. This allows multiple users to perform transactions at the same time without blocking the main server.

Mutex locks are used while updating shared account data to maintain consistency during concurrent transactions.


## License

This project is licensed under the MIT License.

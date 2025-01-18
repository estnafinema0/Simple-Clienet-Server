# README

## About This Repository
Welcome to the **Network Server Collection**! 🎉  
This repository contains multiple variations of a TCP client-server application implemented in C and C++. Each project explores a different approach to handling multiple clients and concurrency or specific purpuses (fot ex. calclator), all while adhering to clean and reusable object-oriented design principles.

### Variations

1. **Simple-Client-Server** *(Coming Soon!)*  
   A simpler, single-client implementation showcasing the basics network socket programming in c.

2. **Client-Server-Calculator** *(Coming Soon!)*  
   A simpler, single-client implementation showcasing the basics network socket programming in c, which is a calculator with basic operations.
   
3. **Client-Server-cpp-fork**  
   A process-based version where the server spawns a new process (`fork`) for each client. While this approach supports multiple clients, shared resources (like the server terminal) are not protected in this version.

4. **Client-Server-cpp-threads**  
   A multithreaded version of the server where each client connection is handled by a separate thread. Synchronization is ensured using `std::mutex`, making it safe for shared resources like the server terminal.


### Key Features Across All Projects
- **Object-Oriented Design**: Classes like `Net::Socket` and `Net::InetSocketAddress` encapsulate low-level networking functionality.  
- **TCP Communication**: Send and receive messages between a client and server.  
- **Graceful Shutdown**: Use the keywords to close a session cleanly.  
- **Port Reuse**: Avoid port conflicts with `SO_REUSEADDR`.  

## Getting Started
Each project has its own directory with a detailed README and `makefile`. Pick a version, compile it, and enjoy! 🚀  

---

## Git Tags
- `v1.0` – Initial version with single-client support.
- `v2.0` – Multithreaded version (`Client-Server-cpp-threads`).
- `v2.1` – Fork-based version (`Client-Server-cpp-fork`).  
- `latest` – Always points to the most recent stable update.

---

Enjoy experimenting with these projects, and feel free to contribute or suggest new ideas!

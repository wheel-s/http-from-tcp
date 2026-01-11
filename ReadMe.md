# HTTP FROM TCP
 Http server built in C from tcp level —— socket management, handles reqeusts, routes traffic , serves responses.


# Features
- Socket management (tcp and udp). 
- Chunked Reading.
- RequestLine , Headers and Body Parsing.
- Response handling,
- Chunked Encoding.
- Binary Data Handling.



# Installation and Usage

- install gcc compiler;
```
#from the root
make
```
- or compile manually
```
#windows
gcc  -Iinclude src/*.c -o main -lws2_32
#linux
gcc -Iinclude src/*.c -o main

```
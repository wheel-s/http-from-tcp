# HTTP/1.1 Server in C

A custom HTTP/1.1 server built from scratch in C, using TCP connection to explore how webservers and the HTTP protocol work internally.

## Overview

This project implements the core pieces of HTTP/1.1. It focuses on understanding how data travels from a client, through a TCP stream, and back as an HTTP response.

## Features

- *Socket Management*: TCP/UDP socket lifecycle management using native networking APIS.

- *HTTP Request Parsing*: Custom parsing routines for HTTP request lines target paths, header fields, and optional body.

- *HTTP Response Generation*: Dynamic generation of status lines, custom headers and payload bodies.

- *Request method and path handling*: Routing logic and handling different request path.

- *Error Handling*: client error detection and built-in HtTTP status code error responses.

- *Memory and Resource Management*: Clean allocation and manual resource cleanup across socket streams and connection lifecycles.

## Technologies

* C
* Winsock2
* TCP/IP
* GNU Make
* GCC / clang
* Munit (µnit)

## Getting Started

### Prequisites

* C compiler (gcc / clalng)
* GNU Make

### Installation

clone the repository:
```bash
git clone https://github.com/wheel-s/http-from-tcp
```
### Running the Application

Using Make
```bash
make http
```
Or build directly
```bash
gcc -Iinclude internal/request/request.c internal/headers/headers.c internal/response/response.c internal/server/server.c httpserver/main.c -o http -lws2_32
```
Run the compiled binary
```bash
./http
```
### Running Tests

Run the tests suite:
```bash
make req_test
make headers_test
```


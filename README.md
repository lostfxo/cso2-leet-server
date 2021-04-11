# cso2-leet-server

[![builds.sr.ht status](https://builds.sr.ht/~leite/cso2-leet-server/commits/alpine-gcc.yml.svg)](https://builds.sr.ht/~leite/cso2-leet-server/commits/alpine-gcc.yml)

A master server for Nexon's Counter-Strike: Online 2 written in C++.

The server was rewritten from [cso2-master-server](https://github.com/L-Leite/cso2-master-server).

## Usage

In order to work properly, the master server requires [this user service](https://github.com/L-Leite/cso2-users-service) to be up and running somewhere reachable.

You may start the server by running the `leet-server [options]` executable.

Example:

```
./leet-server -i 123.45.67.89 -p 55555
```

#### Command line arguments

- `-l, --logging` (_optional_) Sets the log output verbosity, options: [debug, info, warning, error] (default: info)
- `-i, --ip-address [ip]` (_optional_) The IP address to listen on (default: 0.0.0.0)
- `-p, --port-master [port]` (_optional_) The server's (TCP) port (default: 30001)
- `-P, --port-holepunch [port]` (_optional_) The server's holepunch (UDP) port (default: 30002)
- `--usersvc-host` (_optional_) The hostname of an user service (default: 127.0.0.1)
- `--usersvc-port` (_optional_) The port number of an user service (default: 30100)
- `-L, --log-packets` (_optional_) Log the incoming and outgoing packets

## Building

### Requirements

Before building cso2-leet-server, you must have:

- A C++20 compiler
- Boost >=1.73
- CMake >=3.12

### Building with ninja

If you wish to build with ninja,

### Building with Docker

To build with docker, execute this command to build an image:

```
docker build -t leet-server .
```

You can then start an instance with

```
docker run leet-server
```

## License

`cso2-leet-server` is licensed under the BSD 2-Clause License.

This project is not affiliated with either Valve Software or Nexon.

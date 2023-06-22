# Pass
Currently a one-to-many server & client chat system using multi-threading.
Plans to expand on functionality.


## Implemented Features
* one-to-many connection of Server to Client over TCP.
* multi-threaded implementation of server to interact with multiple clients connections
* string message passing between server & all clients (group chat)

## Build & Run
`cmake` to build,

`./build/chatServer/chatServer` to run Server,

`telnet {host} {port}` to run Client. (currently hard set to `localhost 1337`)

Port updates from within source file.


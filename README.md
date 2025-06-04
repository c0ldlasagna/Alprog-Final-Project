# Dyin'


```
████████▄  ▄██   ▄    ▄█  ███▄▄▄▄     
███   ▀███ ███   ██▄ ███  ███▀▀▀██▄   
███    ███ ███▄▄▄███ ███▌ ███   ███   
███    ███ ▀▀▀▀▀▀███ ███▌ ███   ███   
███    ███ ▄██   ███ ███▌ ███   ███   
███    ███ ███   ███ ███  ███   ███   
███   ▄███ ███   ███ ███  ███   ███   
████████▀   ▀█████▀  █▀    ▀█   █▀       
```

Dyin' is a CLI based "mobile banking" app made in C++.

## Features:

- Create accounts with username and password
- Withdraw, deposit, and transfer funds to other users.

## Architecture:

Dyin' uses winsock2 to send json over tcp to the server to handle requests. All connections are stateless. Dyin' uses a json file for persistent storage.

## Pros:

- Simple network protocol
- Human readable database
- Simple to use UI

## Cons:

- Network send/receive buffer size is static, therefore causing errors when message is too large.
- Passwords are stored in plaintext.
- UI could look and function a little better.
- No multithreading
- Uses winsock, so only works on windows.


## Libraries used:

- [nlohmann/json](https://github.com/nlohmann/json)
- [ArthurSonzogni/ftxui](https://github.com/ArthurSonzogni/FTXUI)
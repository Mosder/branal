# A bare-bones analyzer for the game Broken Ranks

Inspired by [BrokenHUD](https://github.com/Brok3nSign/BrokenTools/releases).  
Way more primitive than the one it's inspired by, because I just can't be bothered to add all those features.  
Unless you value FOSS or have troubles setting up BrokenHUD on your linux system, then you should use [it](https://github.com/Brok3nSign/BrokenTools/releases) instead.

## Features
- Capturing packets sent by the game and saving them to a file for later use
- Analyzer for the `.pcap` files containing captured packets:
  - List of fight results in the given file with rewards gained for each player

### Planned features
- File analysis:
  - Total gained rewards in all fights in the given file
- Live analyzer parsing packets in real time:
  - Results of the last fight
  - Total gained rewards, since starting the analyzer

## Getting started

### Prerequisites
- Base utils: `git`, `gcc`, `make`
- [tshark](https://tshark.dev/setup/install/#installing-tshark-only)

### Installation
**1. Clone and enter the repository:**
```
git clone https://github.com/Mosder/branal
cd branal
```

**2. Create the server header file:**  
You will need to find out the IP address of the game server.  
To do so, use [termshark](https://github.com/gcla/termshark) or any other tool you'd prefer.  
Then, create the file (replace `<ip>` with the IP address you found):
```
echo "#define SERVER_IP \"<ip>\"" > analyzer/headers/server.h
```
It's not included in the repository to avoid it being scraped by bots or something.

**3. Modify the interface header file:**  
Right now the used interface is `enp8s0`, which likely isn't the name of your interface.  
To find the name of your interface, use:
```
ip address
```
Then, modify the file (replace `<interface>` with your interface name):
```
echo "#define INTERFACE \"<interface>\"" > analyzer/headers/interface.h
```

**4. Compile the code**:  
Finally, compile the code with `make`.  
To install it into `/usr/bin`:
```
make
```
that will use sudo and require password.

You can instead install it into `~/.local/bin`:
```
make local
```

You can also just compile it in place:
```
make compile
```
and just move the `branal` binary to your preferred directory.

## Usage

### Live analyzer (NOT IMPLEMENTED)
To start live analyzer, use:
```
branal
```
or
```
branal (l | live)
```
It will open the live analyzer in your terminal.

### File analyzer
To analize `.pcap` files, use:
```
branal (f | file) <file...>
```
It will perform analysis of the specified file(s).

### Capturing packets
To just capture packets from the server, use:
```
branal (c | capture) [file]
```
It will start capturing packets to the specified file.  
If file is not specified, it will capture them to the default location (`~/.local/share/branal/captures`).

### Help
You can display help using:
```
branal (h | help)
```

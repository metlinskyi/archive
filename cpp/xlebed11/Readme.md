# Project Title

IPK Project 1

## Author

Anastasiia Lebedenko
xlebed11

### Prerequisites

Linux Ubuntu 20.04 LTS
A web browser
wget, curl on your machine

### Installation

To start using this program, compile it with the command "make" in the directory the source file is located. 

## Usage

To connect to a server, type into your terminal "./hinfosvc portnumber &". Substitute a variable portnumber with the number you prefer or need. 

When the connection is established, you can see a process identifier in your terminal. After that server is ready.

The program can resolve 3 types of commands: 
curl http://servername:12345/hostname (returns a host name, i.e "merlin.fit.vutbr.cz")
curl http://servername:12345/cpu-name (returns CPU name, i.e "Intel(R) Xeon(R) CPU E5-2640 0 @ 2.50GHz")
curl http://servername:12345/load     (returns current CPU load, i.e "65%")

In case of an invalid command, server writes out "400 BAD REQUEST"

It is possible to use wget instead of curl. 

Alternatively, the program writes out a response in a web server. To try it, simply type the links from the commands above (without curl or wget) into a browser like a url.

To exit a server, type CTRL+C into your terminal.

# ft_ping

## setup
### Reference: inetutils 2.0 / ping
- `wget http://ftp.gnu.org/gnu/inetutils/inetutils-2.0.tar.gz` (https://ftp.gnu.org/gnu/inetutils/)
-  `tar -xzvf inetutils-2.0.tar.gz`
- `cd inetutils-2.0`
- follow this : https://www.fr.linuxfromscratch.org/view/lfs-10.1-fr/chapter08/inetutils.html:
    -
    ```
    ./configure --prefix=/usr --localstatedir=/var --disable-logger --disable-whois --disable-rcp --disable-rexec --disable-rlogin --disable-rsh --disable-servers

    ```
- `make`
- then cd ping and `./ping -?` to check all the behavior
- `less ping.c` for the source code

## The fundamental
### Socket
opening a socket is necessary because it allows the program to send and receive ICMP (Internet Control Message Protocol) packets. Here's a breakdown of why opening a socket is essential:

Communication: Sockets provide a communication mechanism between processes running on different hosts over a network. In the case of ft_ping, the program needs to communicate with other hosts on the network by sending ICMP echo request packets and receiving ICMP echo reply packets.

Raw Sockets: ft_ping uses raw sockets, which allow direct access to IP packets. Raw sockets bypass some of the higher-level protocols and provide more control over packet content. This is crucial for implementing features like ICMP ping.

Sending ICMP Packets: With a raw socket, the program can construct ICMP echo request packets and send them to the target host. These packets contain information necessary for the target host to respond, such as the sender's IP address and a sequence number.

Receiving ICMP Packets: After sending an ICMP echo request, ft_ping needs to listen for ICMP echo reply packets from the target host. By opening a socket, the program can listen for incoming ICMP packets and process them accordingly.

Handling ICMP Errors: In addition to echo reply packets, ICMP can also include error messages, such as "destination unreachable" or "time exceeded." ft_ping may need to handle these error messages, which requires the ability to receive ICMP packets.

## Difference Between a Socket and a Raw Socket
Standard Sockets (e.g., TCP/UDP Sockets):

Abstraction Level: Standard sockets operate at a higher level of abstraction, usually handling transport layer protocols like TCP (Transmission Control Protocol) and UDP (User Datagram Protocol).
Use Cases: Used for general network communication, such as web servers (HTTP), file transfers (FTP), and streaming (UDP).
Implementation: When using standard sockets, you don't need to worry about the underlying IP or protocol details; the operating system handles packet construction and processing.
Programming Interface: Functions like socket(), bind(), listen(), connect(), send(), and recv() are used for managing standard sockets.
Raw Sockets:

Abstraction Level: Raw sockets provide a lower-level interface, allowing direct access to the IP layer and below. You can construct and handle packets at the network layer (IP) and even the data link layer.
Use Cases: Used for applications that need to implement custom protocols, perform network diagnostics (like ping), or network security tools (like packet sniffers).
Implementation: When using raw sockets, the programmer has to manually construct packets, including headers and payload, providing greater control over packet contents.
Programming Interface: Functions like socket(), sendto(), and recvfrom() are used, with additional steps to manually construct and parse packet data.

Key Concepts
Endpoint: A socket represents one end of a communication link. Each socket is associated with an IP address and a port number.

Communication Mechanism: Sockets are used by applications to communicate over a network using protocols like TCP, UDP, and ICMP.

File Descriptor: In Unix-like operating systems, a socket is treated as a file descriptor. This means you can use standard file operations like read(), write(), and close() on sockets.

Types of Sockets
Stream Sockets (SOCK_STREAM):

Protocol: Typically used with TCP (Transmission Control Protocol).
Characteristics: Provides reliable, ordered, and error-checked delivery of data. It's like a phone call where the connection is established, and data is exchanged as a continuous stream.
Datagram Sockets (SOCK_DGRAM):

Protocol: Typically used with UDP (User Datagram Protocol).
Characteristics: Provides connectionless communication, where each message is sent individually and may arrive out of order or not at all. It's like sending individual letters through the mail.
Raw Sockets (SOCK_RAW):

Protocol: Can be used with various protocols like ICMP, allowing direct access to lower-level network protocols.
Characteristics: Provides access to the raw network protocol, bypassing the standard protocol stack. This allows custom packet construction and manipulation. It's like having direct access to the postal system infrastructure, where you can create and send custom envelopes.
How Sockets Work
Creating a Socket:

A socket is created using the socket() system call.
You specify the domain (e.g., AF_INET for IPv4), the type (e.g., SOCK_STREAM or SOCK_DGRAM), and the protocol (e.g., IPPROTO_TCP or IPPROTO_UDP).
Binding:

For server sockets, you bind the socket to a specific IP address and port number using the bind() system call.
Listening and Accepting (for TCP):

Servers listen for incoming connections using the listen() system call.
Incoming connections are accepted using the accept() system call.
Connecting:

Clients establish a connection to a server using the connect() system call.
Sending and Receiving Data:

Data is sent and received using send(), recv(), sendto(), and recvfrom() system calls, depending on the socket type.
Closing the Socket:

Once communication is complete, the socket is closed using the close() system call.
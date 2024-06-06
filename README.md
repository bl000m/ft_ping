# ft_ping
[chart](https://whimsical.com/42-projects-7EKQioumBVzf7smU9P6ARA)
## ICMP protocol
- [ICMP protocol specification](https://datatracker.ietf.org/doc/html/rfc792)

## setup
### Reference: inetutils 2.0 / ping
- `wget http://ftp.gnu.org/gnu/inetutils/inetutils-2.0.tar.gz` => here the [details](https://ftp.gnu.org/gnu/inetutils/)
-  `tar -xzvf inetutils-2.0.tar.gz`
- `cd inetutils-2.0`
- Follow [this](https://www.fr.linuxfromscratch.org/view/lfs-10.1-fr/chapter08/inetutils.html) link for detailed instructions.

    ```
    ./configure --prefix=/usr --localstatedir=/var --disable-logger --disable-whois --disable-rcp --disable-rexec --disable-rlogin --disable-rsh --disable-servers

    ```
- `make`
- then cd ping and `./ping -?` to check all the behavior
- `less ping.c` for the source code

## The fundamental
- [raw socket](Doc/socket.md)
- [icmp packet](Doc/icmp_packet.md)
- [resolve dnns](Doc/resolve_dns.md)
- [statistics](Doc/statistics.md)

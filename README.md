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


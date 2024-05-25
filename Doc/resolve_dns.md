## Find first IP address mapped on hostname
- ref for getaddrinfo: https://man7.org/linux/man-pages/man3/getaddrinfo.3.html
- in particular it's important to know the struct addrinfo:
```
The addrinfo structure used by getaddrinfo() contains the
       following fields:

           struct addrinfo {
               int              ai_flags;
               int              ai_family;
               int              ai_socktype;
               int              ai_protocol;
               socklen_t        ai_addrlen;
               struct sockaddr *ai_addr;
               char            *ai_canonname;
               struct addrinfo *ai_next;
           };
```
the ints argument allow to specify criteria for returning socket address structures retruned in the list pointed by res, as follows:
```
ai_family
              This field specifies the desired address family for the
              returned addresses.  Valid values for this field include
              AF_INET and AF_INET6.  The value AF_UNSPEC indicates that
              getaddrinfo() should return socket addresses for any
              address family (either IPv4 or IPv6, for example) that can
              be used with node and service.

       ai_socktype
              This field specifies the preferred socket type, for
              example SOCK_STREAM or SOCK_DGRAM.  Specifying 0 in this
              field indicates that socket addresses of any type can be
              returned by getaddrinfo().

       ai_protocol
              This field specifies the protocol for the returned socket
              addresses.  Specifying 0 in this field indicates that
              socket addresses with any protocol can be returned by
              getaddrinfo().

       ai_flags
              This field specifies additional options, described below.
              Multiple flags are specified by bitwise OR-ing them
              together.
```
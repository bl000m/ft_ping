    Calculate checksum before sending
    extremely important that checksum is calculated on the whole icmp packet
    because the receiver will make the checksum of the whole packet
    if the checksum doesnt' match the receiver discard the packet
    and the consequence is that the sender will no receive packets back

        //  for ICMP the byte order for checksum calculation is standardized to be in big-endian format.

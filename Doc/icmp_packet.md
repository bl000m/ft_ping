## ICMP Packet

ICMP Header (struct icmphdr header):

The struct icmphdr represents the header of the ICMP packet. It typically contains fields such as:
type: Specifies the type of ICMP message (e.g., echo request, echo reply, destination unreachable, etc.).
code: Further specifies the type of ICMP message (e.g., code indicating the reason for a destination unreachable message).
checksum: Used to detect errors in the packet during transmission.
Other fields specific to the type of ICMP message being sent.
Payload (char payload[PAYLOAD_SIZE]):

The payload of an ICMP packet contains additional data specific to the type of ICMP message being sent. For example:
In an ICMP echo request (ping), the payload typically includes data that will be echoed back by the recipient.
In an ICMP echo reply, the payload includes the data received in the corresponding echo request.
For other ICMP message types, the payload may contain different types of data or be empty.
Sequence Number (int seq_num):

The sequence number is used to uniquely identify each ICMP packet, particularly useful when sending multiple packets in succession. It helps in tracking the order of packets and detecting missing or out-of-order packets.
Process ID (pid_t pid):

The process ID is typically the ID of the process (or program) that generated the ICMP packet. It helps in identifying the source of the ICMP message.
Checksum (unsigned short checksum):

The checksum is computed over the entire ICMP packet, including both the ICMP header and payload. It is used to detect errors in the packet during transmission. The calculation of the checksum ensures the integrity of the packet's contents.
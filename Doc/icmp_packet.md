## ICMP Packet
- ICMP message size:
Header: 8 bytes (64 bits)
Type: 8 bits
Code: 8 bits
Checksum: 16 bits
Identifier: 16 bits
Sequence Number: 16 bits
Payload: Typically 56 bytes

What interests us is [here](https://datatracker.ietf.org/doc/html/rfc792):

- Echo or Echo Reply Message

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Type      |     Code      |          Checksum             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |           Identifier          |        Sequence Number        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Data ...
   +-+-+-+-+-

   IP Fields:

   Addresses

      The address of the source in an echo message will be the
      destination of the echo reply message.  To form an echo reply
      message, the source and destination addresses are simply reversed,
      the type code changed to 0, and the checksum recomputed.

   IP Fields:

   Type

      8 for echo message;

      0 for echo reply message.

   Code

      0

   Checksum

      The checksum is the 16-bit ones's complement of the one's
      complement sum of the ICMP message starting with the ICMP Type.
      For computing the checksum , the checksum field should be zero.
      If the total length is odd, the received data is padded with one
      octet of zeros for computing the checksum.  This checksum may be
      replaced in the future.

   Identifier

      If code = 0, an identifier to aid in matching echos and replies,
      may be zero.

   Sequence Number



      If code = 0, a sequence number to aid in matching echos and
      replies, may be zero.

   Description

      The data received in the echo message must be returned in the echo
      reply message.

      The identifier and sequence number may be used by the echo sender
      to aid in matching the replies with the echo requests.  For
      example, the identifier might be used like a port in TCP or UDP to
      identify a session, and the sequence number might be incremented
      on each echo request sent.  The echoer returns these same values
      in the echo reply.

      Code 0 may be received from a gateway or a host.

- Summary of Message Types

    0  Echo Reply

    3  Destination Unreachable

    4  Source Quench

    5  Redirect

    8  Echo

   11  Time Exceeded

   12  Parameter Problem

   13  Timestamp

   14  Timestamp Reply

   15  Information Request

   16  Information Reply


- ICMP Header (struct icmphdr header):

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
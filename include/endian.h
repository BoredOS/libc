// Copyright (c) 2026 Quoc Trung (https://github.com/thisisaname1928)
// This software is released under the GNU General Public License v3.0. See
// LICENSE file for details. This header needs to maintain in any file it is
// present in, as per the GPL license terms.
#ifndef ENDIAN_H
#define ENDIAN_H

#include <stdint.h>

// PDP_ENDIAN too old, no need to implement it
#define LITTLE_ENDIAN 1234
#define BIG_ENDIAN 4321

#if defined(__BYTE_ORDER__)
#define __LIBC_BYTE_ORDER __BYTE_ORDER__
#elif defined(__BYTE_ORDER)
#define __LIBC_BYTE_ORDER __BYTE_ORDER
#endif

// manually detect byte order
#if !defined(__BYTE_ORDER__) && !defined(__BYTE_ORDER)
#if defined(__x86_64__) || defined(__i386__) || defined(_M_IX86)
#define __LIBC_BYTE_ORDER LITTLE_ENDIAN
#elif defined(__powerpc__) || defined(__m68k__)
#define __LIBC_BYTE_ORDER BIG_ENDIAN
#else
#error "BoredOS libc doesn't support this architecture!"
#endif
#endif

#if defined(_MSC_VER)
#define __bswap16(x) _byteswap_ushort(x)
#define __bswap32(x) _byteswap_ulong(x)
#define __bswap64(x) _byteswap_uint64(x)
#elif defined(__GNUC__) || defined(__clang__)
#define __bswap16(x) __builtin_bswap16(x)
#define __bswap32(x) __builtin_bswap32(x)
#define __bswap64(x) __builtin_bswap64(x)
#else
#define __bswap16(x) (uint16_t)(((uint16_t)(x) << 8) | ((uint16_t)(x) >> 8))
#define __bswap32(x)                                                           \
  (uint32_t)((((uint32_t)(x) >> 24) & 0xff) |                                  \
             (((uint32_t)(x) >> 8) & 0xff00) |                                 \
             (((uint32_t)(x) << 8) & 0xff0000) |                               \
             (((uint32_t)(x) << 24) & 0xff000000))
#define __bswap64(x)                                                           \
  (uint64_t)((((uint64_t)(x) >> 56) & 0x00000000000000ffULL) |                 \
             (((uint64_t)(x) >> 40) & 0x000000000000ff00ULL) |                 \
             (((uint64_t)(x) >> 24) & 0x0000000000ff0000ULL) |                 \
             (((uint64_t)(x) >> 8) & 0x00000000ff000000ULL) |                  \
             (((uint64_t)(x) << 8) & 0x000000ff00000000ULL) |                  \
             (((uint64_t)(x) << 24) & 0x0000ff0000000000ULL) |                 \
             (((uint64_t)(x) << 40) & 0x00ff000000000000ULL) |                 \
             (((uint64_t)(x) << 56) & 0xff00000000000000ULL))
#endif

#if (__LIBC_BYTE_ORDER == LITTLE_ENDIAN)
#define be16toh(x) (__bswap16(x))
#define be32toh(x) (__bswap32(x))
#define be64toh(x) (__bswap64(x))

#define htobe16(x) (__bswap16(x))
#define htobe32(x) (__bswap32(x))
#define htobe64(x) (__bswap64(x))

#define htole16(x) ((uint16_t)(x))
#define htole32(x) ((uint32_t)(x))
#define htole64(x) ((uint64_t)(x))

#define le16toh(x) ((uint16_t)(x))
#define le32toh(x) ((uint32_t)(x))
#define le64toh(x) ((uint64_t)(x))
#else
#define be16toh(x) ((uint16_t)(x))
#define be32toh(x) ((uint32_t)(x))
#define be64toh(x) ((uint64_t)(x))

#define htobe16(x) ((uint16_t)(x))
#define htobe32(x) ((uint32_t)(x))
#define htobe64(x) ((uint64_t)(x))

#define htole16(x) (__bswap16(x))
#define htole32(x) (__bswap32(x))
#define htole64(x) (__bswap64(x))

#define le16toh(x) (__bswap16(x))
#define le32toh(x) (__bswap32(x))
#define le64toh(x) (__bswap64(x))
#endif

#endif
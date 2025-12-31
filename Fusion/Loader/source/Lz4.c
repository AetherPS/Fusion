#include "common.h"
#include "Lz4.h"

static inline uint32_t read32(const uint8_t* p)
{
    uint32_t val;
    memcpy(&val, p, 4);
    return val;
}

static inline uint32_t hash(uint32_t val)
{
    return (val * 2654435761U) >> (32 - HASH_LOG);
}

static inline size_t read_varlen(const uint8_t** src, const uint8_t* src_end, size_t initial)
{
    size_t length = initial;
    if (initial == 15)
    {
        while (*src < src_end)
        {
            uint8_t s = *(*src)++;
            length += s;
            if (s != 255) break;
        }
    }
    return length;
}

size_t lz4_compress(const uint8_t* src, size_t src_size, uint8_t* dst, size_t dst_capacity)
{
    if (src_size == 0)
        return 0;

    uint16_t hash_table[HASH_SIZE] = { 0 };

    const uint8_t* src_ptr = src;
    const uint8_t* src_end = src + src_size;
    const uint8_t* src_limit = src_end - LAST_LITERALS;
    const uint8_t* mf_limit = src_end - MF_LIMIT;

    uint8_t* dst_ptr = dst;
    uint8_t* dst_end = dst + dst_capacity;

    const uint8_t* anchor = src;

    if (src_size >= MIN_MATCH)
    {
        hash_table[hash(read32(src_ptr))] = 0;
        src_ptr++;
    }

    while (src_ptr < mf_limit)
    {
        uint32_t h = hash(read32(src_ptr));
        size_t ref_pos = hash_table[h];
        const uint8_t* ref = src + ref_pos;
        hash_table[h] = (uint16_t)(src_ptr - src);

        size_t distance = src_ptr - ref;
        if (distance == 0 || distance > MAX_DISTANCE || read32(ref) != read32(src_ptr))
        {
            src_ptr++;
            continue;
        }

        size_t literal_len = src_ptr - anchor;

        const uint8_t* match_start = src_ptr;
        src_ptr += MIN_MATCH;
        ref += MIN_MATCH;
        while (src_ptr < src_limit && *src_ptr == *ref)
        {
            src_ptr++;
            ref++;
        }
        size_t match_len = src_ptr - match_start;

        if (dst_ptr >= dst_end) return 0;
        uint8_t* token = dst_ptr++;

        if (literal_len >= 15)
        {
            *token = 15 << 4;
            size_t extra = literal_len - 15;
            while (extra >= 255)
            {
                if (dst_ptr >= dst_end) return 0;
                *dst_ptr++ = 255;
                extra -= 255;
            }
            if (dst_ptr >= dst_end) return 0;
            *dst_ptr++ = (uint8_t)extra;
        }
        else
        {
            *token = (uint8_t)(literal_len << 4);
        }

        if (dst_ptr + literal_len > dst_end) return 0;
        memcpy(dst_ptr, anchor, literal_len);
        dst_ptr += literal_len;

        if (dst_ptr + 2 > dst_end) return 0;
        *dst_ptr++ = (uint8_t)distance;
        *dst_ptr++ = (uint8_t)(distance >> 8);

        size_t match_len_code = match_len - MIN_MATCH;
        if (match_len_code >= 15)
        {
            *token |= 15;
            size_t extra = match_len_code - 15;
            while (extra >= 255)
            {
                if (dst_ptr >= dst_end) return 0;
                *dst_ptr++ = 255;
                extra -= 255;
            }
            if (dst_ptr >= dst_end) return 0;
            *dst_ptr++ = (uint8_t)extra;
        }
        else
        {
            *token |= (uint8_t)match_len_code;
        }

        anchor = src_ptr;
    }

    size_t last_literals = src_end - anchor;
    if (dst_ptr >= dst_end) return 0;
    uint8_t* token = dst_ptr++;

    if (last_literals >= 15)
    {
        *token = 15 << 4;
        size_t extra = last_literals - 15;
        while (extra >= 255)
        {
            if (dst_ptr >= dst_end) return 0;
            *dst_ptr++ = 255;
            extra -= 255;
        }
        if (dst_ptr >= dst_end) return 0;
        *dst_ptr++ = (uint8_t)extra;
    }
    else
    {
        *token = (uint8_t)(last_literals << 4);
    }

    if (dst_ptr + last_literals > dst_end) return 0;
    memcpy(dst_ptr, anchor, last_literals);
    dst_ptr += last_literals;

    return dst_ptr - dst;
}

size_t lz4_decompress(const uint8_t* src, size_t src_size, uint8_t* dst, size_t dst_capacity)
{
    if (src_size == 0)
        return 0;

    const uint8_t* src_ptr = src;
    const uint8_t* src_end = src + src_size;

    uint8_t* dst_ptr = dst;
    uint8_t* dst_end = dst + dst_capacity;

    while (src_ptr < src_end)
    {
        uint8_t token = *src_ptr++;
        size_t literal_len = token >> 4;
        size_t match_len = token & 0x0F;

        literal_len = read_varlen(&src_ptr, src_end, literal_len);

        if (src_ptr + literal_len > src_end) return 0;
        if (dst_ptr + literal_len > dst_end) return 0;
        memcpy(dst_ptr, src_ptr, literal_len);
        src_ptr += literal_len;
        dst_ptr += literal_len;

        if (src_ptr >= src_end)
            break;

        if (src_ptr + 2 > src_end) return 0;
        size_t offset = src_ptr[0] | (src_ptr[1] << 8);
        src_ptr += 2;

        if (offset == 0) return 0;
        if ((size_t)(dst_ptr - dst) < offset) return 0;

        match_len = read_varlen(&src_ptr, src_end, match_len) + MIN_MATCH;

        if (dst_ptr + match_len > dst_end) return 0;
        const uint8_t* match = dst_ptr - offset;

        if (offset >= match_len)
        {
            memcpy(dst_ptr, match, match_len);
            dst_ptr += match_len;
        }
        else
        {
            for (size_t i = 0; i < match_len; i++)
                dst_ptr[i] = match[i];
            dst_ptr += match_len;
        }
    }

    return dst_ptr - dst;
}
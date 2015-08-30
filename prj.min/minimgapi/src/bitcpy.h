#pragma once
#ifndef BITCPY_INCLUDED
#define BITCPY_INCLUDED

void bitcpy(
    uint8_t       *p_dst,
    int            dst_shift,
    const uint8_t *p_src,
    int            src_shift, 
    int            size);

#endif // BITCPY_INCLUDED
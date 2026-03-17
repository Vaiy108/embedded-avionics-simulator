#ifndef ARINC429_H
#define ARINC429_H

#include <stdint.h>
#include <stdbool.h>
#include "platform_types.h"

typedef struct
{
    uint8_t label;
    uint8_t sdi;
    int32_t data;
    uint8_t ssm;
} Arinc429Fields;

uint32_t arinc429_encode(uint8_t label, uint8_t sdi, int32_t data, uint8_t ssm);
StatusCode arinc429_decode(uint32_t word, Arinc429Fields *out);
bool arinc429_check_odd_parity(uint32_t word);

#endif
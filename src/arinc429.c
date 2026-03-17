#include "arinc429.h"

/**
 * @brief Compute odd parity for bits 0..30.
 * @req AVSIM_ARINC_001
 */
static uint8_t compute_odd_parity_31(uint32_t word_without_parity)
{
    uint8_t ones = 0;
    int i;

    for (i = 0; i < 31; ++i)
    {
        ones += (uint8_t)((word_without_parity >> i) & 0x1u);
    }

    return (ones % 2u == 0u) ? 1u : 0u;
}

/**
 * @brief Encode ARINC 429 fields into a 32-bit word.
 * @req AVSIM_ARINC_002
 */
uint32_t arinc429_encode(uint8_t label, uint8_t sdi, int32_t data, uint8_t ssm)
{
    uint32_t word = 0u;
    uint32_t sdi_masked = ((uint32_t)sdi) & 0x3u;
    uint32_t ssm_masked = ((uint32_t)ssm) & 0x3u;
    uint32_t data_19 = ((uint32_t)data) & 0x7FFFFu;
    uint8_t parity;

    word |= (uint32_t)label;
    word |= (sdi_masked << 8);
    word |= (data_19 << 10);
    word |= (ssm_masked << 29);

    parity = compute_odd_parity_31(word);
    word |= ((uint32_t)parity << 31);

    return word;
}

bool arinc429_check_odd_parity(uint32_t word)
{
    uint8_t ones = 0;
    int i;

    for (i = 0; i < 32; ++i)
    {
        ones += (uint8_t)((word >> i) & 0x1u);
    }

    return ((ones % 2u) == 1u);
}

/**
 * @brief Decode ARINC 429 word into fields.
 * @req AVSIM_ARINC_003
 */
StatusCode arinc429_decode(uint32_t word, Arinc429Fields *out)
{
    uint32_t raw_data;

    if (out == 0)
    {
        return STATUS_INVALID_PARAM;
    }

    if (!arinc429_check_odd_parity(word))
    {
        return STATUS_PARITY_ERROR;
    }

    out->label = (uint8_t)(word & 0xFFu);
    out->sdi = (uint8_t)((word >> 8) & 0x3u);
    out->ssm = (uint8_t)((word >> 29) & 0x3u);

    raw_data = (word >> 10) & 0x7FFFFu;

    if ((raw_data & (1u << 18)) != 0u)
    {
        out->data = (int32_t)(raw_data | 0xFFF80000u);
    }
    else
    {
        out->data = (int32_t)raw_data;
    }

    return STATUS_OK;
}
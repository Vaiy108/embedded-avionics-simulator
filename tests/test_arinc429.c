#include <assert.h>
#include <stdio.h>
#include "arinc429.h"
#include "platform_types.h"

static void test_arinc429_roundtrip(void)
{
    uint32_t word;
    Arinc429Fields fields;
    StatusCode status;

    word = arinc429_encode(0x8B, 1, 12345, 2);
    status = arinc429_decode(word, &fields);

    assert(status == STATUS_OK);
    assert(fields.label == 0x8B);
    assert(fields.sdi == 1);
    assert(fields.data == 12345);
    assert(fields.ssm == 2);
    assert(arinc429_check_odd_parity(word));
}

static void test_arinc429_negative_data(void)
{
    uint32_t word;
    Arinc429Fields fields;
    StatusCode status;

    word = arinc429_encode(0x22, 0, -321, 0);
    status = arinc429_decode(word, &fields);

    assert(status == STATUS_OK);
    assert(fields.data == -321);
}

static void test_arinc429_bad_parity(void)
{
    uint32_t word;
    Arinc429Fields fields;
    StatusCode status;

    word = arinc429_encode(0x55, 0, 42, 0);
    word ^= (1u << 31);

    status = arinc429_decode(word, &fields);
    assert(status == STATUS_PARITY_ERROR);
}

int main(void)
{
    test_arinc429_roundtrip();
    test_arinc429_negative_data();
    test_arinc429_bad_parity();

    printf("test_arinc429: PASS\n");
    return 0;
}
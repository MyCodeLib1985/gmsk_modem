#include "main.h"
#include "stdlib.h"

#include "codec_8b10b.h"

/*
 ******************************************************************************
 * DEFINES
 ******************************************************************************
 */

/*
 ******************************************************************************
 * EXTERNS
 ******************************************************************************
 */

/*
 ******************************************************************************
 * PROTOTYPES
 ******************************************************************************
 */

/*
 ******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************
 */
static const uint8_t enc_34_neg[] = {
    0b1011,
    0b1001,
    0b0101,
    0b1100,
    0b1101,
    0b1010,
    0b0110,
    0b1110 // 0b0111 - alternate
};

static const uint8_t enc_34_pos[] = {
    0b0100,
    0b1001,
    0b0101,
    0b0011,
    0b0010,
    0b1010,
    0b0110,
    0b0001, // 0b1000 - alternate
};

static const uint8_t enc_56_pos[] = {
    0b011000,
    0b100010,
    0b010010,
    0b110001,
    0b001010,
    0b101001,
    0b011001,
    0b000111,
    0b000110,
    0b100101,
    0b010101,
    0b110100,
    0b001101,
    0b101100,
    0b011100,
    0b101000,
    0b100100,
    0b100011,
    0b010011,
    0b110010,
    0b001011,
    0b101010,
    0b011010,
    0b000101,
    0b001100,
    0b100110,
    0b010110,
    0b001001,
    0b001110,
    0b010001,
    0b100001,
    0b010100,
};

static const uint8_t enc_56_neg[] = {
    0b100111,
    0b011101,
    0b101101,
    0b110001,
    0b110101,
    0b101001,
    0b011001,
    0b111000,
    0b111001,
    0b100101,
    0b010101,
    0b110100,
    0b001101,
    0b101100,
    0b011100,
    0b010111,
    0b011011,
    0b100011,
    0b010011,
    0b110010,
    0b001011,
    0b101010,
    0b011010,
    0b111010,
    0b110011,
    0b100110,
    0b010110,
    0b110110,
    0b001110,
    0b101110,
    0b011110,
    0b101011
};

static int RD = -1; /* running disparity */

/*
 * decoding stuff
 */
#define VALUE_INVALID         0xFF
static uint8_t dec_65[64];
static uint8_t dec_43[16];

/*
 ******************************************************************************
 ******************************************************************************
 * LOCAL FUNCTIONS
 ******************************************************************************
 ******************************************************************************
 */

/**
 *
 */
static int block_disparity(uint32_t data, size_t bits){
  int ret = 0;

  while (bits--){
    if (((data >> bits) & 1) == 0)
      ret--;
    else
      ret++;
  }

  return ret;
}

/**
 *
 */
static void decode_tab_helper(const uint8_t *enc_array,
                      size_t enc_array_len, uint8_t *dec_array){
  size_t idx;
  uint8_t val;

  for (val=0; val<enc_array_len; val++){
    idx = enc_array[val];
    osalDbgCheck((dec_array[idx] == val) || (dec_array[idx] == VALUE_INVALID));
    dec_array[idx] = val;
  }
}

/**
 *
 */
static void fill_decode_tables(void){

  for (size_t i=0; i<sizeof(dec_65); i++)
    dec_65[i] = VALUE_INVALID;
  decode_tab_helper(enc_56_neg, sizeof(enc_56_neg), dec_65);
  decode_tab_helper(enc_56_pos, sizeof(enc_56_pos), dec_65);

  for (size_t i=0; i<sizeof(dec_43); i++)
    dec_43[i] = VALUE_INVALID;
  decode_tab_helper(enc_34_neg, sizeof(enc_34_neg), dec_43);
  decode_tab_helper(enc_34_pos, sizeof(enc_34_pos), dec_43);

//  for (val=0; val<sizeof(enc_56_neg); val++){
//    idx = enc_56_neg[val];
//    osalDbgCheck((dec_65[idx] == val) || (dec_65[idx] == VALUE_INVALID));
//    dec_65[idx] = val;
//  }
//  for (val=0; val<sizeof(enc_56_pos); val++){
//    idx = enc_56_pos[val];
//    osalDbgCheck((dec_65[idx] == val) || (dec_65[idx] == VALUE_INVALID));
//    dec_65[idx] = val;
//  }
//
//  for (val=0; val<sizeof(enc_34_neg); val++){
//    idx = enc_34_neg[val];
//    osalDbgCheck((dec_43[idx] == val) || (dec_43[idx] == VALUE_INVALID));
//    dec_43[idx] = val;
//  }
//  for (val=0; val<sizeof(enc_34_pos); val++){
//    idx = enc_34_pos[val];
//    osalDbgCheck((dec_43[idx] == val) || (dec_43[idx] == VALUE_INVALID));
//    dec_43[idx] = val;
//  }
}

/**
 *
 */
static void check_seq_len(uint32_t code){
  size_t seqlen = 0;
  size_t bit;
  size_t prev_bit;

  for (size_t i=0; i<32; i++){
    prev_bit = code & 1;
    code >>= 1;
    bit = code & 1;
    if (bit == prev_bit)
      seqlen++;
    else
      seqlen = 0;
  }

  osalDbgCheck(seqlen < 5);
}

/*
 ******************************************************************************
 * EXPORTED FUNCTIONS
 ******************************************************************************
 */

/**
 *
 */
void codec_8b10b_init(void){
  fill_decode_tables();
}

/**
 * The disparity of a block is calculated by the number of 1’s minus the number of 0’s.
 */
uint32_t encode_8b10b(uint8_t data){
  uint32_t ret = 0;
  uint32_t tmp = 0;

  uint8_t EDCBA = data & 0b11111;
  uint8_t HGF   = (data >> 5) & 0b111;

  if (RD <= 0)
    tmp = enc_56_neg[EDCBA];
  else
    tmp = enc_56_pos[EDCBA];
  RD += block_disparity(tmp, 6);
  ret = tmp << 4;

  if (RD <= 0)
    tmp = enc_34_neg[HGF];
  else
    tmp = enc_34_pos[HGF];
  RD += block_disparity(tmp, 4);
  ret |= tmp;

  return ret;
}

/**
 *
 */
uint8_t decode_8b10b(uint32_t code){
  osalDbgCheck(code <= 1023);

  uint8_t EDCBA;
  uint8_t HGF;
  size_t idx;

  idx = code & 0b1111;
  HGF = dec_43[idx];
  osalDbgCheck(HGF != VALUE_INVALID);

  idx = code >> 4;
  EDCBA = dec_65[idx];
  osalDbgCheck(EDCBA != VALUE_INVALID);

  return HGF << 5 | EDCBA;
}

/**
 *
 */
void self_test_8b10b(const uint8_t *datap, size_t len){

  uint32_t code = 0x55555555;
  uint8_t byte;

  for (size_t i=0; i<len; i++){
    byte = datap[i];
    code <<= 10;
    code |= encode_8b10b(byte);
    osalDbgCheck(decode_8b10b(code & 1023) == byte);
    osalDbgCheck(abs(RD) < 2);
    osalDbgCheck(abs(block_disparity(code, 10)) < 3);
    check_seq_len(code);
  }
}




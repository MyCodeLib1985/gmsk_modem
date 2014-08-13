#include "main.h"
#include "softmodem.h"

#include "codec_8b10b.h"

/*
 ******************************************************************************
 * DEFINES
 ******************************************************************************
 */
#define SOFTMODEM_TX_LEN  16
#define SOFTMODEM_RX_LEN  16

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
static uint8_t txbuf[SOFTMODEM_TX_LEN];
static size_t txword = 0;
static size_t txbit = 0;

//static uint8_t rxbuf[SOFTMODEM_RX_LEN];
//static size_t rxword = 0;
static size_t rxbit = 0;
static uint16_t rxbuf = 0;

static const uint8_t *testdata = (uint8_t *)0x8000000;
static size_t testdatalen = 20000;
static uint32_t txcode = 0;

static uint16_t sync_buf = 0;
static bool sync = false;

static size_t success_bytes = 0;
static size_t failed_bytes = 0;
static size_t total_bits = 0;
/*
 ******************************************************************************
 ******************************************************************************
 * LOCAL FUNCTIONS
 ******************************************************************************
 ******************************************************************************
 */
///**
// *
// */
//static void tx_underrun_handler(void){
//  return;
//}
//
///**
// *
// */
//static void rx_overflow_handler(void){
//  return;
//}

///**
// *
// */
//static void fill_test_pattern(void){
//  for (size_t i=0; i<SOFTMODEM_TX_LEN; i++){
//    if (0 == (i & 1))
//      txbuf[i] = 0x55;
//    else
//      txbuf[i] = 0xCC;
//  }
//}

/*
 ******************************************************************************
 * EXPORTED FUNCTIONS
 ******************************************************************************
 */
/**
 *
 */
void ModemObjectInit(void){
  return;
}

/**
 * provide next bit to be transmitted via radio channel
 */
unsigned int ModemOutBit(void){

  unsigned int ret;

  ret = (txcode >> txbit) & 1;
  txbit++;

  if (10 == txbit){
    txbit = 0;
    txword++;
    txcode = encode_8b10b(testdata[txword]);
    if (20000 == txword){
      txword = 0;
    }
  }

  return ret;
}

/**
 *
 */
static void store_byte(uint8_t byte){
  (void)byte;
  success_bytes++;
}

static uint16_t catch_sync(unsigned int bit){
  sync_buf <<= 1;
  sync_buf |= bit;
  return decode_8b10b(sync_buf & 1023);
}

/**
 * decode received bit
 */
void ModemInBit(unsigned int bit){
  uint16_t ret;

  total_bits++;

  if (false == sync){
    ret = catch_sync(bit);
    if (DECODE_FAILED != ret){
      store_byte(ret & 0xFF);
    }
    sync = true;
  }
  else{
    rxbuf <<= 1;
    rxbuf |= bit;
    rxbit++;
    if (10 == rxbit){
      rxbit = 0;
      ret = decode_8b10b(rxbuf);
      if (DECODE_FAILED != ret){
        store_byte(ret & 0xFF);
      }
      else{
        sync = false;
        sync_buf = 0;
        failed_bytes++;
      }
    }
  }
}

/**
 *
 */
void ModemStart(void){

  codec_8b10b_init();
  self_test_8b10b(testdata, testdatalen);

  ModemObjectInit();
  palSetPad(IOPORT2, GPIOB_TX_ENABLE);
  palClearPad(IOPORT2, GPIOB_TX_PS);
  palClearPad(IOPORT2, GPIOB_RX_PS);

  palSetPad(IOPORT2, GPIOB_PTT);
}


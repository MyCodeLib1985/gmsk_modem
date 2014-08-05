#include "main.h"
#include "softmodem.hpp"

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

static uint8_t rxbuf[SOFTMODEM_RX_LEN];
static size_t rxword = 0;
static size_t rxbit = 0;

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

/**
 *
 */
static void fill_test_pattern(void){
  for (size_t i=0; i<SOFTMODEM_TX_LEN; i++){
    if (0 == (i & 1))
      txbuf[i] = 0x55;
    else
      txbuf[i] = 0xCC;
  }
}

/*
 ******************************************************************************
 * EXPORTED FUNCTIONS
 ******************************************************************************
 */
/**
 *
 */
void ModemObjectInit(void){
  fill_test_pattern();
}

/**
 *
 */
unsigned int ModemGet(void){
  unsigned int ret;

  ret = (txbuf[txword] >> txbit) & 1;
  txbit++;

  if ((8 * sizeof(txbuf[0])) == txbit){
    txbit = 0;
    txword++;
    if (SOFTMODEM_TX_LEN == txword){
      txword = 0;
    }
  }

  return ret;
}

/**
 *
 */
void ModemPut(unsigned int bit){
  (void)bit;
  (void)rxbuf;
  (void)rxword;
  (void)rxbit;
}

/**
 *
 */
void ModemStart(void){
  ModemObjectInit();
  palSetPad(IOPORT2, GPIOB_TX_ENABLE);
  palClearPad(IOPORT2, GPIOB_TX_PS);
}


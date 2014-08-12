#ifndef SOFTMODEM_H_
#define SOFTMODEM_H_

#ifdef __cplusplus
extern "C" {
#endif
  void ModemObjectInit(void);
  void ModemStart(void);
  unsigned int ModemOutBit(void);
  void ModemInBit(unsigned int bit);
#ifdef __cplusplus
}
#endif

#endif /* SOFTMODEM_H_ */

#ifndef CODEC_8B10B_H_
#define CODEC_8B10B_H_

#define DECODE_FAILED         0xFFFF

#ifdef __cplusplus
extern "C" {
#endif
  uint16_t encode_8b10b(uint8_t data);
  uint16_t decode_8b10b(uint16_t code);
  void codec_8b10b_init(void);
  void self_test_8b10b(const uint8_t *datap, size_t len);
#ifdef __cplusplus
}
#endif

#endif /* CODEC_8B10B_H_ */

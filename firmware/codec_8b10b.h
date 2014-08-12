#ifndef CODEC_8B10B_H_
#define CODEC_8B10B_H_

#ifdef __cplusplus
extern "C" {
#endif
  uint32_t encode(uint8_t data);
  void encode_test(void);
  void codec_8b10b_init(void);
#ifdef __cplusplus
}
#endif

#endif /* CODEC_8B10B_H_ */

#include <stdint.h>
#include <stdlib.h>
char *replaceWord(const char *s, const char *oldW,
                  const char *newW);

void build_decoding_table();

void base64_cleanup();

char *base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length);

unsigned char *base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length);

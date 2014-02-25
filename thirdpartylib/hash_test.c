#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#ifndef FALSE
#define FALSE 0
#endif // FALSE

/* get the first hash value */
#define DP_FIRSTHASH(DP_res, DP_kbuf, DP_ksiz) \
  do { \
    const unsigned char *_DP_p; \
    int _DP_ksiz; \
    _DP_p = (const unsigned char *)(DP_kbuf); \
    _DP_ksiz = DP_ksiz; \
    if((_DP_ksiz) == sizeof(int)){ \
      memcpy(&(DP_res), (DP_kbuf), sizeof(int)); \
    } else { \
      (DP_res) = 751; \
    } \
    while(_DP_ksiz--){ \
      (DP_res) = (DP_res) * 31 + *(_DP_p)++; \
    } \
    (DP_res) = ((DP_res) * 87767623) & INT_MAX; \
  } while(FALSE)

/* get the second hash value */
#define DP_SECONDHASH(DP_res, DP_kbuf, DP_ksiz) \
  do { \
    const unsigned char *_DP_p; \
    int _DP_ksiz; \
    _DP_p = (const unsigned char *)(DP_kbuf) + DP_ksiz - 1; \
    _DP_ksiz = DP_ksiz; \
    for((DP_res) = 19780211; _DP_ksiz--;){ \
      (DP_res) = (DP_res) * 37 + *(_DP_p)--; \
    } \
    (DP_res) = ((DP_res) * 43321879) & INT_MAX; \
  } while(FALSE)

/* get the third hash value */
#define DP_THIRDHASH(DP_res, DP_kbuf, DP_ksiz) \
  do { \
    int _DP_i; \
    (DP_res) = 774831917; \
    for(_DP_i = (DP_ksiz) - 1; _DP_i >= 0; _DP_i--){ \
      (DP_res) = (DP_res) * 29 + ((const unsigned char *)(DP_kbuf))[_DP_i]; \
    } \
    (DP_res) = ((DP_res) * 5157883) & INT_MAX; \
  } while(FALSE)

int main(int argc, const char *argv[])
{
	const char* key = "the string to hash";
	const int keysize = strlen(key);
	int hash1 = 0, hash2 = 0, hash3 = 0;

	DP_FIRSTHASH(hash1, key, keysize);
	printf("hash1('0x%x') of key('%s') and keysize(%d)\n", hash1, key, keysize);

	DP_SECONDHASH(hash2, key, keysize);
	printf("hash2('0x%x') of key('%s') and keysize(%d)\n", hash2, key, keysize);

	DP_THIRDHASH(hash3, key, keysize);
	printf("hash3('0x%x') of key('%s') and keysize(%d)\n", hash3, key, keysize);

	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
* @brief Encode a serial object with URL encoding.
*
* @param ptr a serial object.
* @param size the size of object.
*
* @return URL encoding.
*/
char* urlencode(const char* ptr, int size) {
	char* buf = NULL, * wp = NULL;
	int i = 0, c = 0;
	if (!ptr) { return NULL; }
	if (size < 0) { size = strlen(ptr); }
	buf = (char*) malloc(size * 3  + 1);
	wp = buf;
	for (i = 0; i < size; i++) {
		c = ((unsigned char *)ptr)[i];
		if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
			(c >= '0' && c <= '9') || (c != '\0' && strchr("_-.!~*'()", c))) {
			*(wp++) = c;
		} else {
			wp += sprintf(wp, "%%%02X", c);
		}
	}
	*wp = '\0';
	return buf;
}

/**
* @brief Decode a string encoded with URL encoding.
*
* @param str a string encoded with URL encoding.
* @param sp output size of decoding result.
*
* @return decoding result.
*/
char* urldecode(const char* str, int* sp) {
	if (!str) { return 0; }
	char* buf = NULL, * wp = NULL;
	unsigned char c = 0;
	buf = strdup(str);
	wp = buf;
	while (*str != '\0') {
		if (*str == '%') {
			str++;
			if ( ( (str[0] >= '0' && str[0] <= '9') 
				|| (str[0] >= 'A' && str[0] <= 'F') 
				|| (str[0] >= 'a' && str[0] <= 'f') ) &&
				(  (str[1] >= '0' && str[1] <= '9') 
				|| (str[1] >= 'A' && str[1] <= 'F') 
				|| (str[1] >= 'a' && str[1] <= 'f') ) ) {
				c = *str;
				if (c >= 'A' && c <= 'Z') { c += 'a'-'A'; }
				if (c >= 'a' && c <= 'z') { *wp = c-'a'+10; } else { *wp = c-'0'; }
				*wp *= 0x10; str++; c = *str;
				if (c >= 'A' && c <= 'Z') { c += 'a' - 'A'; }
				if (c >= 'a' && c <= 'z') { *wp += c-'a'+10; } else { *wp += c-'0'; }
				str++; wp++;
			} else {
				break;
			}
		} else if (*str == '+') {
			*wp = ' '; str++; wp++;
		} else {
			*wp = *str; str++; wp++;
		}
	}
	*wp = '\0';
	if (sp) { *sp = wp - buf; }
	return buf;
}

int main(int argc, const char *argv[])
{
	const char* url = "http://cbigu.com/post/post/view?post_id=218";
	printf("origin url = '%s'\n", url);

	char* encode_result = urlencode(url, -1);
	printf("encode result = '%s'\n", encode_result);

	int n_codelen = 0;
	char* decode_result = urldecode(encode_result, &n_codelen);
	printf("decode result = '%s', its lenght = %d\n", decode_result, n_codelen);
	free(encode_result);
	free(decode_result);
	
    return 0;
}

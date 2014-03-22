#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

char* strstr_kmp(const char* haystack, const char* needle) {
	int i = 0, j = 0, hlen = 0, nlen = 0;
	signed char tbl[0x100];
	assert(haystack && needle);
	nlen = strlen(needle);
	if (nlen >= 0x100) return strstr(haystack, needle);
	tbl[0] = -1;
	i = 0, j = -1;
	while (i < nlen) {
		while ((j >= 0) && (needle[i] != needle[j])) {
			j = tbl[j];
		}
		i ++, j ++;
		tbl[i] = j;
	}
	hlen = strlen(haystack);
	i = 0, j = 0;
	while (i < hlen && j < nlen) {
		while ((j >= 0) && (haystack[i] != needle[j])) {
			j = tbl[j];
		}
		i ++, j ++;
	}
	if (j == nlen) { return (char*)(haystack + i - nlen); }
	return NULL;
}

int main(int argc, const char *argv[])
{
	const char* s = "we need just call my small, every say we live.";
	const char* needle = "call";
	printf("strstr_kmp(\"%s\", \"%s\") = \"%s\"\n", s, needle, strstr_kmp(s, 
		needle));

	return 0;
}

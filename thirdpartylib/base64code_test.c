/* Copyright (C) 
* 2014 - firstboy0513
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
* 
*/
/**
* @file base64code_test.c
* @brief Base64 encode and decode function.
* @author firstboy0513
* @version 0.0.1
* @date 2014-02-26
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
* @brief Encode a serial object with Base64 encoding.
*
* @param ptr a serial object.
* @param size serial object size.
*
* @return Base64 encoding.
*/
char* base64encode(const char *ptr, int size){
  char* tbl = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  char* buf = NULL, * wp = NULL;
  const unsigned char* obj;
  int i;
  if (!ptr) { return NULL; }
  if(size < 0) size = strlen(ptr);
  buf = (char*) malloc(4 * (size + 2) / 3 + 1);
  obj = (const unsigned char *)ptr;
  wp = buf;
  for(i = 0; i < size; i += 3){
    switch(size - i){
    case 1:
      *wp++ = tbl[obj[0] >> 2];
      *wp++ = tbl[(obj[0] & 3) << 4];
      *wp++ = '=';
      *wp++ = '=';
      break;
    case 2:
      *wp++ = tbl[obj[0] >> 2];
      *wp++ = tbl[((obj[0] & 3) << 4) + (obj[1] >> 4)];
      *wp++ = tbl[(obj[1] & 0xf) << 2];
      *wp++ = '=';
      break;
    default:
      *wp++ = tbl[obj[0] >> 2];
      *wp++ = tbl[((obj[0] & 3) << 4) + (obj[1] >> 4)];
      *wp++ = tbl[((obj[1] & 0xf) << 2) + (obj[2] >> 6)];
      *wp++ = tbl[obj[2] & 0x3f];
      break;
    }
    obj += 3;
  }
  *wp = '\0';
  return buf;
}

/**
* @brief Decode a string encoded with Base64 encoding.
*
* @param str a Base64 encoding string.
* @param sp output decoding result length.
*
* @return decoding result.
*/
char* base64decode(const char* str, int* sp){
  unsigned char* obj = NULL, * wp = NULL;
  int len = 0, cnt = 0, bpos = 0, i = 0, bits = 0, eqcnt = 0;
  if (!str) { return NULL; }
  len = strlen(str);
  obj = (char*) malloc(len + 4);
  wp = obj;
  while(bpos < len && eqcnt == 0){
    bits = 0;
    for(i = 0; bpos < len && i < 4; bpos++){
      if(str[bpos] >= 'A' && str[bpos] <= 'Z'){
        bits = (bits << 6) | (str[bpos] - 'A');
        i++;
      } else if(str[bpos] >= 'a' && str[bpos] <= 'z'){
        bits = (bits << 6) | (str[bpos] - 'a' + 26);
        i++;
      } else if(str[bpos] >= '0' && str[bpos] <= '9'){
        bits = (bits << 6) | (str[bpos] - '0' + 52);
        i++;
      } else if(str[bpos] == '+'){
        bits = (bits << 6) | 62;
        i++;
      } else if(str[bpos] == '/'){
        bits = (bits << 6) | 63;
        i++;
      } else if(str[bpos] == '='){
        bits <<= 6;
        i++;
        eqcnt++;
      }
    }
    if(i == 0 && bpos >= len) continue;
    switch(eqcnt){
    case 0:
      *wp++ = (bits >> 16) & 0xff;
      *wp++ = (bits >> 8) & 0xff;
      *wp++ = bits & 0xff;
      cnt += 3;
      break;
    case 1:
      *wp++ = (bits >> 16) & 0xff;
      *wp++ = (bits >> 8) & 0xff;
      cnt += 2;
      break;
    case 2:
      *wp++ = (bits >> 16) & 0xff;
      cnt += 1;
      break;
    }
  }
  obj[cnt] = '\0';
  if(sp) *sp = cnt;
  return (char *)obj;
}

int main(int argc, const char *argv[])
{
	const char* s = "这是测试setence.";
	char* s_base64encode = base64encode(s, -1);
	printf("s_base64encode = '%s'\n", s_base64encode);
	char* s_base64decode = base64decode(s_base64encode, NULL);
	printf("s_base64decode = '%s'\n", s_base64decode);

	free(s_base64encode);
	free(s_base64decode);

	return 0;
}

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
* @file nossl_test.c
* @brief use libssl to implement nossl Connection.
* reference:
* 	http://www.ibm.com/developerworks/linux/library/l-openssl/index.html
* @author firstboy0513
* @version 0.0.1
* @date 2014-02-21
*/
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

#include <stdio.h>
#include <string.h>

/**
* @brief test case for nossl.
* The START_BIO_READ goto label is use for BIO_should_retry function.
*
* @param host_and_port host_and_port string.
* @param request request string.
*/
void testcase_nossl(char* host_and_port, const char* request) {
	BIO* bio;
	ERR_load_BIO_strings();
	SSL_load_error_strings();
	OpenSSL_add_all_algorithms();

	if (NULL == (bio = BIO_new_connect(host_and_port))) {
		printf("BIO is null!\n"); return;
	}
	if (BIO_do_connect(bio) <= 0) {
		ERR_print_errors_fp(stderr); BIO_free_all(bio); return;
	}
	BIO_write(bio, request, strlen(request));
	int p = 0; char r[1024] = {0};
START_BIO_READ:
	while ((p = BIO_read(bio, r, 1023)) > 0) {
		r[p] = '\0'; printf("%s", r);
	}
	if (p < 0 && BIO_should_retry(bio)) {
		goto START_BIO_READ;
	}
	BIO_free_all(bio);
}

int main(int argc, const char *argv[])
{
	const char* request = "GET / HTTP/1.1\x0D\x0AHost: www.baidu.com\x0D\x0A"
	"Connection: Close\x0D\x0A\x0D\x0A";
	testcase_nossl("www.baidu.com:80", request);
	return 0;
}

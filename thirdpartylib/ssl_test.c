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
* @file ssl_test.c
* @brief use libssl to implement ssl in https connection.
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
* @brief test case for ssl.
*
* @param host_and_port host_and_port string, end with https.
* @param request request string.
*/
void testcase_ssl(char* host_and_port, const char* request) {
	BIO* bio;
	ERR_load_BIO_strings();
	SSL_load_error_strings();
	OpenSSL_add_all_algorithms();
	SSL_library_init();

	SSL_CTX* ctx = SSL_CTX_new(SSLv23_client_method());
	SSL* ssl;
	if (!SSL_CTX_load_verify_locations(ctx, "TrustStore.pem", NULL)) {
		printf("SSL_LTX_load_verify_locations failed!\n");
		ERR_print_errors_fp(stderr);
		SSL_CTX_free(ctx);
		return;
	}
	bio = BIO_new_ssl_connect(ctx);
	BIO_get_ssl(bio, &ssl);
	SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
	BIO_set_conn_hostname(bio, host_and_port);
	if (BIO_do_connect(bio) <= 0) {
		printf("BIO_do_connect failed!\n");
		ERR_print_errors_fp(stderr);
		BIO_free_all(bio);
		SSL_CTX_free(ctx);
		return;
	}
	if (SSL_get_verify_result(ssl) != X509_V_OK) {
		printf("SSL_get_verify_result error with %ld.\n", 
			SSL_get_verify_result(ssl));
		ERR_print_errors_fp(stderr);
		BIO_free_all(bio);
		SSL_CTX_free(ctx);
		return;
	}
	BIO_write(bio, request, strlen(request));
	int p = 0; char r[1024] = {0};
	while ((p = BIO_read(bio, r, 1023)) > 0) {
		r[p] = '\0'; printf("%s", r);
	}
	BIO_free_all(bio);
	SSL_CTX_free(ctx);
}

int main(int argc, const char *argv[])
{
	#define HOST "www.verisign.com"
	const char* request = "GET / HTTP/1.1\x0D\x0AHost: "HOST"\x0D\x0A"
		"Connection: Close\x0D\x0A\x0D\x0A";
	testcase_ssl(HOST":https", request);
	return 0;
}

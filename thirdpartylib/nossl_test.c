#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

#include <stdio.h>
#include <string.h>

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
	while ((p = BIO_read(bio, r, 1023)) > 0) {
		r[p] = '\0'; printf("%s", r);
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

#include <stdio.h>
#include <time.h>

void print_tm(struct tm* ptm) {
	printf("ptm->tm_sec   = %d\n", ptm->tm_sec   );
	printf("ptm->tm_min   = %d\n", ptm->tm_min   );
	printf("ptm->tm_hour  = %d\n", ptm->tm_hour  );
	printf("ptm->tm_mday  = %d\n", ptm->tm_mday  );
	printf("ptm->tm_mon   = %d\n", ptm->tm_mon   );
	printf("ptm->tm_year  = %d\n", ptm->tm_year  );
	printf("ptm->tm_wday  = %d\n", ptm->tm_wday  );
	printf("ptm->tm_yday  = %d\n", ptm->tm_yday  );
	printf("ptm->tm_isdst = %d\n", ptm->tm_isdst );
}

int main(int argc, const char *argv[])
{
	time_t _t = time(NULL);
	printf("time(NULL) = %ld\n", _t);

	struct tm _tm;
	gmtime_r(&_t, &_tm);
	print_tm(&_tm);

//	struct tm* ptm = gmtime(&_t);
//	print_tm(ptm);

	return 0;
}

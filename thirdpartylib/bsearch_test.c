#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>

struct mi {
	int nr;
	char* name;
} months[] = {
	{1, "jan"}, {2, "feb"}, {3, "mar"}, {4, "apr"},
	{5, "may"}, {6, "jun"}, {7, "jul"}, {8, "aug"},
	{9, "sep"}, {10, "oct"}, {11, "nov"}, {12, "dec"}
};

#define nr_of_months (sizeof(months)/sizeof(months[0]))

static int compmi(const void* m1, const void* m2) {
	// return (((struct mi*)m1)->nr - ((struct mi*)m2)->nr);
	return strcmp(((struct mi*)m1)->name, ((struct mi*)m2)->name);
}

static void printmonths(void) {
	int i = 0, n = nr_of_months;
	for (i = 0; i < n; i ++) {
		if (0 != i) { printf(", "); }
		if (0 != i && 0 == i%4) { printf("\n"); }
		printf("{%d, '%s'}", months[i].nr, months[i].name);
	}
	printf("\n");
}

int main(int argc, const char *argv[])
{
	printf("[INFO] -- Before qsort: months:\n");
	printmonths();
	qsort(months, nr_of_months, sizeof(struct mi), compmi);
	printf("[INFO] -- After qsort: months:\n");
	printmonths();

	struct mi key, *res;
	key.name = "apx";
	res = bsearch(&key, months, nr_of_months, sizeof(struct mi), compmi);
	if (res == NULL) {
		printf("'%s': unknown month\n", key.name);
	} else {
		printf("'%s': month #%d\n", res->name, res->nr);
	}

	int n = nr_of_months;
	res = lfind(&key, months, (size_t *)&n, sizeof(struct mi), compmi);
	if (res == NULL) {
		printf("'%s': unknown month\n", key.name);
	} else {
		printf("'%s': month #%d\n", res->name, res->nr);
	}

	return 0;
}

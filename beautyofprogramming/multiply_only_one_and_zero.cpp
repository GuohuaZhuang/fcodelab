/* Copyright (C) 
* 2013 - firstboy0513
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
* @file multiply_only_one_and_zero.cpp
* @brief given a number N, find a number M to make N*M result to a number which
* is only make up by one and zero in decimal.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-18
*/
#include <cstdio>
#include <cstdlib>
#include <vector>

using std::vector;

/**
* @brief print out the vector<int> valud which is only make up by one and zero
* in decimal number. It just store ever 1 position, not zero in every element. 
* For example, 1001 just store 1 and 4 in the vector<int>.
*
* @param bigint the number M use store a vector<int>, it just store ever 1 
* position, not zero in every element. For example, 1001 just store 1 and 4 in 
* the vector<int>.
*/
void bigint_printout(vector<int>& bigint) {
	int i = 0, j = 0, size = (int)bigint.size();
	for (i = 0, j = 1; i < size; i ++) {
		while (j < bigint[i]) {
			printf("0"); j ++;
		}
		printf("1"); j ++;
	}
}

/**
* @brief given a number N, find a number M to make N*M result to a number which
* is only make up by one and zero in decimal.
* This method use a bigint array to store 0~N moduled value, let find just 
* K*(N-1) times, not the 2^k times.
*
* @param N a given number.
*
* @return find that number M use store a vector<int>, it just store ever 1 
* position, not zero in every element. For example, 1001 just store 1 and 4 in 
* the vector<int>.
*/
vector<int> multiply_only_one_and_zero_optimize(const int N) {
	vector< vector<int> > BigInt; BigInt.resize(N);
	int i = 0, j = 0, k = 0;
	for (i = 1, j = 10%N; ; i ++, j = (j*10)%N) {
		if (0 == BigInt[j].size()) {
			BigInt[j].push_back(i);
		}
		for (k = 1; k < N; k ++) {
			if (BigInt[k].size() > 0 && BigInt[(j+k)%N].size() == 0 
				&& i > BigInt[k][BigInt[k].size()-1]) {
				BigInt[(j+k)%N] = BigInt[k];
				BigInt[(j+k)%N].push_back(i);
			}
		}
		if (BigInt[0].size() > 0) { break; }
	}
	if (0 == BigInt[0].size()) {
		printf("[ERR] -- not find the M\n");
	}
	return BigInt[0];
}

int main(int argc, const char *argv[])
{
	const int N = 99;
	vector<int> bigint = multiply_only_one_and_zero_optimize(N);
	bigint_printout(bigint); printf("\n");
	return 0;
}

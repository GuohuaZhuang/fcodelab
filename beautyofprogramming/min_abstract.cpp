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
* @file min_abstract.cpp
* @brief Get minimum abstract use earthworm minimum abstract algorithm.
* @author firstboy0513
* @version 0.0.1
* @date 2013-11-29
*/
#include <cstdio>
#include <map>
#include <string>

using std::map;
using std::string;
using std::iterator;
typedef std::string ELE;

/**
* @brief Query words map.
*/
class QueryWordMap {
private:
	map<ELE, int> _map;
	int _require_num;
public:
	QueryWordMap(ELE Q[], int nLenQ) {
		_require_num = nLenQ;
		int i = 0;
		for (i = 0; i < _require_num; i ++) {
			_map[ Q[i] ] = 0;
		}
	}
	bool isAllExisted() {
		return (_require_num <= 0);
	}
	void addQueryWordIfExisted(ELE s) {
		map<ELE, int>::iterator it = _map.find(s);
		if (_map.end() == it) return;
		if (it->second == 0) {
			_require_num --;
		}
		it->second ++;
	}
	void removeQueryWordIfExisted(ELE s) {
		map<ELE, int>::iterator it = _map.find(s);
		if (_map.end() == it) return;
		it->second --;
		if (it->second <= 0) {
			_require_num ++;
		}
	}
};

/**
* @brief result to find the abstract and current scan window position.
*/
typedef struct _RET {
	int _abstractLen; // current minimum abstract length
	int _abstractStart; // minimum abstract start position
	int _abstractEnd; // minimum abstract end position (Note note include it)
	_RET() {
		_abstractLen = -1;
		_abstractStart = -1;
		_abstractEnd = -1;
	}
} RET;

/**
* @brief MinAbstract class.
*/
class MinAbstract {
public:
	/**
	* @brief Find minimum abstract, I call it earthworm minimum abstract 
	* algorithm. It use O(2*N*search(M)) == O(N) time complexity.
	*
	* @param S[] scan string word array.
	* @param nLenS scan string length.
	* @param Q[] query keyword array.
	* @param nLenQ query keyword length.
	*
	* @return return include getted minimum abstract information RET struct.
	*/
	static RET FindMinAbstract(ELE S[], int nLenS, ELE Q[], int nLenQ) {
		RET ret;
	    if (!S || nLenS <= 0 || !Q || nLenQ <= 0) {
	        printf("[ERR] -- FindMinAbstract input invalid args.\n");
	        return ret;
	    }
		QueryWordMap* qwm = new QueryWordMap(Q, nLenQ);
		ret._abstractLen = nLenS + 1;
		int pBegin = 0;
		int pEnd = 0;
		while (true) {
			while (!qwm->isAllExisted() && pEnd < nLenS) {
				qwm->addQueryWordIfExisted(S[pEnd]);
				pEnd ++;
			}
			while (qwm->isAllExisted()) {
				if (pEnd - pBegin < ret._abstractLen) {
					ret._abstractLen = pEnd - pBegin;
					ret._abstractStart = pBegin;
					ret._abstractEnd = pEnd;
				}
				qwm->removeQueryWordIfExisted(S[pBegin]);
				pBegin ++;
			}
			if (pEnd >= nLenS) break;
		}
		delete (qwm);
		return ret;
	}

	/**
	 * @brief Print abstract information.
	 *
	 * @param S[] scan string word array.
	 * @param ret include minimum abstract RET struct.
	 */
	static void PrintAbstract(ELE S[], RET ret) {
		int i = 0;
		printf("[ABSTRACT]: ");
		for (i = ret._abstractStart; i < ret._abstractEnd; i ++) {
			if (i != ret._abstractStart) {
				printf(", ");
			}
			printf("\"%s\"", S[i].data());
		}
		printf("\n");
	}
};

int main(int argc, const char *argv[])
{
	// init test string array and query word array
	ELE S[] = {"w0", "w1", "w2", "w3", "q0", "w4", "w5", "q1", 
		"w6", "w7", "w8", "q0", "w9", "q1"};
	int nLenS = sizeof(S)/sizeof(S[0]);
	ELE Q[] = {"q0", "q1"};
	int nLenQ = sizeof(Q)/sizeof(Q[0]);

	// find minimum abstract
	RET ret = MinAbstract::FindMinAbstract(S, nLenS, Q, nLenQ);

	// show RET result
	printf("RET _abstractLen = %d\n", ret._abstractLen);
	printf("RET _abstractStart = %d\n", ret._abstractStart);
	printf("RET _abstractEnd = %d\n", ret._abstractEnd);
	MinAbstract::PrintAbstract(S, ret);

	return 0;
}

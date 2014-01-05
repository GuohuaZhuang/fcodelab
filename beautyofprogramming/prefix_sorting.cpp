#include <iostream>
#include <cstdio>
#include <cassert>

class CPrefixSorting {
public:

	CPrefixSorting() {
		m_nCakeCnt = 0;
		m_nMaxSwap = 0;
	}
	~CPrefixSorting() {
		if (m_CakeArray != NULL) { delete[] m_CakeArray; }
		if (m_SwapArray != NULL) { delete[] m_SwapArray; }
		if (m_ReverseCakeArray != NULL) { delete[] m_ReverseCakeArray; }
		if (m_ReverseCakeArray != NULL) { delete[] m_ReverseCakeArraySwap; }
	}

	/**
	* @brief 计算烙饼翻转信息
	*
	* @param pCakeArray 存储烙饼索引数组
	* @param nCakeCnt 烙饼个数
	*/
	void Run(int* pCakeArray, int nCakeCnt) {
		Init(pCakeArray, nCakeCnt);
		m_nSearch = 0;
		Search(0);
	}

	/**
	* @brief 输出烙饼具体翻转次数
	*/
	void Output() {
		for (int i = 0; i < m_nMaxSwap; i ++) {
			printf("%d ", m_SwapArray[i]);
		}
		printf("\n |Search Times| : %d\n", m_nSearch);
		printf("Total Swap times = %d\n", m_nMaxSwap);
	}

private:
	/**
	* @brief 初奴化数组信息
	*
	* @param pCakeArray 存储烙饼索引数组
	* @param nCakeCnt 烙饼个数
	*/
	void Init(int* pCakeArray, int nCakeCnt) {
		assert(pCakeArray != NULL);
		assert(nCakeCnt > 0);

		m_nCakeCnt = nCakeCnt;

		// 初始化烙饼数组
		m_CakeArray = new int[m_nCakeCnt];
		assert(m_CakeArray != NULL);
		for (int i = 0; i < m_nCakeCnt; i ++) {
			m_CakeArray[i] = pCakeArray[i];
		}
		// 设置最多交换次数信息
		m_nMaxSwap = UpperBound(m_nCakeCnt);
		// 初始化交换结果数组
		m_SwapArray = new int[m_nMaxSwap + 1];
		assert(m_SwapArray != NULL);
		// 初始化中间交换结果信息
		m_ReverseCakeArray = new int[m_nCakeCnt];
		for (int i = 0; i < m_nCakeCnt; i ++) {
			m_ReverseCakeArray[i] = m_CakeArray[i];
		}
		m_ReverseCakeArraySwap = new int[m_nMaxSwap+1];
	}

	/**
	* @brief 寻找当前翻转的上界
	*/
	int UpperBound(int nCakeCnt) {
		return nCakeCnt * 2;
	}

	/**
	* @brief 寻找当前翻转的下界
	*/
	int LowerBound(int* pCakeArray, int nCakeCnt) {
		int t, ret = 0;
		for (int i = 1; i < nCakeCnt; i ++) {
			t = pCakeArray[i] - pCakeArray[i-1];
			if ((t == 1) || (t == -1)) { } else {
				ret ++;
			}
		}
		return ret;
	}

	/**
	* @brief 排序的主函数
	*/
	void Search(int step) {
		int i, nEstimate;
		m_nSearch ++;
		// 估算这次搜索所需要的最小交换次数
		nEstimate = LowerBound(m_ReverseCakeArray, m_nCakeCnt);
		if (step + nEstimate > m_nMaxSwap) { return; }
		// 如果已经排好序，即翻转完成，输出结果
		if (IsSorted(m_ReverseCakeArray, m_nCakeCnt)) {
			if (step < m_nMaxSwap) {
				m_nMaxSwap = step;
				for (i = 0; i < m_nMaxSwap; i ++) {
					m_SwapArray[i] = m_ReverseCakeArraySwap[i];
				}
			}
			return;
		}
		// 递归进行翻转
		for (i = 1; i < m_nCakeCnt; i ++) {
			Reverse(0, i);
			m_ReverseCakeArraySwap[step] = i;
			Search(step+1);
			Reverse(0, i);
		}
	}

	/**
	* @brief 判断是否已经排好序
	*
	* @return true; 已经排好序，false: 未排序
	*/
	bool IsSorted(int* pCakeArray, int nCakeCnt) {
		for (int i = 1; i < nCakeCnt; i ++) {
			if (pCakeArray[i-1] > pCakeArray[i]) {
				return false;
			}
		}
		return true;
	}

	/**
	* @brief 翻转烙饼信息
	*/
	void Reverse(int nBegin, int nEnd) {
		assert(nEnd > nBegin);
		int i, j, t;
		for (i = nBegin, j = nEnd; i < j; i ++, j --) {
			t = m_ReverseCakeArray[i];
			m_ReverseCakeArray[i] = m_ReverseCakeArray[j];
			m_ReverseCakeArray[j] = t;
		}
	}

private:
	int* m_CakeArray; // 烙饼信息数组
	int m_nCakeCnt; // 烙饼个数
	int m_nMaxSwap; // 最多交换次数。根据前面的推断，这里最多为m_nCakeCnt*2
	int* m_SwapArray; // 交换结果数组

	int* m_ReverseCakeArray; // 当前翻转烙饼信息数组
	int* m_ReverseCakeArraySwap; // 当前翻转烙饼交换结果数组
	int m_nSearch; // 当前搜索次数信息
};

int main(int argc, const char *argv[])
{
	int pCakeArray[] = {3, 2, 1, 6, 5, 4, 9, 8, 7, 0};
	int nCakeCnt = sizeof(pCakeArray) / sizeof(pCakeArray[0]);
	CPrefixSorting ps;
	ps.Run(pCakeArray, nCakeCnt);
	ps.Output();
	return 0;
}

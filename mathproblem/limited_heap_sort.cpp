/* Copyright (C) 
* 2016 - firstboy0513
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
* @file limited_heap_sort.cpp
* @brief limited heap sort tree implement in cplusplus. This example is limited 140 elements.
*   use to get top 140 elements.
* complie:
*   g++ -g3 -O0 -Wall limited_heap_sort.cpp -lbz2 -o limited_heap_sort
* @author firstboy0513
* @version 0.0.1
* @date 2016-03-03
*/

#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/**
 * @brief const of top limited count.
 */
#define ARR_LEN 140

/**
 * @brief sort element class.
 */
typedef class Element {
    public:
    long m_id1;
    long m_id2;
    double m_score;
    Element() {
        m_id1 = 0L;
        m_id2 = 0L;
        m_score = 0.0;
    }
    /**
     * @brief constructor use to load 'id1\tid2\score' format text line.
     */
    Element(string& s) {
        m_id1 = 0L;
        m_id2 = 0L;
        m_score = 0.0;

        const char* tmp_p = s.data();
        m_id1 = atol(tmp_p);

        while (*tmp_p != '\0' && *tmp_p != '\t') {
            tmp_p++;
        }
        if ('\0' == *tmp_p) {
            fprintf(stderr, "[ERR] -- [%s] is invalid line\n", s.data());
            return;
        }
        tmp_p++;
        m_id2 = atol(tmp_p);

        while (*tmp_p != '\0' && *tmp_p != '\t') {
            tmp_p++;
        }
        if ('\0' == *tmp_p) {
            fprintf(stderr, "[ERR] -- [%s] is invalid line\n", s.data());
            return;
        }
        tmp_p++;
        m_score = atof(tmp_p);
    }
    Element(const Element& e) {
        this->m_id1 = e.m_id1;
        this->m_id2 = e.m_id2;
        this->m_score = e.m_score;
    }
    Element& operator=(Element const& e) {
        this->m_id1 = e.m_id1;
        this->m_id2 = e.m_id2;
        this->m_score = e.m_score;
        return *this;
    }
    inline bool operator!=(const Element& e) const {
        return (this->m_id1 != e.m_id1);
    }
    inline bool operator>(const Element& e) const {
        return (this->m_score > e.m_score);
    }
    inline bool operator<(const Element& e) const {
        return (this->m_score < e.m_score);
    }
    inline bool operator>=(const Element& e) const {
        return (this->m_score >= e.m_score);
    }
    inline bool operator<=(const Element& e) const {
        return (this->m_score <= e.m_score);
    }
    /**
     * @brief swap element.
     */
    inline void swap(Element& e) {
        Element t(*this);
        this->m_id1 = e.m_id1;
        this->m_id2 = e.m_id2;
        this->m_score = e.m_score;
        e.m_id1 = t.m_id1;
        e.m_id2 = t.m_id2;
        e.m_score = t.m_score;
    }
} ELE;

/**
 * @brief min top heap sort class.
 */
class MinHeapSort {
    public:
        ELE m_arr[ARR_LEN];
        int m_size;
    public:
        MinHeapSort() {
            m_size = 0;
        }
        ~MinHeapSort() {
            m_size = 0;
        }
        void reset() {
            m_size = 0;
        }
        /**
         * @brief adjust heap from start root node to end leaf node.
         */
        void adjust(int size, int start) {
            int j = 0;
            ELE pivot = m_arr[start];
            for (j = ((start << 1) + 1); j < size; j = ((j << 1) + 1)) {
                if (j < (size - 1) && m_arr[j] > m_arr[j+1]) {
                    j++;
                }
                if (m_arr[j] >= pivot) {
                    break;
                }
                m_arr[start] = m_arr[j];
                start = j;
            }
            m_arr[start] = pivot;
        }
        /**
         * @brief adjust heap from start leaf node to end root node.
         */
        void radjust(int start) {
            int j = 0;
            ELE pivot = m_arr[start];
            for (j = ((start - 1) >> 1); j > 0; j = ((j - 1) >> 1)) {
                if (m_arr[j] <= pivot) {
                    break;
                }
                m_arr[start] = m_arr[j];
                start = j;
            }
            m_arr[start] = pivot;
        }
        /**
         * @brief append new element.
         */
        void append(const ELE& e) {
            if (m_size >= ARR_LEN) {
                if (m_arr[0] > e) {
                    return;
                }
                m_arr[0] = e;
                adjust(m_size, 0);
            } else {
                m_arr[m_size] = e;
                m_size++;
                radjust(m_size - 1);
            }
        }
        /**
         * @brief sort whole array.
         */
        void sort() {
            int i = 0;
            for (i = (m_size - 1); i > 0; i--) {
                m_arr[0].swap(m_arr[i]);
                adjust(i, 0);
            }
        }
        /**
         * @brief output array to stdout from 0 to end index, it is descending sort.
         */
        void output(ostream& os = std::cout) {
            int i = 0;
            if (m_size <= 0) {
                return;
            }
            os << m_arr[0].m_id1 << '\t';
            for (i = 0; i < m_size; i++) {
                if (0 != i) {
                    os << ',';
                }
                os << m_arr[i].m_id2;
            }
            os << "\n";
        }
};

int main(int argc, char* argv[]) {
    /**
     * single argument is the filepath, and it require 'id1\tid2\tscore' format.
     */
    if (argc < 2) {
        fprintf(stderr, "[ERR] -- please input filename.\n");
        return -1;
    }
    const char* fname = argv[1];
    ifstream ifs(fname, ifstream::in);

    /**
     * this method require the id1 is sorted, the same id is stack together.
     */
    ELE pre_e;
    MinHeapSort mh;
    string line;
    while (getline(ifs, line)) {
        ELE cur_e(line);
        if (pre_e != cur_e && pre_e.m_id1 != 0L) {
            mh.sort();
            mh.output();
            mh.reset();
        }
        mh.append(cur_e);
        pre_e = cur_e;
    }
    mh.sort();
    mh.output();

    ifs.close();
    return 0;
}


/*
 * =========================================================================================
 * Name        : listLib.h
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
 * Description : library for Assignment 1 - Data structures and Algorithms - Spring 2018
 * =========================================================================================
 */

#ifndef A01_LISTLIB_H
#define A01_LISTLIB_H

#include <string>
using namespace std;

template <class T>
struct L1Item {
    T data;
    L1Item<T> *pNext;
    L1Item(): pNext(NULL) {}
    L1Item(L1Item<T>* p): pNext(p) {}
    L1Item(T &a): data(a), pNext(NULL) {}
    L1Item(T &&a): data(std::move(a)), pNext(NULL) {}
};

template <class T>
class L1List {
    L1Item<T> *pHead;
    int size;
public:
    L1List() {
		pHead = NULL;
		size = 0;
	}

	~L1List() {
		clean();
	}

	void clean() {
		while (pHead) removeHead();
	}

    bool isEmpty() {
        return pHead == NULL;
    }

    int getSize() {
        return size;
    }

	L1Item<T> *getHead() {
		return pHead;
	}

	T& at(int i) {
		if (i < 0 || i > size - 1) {
			throw "Outbound index";
		}
		else {
			L1Item<T> *pCurrent = pHead;
			for (int iCount = 0; iCount < i; iCount++) pCurrent = pCurrent->pNext;
			return pCurrent->data;
		}
	}

	T& operator[](int i) {
		if (i < 0 || i > size - 1) {
			throw "Outbound index";
		}
		else {
			L1Item<T> *pCurrent = pHead;
			for (int iCount = 0; iCount < i; iCount++) pCurrent = pCurrent->pNext;
			return pCurrent->data;
		}
	}

	bool find(T& a, int& idx) {
		if (size == 0) return false;
		else {
			idx = 0;
			L1Item<T> *pCurrent = pHead;
			bool flag = false;
			while (pCurrent) {
				if (pCurrent->data == a) {
					flag = true;
					break;
				}
				pCurrent = pCurrent->pNext;
			}
			return flag;
		}
	}

	L1List<T> *filter(char *id) {
		L1List<T> *pResult = new L1List<T>();
		L1Item<T> *pCurrent = pHead;
		while (pCurrent) {
			if (strcmp(pCurrent->data.id, id) == 0) {
				pResult->insert(pCurrent->data);
			}
			pCurrent = pCurrent->pNext;
		}
		return pResult;
	}

	int insert(T& a) {
		L1Item<T> *pCurrent = pHead;
		L1Item<T> *pPrevious = NULL;
		while (pCurrent && pCurrent->data.timestamp < a.timestamp) {
			pPrevious = pCurrent;
			pCurrent = pCurrent->pNext;
		}
		if (pPrevious == NULL) return insertHead(a);
		else if (pCurrent == NULL) return push_back(a);
		else {
			L1Item<T> *pNew = new L1Item<T>(a);
			pPrevious->pNext = pNew;
			pNew->pNext = pCurrent;
			size++;
			return 0;
		}
	}

	int remove(int i) {
		if (i < 0 || i > size - 1) return -1;
		else if (i == 0) return removeHead();
		else if (i == size - 1) return removeLast();
		else {
			L1Item<T> *pPrevious = NULL;
			L1Item<T> *pCurrent = pHead;
			for (int iCount = 0; iCount < i; iCount++) {
				pPrevious = pCurrent;
				pCurrent = pCurrent->pNext;
			}
			pPrevious->pNext = pCurrent->pNext;
			pCurrent->pNext = NULL;
			delete pCurrent;
			return 0;
		}
	}

	int push_back(T& a) {
		if (pHead == NULL) {
			pHead = new L1Item<T>(a);
		}
		else {
			L1Item<T> *p = pHead;
			while (p->pNext) p = p->pNext;
			p->pNext = new L1Item<T>(a);
		}
		size++;
		return 0;
	}

	int insertHead(T& a) {
		L1Item<T> *p = new L1Item<T>(a);
		p->pNext = pHead;
		pHead = p;
		size++;
		return 0;
	}

    int insertHead() {
        pHead = new L1Item<T>(pHead);
        size++;
        return 0;
    }

	int removeHead() {
		if (pHead) {
			L1Item<T>* p = pHead;
			pHead = p->pNext;
			delete p;
			size--;
			return 0;
		}
		return -1;
	}

	int removeLast() {
		if (pHead) {
			if (pHead->pNext) {
				L1Item<T>* prev = pHead;
				L1Item<T>* pcur = prev->pNext;
				while (pcur->pNext) {
					prev = pcur;
					pcur = pcur->pNext;
				}
				delete pcur;
				prev->pNext = NULL;
			}
			else {
				delete pHead;
				pHead = NULL;
			}
			size--;
			return 0;
		}
		return -1;
	}

	void reverse() {
		L1Item<T> *pPrevious = NULL;
		L1Item<T> *pCurrent = pHead;
		L1Item<T> *pNext = NULL;
		while (pCurrent != NULL)
		{
			pNext = pCurrent->pNext;
			pCurrent->pNext = pPrevious;
			pPrevious = pCurrent;
			pCurrent = pNext;
		}
		pHead = pPrevious;
	}

    void traverse(void (*op)(T&)) {
        L1Item<T> *p = pHead;
        while (p) {
            op(p->data);
            p = p->pNext;
        }
    }

    void traverse(void (*op)(T&, void*), void* pParam) {
        L1Item<T> *p = pHead;
        while (p) {
            op(p->data, pParam);
            p = p->pNext;
        }
    }

	bool operator==(L1List<T> &b) {
		return pHead->data == b.getHead()->data;
	}
};

template <class T>
struct L2Item {
	char *cId = new char[12];
	L1List<T> *pList = new L1List<T>();
	L2Item<T> *pNext;
	L2Item(char *cNewId, L1List<T> *pSource) {
		strcpy(cId, cNewId);
		pList = pSource->filter(cNewId);
		pNext = NULL;
	}
	bool operator==(L2Item<T> &target) {
		return strcmp(cId, target.cId) == 0;
	}
};
#endif

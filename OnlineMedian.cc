#include <iostream>
#include <math.h>
#include <vector>
using namespace std;


class Heap {
protected:
	vector<int> v;
public:
	int parent(int);
	int leftchild(int);
	int rightchild(int);
	void swap(int, int);
	virtual void bubble_up() = 0;
	virtual void bubble_down() = 0;
	void insert(int);
	int remove();
	int getExtreme();
	void output();
};

int Heap::parent(int i) {
	if (i == 0) {
		return -1;
	} else {
		return floor((i-1)/2);
	}
}

int Heap::leftchild(int i) {
	return i*2+1;
}

int Heap::rightchild(int i) {
	return i*2+2;
}

void Heap::swap(int i, int j) {
	int tmp = v.at(i);
	v.at(i) = v.at(j);
	v.at(j) = tmp;
}

void Heap::insert(int value) {
	v.push_back(value);
	bubble_up();
}

int Heap::remove() {
	int i = v.size()-1;
	swap(i,0);
	int value = v.back();
	v.pop_back();
	bubble_down();
	return value;
}

int Heap::getExtreme() {
	return v.at(0);
}

void Heap::output() {
	for (int i = 0; i < v.size(); ++i) {
		cout << i << "th element is " << v.at(i) << endl;
	}
}

class MaxHeap : public Heap {
public:
	void bubble_up();
	void bubble_down();

};

void MaxHeap::bubble_up() {
	int i = v.size()-1;
	int p = parent(i);
	while ((p >= 0) && (i >= 0) && (v.at(p) < v.at(i))) {
		swap(i, p);
		i = p;
		p = parent(i);
	}
}

void MaxHeap::bubble_down() {
	int p = 0;
	while (true) {
		int left = leftchild(p);
		int right = rightchild(p);
		int len = v.size();
		int max = p;

		if (left < len && v.at(left) > v.at(max)) {
			max = left;
		} 
		if (right < len && v.at(right) > v.at(max)) {
			max = right;
		}
		if (max != p) {
			swap(max,p);
			p = max;
		} else {
			break;
		}
	}
}



class MinHeap : public Heap {
public:
	void bubble_up();
	void bubble_down();
};

void MinHeap::bubble_up() {
	int i = v.size()-1;
	int p = parent(i);
	while ((p >= 0) && (i >= 0) && (v.at(p) > v.at(i))) {
		swap(i, p);
		i = p;
		p = parent(i);
	}
}

void MinHeap::bubble_down() {
	int p = 0;
	while (true) {
		int left = leftchild(p);
		int right = rightchild(p);
		int len = v.size();
		int min = p;

		if (left < len && v.at(left) < v.at(min)) {
			min = left;
		} 
		if (right < len && v.at(right) < v.at(min)) {
			min = right;
		}
		if (min != p) {
			swap(min,p);
			p = min;
		} else {
			break;
		}
	}
}



// class to compute median while adding and deleting medians

class OnlineMedian {
private:
	int len;        // keep track of the length of the sequence
	MaxHeap *low;		// MaxHeap which contain floor(len/2) smallest elements
	MinHeap *high;		// MinHeap which contain ceil(len/2) largest elements
public:
	OnlineMedian();
	~OnlineMedian();

	int size(); 
	void insert(int x);
	int getMedian();
	int deleteMedian();
};

OnlineMedian::OnlineMedian() {
// post: initializes empty structure
	len = 0;
	low = new MaxHeap;
	high = new MinHeap;
}

OnlineMedian::~OnlineMedian() {
	delete low;
	delete high;
}

int OnlineMedian::size() {
// post: returns number of items in the structure
	return len;
}

void OnlineMedian::insert(int x) {
// pre: x is different from all items in the structure
// post: x is stored in the structure
	if (len == 0) { 
		low->insert(x); 
	} else if (len == 1) {
		if (low->getExtreme() > x) {
			high->insert(low->remove());
			low->insert(x);
		} else {
			high->insert(x);
		}
	} else {
		if (len%2 == 0) { // even len, increase size of low
			if (low->getExtreme() < x) {
				low->insert(high->remove());
				high->insert(x);
			} else {
				low->insert(x);
			}
		} else { // odd len, increase size of high
			if (high->getExtreme() > x) {
				high->insert(low->remove());
				low->insert(x);
			} else {
				high->insert(x);
			}
		}
	}
	++len;
}

int OnlineMedian::getMedian() {
// pre: size > 0
// post: returns median of all inserted numbers
	return low->getExtreme();
}

int OnlineMedian::deleteMedian() {
// pre: size > 0
// post: deletes and returns median of all inserted numbers
	int ret;
	if (len%2 == 0) { // even len, decrease size of high
		ret = low->remove();
		low->insert(high->remove());
	} else { // odd len, decrease size of low
		ret = low->remove();
	}
	--len;
	return ret;
}


#ifndef TESTING
int main() { 

	OnlineMedian * h = new OnlineMedian();

	/* Testing with some nums */
	h->insert(15);
	cout << "current median is " << h->getMedian() << endl;
	h->insert(10);
	cout << "current median is " << h->getMedian() << endl;
	h->insert(1);
	cout << "current median is " << h->getMedian() << endl;
	h->insert(20);
	cout << "current median is " << h->getMedian() << endl;
	h->insert(30);
	cout << "current median is " << h->getMedian() << endl;

	cout << "STARTING DELETING" << endl;
	cout << "current median is " << h->deleteMedian() << endl;
	cout << "current median is " << h->deleteMedian() << endl;
	cout << "current median is " << h->deleteMedian() << endl;
	cout << "current median is " << h->deleteMedian() << endl;
	cout << "current median is " << h->deleteMedian() << endl;

	delete h;
}
#endif

#include <iostream>
#include <random>
#include <string>
#include <chrono>
#include <list>

class AVLtree {
public:
	void add(int v) {
		v = transform(v);
		mTree = insert(mTree, v);
	}

	void del(int v) {
		v = transform(v);
		mTree = remove(mTree, v);
	}

	bool find(int v) {
		v = transform(v);

		node* tmp = mTree;
		while (tmp) {
			if (tmp->key == v) {
				return true;
			}

			if (tmp->key < v) {
				tmp = tmp->right;
			}
			else {
				tmp = tmp->left;
			}
		}

		return false;
	}

	long long int sum(int l, int r) {
		l = transform(l);
		r = transform(r);
		long long int s = _sum(mTree, l, r);
		mLastSum = s;
		return s;
	}

private:
	struct node // структура для представления узлов дерева
	{
		int key;
		int height;
		node* left;
		node* right;
		node* parent;
		long long int sum;
		node(int k) { key = k; sum = k; parent = left = right = 0; height = 1; }
	};

	long long int sum(node* p) {
		if (!p) return 0;
		return p->sum;
	}

	node* updateSum(node* p) {
		if (!p) return 0;
		p->sum = p->key;
		p->sum += sum(p->left);
		p->sum += sum(p->right);
		return p;
	}

	int height(node* p)
	{
		return p ? p->height : 0;
	}

	node* updateHeight(node* p)
	{
		if (!p) { return nullptr; }
		int hl = height(p->left);
		int hr = height(p->right);
		p->height = (hl > hr ? hl : hr) + 1;
		return p;
	}

	node* rotateRight(node* p) // правый поворот вокруг p
	{
		node* l = p->left;
		node* lr = l ? l->right : nullptr;
		node* ll = l ? l->left : nullptr;
		int dif = height(lr) - height(ll);

		if (!dif) {

			return p;
		}

		if (dif > 0) {


			//std::cout << "BIG RIGHT" << std::endl;
			return bigRotateRight(p);
		}
		else {
			//std::cout << "SMALL RIGHT" << std::endl;
			return smallRotateRight(p);
		}
	}

	node* rotateLeft(node* p) {
		node* r = p->right;
		node* rr = r ? r->right : nullptr;
		node* rl = r ? r->left : nullptr;
		int dif = height(rl) - height(rr);

		if (!dif) {

			return p;
		}

		if (dif > 0) {

			return bigRotateLeft(p);
			
		}
		else {
			return smallRotateLeft(p);
		}
	}

	node* smallRotateRight(node* p) {
		node* l = p->left;
		p->left = l->right;
		l->right = p;

		updateSum(l->left);
		updateSum(l->right);
		updateSum(l);

		return l;
	};

	node* bigRotateRight(node* p) {
		node* l = p->left;
		node* lr = l->right;
		
		p->left = lr->right;
		l->right = lr->left;
		lr->left = l;
		lr->right = p;

		updateSum(lr->right);
		updateSum(lr->left);
		updateSum(lr);

		return lr; // Новая вершина текущего дерева
	}

	node* smallRotateLeft(node* p) {
		node* r = p->right;
		node* l = p->left;

		p->right = r->left;
		r->left = p;

		updateSum(r->left);
		updateSum(r->right);
		updateSum(r);

		return r;
	}

	node* bigRotateLeft(node* p){

		node* r = p->right;
		node* rl = r->left;

		p->right = rl->left;
		r->left = rl->right;
		rl->right = r;
		rl->left = p;

		updateSum(rl->right);
		updateSum(rl->left);
		updateSum(rl);

		return rl; // Новая вершина текущего дерева
	}

	node* balance(node* p) // балансировка узла p
	{
		updateHeight(p);
		updateSum(p);
		node* left = p->left;
		node* right = p->right;
		int hLeft = height(left);
		int hRight = height(right);
		int dif = hLeft - hRight;
		if (dif >= 2) {
			p = rotateRight(p);
		}
		else if (dif <= -2) {
			p = rotateLeft(p);
		}
		return p;
	}

	node* insert(node* p, int k)
	{
		if (!p) {
			return new node(k);
		}

		if (k == p->key) { return p; }

		if (k < p->key)
			p->left = insert(p->left, k);
		else
			p->right = insert(p->right, k);

		return balance(p);
	}

	node* remove(node* p, int k) {
		if (!p) return nullptr;
		if (k < p->key)
			p->left = remove(p->left, k);
		else if (k > p->key)
			p->right = remove(p->right, k);
		else {

			node* l = p->left;
			node* r = p->right;
			delete p;
			if (!r) {
				return updateSum(l);
			};
			node* min = findMin(r);
			min->right = removeMin(r);
			min->left = l;

			min->left = updateSum(min->left);
			min->right = updateSum(min->right);
			min = updateSum(min);

			return balance(min);
		}

		return balance(p);
	}

	node* findMin(node* p){
		return p->left ? findMin(p->left) : p;
	}

	node* removeMin(node* p){
		if (p->left == 0) {
			return p->right;
		}
			
		p->left = removeMin(p->left);

		updateSum(p->left);
		updateSum(p->right);
		updateSum(p);

		return balance(p);
	}

	node* findNode(node* p, int v){
		if (!p) { return nullptr; }
	
		if (p->key == v) {
			return p;
		}

		if (p->key < v) {
			return findNode(p->right, v);
		}

		return findNode(p->left, v);
	}

	long long int _sum(node* n, int min, int max) {
		if (!n) {
			return 0;
		}

		if (min > max) return 0;

		node* tmp = n;

		while (tmp) {
			if (tmp->key > max)
				tmp = tmp->left;
			else if (tmp->key < min)
				tmp = tmp->right;
			else
				break;
		}

		

		if (tmp) {

			if (min == max) { return tmp->key; }

			if (tmp->key == min) {
				long long int s = tmp->sum;
				s -= sum(tmp->left);
				s -= rightSum(tmp->right, max);
				return s;
			}

			if (tmp->key == max) {
				long long int s = tmp->sum;
				s -= sum(tmp->right);
				s -= leftSum(tmp->left, min);
				return s;
			}

			long long int s = tmp->sum;
			long long int l = leftSum(tmp->left, min);
			long long int r = rightSum(tmp->right, max);;
			s = s - l - r;
			return s;
		}

		return 0;
	}

	long long int leftSum(node* p, int min) {
		if (!p) return 0;
		long long int s = sum(p);
		node* tmp = p;
		while (tmp) {
			if (tmp->key == min) {
				s -= (tmp->key + sum(tmp->right));
				break;
			}
			else if (tmp->key < min) {
				tmp = tmp->right;
			}
			else {
				s -= (tmp->key + sum(tmp->right));
				tmp = tmp->left;
			}
		}
		return s;
	}

	long long int rightSum(node* p, int max) {
		if (!p) return 0;
		long long int s = sum(p);
		node* tmp = p;
		while (tmp) {
			if (tmp->key == max) {
				s -=  (tmp->key + sum(tmp->left));
				break;
			}
			else if (tmp->key > max) {
				tmp = tmp->left;
			}
			else {
				s -= (tmp->key + sum(tmp->left));
				tmp = tmp->right;
			}
		}
		return s;

	}

	long long int transform(int v) {
		return (v + mLastSum) % 1000000001;
	}

private:
	node* mTree = nullptr;
	long long int mLastSum = 0;
};

using namespace std;

vector<string> split(string str) {
	string temp;
	vector<string> vec;

	for (auto it = str.begin(); it != str.end(); it++) {
		if (*it == ' ') {
			if (temp != "") { vec.push_back(temp); }
			temp = "";
			continue;
		}
		temp += *it;
	}
	vec.push_back(temp);
	return vec;
}

int main() {
	AVLtree s;
	vector<string> request;
	vector<string> requests;
	string input;
	int count;

	getline(cin, input);
	count = stoi(input);

	for (int i = 0; i < count; i++) {
		getline(cin, input);
		requests.push_back(input);
	}

	for (auto i = requests.begin(); i != requests.end(); i++) {
		request = split(*i);
		if (request[0] == "?") {
			try {
				int value = stoi(request[1]);
				if (s.find(value)) {
					cout << "Found" << endl;
				}
				else {
					cout << "Not found" << endl;
				}
				continue;
			}
			catch (exception e) {
				continue;
			}
		}

		if (request[0] == "+") {
			try {
				long int value = stoi(request[1]);
				s.add(value);
				continue;
			}
			catch (exception e) {
				continue;
			}
		}

		if (request[0] == "-") {
			try {
				long int value = stoi(request[1]);
				s.del(value);
				continue;
			}
			catch (exception e) {
				continue;
			}
		}

		if (request[0] == "s") {
			try {
				int left = stoi(request[1]);
				int right = stoi(request[2]);
				cout << s.sum(left, right) << endl;
				continue;
			}
			catch (exception e) {
				continue;
			}
		}
	}
	return 0;
}
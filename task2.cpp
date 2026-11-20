// BSCS-15-D
// Shivam Kumar
// 555585

#include <iostream>
#include <algorithm>

using namespace std;

struct Node {
	int val;
	Node* left;
	Node* right;
	int h;

	Node(int x) {
		val = x;
		left = nullptr;
		right = nullptr;
		h = 1;
	}
};

// null has height 0
int getH(Node* n) {
	if (n == nullptr) {
		return 0;
	}
	return n->h;
}

// balance factor is left height minus right height
int getBF(Node* n) {
	if (n == nullptr) {
		return 0;
	}
	return getH(n->left) - getH(n->right);
}

// update node height from its children
void fixH(Node* n) {
	if (n != nullptr) {
		n->h = 1 + (max)(getH(n->left), getH(n->right));
	}
}

// right rotation for left heavy tree
Node* rightRotate(Node* y) {
	Node* x = y->left;
	Node* t2 = x->right;

	// pivot around x
	x->right = y;
	y->left = t2;

	// update old root first then new root
	fixH(y);
	fixH(x);

	return x;
}

// left rotation for right heavy tree
Node* leftRotate(Node* x) {
	Node* y = x->right;
	Node* t2 = y->left;

	// pivot around y
	y->left = x;
	x->right = t2;

	// update old root first then new root
	fixH(x);
	fixH(y);

	return y;
}

// insert into avl and rebalance on the way back up
Node* insertAVL(Node* n, int x) {
	// normal bst insert at empty spot
	if (n == nullptr) {
		return new Node(x);
	}

	if (x < n->val) {
		n->left = insertAVL(n->left, x);
	}
	else if (x > n->val) {
		n->right = insertAVL(n->right, x);
	}
	else {
		// ignore duplicates
		return n;
	}

	// refresh height after child insert
	fixH(n);

	int bf = getBF(n);

	// ll case
	if (bf > 1 && x < n->left->val) {
		return rightRotate(n);
	}

	// rr case
	if (bf < -1 && x > n->right->val) {
		return leftRotate(n);
	}

	// lr case
	if (bf > 1 && x > n->left->val) {
		n->left = leftRotate(n->left);
		return rightRotate(n);
	}

	// rl case
	if (bf < -1 && x < n->right->val) {
		n->right = rightRotate(n->right);
		return leftRotate(n);
	}

	return n;
}

void showPreorder(Node* n) {
	if (n == nullptr) {
		return;
	}
	cout << n->val << " ";
	showPreorder(n->left);
	showPreorder(n->right);
}

void showInorder(Node* n) {
	if (n == nullptr) {
		return;
	}
	showInorder(n->left);
	cout << n->val << " ";
	showInorder(n->right);
}

// check that every node has bf in [-1, 0, 1]
bool checkAllBalanced(Node* n) {
	if (n == nullptr) {
		return true;
	}
	int bf = getBF(n);
	if (bf < -1 || bf > 1) {
		return false;
	}
	return checkAllBalanced(n->left) && checkAllBalanced(n->right);
}

void wipeTree(Node* n) {
	if (n == nullptr) {
		return;
	}
	wipeTree(n->left);
	wipeTree(n->right);
	delete n;
}

void runSeq(const char* label, const char* info, int arr[], int len) {
	Node* top = nullptr;
	for (int i = 0; i < len; i++) {
		top = insertAVL(top, arr[i]);
	}

	cout << label << " " << info << "\n";
	cout << "preorder ";
	showPreorder(top);
	cout << "\n";
	cout << "inorder ";
	showInorder(top);
	cout << "\n";
	cout << "all balanced " << (checkAllBalanced(top) ? "yes" : "no") << "\n\n";

	wipeTree(top);
}

int main() {
	int a[] = { 30, 20, 10 };
	int b[] = { 10, 20, 30 };
	int c[] = { 30, 10, 20 };
	int d[] = { 10, 30, 20 };

	runSeq("a", "ll right rotate", a, 3);
	runSeq("b", "rr left rotate", b, 3);
	runSeq("c", "lr left then right rotate", c, 3);
	runSeq("d", "rl right then left rotate", d, 3);

	return 0;
}

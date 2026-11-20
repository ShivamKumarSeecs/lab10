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

int getH(Node* n) {
	if (n == nullptr) {
		return 0;
	}
	return n->h;
}

int getBF(Node* n) {
	if (n == nullptr) {
		return 0;
	}
	return getH(n->left) - getH(n->right);
}

void fixH(Node* n) {
	if (n != nullptr) {
		n->h = 1 + (max)(getH(n->left), getH(n->right));
	}
}

Node* rightRotate(Node* y) {
	Node* x = y->left;
	Node* t2 = x->right;

	x->right = y;
	y->left = t2;

	fixH(y);
	fixH(x);

	return x;
}

Node* leftRotate(Node* x) {
	Node* y = x->right;
	Node* t2 = y->left;

	y->left = x;
	x->right = t2;

	fixH(x);
	fixH(y);

	return y;
}

// rebalance a non-null node using child balance factors
Node* rebalanceNode(Node* n) {
	if (n == nullptr) {
		return nullptr;
	}

	fixH(n);
	int bf = getBF(n);

	// left heavy
	if (bf > 1) {
		if (getBF(n->left) >= 0) {
			return rightRotate(n);
		}
		else {
			n->left = leftRotate(n->left);
			return rightRotate(n);
		}
	}

	// right heavy
	if (bf < -1) {
		if (getBF(n->right) <= 0) {
			return leftRotate(n);
		}
		else {
			n->right = rightRotate(n->right);
			return leftRotate(n);
		}
	}

	return n;
}

Node* insertAVL(Node* n, int x) {
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
		return n;
	}

	return rebalanceNode(n);
}

Node* getMin(Node* n) {
	while (n != nullptr && n->left != nullptr) {
		n = n->left;
	}
	return n;
}

Node* deleteAVL(Node* n, int x) {
	if (n == nullptr) {
		return nullptr;
	}

	if (x < n->val) {
		n->left = deleteAVL(n->left, x);
	}
	else if (x > n->val) {
		n->right = deleteAVL(n->right, x);
	}
	else {
		// 0 or 1 child
		if (n->left == nullptr) {
			Node* next = n->right;
			delete n;
			return next;
		}
		if (n->right == nullptr) {
			Node* next = n->left;
			delete n;
			return next;
		}

		// 2 children: copy inorder successor and delete it
		Node* succ = getMin(n->right);
		n->val = succ->val;
		n->right = deleteAVL(n->right, succ->val);
	}

	if (n == nullptr) {
		return nullptr;
	}

	return rebalanceNode(n);
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

void showStats(Node* n) {
	if (n == nullptr) {
		return;
	}
	cout << "(key=" << n->val << ", h=" << getH(n) << ", bf=" << getBF(n) << ") ";
	showStats(n->left);
	showStats(n->right);
}

void wipeTree(Node* n) {
	if (n == nullptr) {
		return;
	}
	wipeTree(n->left);
	wipeTree(n->right);
	delete n;
}

int main() {
	Node* top = nullptr;
	int keys[] = { 30, 20, 40, 10, 25, 50, 5 };

	for (int x : keys) {
		top = insertAVL(top, x);
	}

	cout << "initial preorder: ";
	showPreorder(top);
	cout << "\ninitial inorder:  ";
	showInorder(top);
	cout << "\nstats: ";
	showStats(top);
	cout << "\n\n";

	// delete 50 -> imbalance at 30 (bf=+2), left child 20 has bf=+1 -> single rightRotate(30)
	cout << "delete 50 (single rightRotate at 30)\n";
	top = deleteAVL(top, 50);
	cout << "preorder: ";
	showPreorder(top);
	cout << "\ninorder:  ";
	showInorder(top);
	cout << "\nstats: ";
	showStats(top);
	cout << "\n\n";

	// delete 40 -> no rotation needed
	cout << "delete 40 (no rotation needed)\n";
	top = deleteAVL(top, 40);
	cout << "preorder: ";
	showPreorder(top);
	cout << "\ninorder:  ";
	showInorder(top);
	cout << "\nstats: ";
	showStats(top);
	cout << "\n\n";

	// missing key test
	cout << "delete missing key 99\n";
	top = deleteAVL(top, 99);
	cout << "inorder after 99: ";
	showInorder(top);
	cout << "\n\n";

	wipeTree(top);
	top = nullptr;

	// empty tree test
	cout << "delete from empty tree -> nullptr? "
		<< (deleteAVL(top, 10) == nullptr ? "yes" : "no") << "\n\n";

	// single node tree test
	top = insertAVL(top, 42);
	top = deleteAVL(top, 42);
	cout << "delete single node 42 -> nullptr? "
		<< (top == nullptr ? "yes" : "no") << "\n\n";

	// heavy child bf == 0 test: insert 20, 10, 30, 5, 15 then delete 30
	Node* bfZeroTree = nullptr;
	int demo[] = { 20, 10, 30, 5, 15 };
	for (int x : demo) {
		bfZeroTree = insertAVL(bfZeroTree, x);
	}
	cout << "before delete 30 (bf(10)=0): ";
	showStats(bfZeroTree);
	cout << "\n";
	bfZeroTree = deleteAVL(bfZeroTree, 30);
	cout << "after delete 30 (single rightRotate): ";
	showStats(bfZeroTree);
	cout << "\n";

	wipeTree(bfZeroTree);
	return 0;
}

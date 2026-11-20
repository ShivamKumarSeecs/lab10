// BSCS-15-D
// Shivam Kumar 
// 555585

#include <iostream>

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

// put a new value in the tree and skip duplicates
Node* addNode(Node* n, int x) {
	// empty spot found so make the new node here
	if (n == nullptr) {
		return new Node(x);
	}
	// smaller values go left
	if (x < n->val) {
		n->left = addNode(n->left, x);
	}
	else if (x > n->val) {
		// bigger values go right
		n->right = addNode(n->right, x);
	}
	// send back the unchanged pointer
	return n;
}

// walk down left until we hit the smallest node
Node* getMin(Node* n) {
	while (n != nullptr && n->left != nullptr) {
		n = n->left;
	}
	return n;
}

// remove a value from the tree and return the new root
Node* removeNode(Node* n, int x) {
	// nothing to delete here
	if (n == nullptr) {
		return nullptr;
	}
	// keep looking left or right for the target
	if (x < n->val) {
		n->left = removeNode(n->left, x);
	}
	else if (x > n->val) {
		n->right = removeNode(n->right, x);
	}
	else {
		// found the node with zero or one child
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
		// two children so copy smallest from right side then delete it
		Node* succ = getMin(n->right);
		n->val = succ->val;
		n->right = removeNode(n->right, succ->val);
	}
	return n;
}

// print values in sorted order
void showInorder(Node* n) {
	if (n == nullptr) {
		return;
	}
	showInorder(n->left);
	cout << n->val << " ";
	showInorder(n->right);
}

// free all nodes when done
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
	int list[] = { 50, 30, 70, 20, 40, 60, 80, 65 };

	// build the starting tree
	for (int x : list) {
		top = addNode(top, x);
	}

	cout << "start inorder ";
	showInorder(top);
	cout << "\n\n";

	// case 1 leaf node 20 goes away
	cout << "delete 20 case 1 leaf\n";
	cout << "predict 30 40 50 60 65 70 80\n";
	top = removeNode(top, 20);
	cout << "actual ";
	showInorder(top);
	cout << "\n\n";

	// case 2 one child 60 gets replaced by 65
	cout << "delete 60 case 2 one child\n";
	cout << "predict 30 40 50 65 70 80\n";
	top = removeNode(top, 60);
	cout << "actual ";
	showInorder(top);
	cout << "\n\n";

	// case 3 two children 50 takes 65 then deletes old 65
	cout << "delete 50 case 3 two children\n";
	cout << "predict 30 40 65 70 80\n";
	top = removeNode(top, 50);
	cout << "actual ";
	showInorder(top);
	cout << "\n\n";

	// missing key 99 leaves tree the same
	cout << "delete 99 missing key\n";
	top = removeNode(top, 99);
	cout << "actual ";
	showInorder(top);
	cout << "\n\n";

	wipeTree(top);
	top = nullptr;

	// empty tree returns null safely
	cout << "delete from empty tree\n";
	top = removeNode(top, 10);
	cout << "is null " << (top == nullptr) << "\n\n";

	// single node tree becomes null after delete
	top = addNode(top, 42);
	cout << "delete only node 42\n";
	top = removeNode(top, 42);
	cout << "is null " << (top == nullptr) << "\n";

	return 0;
}	

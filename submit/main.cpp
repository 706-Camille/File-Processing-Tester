#include <iostream>
#include <string>
#define LEFT 1
#define RIGHT 0

typedef struct Node
{
	int key;
	Node* left;
	Node* right;
}Node;

void inorder(Node* node)
{
	if (node == nullptr)
		return;

	std::cout << "<";
	inorder(node->left);
	std::cout << " " << node->key << " ";
	inorder(node->right);
	std::cout << ">";
}

Node* getNodeBST()
{
	Node* node = new Node();

	return node;
}

Node* searchBST(Node* T, int searchKey)
{
	Node* p = T;
	if (p == nullptr) return nullptr;
	if (searchKey == p->key) return p;
	if (searchKey > p->key)
		return searchBST(p->right, searchKey);
	else
		return searchBST(p->left, searchKey);
}

Node* searchParent(Node* T, int searchKey)
{
	Node* p = T;
	Node* q = nullptr;

	while (p != nullptr)
	{
		if (searchKey == p->key) return q;

		q = p;

		if (searchKey < p->key)
			p = p->left;
		else
			p = p->right;
	}
	return nullptr;
}



int height(Node* T)
{
	if (T == nullptr) return 0; // base case

	return 1 + std::max(height(T->left), height(T->right));
}

Node* maxNode(Node* T)
{
	Node* p = T;
	Node* q = nullptr;

	while (p != nullptr)
	{
		q = p;
		p = p->right;
	}

	return q;
}
Node* minNode(Node* T)
{
	Node* p = T;
	Node* q = nullptr;

	while (p != nullptr)
	{
		q = p;
		p = p->left;
	}

	return q;
}


int noNodes(Node* T)
{
	if (T == nullptr) return 0;

	return 1 + noNodes(T->left) + noNodes(T->right);
}

bool insertBST(Node*& T, int newKey)
{
	Node* p = T;
	Node* q = nullptr;

	while (p != nullptr)
	{
		if (newKey == p->key) // newKey가 이미 트리에 존재함
			return true; // 이미 존재하면, true 리턴 

		q = p;

		if (newKey < p->key)
			p = p->left;
		else
			p = p->right;

	}

	Node* newNode = getNodeBST();
	newNode->key = newKey;
	newNode->left = nullptr;
	newNode->right = nullptr;


	if (T == nullptr) T = newNode;
	else if (newKey < q->key) q->left = newNode;
	else q->right = newNode;

	return false; // 키 삽입하면, false 리턴
}

bool deleteBST(Node*& T, int deleteKey)
{
	Node* p = searchBST(T, deleteKey); // 삭제할 노드
	Node* q = searchParent(T, deleteKey); // 삭제할 노드의 부모

	if (p == nullptr) return true; // 삭제할 key 없음

	// 차수 0
	if (p->left == nullptr && p->right == nullptr)
	{
		if (q == nullptr) T = nullptr; // 루트 노드 삭제
		else if (q->left == p) q->left = nullptr;
		else q->right = nullptr;
		delete(p);
	}
	// 차수 1
	else if (p->left == nullptr || p->right == nullptr)
	{
		Node* child = (p->left) ? p->left : p->right;
		if (q == nullptr) T = child; // 루트 노드
		else if (q->left == p) q->left = child;
		else q->right = child;
		delete(p);
	}
	// 차수 2
	else 
	{
		Node* r;
		int flag;
		if (height(p->left) > height(p->right)) 
		{
			r = maxNode(p->left);
			flag = LEFT;
		}
		else if (height(p->left) < height(p->right)) 
		{
			r = minNode(p->right);
			flag = RIGHT;
		}
		else 
		{
			if (noNodes(p->left) >= noNodes(p->right)) 
			{
				r = maxNode(p->left);
				flag = LEFT;
			}
			else {
				r = minNode(p->right);
				flag = RIGHT;
			}
		}

		p->key = r->key; // 키 대체
		if (flag == LEFT) deleteBST(p->left, r->key);
		else deleteBST(p->right, r->key);
	}

	return false;
}

void handleInsert(Node*& T, int key)
{
	if (insertBST(T, key))
		std::cout << "i " << key << ": The key already exists";
	else
		inorder(T);
}
void handleDelete(Node*& T, int key)
{
	if (deleteBST(T, key))
		std::cout << "d " << key << ": The key does not exist";
	else
		inorder(T);
}

void clear(Node*& T)
{
	if (T == nullptr)
		return;

	clear(T->left);
	clear(T->right);
	delete(T);
}
int main()
{
	Node* T = nullptr;

	char command;
	int key;

	while (std::cin >> command >> key)
	{

		switch (command)
		{
		case 'i':
			handleInsert(T, key);
			break;
		case 'd':
			handleDelete(T, key);
			break;
		}

		std::cout << "\n";
	}

	clear(T);

	return 0;
}

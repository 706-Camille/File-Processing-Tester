#include <iostream>
#include <algorithm>
#include <stack>


typedef enum {
    NO = 0,
    LL = 1,
    LR = 2,
    RL = 3,
    RR = 4,
} ROTATION;

typedef struct Node
{
    int key;
    Node* left;
    Node* right;
    int bf; // Balance Factor
} Node;

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

Node* getNodeAVL(int key)
{
    Node* node = new Node();
    node->key = key;
    node->left = nullptr;
    node->right = nullptr;
    node->bf = 0;
    return node;
}

int height(Node* T)
{
    return (T == nullptr) ? 0 : 1 + std::max(height(T->left), height(T->right));
}

int size(Node* T)
{
    return (T == nullptr) ? 0 : 1 + size(T->left) + size(T->right);
}

Node* minNode(Node* T)
{
    while (T->left != nullptr)
        T = T->left;
    return T;
}

Node* maxNode(Node* T)
{
    while (T->right != nullptr)
        T = T->right;
    return T;
}

void updateBF(Node* T)
{
    if (T == nullptr) return;
    T->bf = height(T->left) - height(T->right);
}

void rotateLL(Node*& root, Node*& x, Node*& f)
{
    Node* subtree = x->left;
    x->left = subtree->right;
    subtree->right = x;

    if (f == nullptr)
        root = subtree;
    else if (f->left == x)
        f->left = subtree;
    else
        f->right = subtree;

    x->bf = 0;
    subtree->bf = 0;
}

void rotateRR(Node*& root, Node*& x, Node*& f)
{
    Node* subtree = x->right;
    x->right = subtree->left;
    subtree->left = x;

    if (f == nullptr)
        root = subtree;
    else if (f->left == x)
        f->left = subtree;
    else
        f->right = subtree;

    x->bf = 0;
    subtree->bf = 0;
}

void rotateLR(Node*& root, Node*& x, Node*& f)
{
    Node* p = x->left;
    Node* q = p->right;

    p->right = q->left;
    x->left = q->right;

    q->left = p;
    q->right = x;

    if (f == nullptr)
        root = q;
    else if (f->left == x)
        f->left = q;
    else
        f->right = q;

    updateBF(x);
    updateBF(p);
    q->bf = 0;
}

void rotateRL(Node*& root, Node*& x, Node*& f)
{
    Node* p = x->right;
    Node* q = p->left;

    x->right = q->left;
    p->left = q->right;

    q->left = x;
    q->right = p;

    if (f == nullptr)
        root = q;
    else if (f->left == x)
        f->left = q;
    else
        f->right = q;

    updateBF(x);
    updateBF(p);
    q->bf = 0;
}

void checkBalance(Node*& root, const int key, Node*& x, Node*& f, ROTATION& rotationType)
{
    Node* p = root;
    Node* q = nullptr; // 부모
    x = nullptr;       // 문제가 되는 node, bf가 정상 범위를 벗어난 노드 저장하는 포인터
    f = nullptr;       // x의 부모를 저장하는 포인터

    while (p != nullptr)
    {
        updateBF(p);

        if (p->bf < -1 || p->bf > 1)
        {
            x = p;
            f = q;
        }

        if (key < p->key)
        {
            q = p;
            p = p->left;
        }
        else if (key > p->key)
        {
            q = p;
            p = p->right;
        }
        else
            break;
    }

    if (x == nullptr)
    {
        rotationType = ROTATION::NO;
        return;
    }

    if (x->bf > 1)
    {
        updateBF(x->left);
        if (0 <= x->left->bf)
            rotationType = ROTATION::LL;
        else
            rotationType = ROTATION::LR;
    }
    else if (x->bf < -1)
    {
        updateBF(x->right);
        if (0 > x->right->bf)
            rotationType = ROTATION::RR;
        else
            rotationType = ROTATION::RL;
    }
}


void rotateTree(Node*& root, Node*& x, Node*& f, ROTATION& rotationType)
{
    if (rotationType == ROTATION::LL)
        rotateLL(root, x, f);
    else if (rotationType == ROTATION::RR)
        rotateRR(root, x, f);
    else if (rotationType == ROTATION::LR)
        rotateLR(root, x, f);
    else if (rotationType == ROTATION::RL)
        rotateRL(root, x, f);
}

std::pair<Node*, bool> insertBST(Node*& root, const int& key) {
    Node* p = root;
    Node* q = nullptr; // 부모

    while (p != nullptr)
    {
        if (p->key == key) // 삽입할 노드가 이미 존재함
            return std::pair<Node*, bool>(p, false);

        q = p;

        if (key > p->key) 
            p = p->right;
        else 
            p = p->left;
    }

    Node* node = new Node();
    node->key = key;
    node->left = nullptr;
    node->right = nullptr;
    node->bf = 0;

    if (root == nullptr) 
        root = node;
    else if (key < q->key) 
        q->left = node;
    else 
        q->right = node;

    return std::pair<Node*, bool>{ node, true };
}

bool insertAVL(Node*& root, const int& key) {
    Node* x = nullptr; // 문제가 되는 노드
    Node* f = nullptr; // x의 부모 노드
    ROTATION rotationType = ROTATION::NO;

    std::pair<Node*, bool> result = insertBST(root, key);
    if (result.second == false) 
        return true;

    checkBalance(root, key, x, f, rotationType);
    rotateTree(root, x, f, rotationType);

    return false;
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

bool deleteAVL(Node*& root, int key)
{
    Node* p = root;   // 삭제할 노드
    Node* q = nullptr; // 삭제할 노드의 부모
    std::stack<Node*> s; 

    while (p != nullptr && p->key != key)
    {
        q = p;
        s.push(q);
        if (key < p->key)
            p = p->left;
        else
            p = p->right;
    }

    if (p == nullptr) // 삭제할 키가 없음
        return true;

    // 차수 0
    if (p->left == nullptr && p->right == nullptr)
    {
        if (q == nullptr) root = nullptr;
        else if (q->left == p)q->left = nullptr;
        else q->right = nullptr;
    }
    // 차수 1
    else if (p->left == nullptr || p->right == nullptr)
    {
        Node* child = (p->left != nullptr) ? p->left : p->right;
        if (q == nullptr) root = child;
        else if (q->left == p) q->left = child;
        else q->right = child;
    }
    // 차수 2: 자식이 둘인 경우
    else
    {
        s.push(p);

        Node* tempNode = p;

        // 오른쪽 서브트리에서 최소값 또는 왼쪽 서브트리에서 최대값 선택
        if (height(p->left) < height(p->right) ||
            ((height(p->left) == height(p->right)) && (size(p->left) < size(p->right))))
        {
            p = p->right;
            while (p->left != nullptr)
            {
                s.push(p);
                p = p->left;
            }
        }
        else
        {
            p = p->left;
            while (p->right != nullptr)
            {
                s.push(p);
                p = p->right;
            }
        }

        // 키 대체
        q = searchParent(root, p->key);

        int temp = tempNode->key;
        tempNode->key = p->key;
        p->key = temp;

        if (q->left == p)
            q->left = (p->right != nullptr) ? p->right : p->left;
        else
            q->right = (p->left != nullptr) ? p->left : p->right;
    }

    delete p; // 삭제된 노드 메모리 해제
    p = nullptr;

    // AVL 트리 균형 조정
    while (!s.empty())
    {
        Node* t = s.top();
        s.pop();
        updateBF(t);

        if (t->bf > 1 || t->bf < -1)
        {
            Node* x = nullptr;
            Node* f = nullptr;
            ROTATION rotationType = NO;

            checkBalance(root, t->key, x, f, rotationType);
            rotateTree(root, x, f, rotationType);
        }
    }

    return false;
}

void handleInsert(Node*& T, int key)
{
    if (insertAVL(T, key))
        std::cout << "i " << key << ": The key already exists";
    else
        inorder(T);
}

void handleDelete(Node*& T, int key)
{
    if (deleteAVL(T, key))
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
    delete T;
    T = nullptr;
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


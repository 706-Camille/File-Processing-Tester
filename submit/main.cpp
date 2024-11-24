#include <iostream>
// std::pair, std::max, assert는 예외적으로 허용하겠습니다.
#include <utility> // std::pair
#include <algorithm> // std::max
#include <cassert> // assert: 디버깅에 활용하세요.
#include <stack>
template <class _Tp> class Node;
template <class _Tp> class AVL;

typedef enum {
	NO = 0,
	LL = 1,
	LR = 2,
	RL = 3,
	RR = 4,
} ROTATION;

typedef enum {
	LEFT  = 0,
	RIGHT = 1,
} DIRECTION;


/*
* 아래는 반드시 사용해야하는 Node 클래스입니다.
* height, size, bf를 제외한 멤버 변수는 추가, 삭제 및 변경이 불가능합니다.
*/
template <class _Tp>
class Node
{
public: // Member types
	typedef _Tp					__key_type;
	typedef Node<__key_type>* __node_pointer_type;

public: // Member variables
	__key_type			__key_;
	__node_pointer_type	__left_;
	__node_pointer_type	__right_;
	int				    __bf_;
public: // Constructor
	Node(const __key_type& key) : __key_(key), __left_(nullptr), __right_(nullptr), __bf_(0) {}

	friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) {
		if (np == nullptr) {
			return os;
		}
		os << "<";
		os << np->__left_;	
		os << " " << np->__key_ << " ";
		os << np->__right_;
		os << ">";
		return os;
	}

};

/*
* 아래 함수들의 설계 방식은 Tree와 Node의 구동부를 구현하기 위해 추천드리는 설계 방식입니다.
* 반드시 아래의 설계 방식을 따라야 하는 것은 아닙니다.
*/

template <class _NodePtr>
unsigned __height(_NodePtr __x) {
	return __x == nullptr ? 0 : 1 + std::max(__height(__x->__left_), __height(__x->__right_));
}

template <class _NodePtr>
unsigned __size(_NodePtr __x) {
	return __x == nullptr ? 0 : 1 + __size(__x->__left_) + __size(__x->__right_);
}

/*
* 아래 함수는 minNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) {
	assert(__x != nullptr);
	return __x->__left_ == nullptr ? __x : __tree_min(__x->__left_);
}

/*
* 아래 함수는 maxNode와 동일한 역할을 하는 함수입니다.
* 매개 변수 타입과 반환 타입을 참조 타입으로 수정하셔도 무방합니다.
*/
template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) {
	assert(__x != nullptr);
	return __x->__right_ == nullptr ? __x : __tree_max(__x->__right_);
}

/*
* PDF에 명시되어있는 출력 형식에 유의하세요.x`
* ❗️잘못된 출력 형식은 0점 처리됩니다.❗️
*/
	template <class _NodePtr>
	void __inorder(_NodePtr __x) {
		// write your own code here
		if (__x == nullptr) {
			return;
		}

		std::cout << "<";
		__inorder(__x->__left_);
		std::cout << " " << __x->__key_ << " ";
		__inorder(__x->__right_);
		std::cout << ">";
	}

template <class _NodePtr>
void __update_bf(_NodePtr __p) 
{
	if (__p == nullptr) return;
	__p->__bf_ = __height(__p->__left_) - __height(__p->__right_);
}


template <class _NodePtr>
void __rotateLL(_NodePtr& __root, _NodePtr __x, _NodePtr __f)
{
	_NodePtr subtree = __x->__left_;
	__x->__left_ = subtree->__right_;
	subtree->__right_ = __x;

	if (__f == nullptr) {
		__root = subtree;
	}
	else if (__f->__left_ == __x) {
		__f->__left_ = subtree;
	}
	else {
		__f->__right_ = subtree;
	}

	__x->__bf_ = 0;
	subtree->__bf_ = 0;

}



template <class _NodePtr>
void __rotateRR(_NodePtr& __root, _NodePtr __x, _NodePtr __f)
{
	_NodePtr subtree = __x->__right_;  
	__x->__right_ = subtree->__left_; 
	subtree->__left_ = __x;          

	if (__f == nullptr) {
		__root = subtree;             
	}
	else if (__f->__left_ == __x) {
		__f->__left_ = subtree;       
	}
	else {
		__f->__right_ = subtree;   
	}

	__x->__bf_ = 0;
	subtree->__bf_ = 0;

}


template <class _NodePtr>
void __rotateLR(_NodePtr& __root, _NodePtr __x, _NodePtr __f)
{
	_NodePtr p = __x->__left_;  // L
	_NodePtr q = p->__right_;   // R

	p->__right_ = q->__left_;   // 1
	__x->__left_ = q->__right_; // 2

	q->__left_ = p;             // 3
	q->__right_ = __x;          // 4

	if (__f == nullptr) {
		__root = q;  
	}
	else if (__f->__left_ == __x) {
		__f->__left_ = q;  
	}
	else {
		__f->__right_ = q; 
	}

	__update_bf(__x);
	__update_bf(p);
	q->__bf_ = 0;
}

template <class _NodePtr>
void __rotateRL(_NodePtr& __root, _NodePtr __x, _NodePtr __f)
{
	_NodePtr p = __x->__right_; 
	_NodePtr q = p->__left_;  

	__x->__right_ = q->__left_;  // 1
	p->__left_ = q->__right_;    // 2
	 
	q->__right_ = p;             // 3
	q->__left_ = __x;            // 4


	if (__f == nullptr) __root = q; 
	else if (__f->__left_ == __x)__f->__left_ = q;  
	else __f->__right_ = q;  
	
	__update_bf(__x);
	__update_bf(p);
	q->__bf_ = 0;
}

template <class _NodePtr, class _Tp>
void __check_balance(_NodePtr __root, const _Tp key, _NodePtr& x, _NodePtr& f, ROTATION& rotationType)
{
	_NodePtr p = __root;
	_NodePtr q = nullptr; // 부모
	x = nullptr; // 문제가 되는 node, bf가 정상 범위를 벗어난 노드 저장하는 ptr
	f = nullptr; // x의 부모를 저장하는 ptr

	while (p != nullptr)
	{
		__update_bf(p);

		if (p->__bf_ < -1 || p->__bf_ > 1)
		{
			x = p;
			f = q;
		}

		if (key < p->__key_)
		{
			q = p;
			p = p->__right_;
		}
		else if (key > p->__key_)
		{
			q = p;
			p = p->__right_;
		}
		else
			break;
	}

	if (x == nullptr)
	{
		rotationType = ROTATION::NO;
		return;
	}

	if (x->__bf_ > 1)
	{
		__update_bf(x->__left_);
		if (0 <= x->__left_->__bf_) rotationType = ROTATION::LL;
		else rotationType = ROTATION::LR;
	}
	else if (x->__bf_ < -1)
	{
		__update_bf(x->__left_);
		if (0 > x->__right_->__bf_) rotationType = ROTATION::RR;
		else rotationType = ROTATION::RL;
	}

}


template <class _NodePtr>
void __rotate_tree(_NodePtr& __root, _NodePtr& __x, _NodePtr& __f, ROTATION& __rotationType) 
{
	if (__rotationType == ROTATION::NO) return;

	if (__rotationType == ROTATION::LL)
		__rotateLL(__root, __x, __f);
	else if (__rotationType == ROTATION::LR)
		__rotateLR(__root, __x, __f);
	else if (__rotationType == ROTATION::RR)
		__rotateRR(__root, __x, __f);
	else if (__rotationType == ROTATION::RL)
		__rotateRL(__root, __x, __f);
	else return;

}

/*
* 아래 함수는 삽입된 노드의 위치와 삽입 여부를 반환합니다.
* 예시) 이미 같은 키 값이 존재하는 경우: return std::pair<_NodePtr, bool>(__p, false);
*/
template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) {
	_NodePtr p = __root;
	_NodePtr q = nullptr; // 부모

	while (p != nullptr)
	{
		if (p->__key_ == key) // 삽입할 노드가 이미 존재함
			return std::pair<_NodePtr, bool>(p, false);

		q = p;

		if (key > p->__key_) p = p->__right_;
		else p = p->__left_;
	}

	_NodePtr node = new Node<_Tp>(key);

	if (__root == nullptr) __root = node;
	else if (key < q->__key_) q->__left_ = node;	
	else q->__right_ = node;


	return std::pair<_NodePtr, bool>{ node, true };
}

template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertAVL(_NodePtr& __root, const _Tp& key) {

	_NodePtr x = nullptr; // 문제가 되는 노드
	_NodePtr f = nullptr; // x의 부모 노드
	ROTATION rotationType = ROTATION::NO;

	std::pair<_NodePtr, bool> result = __insertBST(__root, key);
	if (result.second == false)	return result;
	 
	__check_balance(__root, key, x, f, rotationType);
	__rotate_tree(__root, x, f, rotationType);


	return result;
}


template <class _NodePtr, class _Tp>
_NodePtr searchParent(_NodePtr __root, const _Tp key) {
	_NodePtr p = __root;
	_NodePtr q = nullptr; // 부모 노드

	while (p != nullptr && p->__key_ != key) {
		q = p;
		if (key < p->__key_)
			p = p->__left_;
		else
			p = p->__right_;
	}

	return q; 
}


// 강의 노트 로직대로 했으나, 많은 부분에서 오류가 발생하였습니다.
// 따라서 check_balance 함수를 사용하지 않고, deleteAVL 함수 내에 sudo 코드 로직으로 작성하였습니다.

template <class _NodePtr, class _Tp>
bool __deleteAVL(_NodePtr& __root, const _Tp key) 
{
	_NodePtr p = __root;  // 삭제할 노드
	_NodePtr q = nullptr;  // 삭제할 노드의 부모
	std::stack<_NodePtr> s;

	while (p != nullptr && p->__key_ != key)
	{
		q = p;
		s.push(q);
		if (key < p->__key_)
			p = p->__left_;
		else
			p = p->__right_;
	}

	if (p == nullptr) return false;

	// 차수 0 
	if (p->__left_ == nullptr && p->__right_ == nullptr)
	{
		if (q == nullptr) __root = nullptr;  // 루트 노드 삭제
		else if (q->__left_ == p) q->__left_ = nullptr;
		else q->__right_ = nullptr;
	}
	// 차수 1
	else if (p->__left_ == nullptr || p->__right_ == nullptr) {
		_NodePtr child = (p->__left_ != nullptr) ? p->__left_ : p->__right_;
		if (q == nullptr) __root = child;
		else if (q->__left_ == p) q->__left_ = child;
		else q->__right_ = child;
		
	}
	// 차수 2 
	else 
	{
			s.push(p);
		
		_NodePtr tempNode = p;

		// 오른쪽 서브트리 max
		if ( __height(p->__left_) < __height(p->__right_) ||
			 ( (__height(p->__left_) == __height(p->__right_)) && (__size(p->__left_) < __size(p->__right_)) )
			)
		{
			p = p->__right_; 
			while (p->__left_ != nullptr) 
			{ 
				s.push(p);
				p = p->__left_;
			}
		}
		// 왼쪽 서브트리 min
		else {
			p = p->__left_; 
			while (p->__right_ != nullptr) 
			{ 
				s.push(p);
				p = p->__right_;
			}
		}

		// 키 대체
		q = searchParent(__root, p->__key_);
		tempNode->__key_ = p->__key_;

		if (q->__left_ == p)
			q->__left_ = (p->__right_ != nullptr) ? p->__right_ : p->__left_;
		else
			q->__right_ = (p->__left_ != nullptr) ? p->__left_ : p->__right_;

	}

	delete p;
	p = nullptr;
	_NodePtr x = nullptr;
	_NodePtr f = nullptr; 

	while (!s.empty()) {
		q = s.top(); 
		s.pop();    

		__update_bf(q);

		if (q->__bf_ > 1 || q->__bf_ < -1) 
		{
			if (x == nullptr)
			{
				x = q;
				if(!s.empty())
				f = s.top();
			}
		}
	}

	ROTATION rotationType;

	// 불균형 노드가 없음
	if (x == nullptr) 
	{
		return true;
	}

	if (x->__bf_ > 1)
	{
		__update_bf(x->__left_);
		if (0 < x->__left_->__bf_) rotationType = ROTATION::LL;
		else rotationType = ROTATION::LR;
	}
	else if (x->__bf_ < -1)
	{
		__update_bf(x->__right_);
		if (0 > x->__right_->__bf_) rotationType = ROTATION::RR;
		else rotationType = ROTATION::RL;
	}

	__rotate_tree(__root, x, f, rotationType);

	return true;
}
// Dangling pointer를 방지하기 위해 __x를 참조 타입으로 받도록 설계하였습니다.
template <class _NodePtr>
void __clear(_NodePtr& __x) {
	if (__x == nullptr) {
		return;
	}
	__clear(__x->__left_);
	__clear(__x->__right_);
	delete __x;
	__x = nullptr;
}


// 아래는 반드시 사용해야하는 AVL-Tree 클래스입니다.
template <class _Tp>
class AVL {
public: // Member types
	typedef _Tp						key_type;
	typedef std::size_t				size_type;
	typedef Node<key_type>* pointer;
	typedef const Node<key_type>* const_pointer;

private: // Member variables
	pointer	__root_;

public: // Constructor
	AVL() : __root_(nullptr) {}

public: // Capacity
	size_type height() const {
		return __height(__root_);
	}

	size_type size() const {
		return __size(__root_);
	}

public: // Lookup
	void inorder() const {
		__inorder(__root_);
		std::cout << std::endl;
	}

public: // Modifier
	std::pair<const_pointer, bool> insert(const key_type& key) {
		return __insertAVL(__root_, key);
	}

	bool erase(const key_type& key) {
		bool flag = __deleteAVL(__root_, key);

		return flag;
	}

	void clear() {
		__clear(__root_);
	}

	friend std::ostream& operator<<(std::ostream& os, const AVL& tree) {
		os << tree.__root_;
		return os;
	}
};


int main(int argc, char** argv) {
	(void)argc;
	(void)argv;
	AVL<int>	tree;
	char		command;
	int			key;

	while (std::cin >> command >> key) {
		switch ((int)command) {
		case (int)'i':
			if (tree.insert(key).second == false) {
				std::cerr << "i " << key << ": The key already exists" << std::endl;
				continue;
			}
			break;
		case (int)'d':
			if (tree.erase(key) == false) {
				std::cerr << "d " << key << ": The key does not exist" << std::endl;
				continue;
			}
			break;
		default:
			std::cerr << "Invalid command: " << command << std::endl;
			return (1);
		}
		std::cout << tree << std::endl;
	}

	// 프로그램 종료 전, 메모리 누수가 발생하지 않도록 할당받은 메모리를 반드시 해제해야 합니다.
	tree.clear();
	return 0;

}

/*
#include <iostream>

using namespace std;

//------------------------------------------ Node -------------------------------------------//

template<class KeyType, class ValueType>
class Node
{
protected:
    KeyType key;
    int size;
    ValueType node_value; // Значение, которое представляет собой результат некоторой функции, рассчитываемой на основе значений в узлах
    ValueType value;
    ValueType priority;
    ValueType promise; // Используется для временного изменения приоритета элемента без реальной перестройки структуры дерева

    Node<KeyType, ValueType>* right;
    Node<KeyType, ValueType>* left;
    Node<KeyType, ValueType>* parent;

public:
    Node() : key(0), priority(0), size(1), promise(0), value(0), node_value(0), right(NULL), left(NULL), parent(NULL) { }

    Node(KeyType key) : Node() { this->key = key; }
    Node(KeyType key, ValueType priority) : Node(key) { this->priority = priority; }

    void Update() { ReSize();  RecalculateFunction(); }

    void ReSize() // Обновление размера узла
    {
        int left_size = 0;
        int right_size = 0;
        
        if (left != NULL) { left_size = left->size; }
        if (right != NULL) { right_size = right->size; }

        size = 1 + left_size + right_size;
    } 

    void RecalculateFunction() { // Обновляет значение node_value на основе значений в узлах и их потомках

        ValueType first = (left == NULL ? value : left->node_value);
        ValueType second = (right == NULL ? value : right->node_value);
        node_value = max(value, max(first, second));
    }

    void FullFillPromise() {

        if (left != NULL) left->promise += promise;
        if (right != NULL) right->promise += promise;

        if (promise != ValueType(0)) {
            value += promise; promise = ValueType(0);
        }
    }

    KeyType getKey() { return key; }
    ValueType getPriority() { return priority; }
    ValueType getPromise() { return promise; }
    int getSize() { return size; }
    ValueType getValue() { return value; }
    ValueType getNodeValue() { return node_value; }

    Node<KeyType, ValueType>* getLeft() { return left; }
    Node<KeyType, ValueType>* getRight() { return right; }
    Node<KeyType, ValueType>* getParent() { return parent; }

    void setKey(KeyType key) { this->key = key; }
    void setPriority(ValueType priority) { this->priority = priority; }
    void setPromise(ValueType promise) { this->promise = promise; }
    void setSize(int size) { this->size = size; }
    void setValue(ValueType value) { this->value = value; }
    void setNodeValue(ValueType node_value) { this->node_value = node_value; }

    void setLeft(Node<KeyType, ValueType>* node) { this->left = node; ReSize(); }
    void setRight(Node<KeyType, ValueType>* node) { this->right = node; ReSize(); }
    void setParent(Node<KeyType, ValueType>* node) { this->parent = node; }

    Node<KeyType, ValueType>* successor() {

        Node<KeyType, ValueType>* res = this;

        if (right != NULL) {
            res = right;
            for (; res->left != NULL; res = res->left);
            return res;
        }

        for (Node<KeyType, ValueType>* up = this->parent; up != NULL; res = res->parent, up = up->parent)
            if (up->left == res)
                return up;

        return this;
    }

    Node<KeyType, ValueType>* predecessor() {

        Node<KeyType, ValueType>* res = this;
        if (left != NULL) {
            res = left;
            for (; res->right != NULL; res = res->right);
            return res;
        }

        for (Node<KeyType, ValueType>* up = this->parent; up != NULL; res = res->parent, up = up->parent)
            if (up->right == res)
                return up;

        return this;
    }

    template<class T1, class T2>
    friend ostream& operator<<(ostream& stream, Node<T1, T2>& node);
};

template<class T1, class T2>
ostream& operator<<(ostream& stream, Node<T1, T2>& node) {

    stream << "\nKey: " << node.key << "\nPriority: " << node.priority << "\nSize: " << node.size
        << "\nPromise: " << node.promise << "\nNodeValue: " << node.node_value << "\nValue: " << node.value << "\n";

    return stream;
}

template<class KeyType, class ValueType>
void print(Node<KeyType, ValueType>* N) { cout << *N; }

//------------------------------------------ TreapIterator -------------------------------------------//

template<class KeyType, class ValueType>
class TreapIterator
{
protected:
    Node<KeyType, ValueType>* ptr;

public:
    TreapIterator(Node<KeyType, ValueType>* node) : ptr(node) { }
    TreapIterator(const TreapIterator<KeyType, ValueType>& it) : ptr(it.ptr) { } // Конструктор копий

    TreapIterator<KeyType, ValueType>& operator=(const TreapIterator<KeyType, ValueType>& other) {
        ptr = other.ptr;
        return *this;
    }

    TreapIterator<KeyType, ValueType>& operator=(Node<KeyType, ValueType>* node) {
        ptr = node;
        return *this;
    }

    Node<KeyType, ValueType>* operator*() { return ptr; }

    bool operator==(const TreapIterator& other) { return ptr == other.ptr; }
    bool operator!=(const TreapIterator& other) { return ptr != other.ptr; }

    TreapIterator<KeyType, ValueType>& operator++() { // Префиксный ++

        if (ptr != NULL) { ptr = ptr->successor(); }

        return *this;
    }

    TreapIterator<KeyType, ValueType>& operator++(int v) { // Постфиксный ++

        Node<KeyType, ValueType>* res = ptr;

        if (ptr != NULL) { ptr = ptr->successor(); }

        return TreapIterator(ptr);
    }

    TreapIterator<KeyType, ValueType>& operator--() { // Префиксный --

        if (ptr != NULL) { ptr = ptr->predecessor(); }

        return *this;
    }

    TreapIterator<KeyType, ValueType>& operator--(int v) { // Постфиксный --

        Node<KeyType, ValueType>* res = ptr;

        if (ptr != NULL) { ptr = ptr->predecessor(); }
        
        return TreapIterator(ptr);
    }
};

//---------------------------------------- «Декартово дерево» («Курево») ----------------------------------//

template<class KeyType, class ValueType>
class Treap
{
protected:
    Node<KeyType, ValueType>* root; // корень - его достаточно для хранения всего дерева

    // Операция слияния (объединяет два поддерева в одно, учитывая приоритеты элементов)
    void merge(Node<KeyType, ValueType>* Left, Node<KeyType, ValueType>* Right, bool changeConnection = true) {

        if (Left == NULL) return;

        // Выполнены ли обещания (если есть) ?

        Left->FullFillPromise(); 
        Right->FullFillPromise();

        if (Left->getPriority() < Right->getPriority()) { // Меняем связи между узлами так, чтобы Right стал корнем, а Left его левым поддеревом.

            if (Left->getParent() != NULL)
                if (Left->getParent()->getLeft() == Left) Left->getParent()->setLeft(Right);
                else Left->getParent()->setRight(Right);

            if (changeConnection) { // Если нужно изменять связи между узлами
                Right->setParent(Left->getParent());
                Left->setParent(NULL);
            }

            if (Right->getLeft() != NULL) merge(Left, Right->getLeft(), false); // Left становится левым поддеревом Right,
            else {                                                              // и обновляется размер поддерева Right
                Right->setLeft(Left);
                Left->setParent(Right);
            }
            Right->Update();
        }
        else // Left становится корнем, а Right его правым поддеревом
        {
            if (Right->getParent() != NULL)
                if (Right->getParent()->getLeft() == Right) Right->getParent()->setLeft(Left);
                else Right->getParent()->setRight(Left);

            if (!changeConnection) Left->setParent(Right->getParent()); // Обновляем родительскую связь для Left
            Right->setParent(NULL);


            if (Left->getRight() != NULL) merge(Left->getRight(), Right); // Если у Left нет правого поддерева, 
            else {                                                        // то Right становится его правым поддеревом
                Left->setRight(Right);
                Right->setParent(Left);
            }

            Left->Update();
        }
    }

    void merge(Treap<KeyType, ValueType>* tree) { // Вспомогательный merge, чтобы без проблем объединить два дерева

        if (tree->root == NULL) return;
        if (root == NULL) { root = tree->root; return; }

        bool isLeftTree = (root->getKey() < tree->root->getKey()); 
        bool changeRoot = (root->getPriority() < tree->root->getPriority());

        if (isLeftTree) {

            merge(root, tree->root);
            if (changeRoot) root = tree->root;
        }
        else {

            tree->merge(this);
            root = tree->root;
        }
        root->Update();
    }

    Node<KeyType, ValueType>* recursiveFind(KeyType key, Node<KeyType, ValueType>* Current) {

        if (Current->getKey() == key) return Current;

        if (Current->getKey() > key) {

            if (Current->getLeft() == NULL) return NULL; // Если левого потомка нет, значит, ключ не найден в дереве.

            return recursiveFind(key, Current->getLeft());
        }

        if (Current->getRight() == NULL) return NULL; // Если правого потомка нет, значит, ключ не найден в дереве.

        return recursiveFind(key, Current->getRight()); 
    }

    Treap<KeyType, ValueType>* split(Node<KeyType, ValueType>* root, int x, Treap<KeyType, ValueType>& left, Treap<KeyType, ValueType>& right)
    {
        for (Node* node = root->getMinimum(); node != nullptr; node = node->getSuccessor())
        {
            if (node->m_key < x)
                left.merge(new Node(node->m_key, node->m_priority));
            else
                right.merge(new Node(node->m_key, node->m_priority));
        }

        return root;
    }

public:

    Treap() : root(NULL) { }
    Treap(Node<KeyType, ValueType>* root) : root(root) { }

    TreapIterator<KeyType, ValueType> begin() { return TreapIterator<KeyType, ValueType>(Min()); }
    TreapIterator<KeyType, ValueType> end() { return TreapIterator<KeyType, ValueType>(Max()); }

    Node<KeyType, ValueType>* getRoot() { return root; }

    void push(KeyType key, ValueType priority, ValueType value = ValueType(0)) {

        Node<KeyType, ValueType>* node = new Node<KeyType, ValueType>(key, priority);
        node->setValue(value);

        Treap<KeyType, ValueType>* Right = split(key); // Делим дерево на две части

        Right->merge(new Treap<KeyType, ValueType>(node));

        merge(Right);
    }

    Node<KeyType, ValueType>* Find(KeyType key) { return recursiveFind(key, root); }

    Node<KeyType, ValueType>* ExtractTop() { // Извлечение узла с наивысшим приоритетом
       
        if (root == NULL) return NULL;

        Treap<KeyType, ValueType>* Right = split(root->getKey());

        if (Right->root->getRight() == NULL) { return Right->root; }

        Right->root->getRight()->setParent(NULL); // Отсоединяем корень правого поддерева от его родителя

        Node<KeyType, ValueType>* res = Right->root;
        Right->root = res->getRight();

        merge(Right);

        return res;
    }

    Node<KeyType, ValueType>* Min() {

        Node<KeyType, ValueType>* res = root;

        if (res == NULL) { return res; }

        for (; res->getLeft() != NULL; res = res->getLeft()); // В самом левом конце

        return res;
    }

    Node<KeyType, ValueType>* Max() {

        Node<KeyType, ValueType>* res = root;

        if (res == NULL) { return res; }

        for (; res->getRight() != NULL; res = res->getRight()); // В самом правом конце

        return res;
    }

    void Remove(KeyType key) {

        Treap<KeyType, ValueType>* Right = split(key);

        // Нахождение ключа, который меньше ключа узла с заданным ключом и больше всех остальных ключей в Right
        KeyType less = Right->Min()->successor()->getKey();

        if (Right->root->getLeft() == NULL && Right->root->getRight() == NULL) return;

        Right = Right->split(less);
        
        merge(Right);
    }

    void PreOrder(Node<KeyType, ValueType>* N, void (*f)(Node<KeyType, ValueType>*))
    {
        if (N != NULL)
            f(N);
        if (N != NULL && N->getLeft() != NULL)
            PreOrder(N->getLeft(), f);
        if (N != NULL && N->getRight() != NULL)
            PreOrder(N->getRight(), f);
    }

    void InOrder(Node<KeyType, ValueType>* N, void (*f)(Node<KeyType, ValueType>*)) {
        for (; N != N->successor(); N = N->successor())
            f(N);
        f(N);
    }

    void BackOrder(Node<KeyType, ValueType>* N, void (*f)(Node<KeyType, ValueType>*)) {
        for (; N != N->predecessor(); N = N->predecessor())
            f(N);
        f(N);
    }
};

//---------------------------------- «Декартово дерево по неявному ключу» ------------------------------------//
/*
template<class ValueType>
class ImplicitTreap : protected Treap<int, ValueType>
{
protected:

    void merge(ImplicitTreap<ValueType>* tree) {

        if (tree->root == NULL) return;
        if (Treap<int, ValueType>::root == NULL) { Treap<int, ValueType>::root = tree->root; return; }

        bool changeRoot = (Treap<int, ValueType>::root->getPriority() < tree->root->getPriority());

        Treap<int, ValueType>::merge(Treap<int, ValueType>::root, tree->root);

        if (changeRoot) Treap<int, ValueType>::root = tree->root;

        Treap<int, ValueType>::root->Update();

    }

    int size(Node<int, ValueType>* current) {

        if (current == NULL) { return 0; }

        return size(current->getLeft()) + size(current->getRight()) + 1;
    }

    int size() { return size(this->root); }

    Node<int, ValueType>* recursiveFind(int index, Node<int, ValueType>* Current) {

        int leftSize = (Current->getLeft() == NULL ? 1 : Current->getLeft()->getSize() + 1);

        if (index == leftSize) return Current;

        if (leftSize > index) {
            if (Current->getLeft() == NULL) return NULL;
            return recursiveFind(index, Current->getLeft());
        }

        if (Current->getRight() == NULL) return NULL;
        return recursiveFind(index - leftSize, Current->getRight());
    }

public:

    ImplicitTreap() : Treap<int, ValueType>() {  }
    ImplicitTreap(Node<int, ValueType>* node) : Treap<int, ValueType>(node) {}

    Node<int, ValueType>* getRoot() { return Treap<int, ValueType>::root; }

    TreapIterator<int, ValueType> begin() { return Treap<int, ValueType>::begin(); }
    TreapIterator<int, ValueType> end() { return Treap<int, ValueType>::end(); }

    Node<int, ValueType>* Min() { return Treap<int, ValueType>::Min(); }
    Node<int, ValueType>* Max() { return Treap<int, ValueType>::Max(); }

    Node<int, ValueType>* Find(int index) {
        return recursiveFind(index + 1, Treap<int, ValueType>::root);
    }
    
    void AddToMultiple(int key1, int key2, ValueType value) {
        if (key2 < key1) return;

        ImplicitTreap<ValueType>* Right = split(key1);

        ImplicitTreap<ValueType>* RightRight = Right->split(key2 - key1 + 1);

        Right->root->setPromise(value);

        Right->merge(RightRight);

        merge(Right);
    }
    
    void Insert(int index, ValueType value, ValueType priority) {
        Node<int, ValueType>* node = new Node<int, ValueType>(0, priority); node->setValue(value);
        node->setNodeValue(value);

        ImplicitTreap<ValueType>* Right = new ImplicitTreap<ValueType>(node);

        Right->merge(split(index + 1));
        merge(Right);
    }

    void PushBack(ValueType value, ValueType priority) {
        Node<int, ValueType>* node = new Node<int, ValueType>(0, priority); node->setValue(value);
        node->setNodeValue(value);

        merge(new ImplicitTreap<ValueType>(node));
    }

    ValueType FunctionOnRange(int key1, int key2) {

        if (key1 > key2) return ValueType(0);

        ImplicitTreap<ValueType>* Right = split(key1);

        ImplicitTreap<ValueType>* RightRight = Right->split(key2 - key1 + 1);

        ValueType res = ValueType(1);

        auto N = Right->Min();
        for (; N != Right->Max(); N = N->successor())
            res = res * N->getNodeValue();
        res = res * N->getNodeValue();

        Right->merge(RightRight);
        merge(Right);

        return res;
    }

    void Remove(int index) {
        ImplicitTreap<ValueType>* Right = split(index);
        Right = Right->split(1);
        merge(Right);
    }

    void PreOrder(Node<int, ValueType>* N, void (*f)(Node<int, ValueType>*)) { Treap<int, ValueType>::PreOrder(N, f); }

    void InOrder(Node<int, ValueType>* N, void (*f)(Node<int, ValueType>*)) { Treap<int, ValueType>::InOrder(N, f); }

    void BackOrder(Node<int, ValueType>* N, void (*f)(Node<int, ValueType>*)) { Treap<int, ValueType>::BackOrder(N, f); }

};

int main()
{
    cout << "Treap\n";

    Treap<int, int> test;

    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            test.push(i, j);

    test.PreOrder(test.getRoot(), print);

    cout << "\nExtract From Top: ";
    for (int i = 0; i < 9; i++) {
        cout << *test.ExtractTop();
    }

    //-------------------------------------------- ImplicitTreap -------------------------------------------//
    /*
    cout << "\nImplicitTreap\n";

    ImplicitTreap<double> implicitTreap;

    for (int i = 1; i <= 5; ++i) {

        implicitTreap.PushBack(i * 1.0, i);
    }

    cout << "\nInitial ImplicitTreap:" << endl;
    implicitTreap.PreOrder(implicitTreap.getRoot(), print);

    // Выполняем групповую операцию - умножение элементов в интервале [2, 4]
    implicitTreap.AddToMultiple(2, 4, 10.0);

    cout << "\nImplicitTreap after group operation:" << endl;

    implicitTreap.PreOrder(implicitTreap.getRoot(), print);

    
    double result = implicitTreap.FunctionOnRange(2, 4);

    cout << "\nResult of group operation on interval [2, 4]: " << result << endl;
    
    return 0;

}
*/
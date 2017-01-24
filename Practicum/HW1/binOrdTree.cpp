#include <iostream>
#include <cassert>

using namespace std;

template <class T>
struct node_bin
{
  T inf;
  node_bin *left;
  node_bin *right;
};


template <typename T = int>
class binOrdTree {
public:

  binOrdTree();
  ~binOrdTree();
  binOrdTree(const binOrdTree&);
  binOrdTree& operator=(const binOrdTree&);


  bool empty() const;
  T rootTree() const;
  binOrdTree leftTree() const;
  binOrdTree rightTree() const;
  void create3(const T&, const binOrdTree&, const binOrdTree&);
  void create();

  void print() const
  { pr(root);
    cout << endl;
  }

  void addNode(const T& x)
  { add(root, x);
  }

  void deleteNode(const T&);
  void minTree(T&, binOrdTree&) const;

  T minPred(const T&, const T&);
  T minNodeLevel(int);
  void toSumTree();
  void toReverseTree();
private:
  node_bin<T>* root;
  void copyTree(const binOrdTree&);
  void copy(node_bin<T>*&, node_bin<T>* const&) const;
  void deleteTree(node_bin<T>*&) const;
  void pr(const node_bin<T>*) const;
  void add(node_bin<T>*&, const T&) const;
  T minPredHelper(const T&, const T&, binOrdTree<T>);
  bool minNodeLevelHelper(int&, T&, binOrdTree<T>);
  T sumNodes(node_bin<T>*&);
  void toSumTreeHelper(node_bin<T>*&);
  void toReverseTreeHelper(node_bin<T>*&);
};

template <typename T>
binOrdTree<T>::binOrdTree() {
  root = NULL;
}

template <typename T>
binOrdTree<T>::~binOrdTree() {
  deleteTree(root);
}

template <typename T>
binOrdTree<T>::binOrdTree(const binOrdTree<T>& r) {
  copyTree(r);
}

template <typename T>
binOrdTree<T>& binOrdTree<T>::operator=(const binOrdTree<T>& r)
{ if (this != &r)
     { deleteTree(root);
        copyTree(r);
     }
   return *this;
}

template <typename T>
void binOrdTree<T>::copyTree(const binOrdTree<T>& r)
{ copy(root, r.root);
}
template <typename T>
void binOrdTree<T>::copy(node_bin<T>*& pos,
                                             node_bin<T>* const& r) const
{ pos = NULL;
   if (r) { pos = new node_bin<T>;
               assert(pos != NULL);
               pos->inf = r->inf;
               copy(pos->left, r->left);
               copy(pos->right, r->right);
            }
}

template <typename T>
void binOrdTree<T>::deleteTree(node_bin<T>*& p) const
{ if (p)
     { deleteTree(p->left);
        deleteTree(p->right);
        delete p;
         p = NULL;
      }
}

template <typename T>
bool binOrdTree<T>::empty() const
{ return root == NULL;
}

template <typename T>
T binOrdTree<T>::rootTree() const
{ return root->inf;
}

template <typename T>
binOrdTree<T> binOrdTree<T>::leftTree() const
{ binOrdTree<T> t;
   copy(t.root, root->left);
   return t;
}

template <typename T>
binOrdTree<T> binOrdTree<T>::rightTree() const
{ binOrdTree<T> t;
   copy(t.root, root->right);
   return t;
}

template <typename T>
void binOrdTree<T>::pr(const node_bin<T>* p) const {
  if (p) {
    pr(p->left);
    cout << p->inf << " ";
    pr(p->right);
  }
}

template <typename T>
void binOrdTree<T>::add(node_bin<T>*& p, const T& x)
const
{ if (!p)
     { p = new node_bin<T>;
       assert(p != NULL);
       p->inf = x;
       p->left = NULL;
       p->right = NULL;
    }
  else
     if (x < p->inf) add(p->left, x);
     else add(p->right, x);
}

template <typename T>
void binOrdTree<T>::create()
{ root = NULL;
   T x; char c;
   do
   { cout << "> "; cin >> x;
      addNode(x);
      cout << "next elem y/n? "; cin >> c;
    } while (c == 'y');
}

template <typename T>
void binOrdTree<T>::create3(const T& x, const binOrdTree<T>& l, const binOrdTree<T>& r)
{ if (root) deleteTree(root);
   root = new node_bin<T>;
   assert(root != NULL);
   root->inf = x;
   copy(root->left, l.root);
   copy(root->right, r.root);
}

template <typename T>
void binOrdTree<T>::minTree(T& x, binOrdTree<T>& mint) const {
  T a = rootTree();
  if (leftTree().empty()) {
    x = a;
    mint = rightTree();
  } else {
    binOrdTree<T> t, t1 = rightTree();
    leftTree().minTree(x, t);
    mint.create3(a, t, t1);
  }
}

template <typename T>
void binOrdTree<T>::deleteNode(const T& x) {
  if (root) {
    T a = rootTree();
    binOrdTree<T> t1, t2;

    if (a == x && leftTree().empty()) {
      *this = rightTree();
    } else if (a == x && rightTree().empty()) {
      *this = leftTree();
    } else if (a == x) {
      T c;
      t1 = leftTree();
      rightTree().minTree(c, t2);
      create3(c, t1, t2);
    } else if (x < a) {
      t2 = rightTree();
      *this = leftTree();
      deleteNode(x);
      t1 = *this;
      create3(a, t1, t2);
    } else {
      t1 = leftTree();
      *this = rightTree();
      deleteNode(x);
      t2 = *this;
      create3(a, t1, t2);
    }
  }
}

template <typename T>
T binOrdTree<T>::minPredHelper(const T& x, const T& y, binOrdTree<T> tree) {
  if(tree.empty()) return T();
  if(x < tree.rootTree() && y < tree.rootTree()) return minPredHelper(x, y, tree.leftTree());
  if(x > tree.rootTree() && y > tree.rootTree()) return minPredHelper(x, y, tree.rightTree());

  return tree.rootTree();
}

template <typename T>
T binOrdTree<T>::minPred(const T& x, const T& y) {
  return minPredHelper(x, y, *this);
}

template <typename T>
bool binOrdTree<T>::minNodeLevelHelper(int& k, T& result, binOrdTree<T> tree) {
  // Idea: http://stackoverflow.com/a/4321136

  if(tree.empty()) return false;
  if(k < 0) return false;
  if(minNodeLevelHelper(k, result, tree.leftTree())) return true;

  --k;

  if(k == 0) {
    result = tree.rootTree();
    return true;
  }

  return minNodeLevelHelper(k, result, tree.rightTree());
}

template <typename T>
T binOrdTree<T>::minNodeLevel(int k) {
  T result;
  return minNodeLevelHelper(k, result, *this) ? result : T();
}

template <typename T>
T binOrdTree<T>::sumNodes(node_bin<T>*& tree) {
  if(!tree) return T();
  return tree->inf + sumNodes(tree->left) + sumNodes(tree->right);
}

template <typename T>
void binOrdTree<T>::toSumTreeHelper(node_bin<T>*& tree) {
  if(!tree) return;
  tree->inf = sumNodes(tree);
  toSumTreeHelper(tree->left);
  toSumTreeHelper(tree->right);
}

template <typename T>
void binOrdTree<T>::toSumTree() {
  toSumTreeHelper(root);
}

template <typename T>
void binOrdTree<T>::toReverseTreeHelper(node_bin<T>*& tree) {
  if(!tree) return;

  node_bin<T>* nb = tree->left;
  tree->left = tree->right;
  tree->right = nb;
  toReverseTreeHelper(tree->left);
  toReverseTreeHelper(tree->right);
}

template <typename T>
void binOrdTree<T>::toReverseTree() {
  toReverseTreeHelper(root);
}

int main() {
  binOrdTree<int> tree;
  tree.create();

  cout<<endl<<endl<<"binOrdTree:"<<endl;
  tree.print();

  // Tests work with the following input:
  // 20 8 22 4 12 10 14

  // Task 1 tests
  assert(tree.minPred(10, 14) == 12);
  assert(tree.minPred(14, 4) == 8);
  assert(tree.minPred(20, 22) == 20);

  // Task 2 tests
  assert(tree.minNodeLevel(3) == 10);
  assert(tree.minNodeLevel(6) == 20);

  // Task 3 tests
  tree.toSumTree();
  cout<<endl<<"Sum Tree:"<<endl;
  tree.print(); // => "4 48 10 36 14 90 22"

  // Task 4 tests
  cout<<endl<<"Reverse Tree:"<<endl;
  tree.toReverseTree();
  tree.print(); // => "22 90 14 36 10 48 4"

  cout<<endl<<"All tests have passed successfully."<<endl;

  return 0;
}

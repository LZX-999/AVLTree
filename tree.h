#ifndef TREE_H
#define TREE_H

#include <algorithm>
#include <cassert>
#include <functional>
#include <string>

using std::string;

template <typename T>
std::string my_to_string(const T& t) {
    return std::to_string(t);
}

template <>
std::string my_to_string(const std::string& t) {
    return "\"" + t + "\"";
}

template <typename T> struct Node {
    T element;
    int height;
    int weight;
    T sum;
    Node<T>* left;
    Node<T>* right;

    Node(T element)
        : element{ element }, height{ 0 }, left{ nullptr }, right{ nullptr }, weight{ 1 }, sum{ element } {}
    Node(T element, int height, int weight)
        : element{ element }, height{ height }, left{ nullptr }, right{ nullptr }, weight{ weight }, sum{ element } {}
};

template <typename T> class Tree {
private:
    size_t m_size;
    Node<T>* m_root;

public:
    // Constructor
    Tree();
    
    // Destructor
    ~Tree();

    // Returns a pointer to the root
    Node<T>* root();

    // Checks whether the tree is empty
    bool empty() const;

    // Returns the number of elements
    size_t size() const;

    // Returns the height of the tree
    int height() const;

    // Inserts the specified element
    void insert(T element);

    // Checks whether the container contains the specified element
    bool contains(T element) const;

    // Returns the maximum element
    T max() const;

    // Returns the minimum element
    T min() const;

    // Returns the successor of the specified element
    T successor(T element);

    // Convert each element in the tree to string in pre-order.
    string pre_order();

    // Convert each element in the tree to string in order.
    string in_order();

    // Convert each element in the tree to string in post-order.
    string post_order();

    int weight();

    T selectRank(int k);

    int rank(T element);

    T returnSum();

    T predecessor(T element);

    int get_sum();



    void deleteNode(T element);



    // Returns a string equivalent of the tree
    string to_string(bool with_height = true) const {
        return m_to_string(with_height, m_root, 0);
    }



private:
    string m_to_string(bool with_height, Node<T>* node, int ident) const {
        string res;
        if (node == nullptr) {
            return res;
        }
        if (node->right != nullptr) {
            res += m_to_string(with_height, node->right, ident + 2);
        }
        for (int i = 0; i < ident; i++) {
            res += " ";
        }
        res += my_to_string(node->element);
        if (with_height) {
            res += "(h=" + my_to_string(node->height) + ")";
        }
        res += "\n";
        if (node->left != nullptr) {
            res += m_to_string(with_height, node->left, ident + 2);
        }
        return res;
    }

    int height(Node<T>* root) {
        return root == nullptr ? -1 : root->height;
    }

    int bal(Node<T>* root) {
        return root == nullptr ? 0 : height(root->left) - height(root->right);
    }

    void update(Node<T>* root) {
        if (!root) {
            return;
        }
        root->height = std::max(height(root->left), height(root->right)) + 1;
    }

    T getSum(Node<T>* root) {
        return root == nullptr ? 0 : root->sum;
    }

    void updateSum(Node<T>* root) {
        if (!root) {
            return;
        }
        T curr = root->element;
        T left = getSum(root->left);
        T right = getSum(root->right);
        root->sum = left + right + curr;
    }

    Node<T>* leftRotate(Node<T>* root) {
        Node<T>* newRoot = root->right;
        root->right = newRoot->left;
        //Node<T>* hold = newRoot->left;
        newRoot->left = root;

        update(newRoot);
        update(root);
        updateWeight(newRoot);
        updateWeight(root);
        updateSum(root);
        updateSum(newRoot);
        return newRoot;

    }

    int getWeight(Node<T>* root) {
        return root ? root->weight : 0;
    }

    Node<T>* rightRotate(Node<T>* root) {
        Node<T>* newRoot = root->left;
        Node<T>* hold = newRoot->right;
        root->left = hold;
        newRoot->right = root;

        update(newRoot);
        update(root);
        updateWeight(newRoot);
        updateWeight(root);
        updateSum(root);
        updateSum(newRoot);
        return newRoot;

    }

    Node<T>* insertHelper(Node<T>* root, T x) {
        if (root == nullptr) {
            root = new Node<T>(x);
        }
        else if (root->element > x) {
            root->left = insertHelper(root->left, x);
        }
        else {
            root->right = insertHelper(root->right, x);
        }

        update(root);
        updateWeight(root);
        updateSum(root);
        //int diff = bal(root);

        if (bal(root) > 1) {
            if (bal(root->left) > 0) {
                root = rightRotate(root);

            }
            else {
                root->left = leftRotate(root->left);
                root = rightRotate(root);

            }
        }

        if (bal(root) < -1) {
            if (bal(root->right) < 0) {
                root = leftRotate(root);
            }
            else {
                root->right = rightRotate(root->right);
                root = leftRotate(root);
            }
        }
        update(root);
        updateWeight(root);
        updateSum(root);
        return root;
    }

    Node<T>* deleteHelper(Node<T>* root, T x) {
        if (root == nullptr) {
            throw std::out_of_range("Tree is empty or item not found");
        }
        else if (root->element == x) {
            if (!root->left || !root->right) {
                Node<T>* replace = root->left ? root->left : root->right;
                if (!replace) {
                    Node<T>* del = root;
                    root = nullptr;
                }
                else {
                    root = replace;
                }
            }
            else {
                T replace = succ(m_root, root->element);
                deleteHelper(m_root, replace);
                root->element = replace;

            }
        }

        else if (root->element > x) {
            root->left = deleteHelper(root->left, x);
        }
        else {
            root->right = deleteHelper(root->right, x);
        }

        updateWeight(root);
        update(root);
        updateSum(root);

        if (bal(root) > 1 && bal(root->left) >= 0) {
            root = rightRotate(root);

        }
        else if (bal(root) > 1 && bal(root->left) < 0) {
            root->left = leftRotate(root->left);
            root = rightRotate(root);
        }
        else if (bal(root) < -1 && bal(root->right) <= 0) {
            root = leftRotate(root);
        }
        else if (bal(root) < -1 && bal(root->right) > 0) {
            root->right = rightRotate(root->right);
            root = leftRotate(root);
        }
        update(root);
        updateWeight(root);
        updateSum(root);
        return root;

    }

    string inOrder(Node<T>* node) {
        if (node == nullptr) {
            return "";
        }
        string s = " With sum subtree of: " + my_to_string(node->sum);
        return inOrder(node->left) + " " + my_to_string(node->element) + s + inOrder(node->right);
    }

    string postOrder(Node<T>* node) {
        if (node == nullptr) {
            return "";
        }
        return postOrder(node->left) + postOrder(node->right) + my_to_string(node->element) + " ";
    }

    bool bstSearch(Node<T>* root, T x) const {
        if (root == nullptr) {
            return false;
        }
        if (x == root->element) {
            return true;
        }
        if (x < root->element) {
            return bstSearch(root->left, x);
        }
        if (x > root->element) {
            return bstSearch(root->right, x);
        }
    }

    T minimum(Node<T>* root) const {
        if (root == nullptr) {
            throw std::out_of_range("Tree is empty");
        }
        Node<T>* minNode = root;
        while (minNode->left != nullptr) {
            minNode = minNode->left;
        }
        return minNode->element;
    }


    T maximum(Node<T>* root) const {
        if (root == nullptr) {
            throw std::out_of_range("Tree is empty");
        }
        Node<T>* maxNode = root;
        while (maxNode->right != nullptr) {
            maxNode = maxNode->right;
        }
        return maxNode->element;
    }

    T sel(Node<T>* root, int k) {
        if (!root) {
            throw std::out_of_range("Tree is empty");
        }
        int currW = getWeight(root->left);
        if (currW + 1 == k) {
            return root->element;
        }
        else if (currW >= k) {
            return sel(root->left, k);
        }
        else {
            return sel(root->right, k - currW - 1);
        }
    }

    int rankHelper(Node<T>* root, T x) {
        if (!root) {
            return 0;
        }
        int currRank = 0;
        if (root->element == x) {
            return currRank + 1 + getWeight(root->left);
        }
        else if (root->element < x) {
            return currRank += 1 + getWeight(root->left) + rankHelper(root->right, x);
        }
        else {
            return rankHelper(root->left, x);
        }
    }

    void destroy(Node<T>* root) {
        if (root == nullptr) {
            return;
        }
        destroy(root->left);
        destroy(root->right);
        delete root;
    }

    T succ(Node<T>* root, T val) {
        Node<T>* hold = root;
        Node<T>* lastLeft = nullptr;
        while (hold != nullptr) {
            if (hold->element > val) {
                lastLeft = hold; //keep track of lastleft
                hold = hold->left; //value may be too large, visit leftnode
            }
            else {
                hold = hold->right; //the parent of succ node is may be on the right
            }
        }

        if (lastLeft != nullptr) {
            return lastLeft->element; //left turn occured means this is the smallest larger element
        }
        else {
            throw std::out_of_range("No succ"); //no left turn ever occured, this means the value is too big
        }
    }

    Node<T>* succNodeHelper(Node<T>* root, T val) {
        Node<T>* hold = root;
        Node<T>* lastLeft = nullptr;
        while (hold != nullptr) {
            if (hold->element > val) {
                lastLeft = hold; //keep track of lastleft
                hold = hold->left; //value may be too large, visit leftnode
            }
            else {
                hold = hold->right; //the parent of succ node is may be on the right
            }
        }

        if (lastLeft != nullptr) {
            return lastLeft; //left turn occured means this is the smallest larger element
        }
        else {
            throw std::out_of_range("No successor"); //no left turn ever occured, this means the value is too big
        }
    }

    T pred(Node<T>* root, T val) {
        Node<T>* hold = root;
        Node<T>* lastRight = nullptr;
        while (hold != nullptr) {
            if (hold->element < val) {
                lastRight = hold;
                hold = hold->right;
            }
            else {
                hold = hold->left;
            }
        }
        if (lastRight != nullptr) {
            return lastRight->element;
        }
        else {
            throw std::out_of_range("No pred");
        }

    }

    void updateWeight(Node<T>* root) {
        if (!root) {
            return;
        }
        int left = root->left ? root->left->weight : 0;
        int right = root->right ? root->right->weight : 0;
        root->weight = left + right + 1;
    }

};

// Constructor
template <typename T> Tree<T>::Tree() : m_root(nullptr) {
    // TODO: Implement this method
    m_root = nullptr;
    m_size = 0;
}

// Destructor
template <typename T> Tree<T>::~Tree() {
    // TODO: Implement this method
    destroy(m_root);
}

template <typename T> T Tree<T>::returnSum() {
    return getSum(m_root);
}

// Returns a pointer to the root
template <typename T> Node<T>* Tree<T>::root() {
    // TODO: Implement this method
    return m_root;
}

// Checks whether the tree is empty
template <typename T> bool Tree<T>::empty() const {
    // TODO: Implement this method
    return m_root == nullptr;
}

// Returns the number of elements
template <typename T> size_t Tree<T>::size() const {
    // TODO: Implement this method
    return m_size;
}

// Returns the height of the tree
template <typename T> int Tree<T>::height() const {
    // TODO: Implement this method
    return empty() ? -1 : m_root->height;

}

// Inserts an element
template <typename T> void Tree<T>::insert(T element) {
    // TODO: Implement this method
    if (m_root == nullptr) {
        m_root = new Node<T>(element);
        m_size++;
    }
    else {
        m_root = insertHelper(m_root, element);
        m_size++;
    }
}

// Checks whether the container contains the specified element
template <typename T> bool Tree<T>::contains(T element) const {
    // TODO: Implement this method
    return bstSearch(m_root, element);

}

// Returns the maximum element
template <typename T> T Tree<T>::max() const {
    // TODO: Implement this method
    return maximum(m_root);
}

// Returns the minimum element
template <typename T> T Tree<T>::min() const {
    // TODO: Implement this method
    return minimum(m_root);
}

// Returns the successor of the specified element
template <typename T> T Tree<T>::successor(T element) {
    // TODO: Implement this method
    return succ(m_root, element);
}

template<typename T>
T Tree<T>::predecessor(T element) {
    return pred(m_root, element);
}

template <typename T>
string _pre_order(Node<T>* node) {
    return my_to_string(node->element)
        + (node->left == nullptr ? "" : " " + _pre_order(node->left))
        + (node->right == nullptr ? "" : " " + _pre_order(node->right));
}

template <typename T>
string Tree<T>::pre_order() {
    if (m_root == nullptr) {
        return "";
    }
    return _pre_order(m_root);
}

template <typename T>
string Tree<T>::in_order() {
    // TODO: Implement this method
    if (m_root == nullptr) {
        return "";
    }
    string s = inOrder(m_root);
    string ans = "";
    for (int i = 1; i < s.size(); i++) {
        ans += s[i];
    }
    return ans;
}

template <typename T>
string Tree<T>::post_order() {
    // TODO: Implement this method
    if (m_root == nullptr) {
        return "";
    }
    string s = postOrder(m_root);
    string ans = "";
    for (int i = 0; i < s.size() - 1; i++) {
        ans += s[i];
    }
    return ans;
}
template <typename T>
int Tree<T>::weight() {
    return m_root ? m_root->weight : 0;
}

template <typename T>
T Tree<T>::selectRank(int k) {
    return sel(m_root, k);
}

template <typename T>
int Tree<T>::rank(T element) {
    return rankHelper(m_root, element);
}

template<typename T>
void Tree<T>::deleteNode(T element) {
    if (m_size == 1) {
        m_root = nullptr;
        m_size--;
    }
    else {
        m_root = deleteHelper(m_root, element);
        m_size--;
    }
}

template<typename T>
int Tree<T>::get_sum() {
    if (m_root == nullptr) {
        return 0;
    }
    else {
        return m_root->sum;
    }
}


#endif


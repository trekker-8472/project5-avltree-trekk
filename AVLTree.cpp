/**
 * AVLTree.pp
 * Robert Pohl
 * Project 5
 */

#include "AVLTree.h"
#include <string>
#include <optional>
#include <vector>
#include <algorithm>

using namespace std;

// All classes need a constructor this one is an empty tree
AVLTree::AVLTree() {
}

// Inserts a new key-value pair into tree No Duplicates if successful insert rebalance (if necessary) false on fail
bool AVLTree::insert(const std::string &key, size_t value) {

    if (this->root == nullptr) {//special case root node
        this->root = new BSTNode(key, value);
        this->AVLTreeSize++;
        return true;
    }

    vector<BSTNode*> ancestorStorage;
    BSTNode* currentNode = this->root;
    BSTNode* parentNode = nullptr;

    while (currentNode != nullptr) { //move through the tree
        if (currentNode->key == key) {//elimate duplicates
            return false;
        }
        ancestorStorage.push_back(currentNode);
        parentNode = currentNode;

        if (currentNode->key > key) {
            currentNode = currentNode->left;
            }
        else {
            currentNode = currentNode->right;
        }
    }

    BSTNode* newNode = new BSTNode(key, value);//create new node

    if (parentNode->key > key) {//left right placement
        parentNode->left = newNode;
    }
    else {
        parentNode->right = newNode;
    }

    this->AVLTreeSize++;

    //time to balance check

    for (int i = ancestorStorage.size() - 1; i >= 0; i--) {
        BSTNode* ancestor = ancestorStorage[i];
        BSTNode* ancestorParent = nullptr;
        if (i > 0) { // setup for rotation if and only if necessary
            ancestorParent = ancestorStorage[i-1];
        }
        else {
            ancestorParent = nullptr;
        }
        ancestor->updateHeight();//height and balance
        int currBalance = ancestor->balanceFactor();

        BSTNode* newAncestor = ancestor;

        if (currBalance >= 2 && ancestor->left) {
            //heavy left
            if (ancestor->left->balanceFactor() < 0) {//double rotate
                ancestor->left = rotateSetLeft(ancestor->left);
                newAncestor = rotateSetRight(ancestor);
            }
            else {//single rotate
                newAncestor = rotateSetRight(ancestor);
            }
        }
        else if (currBalance <= -2 && ancestor->right) {//double rotate
            if (ancestor->right->balanceFactor() > 0) {
                ancestor->right = rotateSetRight(ancestor->right);
                newAncestor = rotateSetLeft(ancestor);
            }
            else {//single rotate
                newAncestor = rotateSetLeft(ancestor);
            }
        }
        if (newAncestor != ancestor) {//check the regular not the parent
            if (ancestorParent == nullptr) {
                this->root = newAncestor;
            }
            else if (ancestor == ancestorParent->left) {
                ancestorParent->left = newAncestor;
            }
            else {
                ancestorParent->right = newAncestor;
            }
        break;
        }
    }
    return true;
}

// Removes the key-value pair associated of key from tree; true if the key removed else false
bool AVLTree::remove(const std::string &key) {

    vector<BSTNode*> ancestorStorage;
    BSTNode* currentNode = this->root;
    BSTNode* parentNode = nullptr;

    while (currentNode) { //move through the tree
        if (currentNode->key == key) {//verify existence
            break;
        }//removed redundant check
        ancestorStorage.push_back(currentNode);
        parentNode = currentNode;

        if (currentNode->key > key) {
            currentNode = currentNode->left;
            }
        else if (currentNode->key < key) {
            currentNode = currentNode->right;
        }
    }

    BSTNode* deletionNode = currentNode;


    if (!currentNode) {
        return false;
    }

    if (currentNode->isLeaf()) {//leaf node case
        if (!parentNode) {
            root = nullptr; //special case root node deletion
        }
        if (parentNode->left == currentNode) {
            parentNode->left = nullptr;
        }
        if (parentNode->right == currentNode) {
            parentNode->right = nullptr;
        }
    }
    if (currentNode->childCount() == 1) {//if and only if one child
        BSTNode* childNode = nullptr;
        if (currentNode->left) {//account for the one child for later insertion
            childNode = currentNode->left;
        }
        if (currentNode->right) {
            childNode = currentNode->right;
        }

        if (!parentNode) {//still fairly simpl because you just move up and rebalance if need
            root = childNode; // special case root node deletion
        } else if (parentNode->left == currentNode) {
            parentNode->left = childNode;
        } else {
            parentNode->right = childNode;
        }
    }
    if (currentNode->childCount() == 2) {
        BSTNode* decendent = nullptr;
        BSTNode* decendentParent = nullptr;

        decendentParent = currentNode;
        int ltHeight = getSubnodeHeight(currentNode->left);
        int rtHeight = getSubnodeHeight(currentNode->right);

        if (rtHeight < ltHeight) {//piking shorter branch
            decendent = currentNode->right;
            if (!decendent) {
                return false;
            }
            while (decendent->left) {
                ancestorStorage.push_back(decendentParent);
                decendentParent = decendent;
                decendent = decendentParent->left;
            }
        }
        else {
            decendent = currentNode->left;
            while (decendent->right) {
                ancestorStorage.push_back(decendentParent);
                decendentParent = decendent;
                decendent = decendent->right;
            }
        }

        currentNode->key = decendent->key;
        currentNode->value = decendent->value;
        BSTNode* childNode = nullptr;

        if (decendent->left == nullptr) {
            childNode = decendent->right;
        }
        if (decendent->right == nullptr) {
            childNode = decendent->left;
        }

        if (decendentParent->left == decendent) {
            decendentParent->left = childNode;
        }
        if (decendentParent->right == decendent) {
            decendentParent->right = childNode;
        }
        deletionNode = decendent;
    }

    delete deletionNode;
    this->AVLTreeSize--;

    //time to balance check and fix if necessary

    for (int i = ancestorStorage.size() - 1; i >= 0; i--) {
        BSTNode* ancestor = ancestorStorage[i];
        BSTNode* ancestorParent = nullptr;
        if (i > 0) { // setup for rotation if and only if necessary
            ancestorParent = ancestorStorage[i-1];
        }
        else {
            ancestorParent = nullptr;
        }
        ancestor->updateHeight();//height and balance
        int currBalance = ancestor->balanceFactor();

        BSTNode* newAncestor = ancestor;

        if (currBalance >= 2 && ancestor->left) {//added guard like in insert
            //heavy left
            if (ancestor->left->balanceFactor()>0) {//double rotate
                ancestor->left = rotateSetLeft(ancestor->left);
                newAncestor = rotateSetRight(ancestor);
            }
            else {//single rotate
                newAncestor = rotateSetRight(ancestor);
            }
        }
        else if (currBalance <= -2 && ancestor->right) {//double rotate
            if (ancestor->right->balanceFactor()<0) {
                ancestor->right = rotateSetRight(ancestor->right);
                newAncestor = rotateSetLeft(ancestor);
            }
            else {//single rotate
                newAncestor = rotateSetLeft(ancestor);
            }
        }
        if (newAncestor != ancestor) {// check the real one not the parent one
            if (ancestorParent == nullptr) {
                this->root = newAncestor;
            }
            else if (ancestor == ancestorParent->left) {
                ancestorParent->left = newAncestor;
            }
            else {
                ancestorParent->right = newAncestor;
            }
        }
    }
    return true;
}

// Checks whether key exists in tree.
bool AVLTree::contains(const std::string& key) const {
    BSTNode* currentNode = this->root;//start at beginning
    while (currentNode != nullptr) {
        if (currentNode->key == key) {
            return true;
        }
        else if (currentNode->key > key) {
            currentNode = currentNode->left;
        }
        else {
            currentNode = currentNode->right;
        }
    }
    return false;
}

// Retrieves the value of the key. and nullopt if empty
optional<size_t> AVLTree::get(const std::string &key) const {
    BSTNode* currentNode = this->root;//start at beginning
    while (currentNode != nullptr) {//basically the same structure as contains but return value
        if (currentNode->key == key) {
            return currentNode->value;
        }
        else if (currentNode->key > key) {
            currentNode = currentNode->left;
        }
        else {
            currentNode = currentNode->right;
        }
    }
    return nullopt;
}

// Accesses value associated key returning value or changing it as needed (don't worry if invalid)
size_t & AVLTree::operator[](const std::string &key) {
    BSTNode* currentNode = this->root;//start at beginning
    BSTNode* currentParent = nullptr;
    while (currentNode != nullptr) {
        if (currentNode->key == key) {
            return currentNode->value;
        }
        currentParent = currentNode;
        if (currentNode->key > key) {
            currentNode = currentNode->left;
        }
        else {
            currentNode = currentNode->right;
        }
    }
}

// Finds all values associated with range giving vector of the key range unless empty, =0.
vector<size_t> AVLTree::findRange(const std::string &lowKey, const std::string &highKey) const {
}

// Returns vector with all keys in tree length equals tree size
std::vector<std::string> AVLTree::keys() const {
}

// Counts number key value pairs in tree and returns count
size_t AVLTree::size() const {
    return this->AVLTreeSize;
}

// Returns AVL tree height
size_t AVLTree::getHeight() const {
    if (this->root == nullptr) {
        return 0;
    }
    else {
        return 1 + this->root->height;
    }
}

int AVLTree::getSubnodeHeight(BSTNode* node) const {
    if (node == nullptr) {
        return 0;
    }
    else {
        return node->height;
    }
}

// Copy constructor creates a deep copy of the other tree.
AVLTree::AVLTree(const AVLTree& other) {
}

// Assignment operator creates a deep copy of the other tree. releases memory
AVLTree& AVLTree::operator=(const AVLTree& other) {
    return *this;
}

// destructor
AVLTree::~AVLTree() {
}

// Stream insertion operator. Prints tree contents
std::ostream& operator<<(std::ostream& os, const AVLTree& tree) {
    printSideways(os, tree.root, 0);
    return os;
}

// constructor, default
BSTNode::BSTNode(const std::string& k, size_t v) {
    // assign key and value
    key = k;
    value = v;

    // initialize children to null
    left = nullptr;
    right = nullptr;

    // initialize height to 1 (leaf node height)
    height = 1;

}

// Checks if leaf
bool BSTNode::isLeaf() const {
    if (left == nullptr && right == nullptr) { //if both null then is leaf
        return true;
    }
    return false;
}

// Returns Children number
int BSTNode::childCount() const {
    int count = 0;
    if (left != nullptr) {//if null then nothing to add
        count++;
    }
    if (right != nullptr) {//if null then nothing to add
        count++;
    }
    return count;
}

// returns balance factor (left minus right)
int BSTNode::balanceFactor() const {
    int balanceFactor = 0;//start point
    if (left != nullptr) {//if not null add value of height from left
        balanceFactor = balanceFactor + left->height;
    }
    if (right != nullptr) {//if not null subtract right value
        balanceFactor = balanceFactor - right->height;
    }
    return balanceFactor;
}

// Updates the height based on children after insert/remove operations.
void BSTNode::updateHeight() {
    int leftHeight;
    int rightHeight;
    if (left != nullptr) {
        leftHeight = left->height;
    }
    else {
        leftHeight = 0;
    }
    if (right != nullptr) {
        rightHeight = right->height;
    }
    else {
        rightHeight = 0;
    }
    height = max(leftHeight, rightHeight) + 1;

}

// return all Trees values summed self inclusive
// size_t BSTNode::subtreeValueSum() const {
//     return 0;
// }

BSTNode* rotateSetRight(BSTNode *pivotPoint) { //if the set of nodes is heavy left they must be rotated right

    if (!pivotPoint || !pivotPoint->left) {//added guard
        return pivotPoint;
    }

    BSTNode* newPoint = pivotPoint->left; //new node top point
    BSTNode* rtSubTree = newPoint->right; //define right subtree

    newPoint->right = pivotPoint; //rotates the pivot point around
    pivotPoint->left = rtSubTree; //new child of pivot point
    pivotPoint->updateHeight(); //update the height
    newPoint->updateHeight(); //update the height
    return newPoint;
}

BSTNode* rotateSetLeft(BSTNode *pivotPoint) {//mirror above

    if (!pivotPoint || !pivotPoint->right) {//added guard
        return pivotPoint;
    }

    BSTNode* newPoint = pivotPoint->right; //new node top point
    BSTNode* ltSubTree = newPoint->left; //define right subtree

    newPoint->left = pivotPoint; //rotates the pivot point around
    pivotPoint->right = ltSubTree; //new child of pivot point
    pivotPoint->updateHeight(); //update the height
    newPoint->updateHeight(); //update the height
    return newPoint;
}

void printSideways(std::ostream& os, const BSTNode* node, int depth) { //prints tree sideways using right-child-first traversal
    if (!node) return; //base case: null node

    printSideways(os, node->right, depth + 1); //print right subtree first

    for (int i = 0; i < depth; ++i) os << "    "; //indent based on depth
    os << "{" << node->key << ": " << node->value << "}" << std::endl; //print current node

    printSideways(os, node->left, depth + 1); //print left subtree last
}

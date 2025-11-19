/**
 *
 * AVLTree.h
 * Robert Pohl
 * Project 5
 */

#ifndef AVLTREE_H
#define AVLTREE_H
#include <optional>
#include <string>
#include <vector>
#include <ostream>

using namespace std;
class BSTNode;
class AVLTree {
public:
    size_t AVLTreeSize;
    BSTNode* root; //roots are special

    // All classes need a constructor this one is an empty tree
    AVLTree();

    // Inserts a new key-value pair into tree No Duplicates if successful insert rebalance (if necessary) false on fail
    bool insert(const std::string& key, size_t value);

    // Removes the key-value pair associated of key from tree; true if the key removed else false
    bool remove(const std::string& key);

    // Checks whether key exists in tree.
    bool contains(const std::string& key) const;

    // Retrieves the value of the key. and nullopt if empty
    optional<size_t> get(const std::string& key) const;

    // Accesses value associated key returning value or changing it as needed (don't worry if invalid)
    size_t& operator[](const std::string& key);

    // Finds all values associated with range giving vector of the key range unless empty, =0.
    vector<size_t> findRange(const std::string& lowKey,
                             const std::string& highKey) const;

    // Returns vector with all keys in tree length equals tree size
    std::vector<std::string> keys() const;

    // Counts number key value pairs in tree and returns count
    size_t size() const;

    // Returns AVL tree height
    int getHeight() const;

    //helper to get subtree height
    int getSubnodeHeight(BSTNode* node) const;

    // Copy constructor creates a deep copy of the other tree.
    AVLTree(const AVLTree& other);

    // Assignment operator creates a deep copy of the other tree. releases memory
    AVLTree& operator=(const AVLTree& other);

    BSTNode* copyTreeRecursive(const BSTNode* otherNode);

    // For findRange
    void findRangeRecursive(const BSTNode* node, const string& lowKey, const string& highKey, vector<size_t>&
        rangeResult) const;

    void keysRecursive(const BSTNode* node, std::vector<std::string>& keyVector) const; // For keys

    // destructor
    ~AVLTree();

    void deleteNodesRecursive(BSTNode* node);//why the heck not everything else needs a little help and the sub helper functions are getting better

    //void refreshSubtree(BSTNode *node);

    // Stream insertion operator. Prints tree contents
    friend std::ostream& operator<<(std::ostream& os, const AVLTree&);
};

class BSTNode {
public:
    std::string key;   // key stored
    size_t value;      // value associated
    BSTNode* left;     // left child
    BSTNode* right;    // right child
    int height;        // node height (O(1))

    // constructor, default
    BSTNode(const std::string& k, size_t v);

    // Checks if leaf
    bool isLeaf() const;

    // Returns Children number
    int childCount() const;

    // returns balance factor (left minus right)
    int balanceFactor() const;

    // Updates the height based on children after insert/remove operations.
    void updateHeight();

    // return all Trees values summed self inclusive
//    size_t subtreeValueSum() const;
};

//helper functions

BSTNode* rotateSetRight(BSTNode* pivotPoint);
BSTNode* rotateSetLeft(BSTNode* pivotPoint);
static void printSideways(std::ostream& os, const BSTNode* node, int depth);


#endif // AVLTREE_H

#define rb_parent(r)   ((r)->parent)
#define rb_color(r) ((r)->color)
#define rb_is_red(r)   ((r)->color==RED)
#define rb_is_black(r)  ((r)->color==BLACK)
#define rb_set_black(r)  do { (r)->color = BLACK; } while (0)
#define rb_set_red(r)  do { (r)->color = RED; } while (0)
#define rb_set_parent(r,p)  do { (r)->parent = (p); } while (0)
#define rb_set_color(r,c)  do { (r)->color = (c); } while (0)

#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

enum RBTColor { RED, BLACK };

class RBTNode {
public:

    int key;    // data value
    int row;    // data row index
    int col;    // data column index
    RBTColor color;     // color of each node
    RBTNode* left;      // left child
    RBTNode* right;     // right child
    RBTNode* parent;    // parent node

    RBTNode(int value, int row, int col, RBTColor c, RBTNode* p, RBTNode* l, RBTNode* r);
};


// initialization
RBTNode::RBTNode(int value, int ro, int co, RBTColor c, RBTNode* p, RBTNode* l, RBTNode* r) :
    key(value), row(ro), col(co), color(c), parent(), left(l), right(r) {}


class RBTree {
private:
    RBTNode* mRoot;    // root node

public:
    RBTree();
    ~RBTree();

    // (recursively) find the node by its data value
    // Of course, we can find the node by row and column index, however it is much easier
    // to search by key, considering that we made the tree with data value as split threshold
    RBTNode* search(int row, int col, int key);

    // (iteratively) find the node by its data value
    void iterativeSearch(int key);
    // find minimum node and output related information
    void minimum();
    // find maximum node and output related information
    void maximum();
    // insert new node by its value, including row and column index
    void insert(int key, int row, int col);
    // delete new node by its row and column index
    void remove(int row, int col, int key);
    // destroy
    void destroy();
    // print
    void print();

private:


    RBTNode* search(RBTNode* x, int row, int col, int key) const;

    RBTNode* iterativeSearch(RBTNode* x, int key) const;


    RBTNode* minimum(RBTNode* tree);

    RBTNode* maximum(RBTNode* tree);

    // left rotate the tree
    void leftRotate(RBTNode*& root, RBTNode* x);
    // right rotate the tree
    void rightRotate(RBTNode*& root, RBTNode* y);

    void insert(RBTNode*& root, RBTNode* node);

    void insertFixUp(RBTNode*& root, RBTNode* node);

    void remove(RBTNode*& root, RBTNode* node);

    void removeFixUp(RBTNode*& root, RBTNode* node, RBTNode* parent);

    void destroy(RBTNode*& tree);

    void print(RBTNode* tree, int key, int direction);

};

/*
 * Constructor
 */

RBTree::RBTree() :mRoot(NULL)
{
    mRoot = NULL;
};

/*
 * Destructor
 */

RBTree::~RBTree()
{
    destroy();
}

/*
 * (Recursively) find the node by its data value
 */
RBTNode* RBTree::search(RBTNode* x, int row, int col, int key) const
{
    if (x == NULL || (x->row == row && x->col == col))
        return x;

    if (key < x->key)
        return search(x->left, row, col, key);
    else
        return search(x->right, row, col, key);
}


RBTNode* RBTree::search(int row, int col, int key)
{
    return search(mRoot, row, col, key);
}


/*
 * (Iteratively) find the node by its data value
 */

RBTNode* RBTree::iterativeSearch(RBTNode* x, int key) const
{
    //    while ((x!=NULL) && (x->key!=key))
    while ((x != NULL))
    {
        if (x->key == key)
        {
            cout << "(" << x->row << "," << x->col << ")" << endl;
        }
        if (key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    return x;
}


//RBTNode* RBTree::iterativeSearch(int key)
void RBTree::iterativeSearch(int key)
{
    iterativeSearch(mRoot, key);
}

/*
 * find minimum node and output related information
 */

RBTNode* RBTree::minimum(RBTNode* tree)
{
    if (tree == NULL)
        return NULL;

    while (tree->left != NULL)
        tree = tree->left;

    return tree;
}


void RBTree::minimum()
{
    RBTNode* p = minimum(mRoot);
    if (p != NULL)
    {
        cout << "Minimum: " << p->key << " at (" << p->row << "," << p->col << ")";
        return;
    }
    cout << "Cannot find minimum!" << endl;
}

/*
 * find maximum node and output related information
 */

RBTNode* RBTree::maximum(RBTNode* tree)
{
    if (tree == NULL)
        return NULL;

    while (tree->right != NULL)
        tree = tree->right;
    return tree;
}


void RBTree::maximum()
{
    RBTNode* p = maximum(mRoot);
    if (p != NULL)
    {
        cout << "Maximum: " << p->key << " at (" << p->row << "," << p->col << ")" << endl;
        return;
    }
    cout << "Cannot find maximum!" << endl;
}


void RBTree::leftRotate(RBTNode*& root, RBTNode* x)
{
    // set right child of x as y
    RBTNode* y = x->right;

    // Set "left child of Y" to "right child of X"
    // If y's left child is not empty, set "x" to "Father of Y's left child"
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;

    // Set "Father of X" to "Father of Y"
    y->parent = x->parent;

    if (x->parent == NULL)
    {
        root = y;            // If the "father of X" is an empty node, y is set as the root node
    }
    else
    {
        if (x->parent->left == x)
            x->parent->left = y;    // If X is the left child of its parent, then y is set to "the left child of x's parent"
        else
            x->parent->right = y;    // If X is the left child of its parent, then y is set to "the left child of x's parent"
    }

    // Set "X" to "Left child of Y"
    y->left = x;
    // Set "parent node of X" to "y"
    x->parent = y;
}

/*
 * Right rotate the node (y) of the red-black tree
 *
 * Diagram of right-rotation (left-rotation of node Y)
 *            py                               py
 *           /                                /
 *          y                                x
 *         /  \      --(rr)-->            /  \                     #
 *        x   ry                           lx   y
 *       / \                                   / \                   #
 *      lx  rx                                rx  ry
 *
 */

void RBTree::rightRotate(RBTNode*& root, RBTNode* y)
{
    // Set x to be the left child of the current node.
    RBTNode* x = y->left;

    // Set "right child of X" to "left child of Y"
    // If "right child of X" is not empty, set "y" to "father of right child of X"
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;

    // Set "Father of Y" to "Father of X"
    x->parent = y->parent;

    if (y->parent == NULL)
    {
        root = x;            // If the "father of Y" is an empty node, then x is set as the root node
    }
    else
    {
        if (y == y->parent->right)
            y->parent->right = x;    // If Y is the right child of its parent, set x to be "the right child of its parent"
        else
            y->parent->left = x;    // (y is the left child of its parent) set X to "the left child of the parent of X"
    }

    // Set "Y" to "the right child of X"
    x->right = y;

    // Set "parent node of Y" to "x"
    y->parent = x;
}

/*
 * Red-black tree insert correction function
 *
 * This function is called after the node has been inserted into the red-black tree (out of balance)£»
 * The aim was to recreate it as a red-black tree
 */

void RBTree::insertFixUp(RBTNode*& root, RBTNode* node)
{
    RBTNode* parent, * gparent;

    // If the parent node exists and the parent node's color is red
    while ((parent = rb_parent(node)) && rb_is_red(parent))
    {
        gparent = rb_parent(parent);

        // If the parent node is the left child of the grandparent node
        if (parent == gparent->left)
        {
            // Case 1 condition: Uncle node is red
            {
                RBTNode* uncle = gparent->right;
                if (uncle && rb_is_red(uncle))
                {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gparent);
                    node = gparent;
                    continue;
                }
            }

            // Case 2 conditions: the uncle is black, and the current node is the right child
            if (parent->right == node)
            {
                RBTNode* tmp;
                leftRotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            // Case 3 conditions: the uncle is black, and the current node is the left child.
            rb_set_black(parent);
            rb_set_red(gparent);
            rightRotate(root, gparent);
        }
        else// If Z's parent is the right child of Z's grandparent
        {
            // Case 1 condition: Uncle node is red
            {
                RBTNode* uncle = gparent->left;
                if (uncle && rb_is_red(uncle))
                {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gparent);
                    node = gparent;
                    continue;
                }
            }

            // Case 2 conditions: the uncle is black, and the current node is the left child
            if (parent->left == node)
            {
                RBTNode* tmp;
                rightRotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            // Case 3 conditions: the uncle is black, and the current node is the right child.
            rb_set_black(parent);
            rb_set_red(gparent);
            leftRotate(root, gparent);
        }
    }

    // Set the root node to black
    rb_set_black(root);
}

/*
 * Insert the node into the red-black tree
 */

void RBTree::insert(RBTNode*& root, RBTNode* node)
{
    RBTNode* y = NULL;
    RBTNode* x = root;

    // 1. Take the red-black tree as a binary search tree and add nodes to the binary search tree.
    while (x != NULL)
    {
        y = x;
        if (node->key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    node->parent = y;
    if (y != NULL)
    {
        if (node->key < y->key)
            y->left = node;
        else
            y->right = node;
    }
    else
        root = node;

    // 2. Set the color of the node to red
    node->color = RED;

    // Fix it to be a binary search tree
    insertFixUp(root, node);
}

/*
 * Insert the node (key for the node key value) into the red-black tree
 *
 */

void RBTree::insert(int key, int row, int col)
{
    RBTNode* z = NULL;

    // Returns if the new node fails.
    if ((z = new RBTNode(key, row, col, BLACK, NULL, NULL, NULL)) == NULL)
        return;

    insert(mRoot, z);
}

/*
 * Deletion correction function
 */

void RBTree::removeFixUp(RBTNode*& root, RBTNode* node, RBTNode* parent)
{
    RBTNode* other;

    while ((!node || rb_is_black(node)) && node != root)
    {
        if (parent->left == node)
        {
            other = parent->right;
            if (rb_is_red(other))
            {
                // Case 1: Brother W of X is red
                rb_set_black(other);
                rb_set_red(parent);
                leftRotate(root, parent);
                other = parent->right;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right)))
            {
                // Case 2: X's brother W is black, and both of W's children are black
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else
            {
                if (!other->right || rb_is_black(other->right))
                {
                    // Case 3: X's brother W is black, and W's left child is red, and the right child is black.
                    rb_set_black(other->left);
                    rb_set_red(other);
                    rightRotate(root, other);
                    other = parent->right;
                }
                // Case 4: X's brother W is black;And w's right child is red, and the left child is any color.
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->right);
                leftRotate(root, parent);
                node = root;
                break;
            }
        }
        else
        {
            other = parent->left;
            if (rb_is_red(other))
            {
                // Case 1: Brother W of X is red
                rb_set_black(other);
                rb_set_red(parent);
                rightRotate(root, parent);
                other = parent->left;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right)))
            {
                // Case 2: X's brother W is black, and both of W's children are black
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else
            {
                if (!other->left || rb_is_black(other->left))
                {
                    // Case 3: X's brother W is black, and W's left child is red, and the right child is black.
                    rb_set_black(other->right);
                    rb_set_red(other);
                    leftRotate(root, other);
                    other = parent->left;
                }
                // Case 4: X's brother W is black;And w's right child is red, and the left child is any color.
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->left);
                rightRotate(root, parent);
                node = root;
                break;
            }
        }
    }
    if (node)
        rb_set_black(node);
}

/*
 * Deletes a node and returns the deleted node
 */

void RBTree::remove(RBTNode*& root, RBTNode* node)
{
    RBTNode* child, * parent;
    RBTColor color;

    // The "left and right children are not empty" condition of the deleted node.
    if ((node->left != NULL) && (node->right != NULL))
    {
        RBTNode* replace = node;

        // Get subsequent nodes
        replace = replace->right;
        while (replace->left != NULL)
            replace = replace->left;

        // "Node" is not the root node (only the root node does not have a parent)
        if (rb_parent(node))
        {
            if (rb_parent(node)->left == node)
                rb_parent(node)->left = replace;
            else
                rb_parent(node)->right = replace;
        }
        else
            // "Node" is the root node, update the root node.
            root = replace;

        // The Child is the right child of the "replace node" and also the "adjust node".
        // There must be no left child in the "substitution node"!Because it's a successor node.
        child = replace->right;
        parent = rb_parent(replace);
        // Save the color of the "replace node"
        color = rb_color(replace);

        // The deleted node is the parent of its successor node.
        if (parent == node)
        {
            parent = replace;
        }
        else
        {
            // child is not empty
            if (child)
                rb_set_parent(child, parent);
            parent->left = child;

            replace->right = node->right;
            rb_set_parent(node->right, replace);
        }

        replace->parent = node->parent;
        replace->color = node->color;
        replace->left = node->left;
        node->left->parent = replace;

        if (color == BLACK)
            removeFixUp(root, child, parent);

        delete node;
        return;
    }

    if (node->left != NULL)
        child = node->left;
    else
        child = node->right;

    parent = node->parent;
    // Save the color of the "replace node"
    color = node->color;

    if (child)
        child->parent = parent;

    // The "Node node" is not the root node
    if (parent)
    {
        if (parent->left == node)
            parent->left = child;
        else
            parent->right = child;
    }
    else
        root = child;

    if (color == BLACK)
        removeFixUp(root, child, parent);
    delete node;
}

/*
 * Deletes a key node in the red-black tree
 *
 */

void RBTree::remove(int row, int col, int key)
{
    RBTNode* node;

    // Find the node that corresponds to the key, and delete it if you find it
    if ((node = search(mRoot, row, col, key)) != NULL)
        remove(mRoot, node);
}

/*
 * Destroy the red black tree
 */

void RBTree::destroy(RBTNode*& tree)
{
    if (tree == NULL)
        return;

    if (tree->left != NULL)
        destroy(tree->left);
    if (tree->right != NULL)
        destroy(tree->right);

    delete tree;
    tree = NULL;
}


void RBTree::destroy()
{
    destroy(mRoot);
}

/*
 * Print
 *
 * direction  --  0£¬Indicates that the node is the root node;
 *               -1£¬Indicates that the node is the left child of its parent;
 *                1£¬Indicates that the node is the right child of its parent.
 */

void RBTree::print(RBTNode* tree, int key, int direction)
{
    if (tree != NULL)
    {
        if (direction == 0)    // tree is the root node
            cout << setw(2) << tree->key << "(B) is root" << endl;
        else                // tree is a branch node
            cout << setw(2) << tree->key << (rb_is_red(tree) ? "(R)" : "(B)") << " is " << setw(2) << key << "'s " << setw(12) << (direction == 1 ? "right child" : "left child") << endl;

        print(tree->left, tree->key, -1);
        print(tree->right, tree->key, 1);
    }
}


void RBTree::print()
{
    if (mRoot != NULL)
        print(mRoot, mRoot->key, 0);
}

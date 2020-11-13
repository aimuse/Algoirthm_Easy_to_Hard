#include <iostream>
#include <vector>
using namespace std;

typedef struct node {
    int data;
    int row;
    int col;
    struct node* next;
}hashNode;


// Hashtable with linked list handling the hashing conflict
class HashTable {
private:
    vector<hashNode*> nodes;
    int length;
    int Max_Prime;

    // check if num is a prime number
    bool isPrime(int num) {
        bool flag = true;
        if (num <= 1) {
            flag = false;
        }
        else if (num == 2) {
            flag = true;
        }
        else {
            for (int i = 2; i < num - 1; i++) {
                //
                if (num % i == 0) {
                    flag = false;
                    break;
                }
            }
        }
        return flag;
    }

    // calculate the max prime number less than the length of datasheet
    int getMaxPrime() {
        for (int i = this->length - 1; i >= 0; i--) {
            if (this->isPrime(i)) {
                return i;
            }
        }
    }

    // hash address calculation
    int hash(int num) {
        return num % this->Max_Prime;
    }

    hashNode* attach(hashNode* node1, hashNode* node2) {
        hashNode* cur = node1;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = node2;
        return node1;
    }

    void printList(hashNode* node) {
        hashNode* cur = node;
        while (cur) {
            cout << "(" << cur->row << "," << cur->col << ") ";
            cur = cur->next;
        }
        cout << endl;
    }
public:
    // Constructor
    HashTable() {
    }

    // initialize a hashtable by given size
    void init(int length) {
        this->length = length;
        nodes.reserve(this->length);
        this->Max_Prime = this->getMaxPrime();
        for (int i = 0; i < this->length; i++) {
            hashNode* node = new hashNode;
            node->data = i;
            node->next = NULL;
            this->nodes.push_back(node);
        }
    }

    // insert a number
    void insert(int row, int col, int num) {
        // calculate hash address
        int index = this->hash(num);
        // initialize a new node for given number
        hashNode* node = new hashNode;
        node->data = num;
        node->row = row;
        node->col = col;
        node->next = NULL;
        // concatenate the new code with the hashNode linked list
        hashNode* tmp = NULL;
        tmp = this->nodes[index];
        this->nodes[index] = this->attach(tmp, node);
    }

    // find the given number and output its location in datasheet
    void find(int num) {
        int index = this->hash(num);
        hashNode* cur = this->nodes[index];
        while (cur->data != num && cur != NULL) {
            cur = cur->next;
        }
        if (cur == NULL) {// if there is no num
            cout << "There is not " << num << " in the datasheet." << endl;
        }
        printList(nodes[index]->next);
    }


    // print out the hashtable structure
    void print() {
        for (int i = 0; i < this->nodes.size(); i++) {
            if (this->nodes[i]->next != NULL) {
                cout << i << ": ";
                this->printList(this->nodes[i]->next);
            }
        }
    }
};
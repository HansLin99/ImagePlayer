#include "chain.h"
#include "chain_given.cpp"

// PA1 functions

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain(){
    clear();
    delete head_;
    head_ = NULL;
}

/**
 * Inserts a new node at the end of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
void Chain::insertBack(const Block & ndata){
    Node* inserted = new Node(ndata);
    Node* prevNode = head_->prev;
    prevNode->next = inserted;
    inserted->prev = prevNode;
    head_->prev = inserted;
    inserted->next = head_;
    length_++;
}

/**
 * Modifies the Chain by moving the subchain of len Nodes,
 * starting at position startPos, dist positions toward the
 * end of the chain.  If startPos + len - 1 + dist > length
 * then dist = length - startPos - len + 1 (to prevent gaps
 * in the new chain).
 * The subchain occupies positions (startPos + dist) through
 * (startPos + dist + len - 1) of the resulting chain.
 * The order of subchain nodes is not changed in the move.
 * You may assume that: 1 <= startPos <= length - len + 1,
 * 0 <= dist <= length, and 0 <= len <= length.
 */
void Chain::moveBack(int startPos, int len, int dist){
    Node* curr1= walk(head_, startPos), * curr2, * targ;
    Node* prevN, * nextN;
    
    if (dist>0) {
        if (startPos + len - 1 + dist > length_) {
            dist = length_ - startPos - len + 1;
            curr2 = walk(curr1, len-1);
            
            targ = head_->prev;
            
            prevN = curr1->prev;
            nextN = curr2->next;
            
            targ->next = curr1;
            curr1->prev = targ;
            
            curr2->next = head_;
            head_->prev = curr2;
            
            prevN->next = nextN;
            nextN->prev = prevN;
        } else {
            
            curr2 = walk(curr1, len-1);
            
            targ = walk(curr2, dist);
            
            prevN = curr1->prev;
            nextN = curr2->next;
            
            Node* targNext = targ->next;
            
            targ->next = curr1;
            curr1->prev = targ;
            
            curr2->next = targNext;
            targNext->prev = curr2;
            
            prevN->next = nextN;
            nextN->prev = prevN;
        }
    }
    
}

/**
 * Rolls the current Chain by k nodes: reorders the current list
 * so that the first k nodes are the (n-k+1)th, (n-k+2)th, ... , nth
 * nodes of the original list followed by the 1st, 2nd, ..., (n-k)th
 * nodes of the original list where n is the length.
 */
void Chain::roll(int k){
    
    if (k>0 && k<length_) {
        Node* curr = walk(head_, length_ - k);
        Node* prevNode = head_->prev;
        Node* nextNode = head_->next;
        Node * nextNode1 = curr->next;
        curr->next = head_;
        head_->prev = curr;
        head_->next=nextNode1;
        nextNode1->prev=head_;
        prevNode->next = nextNode;
        nextNode->prev = prevNode;
    }
    
    
}

/**
 * Modifies the current chain by reversing the sequence
 * of nodes from pos1 up to and including pos2. You may
 * assume that pos1 and pos2 exist in the given chain,
 * and pos1 <= pos2.
 * The positions are 1-based.
 */
void Chain::reverseSub(int pos1, int pos2){
    Node* tmp1= walk(head_, pos1), *tmp2=walk(head_, pos2);
    
    
    Node* prevNode = tmp1->prev;
    Node* nextNode = tmp2->next;
    
    
    
    prevNode->next = tmp2;
    nextNode->prev = tmp1;

    Node* curr = tmp1;
    Node* prevN = NULL;
    
    while (curr != tmp2) {
        Node* nextN = curr->next;
        
        if (prevN == NULL) {
            curr->next = nextNode;
            curr->prev = nextN;
        } else {
            curr->next = prevN;
            curr->prev = nextN;
        }
        
        prevN = curr;
        curr = nextN;
        
        
    }
    
    curr->next = prevN;
    curr->prev = prevNode;
    
}

/*
 * Modifies both the current chain and the "other" chain by removing
 * nodes from the other chain and adding them between the nodes
 * of the current chain -- one "other" node between two current nodes --
 * until one of the two chains is exhausted.  Then the rest of the
 * nodes in the non-empty chain follow.
 * The length of the resulting chain should be the sum of the lengths
 * of current and other. The other chain should have only
 * the head_ sentinel at the end of the operation.
 * The weave fails and the original
 * chains should be unchanged if block sizes are different.
 * In that case, the result of the function should be:
 * cout << "Block sizes differ." << endl;
 */
void Chain::weave(Chain & other) {
    Node* curr = head_->next;
    Node* curr2 = other.head_->next;
    Node* next1;
    bool isFirst = true;
    
    if (width_ == other.width_ && height_ == other.height_) {
        while (curr != head_ && curr != other.head_) {
            
            if (isFirst) {
                next1 = curr->next;
                curr->next = curr2;
                curr2->prev = curr;
                isFirst = !isFirst;
                curr = curr2;
            } else {
                curr2 = curr->next;
                curr->next = next1;
                next1->prev = curr;
                isFirst = !isFirst;
                curr = next1;
            }
            
        }
        
        if (curr == head_) {
            Node* prevN = curr->prev;
            prevN->next = curr2;
            curr2->prev = prevN;
            
            while (curr->next != other.head_) {
                curr=curr->next;
            }
            
            curr->next = head_;
            head_->prev = curr;
        } else {
            Node* prevN = curr->prev;
            prevN->next = next1;
            next1->prev = prevN;
        }
        other.head_->prev = other.head_;
        other.head_->next = other.head_;
        length_ += other.length_;
    } else cout << "Block sizes differ." << endl;

    
    
    
}




/**
 * Destroys all dynamically allocated memory associated with the
 * current Chain class except for the sentinel head_. Sets length_
 * to zero.  After clear() the chain represents an empty chain.
 */
void Chain::clear() {
    Node * nextNode;
    for (Node * curr = head_->next; curr != head_ ; curr = nextNode) {
        nextNode = curr->next;
        delete curr;
        curr = NULL;
    }
    head_->next = head_;
    head_->prev = head_;
    length_ = 0;
}

/**
 * Makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */
void Chain::copy(Chain const& other) {
    Node* curr1 = head_;
    Node* curr2 = other.head_->next;
    
    while (curr2 != other.head_) {
        
        Node* insertedNode = new Node(curr2->data);
        
        curr1->next = insertedNode;
        insertedNode->prev = curr1;
        
        curr1 = insertedNode;
        curr2 = curr2->next;
        
        
    }
    
    curr1->next = head_;
    head_->prev = curr1;
    
    length_=other.length_;
    height_=other.height_;
    width_=other.width_;
}

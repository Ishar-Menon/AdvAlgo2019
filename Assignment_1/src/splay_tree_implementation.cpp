#include "../include/splay_tree.hpp"

using namespace std;


typedef struct Node
{
    int key;
    struct Node* left;
    struct Node* right;
}Node;

// Creates a new node 
Node* generateNewNode(int key)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

class splay_tree_implementation : public splay_tree
{   
    private:
        int number_of_nodes;
        Node* leftRotate(Node* curentNode);
        Node* rightRotate(Node* curentNode);
        Node* getBalancedTree(Node* root,int key);
        void getPreOrder(Node* root,vector<int> &visitOrder);
        void getPostOrder(Node* root,vector<int> &visitOrder);
        int searchWithoutSplay(Node* root,int key);
        void insertNodeIntoTree(Node* root,int key);
        int deleteElement(Node* root,Node* parent,int key);
        int getIndordeSuccessor(Node* root);
        void changeChild(Node* currentNode,Node* parent,Node* child);
        void getInOrder(Node* root,vector<int> &visitOrder);
    public:
        Node* node;
        Node* root;
        splay_tree_implementation();
        int get_num_nodes();
        int find(int);
        void insert(int);
        void remove(int);
        vector<int> post_order();
        vector<int> pre_order();
        vector<int> in_order();
        void printVector(vector<int> &visitOrder);
                
};

splay_tree_implementation :: splay_tree_implementation()
{
    number_of_nodes = 0;
    root = NULL;
}
Node* splay_tree_implementation :: leftRotate(Node* currentNode)
{
    Node* child = currentNode->right;
    currentNode->right = child->left;
    child->left = currentNode;
    return child;
}
Node* splay_tree_implementation :: rightRotate(Node* curentNode)
{
    Node* child = curentNode->left;
    curentNode->left = child->right;
    child->right = curentNode;
    return child;
}
Node* splay_tree_implementation :: getBalancedTree(Node *root, int key)  
{  
   
    if (root == NULL || root->key == key)  
        return root;  
  
      
    if (root->key > key)  
    {  
       
        if (root->left == NULL) return root;  
  
        // Zig-Zig Case
        if (root->left->key > key)  
        {  
           
            root->left->left = getBalancedTree(root->left->left, key);  
  
             
            root = rightRotate(root);  
        }
        // Zig-Zag Case  
        else if (root->left->key < key)
        {  
             
            root->left->right = getBalancedTree(root->left->right, key);  
  
              
            if (root->left->right != NULL)  
                root->left = leftRotate(root->left);  
        }  
  
       
        return (root->left == NULL)? root: rightRotate(root);  
    }  
    else 
    {  
         
        if (root->right == NULL) return root;  
  
        // Zag-Zig Case  
        if (root->right->key > key)  
        {  
             
            root->right->left = getBalancedTree(root->right->left, key);  
  
             
            if (root->right->left != NULL)  
                root->right = rightRotate(root->right);  
        }  
        // Zag-Zag Case
        else if (root->right->key < key)
        {  
             
            root->right->right = getBalancedTree(root->right->right, key);  
            root = leftRotate(root);  
        }  
  
        return (root->right == NULL)? root: leftRotate(root);  
    }  
}
int splay_tree_implementation :: get_num_nodes()
{
    return number_of_nodes;
} 

int splay_tree_implementation :: find(int key)
{
    Node* newRoot = getBalancedTree(this->root,key);
    this->root = newRoot;
    if(newRoot == NULL || newRoot->key != key)
    {
        return 0;
    }
    return 1;
}

// BST search
int splay_tree_implementation :: searchWithoutSplay(Node* root,int key)
{
    if(root == NULL)
    {
        return 0;
    }

    if(root->key == key)
    {
        return 1;
    }

    if(root->key > key)
    {
        return this->searchWithoutSplay(root->left,key);
    }

    else
    {
        return this->searchWithoutSplay(root->right,key);
    }    

}
// BST insertion
void splay_tree_implementation :: insertNodeIntoTree(Node* root,int key){
    if(root->key > key)
    {
        if(root->left == NULL)
        {
            Node* newNode = generateNewNode(key);
            root->left = newNode;
        }
        else
        {
            return this->insertNodeIntoTree(root->left,key);
        }
    }
    else
    {
        if(root->right == NULL)
        {
            Node* newNode = generateNewNode(key);
            root->right = newNode;
        }
        else
        {
            return this->insertNodeIntoTree(root->right,key);
        }
    }
}
void splay_tree_implementation :: insert(int key)
{
    // Check if tree is empty
    if(this->root == NULL)
    {
        this->root = generateNewNode(key);
        this->number_of_nodes += 1;
        return;
    }
    // If key is the root no insert and splay to be done
    if(this->root->key == key)
    {
        return;
    }

    // Check if node is present
    int alredyPresent = searchWithoutSplay(this->root,key);

    // If node is not present insert into tree
    if(!alredyPresent)
    {
        // normal bst insertion
        insertNodeIntoTree(this->root,key);
        this->number_of_nodes += 1;
    }

    // Splay the inserted node
    this->find(key);

}

// Gets the inorder successor of the node passed
int splay_tree_implementation :: getIndordeSuccessor(Node* root)
{
    Node* current = root->right;

    while(current->left != NULL)
    {
        current = current->left;
    }

    return current->key;
}

// removes the node and connects the node's parent to the nodes's child
void splay_tree_implementation :: changeChild(Node* currentNode,Node* parent,Node* child)
{
    // If current node is root change the root
    if(parent == NULL)
    {
        this->root = child;
        return;
    }
    
    if(parent->left == currentNode)
    {
        parent->left = child;
    }
    else
    {
        parent->right = child;
    }
}
// BST delete
int splay_tree_implementation :: deleteElement(Node* root,Node* parent,int key)
{
    if(key < root->key)
    {
        return this->deleteElement(root->left,root,key);
    }
    if(key > root->key)
    {
        return this->deleteElement(root->right,root,key);
    }

    // both children present
    if(root->left != NULL and root->right != NULL)
    {
        int newkeyValue =  this->getIndordeSuccessor(root);
        root->key = newkeyValue;
        this->deleteElement(root->right,root,newkeyValue);
          
    }
    // only left child present
    else if (root->left != NULL)
    {
        this->changeChild(root,parent,root->left);
        free(root);
    }
    // only right child present 
    else if (root->right != NULL)
    {
        this->changeChild(root,parent,root->right);
        free(root);
    }
    // no child present
    else
    {
        this->changeChild(root,parent,NULL);
        free(root);
    }
    // return the parent of the deleted node
    // If the node is root return 0 
    if(parent != NULL)
    {
        return parent->key;
    }
    else
    {
        return 0;
    }
}
void splay_tree_implementation :: remove(int key)
{
    // If node does not exits splay the last seen node and exit
    if(!this->searchWithoutSplay(this->root,key))
    {
        this->find(key);
        return;
    }

    // Check if node is root 
    int isroot = (this->root->key == key);
    
    // delete element and get parent
    int parent = this->deleteElement(this->root,NULL,key);
    this->number_of_nodes -= 1;

    // If node is not root splay its parent
    if(! isroot)
    {
        this->find(parent);
    }
}

vector<int> splay_tree_implementation :: post_order()
{
    vector<int> visitorder;
    this->getPostOrder(this->root,visitorder);
    return visitorder;
}
void splay_tree_implementation :: getPostOrder(Node* root,vector<int> &visitOrder)
{
    if (root != NULL) 
	{ 
		this->getPostOrder(root->left,visitOrder); 
		this->getPostOrder(root->right,visitOrder);
        visitOrder.push_back(root->key); 
	}
}
vector<int> splay_tree_implementation :: pre_order()
{
    vector<int> visitorder;
    this->getPreOrder(this->root,visitorder);
    return visitorder;
}
void splay_tree_implementation :: getPreOrder(Node* root,vector<int> &visitOrder)
{
    if (root != NULL) 
	{ 
        visitOrder.push_back(root->key); 
		this->getPreOrder(root->left,visitOrder); 
		this->getPreOrder(root->right,visitOrder); 
	}
}

vector<int> splay_tree_implementation :: in_order()
{
    vector<int> visitorder;
    this->getInOrder(this->root,visitorder);
    return visitorder;
}
void splay_tree_implementation :: getInOrder(Node* root,vector<int> &visitOrder)
{
    if (root != NULL) 
	{ 
        
		this->getInOrder(root->left,visitOrder);
        visitOrder.push_back(root->key);  
		this->getInOrder(root->right,visitOrder); 
	}
}
// Prints the vector passed 
void splay_tree_implementation :: printVector(vector<int> &visitOrder)
{
    for(int i=0;i<visitOrder.size();i++)
    {
        printf("%d ",visitOrder[i]);
    }
    printf("\n");
}





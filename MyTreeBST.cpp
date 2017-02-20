#include <iostream>
using namespace std;

/*
 *  1) add null node to avoid  null assignments
 *  2) what about repeated keys?  -> lists in nodes?
 */

class TreeBST {

private:

    struct Node {
        Node(int newData = 0){
            key = newData;
            up = left = right = nullptr;
        }
        int key;
        Node *up, *left, *right;
    };
    unsigned int treeSize;
    bool isEmpty;
    Node *node, *nullNode;;
    
//                          Prywatne metody
    void addNodeToTree(Node *father, int keyToAdd){
        if (nodeExist(father)){
            if (keyToAdd < father->key){
                if (nodeHasLeftSon(father)){
                    addNodeToTree(father->left, keyToAdd);
                }
                else{
                    linkAsLeftSon(father);
                    setTreeSize(treeSize + 1);
                }
            }
            else if (keyToAdd >= father->key){
                if (nodeHasRightSon(father)){
                    addNodeToTree(father->right, keyToAdd);
                }
                else{
                    linkAsRightSon(father);
                    setTreeSize(treeSize + 1);
                }
            }
        }
    }

    void deleteNode(Node *nodeToDelete){
        if (nodeHasNoSons(nodeToDelete)){
            deleteNodeWith_NoSons(nodeToDelete);
        }
        else if (nodeHasOneSon(nodeToDelete)){
            deleteNodeWith_OneSon(nodeToDelete);
        }
        else if (nodeHasTwoSons(nodeToDelete)){
            deleteNodeWith_TwoSons(nodeToDelete);
        }
        setTreeSize(treeSize - 1);
    }

    void deleteNodeWith_NoSons(Node* nodeToDelete){
        if (isLeftSon(nodeToDelete)){
            nodeToDelete->up->left = NULL;
        }
        else{
            nodeToDelete->up->right = NULL;
        }
        delete nodeToDelete;
    }

    void deleteNodeWith_OneSon(Node* nodeToDelete){
        if (isLeftSon(nodeToDelete)){
            if (nodeHasLeftSon(nodeToDelete)){
                nodeToDelete->up->left = nodeToDelete->left;
                nodeToDelete->left->up = nodeToDelete->up;
            }
            else{
                nodeToDelete->up->left = nodeToDelete->right;
                nodeToDelete->right->up = nodeToDelete->up;
            }
        }
        else {
            if (nodeHasLeftSon(nodeToDelete)){
                nodeToDelete->up->right = nodeToDelete->left;
                nodeToDelete->left->up = nodeToDelete->up;
            }
            else{
                nodeToDelete->up->right = nodeToDelete->right;
                nodeToDelete->right->up = nodeToDelete->up;
            }
        }
        delete nodeToDelete;
    }

    void deleteNodeWith_TwoSons(Node* nodeToDelete){
        //can also find predecessor
        findNodeSuccesor(nodeToDelete);
        //node is a succesor
        copyDataFromTo(node, nodeToDelete);
        deleteNode(node);
    }

    void addRootNode(int keyToAdd){
        try{
            root = new Node(keyToAdd);
        }
        catch(exception exc){
            delete root;
        }
        setTreeSize(treeSize + 1);
        isEmpty = false;
    }

    void createNode(int keyToAdd){
        try{
            node = new Node(keyToAdd);
        }
        catch(exception exc){
            ;
        }
    }

    inline bool nodeExist(Node *node){
        if (node){
            return true;
        }
        else{
            return false;
        }
    }

    inline bool nodeHasLeftSon(Node *nodeToCheck){
        if (nodeToCheck->left){
            return true;
        }
        else{
            return false;
        }
    }

    inline bool nodeHasRightSon(Node *nodeToCheck){
        if (nodeToCheck->right){
            return true;
        }
        else{
            return false;
        }
    }

    inline bool nodeHasNoSons(Node *nodeToCheck){
        if (nodeHasLeftSon(nodeToCheck) || nodeHasRightSon(nodeToCheck)){
            return false;
        }
        else{
            return true;
        }
    }

    bool nodeHasOneSon(Node *node){
        if (nodeHasLeftSon(node)){
            if (!nodeHasRightSon(node)){
                return true;
            }
            return false;
        }
        else if (nodeHasRightSon(node)){
            if (!nodeHasLeftSon(node)){
                return true;
            }
            return false;
        }
        else{
            return false;
        }
    }

    inline bool nodeHasTwoSons(Node* node){
        if (nodeHasLeftSon(node) && nodeHasRightSon(node)){
            return true;
        }
        else{
            return false;
        }
    }

    inline bool isLeftSon(Node* node){
        if (node == node->up->left){
            return true;
        }
        else{
            return false;
        }
    }

    inline bool isRightSon(Node* node){
        if (node == node->up->right){
            return true;
        }
        else{
            return false;
        }
    }

    inline void linkAsLeftSon(Node *father){
        father->left = node;
        node->up = father;
    }

    inline void linkAsRightSon(Node *father){
        father->right = node;
        node->up = father;
    }

    inline void copyDataFromTo(Node *source, Node *direction){
        direction->key = getNodeKey(source);
    }

    inline void setTreeSize(unsigned int newSize){
        treeSize = newSize;
    }

public:

    Node *root;

    TreeBST(){
        isEmpty = true;
        treeSize = 0;
        root = NULL;
        //Student number as important information that it is special node;
        nullNode = new Node(155034);
    }

    void addNodeToTree(int keyToAdd){
        if (isEmpty){
            addRootNode(keyToAdd);
        }
        else{
            createNode(keyToAdd);
            addNodeToTree(root,keyToAdd);
        }
    }

    void deleteNodeFromTree(Node *startNode, int searchKey){
        //usually start from root
        if(nodeExist(startNode)){
            findFirstKeyFrom(startNode, searchKey);
            deleteNode(node);
        }
    }

    void findFirstKeyFrom(Node *nodeToStart, int searchKey){
        if (nodeExist(nodeToStart)){
            if (searchKey < nodeToStart->key){
                findFirstKeyFrom(nodeToStart->left, searchKey);
            }
            else if (searchKey > nodeToStart->key){
                findFirstKeyFrom(nodeToStart->right, searchKey);
            }
            else if (searchKey == nodeToStart->key){
                node = nodeToStart;
                node->key = getNodeKey(nodeToStart);
            }
        }
    }

    void findNodeSuccesor(Node *succesor){
        if (nodeHasRightSon(succesor)){
            findMin(succesor->right);
        }
        else{
            while (nodeExist(succesor->up) && isRightSon(succesor)){
                succesor = succesor->up;
            }

            if (nodeExist(succesor->up) && isLeftSon(succesor)){
                node = succesor->up;
            }
            else{
                node = NULL;
            }
        }
    }

    void findNodePredecessor(Node *predecessor){
        if (nodeHasLeftSon(predecessor)){
            findMax(predecessor);
        }
        else{
            while (nodeExist(predecessor->up) && isLeftSon(predecessor)){
                predecessor = predecessor->up;
            }

            if (nodeExist(predecessor->up) && isRightSon(predecessor)){
                node = predecessor->up;
            }
            else{
                node = NULL;
            }
        }
    }

    void findMin(Node *minNode){
        if (nodeHasLeftSon(minNode)){
            findMin(minNode->left);
        }
        else{
            node = minNode;
        }
    }

    void findMax(Node *maxNode){
        if (nodeHasRightSon(maxNode)){
            findMax(maxNode->right);
        }
        else{
            node = maxNode;
        }
    }


    void writeKeys_InOrder_From(Node *topNode){
        if (nodeExist(topNode)){
            if(nodeHasLeftSon(topNode)){
                writeKeys_InOrder_From(topNode->left);
                writeKeyWithComma(topNode);
                if (nodeHasRightSon(topNode)){
                    writeKeys_InOrder_From(topNode->right);
                }
            }
            else{
                writeKeyWithComma(topNode);
                if (nodeHasRightSon(topNode)){
                    writeKeys_InOrder_From(topNode->right);
                }
            }
        }
    }

    void writeKeys_PreOrder_From(Node *topNode){
        if (nodeExist(topNode)){
            writeKeyWithComma(topNode);
            if (nodeHasLeftSon(topNode)) {
                writeKeys_PreOrder_From(topNode->left);
            }
            if (nodeHasRightSon(topNode)){
                writeKeys_PreOrder_From(topNode->right);
            }
        }
    }

    void writeKeys_PostOrder_From(Node *topNode){
        if (nodeExist(topNode)){
            if (nodeHasLeftSon(topNode)) {
                writeKeys_PostOrder_From(topNode->left);
            }
            if (nodeHasRightSon(topNode)){
                writeKeys_PostOrder_From(topNode->right);
            }
            writeKeyWithComma(topNode);
        }
    }

    inline void writeKeyWithComma(Node *node){
        int number = getNodeKey(node);
        cout<<number<<",";
    }

    inline int getNodeKey(Node *node){
        return node->key;
    }

};

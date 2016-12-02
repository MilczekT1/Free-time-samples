#include <iostream>
#include <String.h>
using namespace std;
#define RED -1
#define BLACK 1

/*  Every node is black or red
 *  Every live is black
 *  Red node has 2 black sons
 *  Every simple route from node to live contain the same amount of black nodes
 */

/*
 *  TODO:
 *  1) exceptions
 *  2) destructor
 */

class TreeRB {

private:

    struct Node{
        Node(int newData = 0){
            if (newData == 155034){
                color = BLACK;
            }
            else{
                color = RED;
            }
            key = newData;
            additionalBlack = false;
        }
        void setPointersToGuardian(Node *Guard){
            up = left = right = Guard;
        }
        int key;
        int color;
        bool additionalBlack;
        Node *up, *left, *right;
    };

    unsigned int treeSize;
    bool isEmpty;
    Node *node;
    Node *Guardian;


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
            deleteAdditionalBlackFrom(Guardian);
        }
        setTreeSize(treeSize - 1);
        checkIfEmptyAndSet(treeSize);
    }

    void deleteNodeWith_NoSons(Node *nodeToDelete){
        if (isLeftSon(nodeToDelete)){
            nodeToDelete->up->left = Guardian;
        }
        else{
            nodeToDelete->up->right = Guardian;
        }
        delete nodeToDelete;
    }

    void deleteNodeWith_OneSon(Node *nodeToDelete){
        if (isLeftSon(nodeToDelete)){
            if (nodeHasLeftSon(nodeToDelete)){
                nodeToDelete->up->left = nodeToDelete->left;
                nodeToDelete->left->up = nodeToDelete->up;
                if (nodeIs(BLACK, nodeToDelete)){
                    setColor(nodeToDelete->left, BLACK);
                }
            }
            else{
                nodeToDelete->up->left = nodeToDelete->right;
                nodeToDelete->right->up = nodeToDelete->up;
                if (nodeIs(BLACK, nodeToDelete)){
                    setColor(nodeToDelete->right, BLACK);
                }
            }
        }
        else {
            if (nodeHasLeftSon(nodeToDelete)){
                nodeToDelete->up->right = nodeToDelete->left;
                nodeToDelete->left->up = nodeToDelete->up;
                if (nodeIs(BLACK, nodeToDelete)){
                    setColor(nodeToDelete->left, BLACK);
                }
            }
            else{
                nodeToDelete->up->right = nodeToDelete->right;
                nodeToDelete->right->up = nodeToDelete->up;
                if (nodeIs(BLACK, nodeToDelete)){
                    setColor(nodeToDelete->right, BLACK);
                }
            }
        }
        delete nodeToDelete;
    }

    void deleteNodeWith_TwoSons(Node *nodeToDelete){

        findNodeSuccesor(nodeToDelete);
        Node *succesor = node;
        if (isRightSon(succesor)){
            //3)
            if (isLeftSon(nodeToDelete)){
                nodeToDelete->up->left = nodeToDelete->right;
            }
            else{
                nodeToDelete->up->right = nodeToDelete->right;
            }
            nodeToDelete->right->up = nodeToDelete->up;
            nodeToDelete->right->left = nodeToDelete->left;
            nodeToDelete->left->up = nodeToDelete->right;


            if (nodeIs(BLACK, nodeToDelete)){
                setColor(succesor, BLACK); // f."give son father's color"
            }
            else{
                setColor(succesor, RED);
            }

            setAdditionalBlackTo(succesor->right);
            delete nodeToDelete;

        }
        else{
            //4)
            copyDataFromTo(succesor, nodeToDelete);
            succesor->up->left = succesor->right;
            if (nodeExist(succesor->right)) {
                succesor->right->up = succesor->up;
                if (nodeIs(BLACK, succesor)){
                    setColor(succesor->right, BLACK);
                    setAdditionalBlackTo(succesor->right);

                    succesor = succesor->right;
                    delete node;//old succesor

                    repairAfterDeletion(succesor);
                }
            }
            else{
                delete succesor;
                //setAdditionalBlackTo(succesor->up->left); //at guardian
                //repairAfterDeletion(succesor->up->left);
            }
        }
    }

    void repairAfterDeletion(Node *startNode){
        //I)
        if (brotherIs(RED, startNode)){
            if (isLeftSon(startNode)) {
                changeColor(startNode->up);
                changeColor(startNode->up->right);
                rotate_Left(startNode->up);

            }
            else{
                changeColor(startNode->up);
                changeColor(startNode->up->left);
                rotate_Right(startNode->up);
            }
        }
        else{
            if (isLeftSon(startNode)){
                //II)
                if (nodeHasTwoBlackSons(startNode->up->right)){
                    deleteAdditionalBlackFrom(startNode);
                    setAdditionalBlackTo(startNode->up);
                    changeColor(startNode->up->right);
                    repairAfterDeletion(startNode->up);
                }
                //III) i IV)
                else if (nodeHasTwoSons(startNode->up->right)){
                    Node *brother = startNode->up->right;
                    if (nodeIs(RED, brother->left) && nodeIs(BLACK, brother->right)){
                        changeColor(brother);
                        changeColor(brother->left);
                        rotate_Right(brother);
                    }
                    //IV)
                    if (nodeIs(RED, brother->right)){
                        if (nodeIs(RED, startNode->up)){
                            setColor(brother, RED);
                        }
                        else{
                            setColor(brother, BLACK);
                        }

                        rotate_Left(startNode->up);

                        deleteAdditionalBlackFrom(startNode);
                        if (nodeIs(RED, startNode->up)){
                            changeColor(startNode->up);
                        }
                        else{
                            ;
                        }

                        setColor(startNode->up->up->right, BLACK);
                        //#
                        if (isRoot(startNode->up->up)){
                            setColor(startNode->up->up, BLACK);
                        }
                    }
                }
            }
            else{
                //II)
                if (nodeHasTwoBlackSons(startNode->up->left)){
                    deleteAdditionalBlackFrom(startNode);
                    setAdditionalBlackTo(startNode->up);
                    changeColor(startNode->up->left);
                    repairAfterDeletion(startNode->up);
                }
                //III) i IV)
                else if (nodeHasTwoSons(startNode->up->left)){
                    Node *brother = startNode->up->left;
                    if (nodeIs(RED, brother->left) && nodeIs(BLACK, brother->right)){
                        changeColor(brother);
                        changeColor(brother->right);
                        rotate_Left(brother);
                    }

                    //IV)
                    if (nodeIs(RED, brother->right)){
                        if (nodeIs(RED, startNode->up)){
                            setColor(brother, RED);
                        }
                        else{
                            setColor(brother, BLACK);
                        }
                        rotate_Right(startNode->up);
                        deleteAdditionalBlackFrom(startNode);
                        if (nodeIs(RED, startNode->up)){
                            changeColor(startNode->up);
                        }
                        else{
                            ;
                        }
                        setColor(startNode->up->up->left, BLACK);
                        //#
                        if (isRoot(startNode->up->up)){
                            setColor(startNode->up->up, BLACK);
                        }
                    }
                }
            }
        }
    }

    void rotate_Right(Node *nodeToRotate){
        Node *helpNode;
        if (nodeExist(nodeToRotate->up)){//its not root
            nodeToRotate->up->left = nodeToRotate->left;
            if(isLeftSon(nodeToRotate)){
                nodeToRotate->up->left = nodeToRotate->left;
            }
            else{
                nodeToRotate->up->right = nodeToRotate->left;
            }
        }
        nodeToRotate->left->up = nodeToRotate->up;
        nodeToRotate->up = nodeToRotate->left;

        helpNode = nodeToRotate->left->right;
        nodeToRotate->left->right = nodeToRotate;
        nodeToRotate->left = helpNode;
    }

    void rotate_Left(Node *nodeToRotate){
        Node *helpNode;
        if (nodeExist(nodeToRotate->up)){//its not root
            if(isLeftSon(nodeToRotate)){
                nodeToRotate->up->left = nodeToRotate->right;
            }
            else{
                nodeToRotate->up->right = nodeToRotate->right;
            }
        }
        nodeToRotate->right->up = nodeToRotate->up;
        nodeToRotate->up = nodeToRotate->right;

        helpNode = nodeToRotate->right->left; //TODO: may be null
        nodeToRotate->right->left = nodeToRotate;
        nodeToRotate->right = helpNode;
    }

    void addRootNode(int keyToAdd){
        try{
            root = new Node(keyToAdd);
            root->setPointersToGuardian(Guardian);
            setColor(root, BLACK);
            setTreeSize(treeSize + 1);
            isEmpty = false;
        }
        catch(exception exc){
            ;
        }
    }

    void createNode(int keyToAdd){
        try{
            node = new Node(keyToAdd);
            node->setPointersToGuardian(Guardian);
        }
        catch(exception exc){
            ;
        }
    }

    bool nodeExist(Node *node){
        if (node != Guardian){
            return true;
        }
        else{
            return false;
        }
    }

    bool nodeHasLeftSon(Node *nodeToCheck){
        if (nodeToCheck->left != Guardian){
            return true;
        }
        else{
            return false;
        }
    }

    bool nodeHasRightSon(Node *nodeToCheck){
        if (nodeToCheck->right != Guardian){
            return true;
        }
        else{
            return false;
        }
    }

    bool nodeHasNoSons(Node *nodeToCheck){
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

    bool nodeHasTwoSons(Node *node){
        if (nodeHasLeftSon(node) && nodeHasRightSon(node)){
            return true;
        }
        else{
            return false;
        }
    }

    bool nodeHasTwoBlackSons(Node *node){
        if (nodeExist(node)){
            if (nodeHasTwoSons(node)){
                if(nodeIs(BLACK, node->left) && nodeIs(BLACK, node->right)){
                    return true;
                }
                else{
                    return false;
                }
            }
            else{
                return false;
            }
        }
        return false;
    }

    inline bool isLeftSon(Node* node){
        if (node == node->up->left && node->up != Guardian){
            return true;
        }
        else{
            return false;
        }
    }

    inline bool isRightSon(Node* node){
        if (node == node->up->right && node->up != Guardian){
            return true;
        }
        else{
            return false;
        }
    }

    void linkAsLeftSon(Node *father){
        father->left = node;
        node->up = father;
    }

    void linkAsRightSon(Node *father){
        father->right = node;
        node->up = father;
    }

    inline bool isRoot(Node *baseNode){
        if (baseNode->up == Guardian){
            return true;
        }
        else{
            return false;
        }
    }

    inline bool nodeIs(int color, Node *nodeToCheck){
        if (color == BLACK){
            if (nodeToCheck->color == BLACK){
                return true;
            }
            else{
                return false;
            }
        }
        else{
            if (nodeToCheck->color == RED){
                return true;
            }
            else{
                return false;
            }
        }
    }

    inline bool brotherIs(int color, Node *baseNode){
        if (isLeftSon(baseNode)){
            if (baseNode->up->right->color == color){
                return true;
            }
            else{
                return false;
            }
        }
        else{
            if (baseNode->up->left->color == color){
                return true;
            }
            else{
                return false;
            }
        }
    }

    bool NodeUncleIs(int color, Node *baseNode){
        if(nodeExist(baseNode->up->up)){
            if(brotherIs(color, baseNode->up)){
                return true;
            }
        }
        return false;
    }

    void copyDataFromTo(Node *source, Node *direction){
        direction->key = getNodeKey(source);
    }

    void setTreeSize(unsigned int newSize){
        treeSize = newSize;
    }

    void setColor(Node *nodeToColor, int color){
        if (color == RED){
            nodeToColor->color = RED;
        }
        else{
            nodeToColor->color = BLACK;
        }
    }

    void checkIfEmptyAndSet(const unsigned int size){
        if (treeSize == 0){
            isEmpty = true;
        }
    }

    bool isAdditionalBlack(Node *nodeToCheck){
        if (nodeToCheck->additionalBlack){
            return true;
        }
        else{
            return false;
        }
    }

    void setAdditionalBlackTo(Node *nodeToSet){
        if (nodeIs(BLACK, nodeToSet)){
            nodeToSet->additionalBlack = true;
        }
        else{
            setColor(nodeToSet, BLACK);
        }
    }

    void deleteAdditionalBlackFrom(Node *nodeToSet){
        nodeToSet->additionalBlack = false;
    }

    int getColor(Node *node){
        return node->color;
    }

    void changeUncleColor(Node *baseNode){
        if(isLeftSon(baseNode->up)){
            changeColor(baseNode->up->up->right);
        }
        else{
            changeColor(baseNode->up->up->left);
        }
    }

    void changeColor(Node *nodeToColor){
        switch (getColor(nodeToColor)) {
            case RED:
                setColor(nodeToColor, BLACK);
                break;
            case BLACK:
                setColor(nodeToColor, RED);
        }
    }

    bool isRightUncleFor(Node *baseNode){
        if(isLeftSon(baseNode->up)){
            return true;
        }
        else{
            return false;
        }
    }

    bool isLeftUncleFor(Node *baseNode){
        if(isRightSon(baseNode->up)){
            return true;
        }
        else{
            return false;
        }
    }

    bool isRightUncleAndRightSonFor(Node *baseNode){
        if (isRightUncleFor(baseNode) && isRightSon(baseNode)){
            return true;
        }
        else{
            return false;
        }
    }

    bool isRightUncleAndLeftSonFor(Node *baseNode){
        if (isRightUncleFor(baseNode) && isLeftSon(baseNode)){
            return true;
        }
        else{
            return false;
        }
    }

    bool isLeftUncleAndLeftSonFor(Node *baseNode){
        if (isLeftUncleFor(baseNode) && isLeftSon(baseNode)){
            return true;
        }
        else{
            return false;
        }
    }

    bool isLeftUncleAndRightSonFor(Node *baseNode){
        if (isLeftUncleFor(baseNode) && isRightSon(baseNode)){
            return true;
        }
        else{
            return false;
        }
    }

    bool thereIsNoUncle(Node *baseNode){
        if (isLeftUncleFor(baseNode) && isRightUncleFor(baseNode)){
            return false;
        }
        else{
            return true;
        }
    }

public:

    Node *root;

    TreeRB(){
        isEmpty = true;
        treeSize = 0;
        Guardian = new Node(155034);
        Guardian->setPointersToGuardian(Guardian);
        //Student number as important information that it is special node;
    }

    void addNodeToTree(int keyToAdd){
        if (isEmpty){
            addRootNode(keyToAdd);
        }
        else{
            createNode(keyToAdd);
            addNodeToTree(root,keyToAdd);
            setColor(node, RED);
            checkAndRepairRB(node);
        }
    }

    void checkAndRepairRB(Node *addedNode){

        if (nodeExist(addedNode->up)){
            if (nodeIs(BLACK, addedNode->up)){
                ;
            }
            else{
                if (NodeUncleIs(RED, addedNode)){
                    changeColor(addedNode->up);
                    changeUncleColor(addedNode);
                    changeColor(addedNode->up->up);
                    if (isRoot(addedNode->up->up)){
                        setColor(addedNode->up->up, BLACK);
                    }
                    else{
                        checkAndRepairRB(addedNode->up->up);
                    }
                }
                else if(NodeUncleIs(BLACK, addedNode)){
                    if (isRightUncleAndRightSonFor(addedNode)){
                        rotate_Left(addedNode->up);
                        checkAndRepairRB(addedNode);//#->left 3)
                    }
                    else if (isLeftUncleAndLeftSonFor(addedNode)){
                        rotate_Right(addedNode->up);
                        checkAndRepairRB(addedNode);//->right 3)
                    }
                    //3)
                    else if (isRightUncleAndLeftSonFor(addedNode)){
                        rotate_Right(addedNode->up->up);
                        if (isRoot(addedNode->up)){
                            root = addedNode->up;
                        }
                        changeColor(addedNode->up);
                        changeColor(addedNode->up->right);
                    }
                    else if (isLeftUncleAndRightSonFor(addedNode)){
                        rotate_Left(addedNode->up->up);
                        if (isRoot(addedNode->up)){
                            root = addedNode->up;
                        }
                        changeColor(addedNode->up);
                        changeColor(addedNode->up->left);
                    }
                }
            }
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
                node = Guardian;
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
                node = Guardian;
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

    inline void writeKeyWithComma(Node *node){
        int number = getNodeKey(node);
        cout<<number<<",";
    }
    
    inline int getNodeKey(Node *node){
        return node->key;
    }
    
};

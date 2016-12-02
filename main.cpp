#include <iostream>
using namespace std;

#include "Kolejka.cpp"
#include "MyStack.cpp"
#include "MyTreeBST.cpp"
#include "MyTreeRB.cpp"

int main() {

/*
    TreeBST bst;

    bst.addNodeToTree(11);
    bst.addNodeToTree(7);
    bst.addNodeToTree(15);
    bst.addNodeToTree(9);
    bst.addNodeToTree(10);
    bst.addNodeToTree(13);
    bst.addNodeToTree(20);
    bst.addNodeToTree(2);
    bst.addNodeToTree(26);
    bst.addNodeToTree(23);
    bst.addNodeToTree(24);
    bst.addNodeToTree(28);
    bst.addNodeToTree(30);
    bst.deleteNodeFromTree(bst.root, 11);
    bst.deleteNodeFromTree(bst.root, 20);

    bst.writeKeys_InOrder_From(bst.root);
*/
/*
    TreeRB RBrosnaco, RBmalejaco, RBmieszane;

    for (int i=1; i<15; i++){
        RBrosnaco.addNodeToTree(i);
    }
    for (int i = 15; i>0; i--){
        RBmalejaco.addNodeToTree(i);
    }
    for (int j=0; j<=4; j++){
        for (int i = 1+j; i<15; i+=5){
            RBmieszane.addNodeToTree(i);
        }
    }
 */
    TreeRB rb;
    rb.addNodeToTree(1);
    rb.addNodeToTree(6);
    rb.addNodeToTree(11);
    rb.addNodeToTree(2);
    rb.addNodeToTree(7);
    rb.addNodeToTree(12);
    rb.addNodeToTree(3);
    rb.addNodeToTree(8);
    rb.addNodeToTree(13);
    rb.addNodeToTree(4);
    rb.addNodeToTree(9);
    rb.addNodeToTree(14);
    rb.addNodeToTree(5);
    rb.addNodeToTree(10);
    rb.addNodeToTree(15);
    rb.deleteNodeFromTree(rb.root, 6);
    rb.deleteNodeFromTree(rb.root, 4);
    rb.deleteNodeFromTree(rb.root, 9);
    rb.deleteNodeFromTree(rb.root, 13);

    rb.writeKeys_InOrder_From(rb.root);
    cout<<endl;

    return 0;
}

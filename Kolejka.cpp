#include <iostream>
using namespace std;

class MyVector {
private:

    struct Node {
        Node(int data = 0){
            number = data;
            next = prev = NULL;
        }
        int number;
        Node *next, *prev;
    };

    Node *first, *last, *pointer;
    bool isEmpty;
    unsigned int size;

public:

    MyVector (){
        isEmpty = true;
        size = 0;
        first = last = pointer = NULL;
    }
    
    ~MyVector(){
        if (isEmpty == false){
            try {
                while (first != NULL){
                    pointer = first->next;
                    delete first;
                    first = pointer;
                }
            }
            catch (exception exc){
                ;
            }
        }
        else{
            ;
        }
    }

    void push_back(int data){
        if (isEmpty){
            addToEmptyVector(data);
        }
        else {
            addToExistingVector(data);
        }
    }

    void addToEmptyVector(int data){
        try{
            first = new Node(data);
            last = first;
            isEmpty = false;
            size++;
        }
        catch(exception exc){
            cout<<"Error: object Node in MyVector not created\n";
        }
    }

    void addToExistingVector(int data){
        try {
            pointer = new Node(data);
            last->next = pointer;
            pointer->prev = last;
            last = pointer;
            size++;
        }
        catch(exception exc){
            cout<<"Error: object Node in MyVector not created\n";
        }
    }

    int pop_back(){
        if (isEmpty) {
            return NULL;
        }
        else{
            return deleteFromExistingVector();
        }
    }

    int deleteFromExistingVector(){
        try {
            int dataToPoP;
            pointer = first;
            first = first->next;
            dataToPoP = pointer->number;
            delete pointer;

            size--;
            checkIfEmptyVector();
            return dataToPoP;
        }
        catch (exception exc){
            cout<<"Error: object Node in MyVector not deleted properly\n";
            return NULL;
        }
    }

    bool checkIfEmptyVector(){
        if (size){
            return false;
        }
        else{
            return true;
        }
    }

    int at(int index){
            //TODO: wykonaj czytelniej funkcje at
        if (index >0 && index < size){
            pointer = first;
            for (int i=0; i<=index;i++){
                try{
                    pointer = pointer->next;
                }
                catch (exception exc){
                    cout<<"NULL pointer exception (pointer->next)\n";
                    return NULL;
                }
                if (pointer){
                    return pointer->number;
                }
            }
        }
        return NULL;
    }

    Node* findNodeByKey(int keyToSearch){
        pointer = first;
        while (pointer != NULL && pointer->number != keyToSearch) {
            pointer = pointer->next;
        }
        if (pointer->number == keyToSearch){
            return pointer;
        }
        else{
            return NULL;
        }
    }
    /*  
        TODO:
        insertAt(index);
        eraseAt(index);
     */
    unsigned int getSize(){
        return size;
    }
};
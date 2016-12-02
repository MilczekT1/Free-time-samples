#include <iostream>
using namespace std;

class MyStack {
public:
    MyStack(){
        isEmpty = true;
        size = 0;
        last = pointer = NULL;
    }

    ~MyStack(){
        if (isEmpty){
            ;
        }
        else{
            try{
                while(last != NULL){
                    pointer = last->prev;
                    delete last;
                    last = pointer;
                }
            }
            catch(exception exc){
                ;//#
            }
        }
    }

    void push(int data){
        if (isEmpty){
            addToEmptyStack(data);
        }
        else{
            addToExistingStack(data);
        }
    }

    void addToEmptyStack(int data) {
        try{
            last = new Node();
            last->number = data;
            isEmpty = false;
            size++;
        }
        catch (exception exc){
            cout<<"Error: object Node in MyStack not created\n";
        }
    }
    void addToExistingStack(int data){
        try{
            pointer = new Node();
            pointer->number = data;
            pointer->prev = last;
            last = pointer;
            size++;
        }
        catch (exception exc){
            cout<<"Error: object Node in MyStack not created\n";
        }
    }

    int pop(){
        if (isEmpty){
            return NULL;
        }
        else{
            return deleteFromExistingStack();
        }
    }

    int deleteFromExistingStack(){
        try{
            int temporaryIntForData = last->number;
            pointer = last->prev;
            delete last;
            last = pointer;
            size--;
            isEmpty = checkIfEmptyStack();
            return temporaryIntForData;
        }
        catch (exception exc){
            cout<<"Error: unable to gather data from stack (nullptr exception)\n";
            return NULL;
        }
    }

    bool checkIfEmptyStack(){
        if (size){
            return false;
        }
        else{
            return true;
        }
    }

    unsigned int getSize(){
        return size;
    }

private:
    struct Node {
        Node(int data = 0){
            number = data;
            prev = NULL;
        }
        int number;
        Node *prev;
    };
    Node *last, *pointer;
    bool isEmpty;
    unsigned int size;
};

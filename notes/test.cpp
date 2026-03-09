#include <iostream>
using namespace std;

struct Node
{
    int data;
    Node* next;
};


int main () {
    // int a = 3;
    // int b = 5;

    // int * p;

    // p = &b;

    // *p = 8;

    // cout << p <<' ' << *p <<' ' << b;

    // int a[3];
    // a[0] = 1;
    // a[1] = 2;
    // a[2] = 3;

    // cout << a[1] << '\n';

    Node a, b, c;

    a.data = 1;
    b.data = 2;
    c.data = 3;

    a.next = &b;
    b.next = &c;
    c.next = &a;
    
    a.next = b.next;
    

    Node* p = &a;

    for (int i=0; i<10; ++i) {
        cout << p->data <<' ';
        p = p->next;
    }

}

/*

    1 2 3
    a -> b -> c
    a -> c;

*/

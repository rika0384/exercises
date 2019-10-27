#include<iostream>
#include <cassert>

struct queue{
    int max_size, top, bottom;
    int *q;
    queue(int n): max_size(n), top(0), bottom(0){
        q = new int[max_size];
    }
    void push(int x){
        q[top] = x;
        top++;
    }
    void pop(){
        assert(size());
        bottom++;
    }
    int size(){
        return top-bottom;
    }
    int front(){
        assert(size());
        return q[bottom];
    }
    void print(){
        for(int i = bottom; i < top; i++)std::cout << " " << q[i];
        std::cout << std::endl;
    }
};

int main(){
    queue st(10);
    st.push(1);
    st.push(5);
    st.push(1);
    st.push(5);
    st.print();
    st.pop();
    st.print();
}

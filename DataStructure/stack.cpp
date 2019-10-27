#include<iostream>
#include <cassert>

struct stack{
    int max_size, highest;
    int *s;
    stack(int n): max_size(n), highest(0){
        s = new int[max_size];
    }
    void push(int x){
        s[highest] = x;
        highest++;
    }
    void pop(){
        highest--;
    }
    int top(){
        assert(size());
        return s[highest];
    }
    int size(){
        return highest;
    }
    void print(){
        for(int i = 0; i < highest; i++)std::cout << " " << s[i];
        std::cout << std::endl;
    }
};

int main(){
    stack st(10);
    st.push(1);
    st.push(5);
    st.push(1);
    st.push(5);
    st.print();
    st.pop();
    st.print();
}

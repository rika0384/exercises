#include<iostream>

struct stack{
    int max_size, top;
    int *s;
    stack(int n): max_size(n), top(0){
        s = new int[max_size];
    }
    void push(int x){
        s[top] = x;
        top++;
    }
    void pop(){
        top--;
    }
    int size(){
        return top;
    }
    void print(){
        for(int i = 0; i < top; i++)std::cout << " " << s[i];
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

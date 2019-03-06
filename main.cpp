#include <decisiontree.h>
#include <iostream>
#include <fstream>
#include <stack>
using namespace std;

int main(int argc, char* argv[]) {
    Tree *t = new Tree();   //建树
//    t->Display();
    t->Testing();   //测试
    string str;
    str = t->Visualize();   //生成对应字符串

    ofstream outputfile;
    outputfile.open("C:\\Users\\linqi\\Desktop\\decisiontree\\jsontree.txt", ios::ate);
    if (outputfile.is_open()) {
        cout << str << endl;
        outputfile << str;
    }
    outputfile.close();
}

/**
 * 2019/1
 * ID3 DecisionTree
 * Implemented by Qinghong Lin
 **/

#include "decisiontree.h"
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <string.h>
#include <math.h>
using namespace std;

Tree::Tree(){
    ImportData();   //导入数据集
    vector<string> usedAttr;    //记录使用过的父属性
    cout << "Start Training..." << endl;
    this->Root = TreeGenerate(trainingSet, usedAttr);
    cout << "Finished training" << endl;
}
/*返回属性对应的下标*/
int Tree::getAttrIndex(string attrName){
    for(int i=0; i<attrMap.size(); i++)
        if(attrMap[i] == attrName)
            return i;
}
/*返回属性值对应的下标*/
int Tree::getAttrValueIndex(string attrValue, int attrIdx){
    for(int i=0; i<attrSet[attrIdx].size(); i++)
        if(attrSet[attrIdx][i] == attrValue)
            return i;
}
/* 递归建树 */
Node* Tree::TreeGenerate(Dataset data, vector<string> usedAttr){
    Node* root = new Node(false, "NULL");
    //Case 1 数据集中样本全属于同一类别
    if(belongSameType(data)){
        return new Node(true, data.label[0]);   //将节点标记为该类叶节点
    }
    //Case 2 属性集为空 (属性全部使用完)
    if(this->attrMap.size() == usedAttr.size()){
        return new Node(true, getMostType(data));   //将节点标记为叶节点,其类别为样本中最多的类
    }
    //选择最优划分属性
    string bestAttr = getBestAttr(data, usedAttr);
    int bestIdx = getAttrIndex(bestAttr);
    usedAttr.push_back(bestAttr);   //记录为使用过
    root->attr = bestAttr;

    //对最优属性的每一个值进行操作
    for(int i=0; i<attrSet[bestIdx].size(); i++){
        Dataset dataIdx;
        //生成每一个属性值对应的样本子集
        for(int j=0; j<data.label.size(); j++)
            if(data.feature[j][bestIdx] == attrSet[bestIdx][i])
                dataIdx.InsertData(data.feature[j], data.label[j]);
        //Case3 若样本子集为空
        if(dataIdx.label.empty())
            root->child.push_back(new Node(true, getMostType(data)));   //将分支节点标记为数据集中样本最多的类
        else    //递归建树
            root->child.push_back(TreeGenerate(dataIdx, usedAttr));
    }
    return root;
}
/*导入数据集*/
void Tree::ImportData(){
    int attrNum;
    cout << "Please input the number of attribute?" << endl;
    cin >> attrNum;
    attrSet.resize(attrNum);

    cout << "Please input the attibute in order" << endl;
    string attrName;
    for(int i=0; i<attrNum; i++){
        cin >> attrName;
        attrMap.push_back(attrName);
    }

    int dataNum;
    cout << "Please input the number of training sample" << endl;
    cin >> dataNum;
    cout << "Please input the feature and label for every training sample" << endl;
    while(dataNum > 0){
        vector <string> feature;
        string attr, label;
        for(int i=0; i<this->attrMap.size(); i++){
            cin >> attr;
            feature.push_back(attr);

            int flag = 0;
            for(int j=0; j<this->attrSet[i].size(); j++){
                if(this->attrSet[i][j] == attr)
                    flag = 1;
            }
            if(flag == 0)
                this->attrSet[i].push_back(attr);
        }
        cin >> label;
        trainingSet.InsertData(feature, label);
        dataNum--;
    }
    cout << "Complete the import of training sets and feature sets" << endl;
}
/*判断数据集是否为同一类*/
bool Tree::belongSameType(Dataset data){
    for(int i=1; i<data.label.size(); i++)
        if(data.label[i-1] != data.label[i])
            return false;
    return true;
}
/*返回数据集中数目最多一类*/
string Tree::getMostType(Dataset data){
    vector<string> appeared;
    vector<int> count;
    for(int i=0; i<data.label.size(); i++){
        int flag = 0;
        for(int j=0; j<appeared.size(); j++){
            if(data.label[i]==appeared[j]){ //若出现,计数+1
                count[j]++;
                flag = 1;
                break;
            }
        }
        if(flag==0){    //若未出现,记录该属性
            appeared.push_back(data.label[i]);
            count.push_back(1);
        }
    }

    int maxCount=0;
    string mostType;
    for(int k=0; k<count.size(); k++){  //找到计数最多一类
        if(count[k]>maxCount){
            maxCount = count[k];
            mostType = appeared[k];
        }
    }
    return mostType;
}
/*计算信息熵*/
double Tree::getEntropy(Dataset data){
    double Ent = 0;
    vector<string> appeared;
    vector<int> count;
    for(int i=0; i<data.label.size(); i++){ //用于计算频率
        int flag = 0;
        for(int j=0; j<appeared.size(); j++){
            if(data.label[i]==appeared[j]){
                count[j]++;
                flag = 1;
                break;
            }
        }
        if(flag==0){
            appeared.push_back(data.label[i]);
            count.push_back(1);
        }
    }
    for(int k=0; k<count.size(); k++){
        double pk = (float)count[k]/(float)data.label.size();
        Ent -= pk * log2(pk);
    }
    return Ent;
}
/*计算某一属性的信息增益*/
double Tree::getGain(Dataset data, int idx){
    double GainIdx = getEntropy(data);
    vector<string> appeared;
    vector<int> count;
    count.resize(attrMap[idx].size()); //思考为什么要这样做

    for(int i=0; i<data.feature.size(); i++){   //用于计算属性值的出现频率
        int flag = 0;
        int valueIdx = getAttrValueIndex(data.feature[i][idx], idx);
        for(int j=0; j<appeared.size(); j++)
            if(data.feature[i][idx]==appeared[j]){
                count[valueIdx]++;
//                count[j]++;
                flag = 1;
                break;
            }
        if(flag==0){
            appeared.push_back(data.feature[i][idx]);
            count[valueIdx] = 1;
//            count.push_back(1);
        }
    }

    for(int i=0; i<appeared.size(); i++){   //遍历每一属性的属性值
        Dataset dataIdx;
        for(int j=0; j<data.feature.size(); j++){
            if(data.feature[j][idx] == appeared[i])
                dataIdx.InsertData(data.feature[j], data.label[j]);
        }
        int valueIdx = getAttrValueIndex(appeared[i], idx);
        double p = (float)count[valueIdx]/(float)data.label.size();
//        double p = (float)count[i]/(float)data.label.size();
        GainIdx -= p * getEntropy(dataIdx);
    }
    return GainIdx;
}
/*返回最优划分属性*/
string Tree::getBestAttr(Dataset data, vector<string> usedAttr){
    vector<double> Gain;
    for(int i=0; i<this->attrMap.size(); i++){  //遍历所有属性
        int flag = 0;
        for(int j=0; j<usedAttr.size(); j++)
            if(attrMap[i]==usedAttr[j])
                flag = 1;
        if(flag == 0)   //若属性未使用过,则计算信息增益
            Gain.push_back(getGain(data, i));
        else    //若属性使用过,其信息增益为0
            Gain.push_back(0.0);
    }
    double maxGain = 0;
    string bestAttr;
    for(int k=0; k<Gain.size(); k++)    //找到信息增益最大的属性
        if(Gain[k] > maxGain){
            maxGain = Gain[k];
            bestAttr = attrMap[k];
        }
    return bestAttr;
}
/*层次遍历输出各节点*/
void Tree::Display(){
    queue<Node*> q;
    Node* cur;

    q.push(Root);
    while(!q.empty()){
        cur = q.front();
        cout<<cur->attr<<'('<<cur->child.size()<<')'<<' ';
        q.pop();

        if(!cur->child.empty())
            for(int i=0; i<cur->child.size(); i++)
                q.push(cur->child[i]);
    }
}
/*用于测试时计算下一步的节点*/
int Tree::getNextNode(vector<string> feature, Node* cur){
    int idx = getAttrIndex(cur->attr);
    for(int i=0; i<this->attrSet[idx].size(); i++)  //通过特征属性值的下标来判断该走第几分支
        if(feature[idx] == this->attrSet[idx][i])
            return i;
}
/*测试*/
void Tree::Testing(){
    char flag = 'y';
    cout << "Please input the feature of testing sample" << endl;
    while(flag=='y'){
        vector<string> feature;
        string feat,label;
        for(int i=0; i<attrSet.size(); i++){    //输入样本特征
            cin >> feat;
            feature.push_back(feat);
        }
        if(Root->IsLeaf)    //树根若为叶子结点
            cout << "the predict result is :"<< Root->attr << endl;
        else{
            Node* cur = Root->child[getNextNode(feature, Root)];
            while(!cur->IsLeaf)
                cur = cur->child[getNextNode(feature, Root)];
            cout << "the predict result is: "<< cur->attr << endl;
        }
    }
}
/*生成决策树对应的json字符串
 * e.g {'是否名校':{'是':'录用','否':{'项目经验':{'有':'录用','无':'不录用'}}}}
 * */
string Tree::DFS(Node* root){
    int idx, flag;
    stack<Node*> s1;    //用来记录节点的属性
    stack<string> s2;   //用来记录分支对应的属性值
    stack<int> s3;  //用来判断输出的右括号个数
    Node* node;
    string branch;
    string str = "";

    s1.push(root);
    s3.push(-1);
    while(!s1.empty()){
        node = s1.top();
        s1.pop();
        if(node!=this->Root){
            branch = s2.top();
            str += "\'" + branch + "\':" ;  //'是':
            s2.pop();
        }

        if(!node->IsLeaf)
            str += "{\'" + node->attr + "\':{"; //'是否名校':{
        else
            str += "\'" + node->attr + "\',";   //'录用',

        if(s3.top() > 0){
            flag = s3.top();
            s3.pop();
            while(flag > 0){
                str = str.substr(0, str.length() - 1);
                str += "}},";
                flag --;
            }
        }
        else if(s3.top() == 0){
            s3.pop();
        }

        if(!node->IsLeaf){
            idx = getAttrIndex(node->attr);
            for(int i = node->child.size()-1; i>=0; i--){
                s1.push(node->child[i]);
                s2.push(this->attrSet[idx][i]);
                //若为同层最后一个节点且为叶子节点,则该节点输出一定跟着右括号
                if(i == node->child.size()-1 && node->child[i]->IsLeaf){
                    int temp = s3.top();
                    s3.pop();
                    s3.push(-temp);
                }
                //若为同层最后一个节点且非叶子节点,则该节点输出无需跟着右括号,但下一次输出的右括号数目要加倍
                else if(i == node->child.size()-1){
                    int temp = s3.top();
                    s3.pop();
                    s3.push(--temp);
                }
                //若非同层最后一个节点且非叶子节点,则该节点将产生一个右括号
                else if(i != node->child.size()-1 && ! node->child[i]->IsLeaf)
                    s3.push(-1);
                else    //若非同层最后一个节点且为叶子节点,则该节点不能动用栈中已有的元素
                    s3.push(0); //采用0来占位从而保证一一对应
            }
        }
    }
    str = str.substr(0, str.length() - 1);

    return str;
}
/*公有接口*/
string Tree::Visualize(){
    return DFS(this->Root);
}
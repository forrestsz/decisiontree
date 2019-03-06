/**
 * 2019/1
 * ID3 DecisionTree
 * Implemented by Qinghong Lin
 **/

#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include <vector>
#include <iostream>
using namespace std;

class Dataset{	//数据集
public:
    vector<vector<string> > feature;	//特征
    vector<string> label;	//标签
    void InsertData(vector<string> x, string y){	//插入一条数据
        feature.push_back(x);
        label.push_back(y);
    }
};

class Node{	//节点
public:
    bool IsLeaf;	//是否为叶子
    string attr;	//叶子对应类别、非叶对应属性
    vector<Node*> child;	//孩子结点
    Node(bool a, string b):IsLeaf(a), attr(b){}
};

class Tree{
private:
	Node* Root;	//根节点
	vector<string> attrMap;	//属性集 e.g [实习经历,项目经验,是否名校]
	vector<vector<string> > attrSet;	//属性值 e.g [[有,无],[有,无],[是,否]]
	Dataset trainingSet;	//训练集 e.g. [[有,有,是],录用]

    Node* TreeGenerate(Dataset data, vector<string> usedAttr);	//递归建树
    void ImportData();	//导入数据
	bool belongSameType(Dataset data);	//判断数据集是否同一类
	string getMostType(Dataset data);	//返回数据集中数目最多一类
	string getBestAttr(Dataset data, vector<string> usedAttr);	//返回数据集最优划分属性
	double getGain(Dataset data, int idx);	//计算信息增益
	double getEntropy(Dataset data);	//计算信息熵
	int getNextNode(vector<string> feature, Node* cur);	//训练时用于判断下一节点
	int getAttrIndex(string attrName);	//返回属性对应下标
	int getAttrValueIndex(string attrValue, int attrIdx);    //返回属性值对应下标

	string DFS(Node* root);	//深度优先遍历生成决策树对应的json字符串
public:
	Tree();	//建树
	void Testing();	//测试
	void Display();	//层次顺序输出各结点
	string Visualize();	//公有接口,返回json字符串
};

#endif // DECISIONTREE_H

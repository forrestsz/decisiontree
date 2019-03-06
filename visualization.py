# -*- coding:utf-8 -*-
import matplotlib.pyplot as plt
from pylab import mpl

mpl.rcParams['font.sans-serif'] = ['KaiTi']

decision_node = dict(boxstyle="sawtooth", fc="0.8")
leaf_node = dict(boxstyle="round4", fc="0.8")
arrow_args = dict(arrowstyle="<-")

#绘制节点
def plot_node(node_text, center, parent, node_type):    #center中心坐标、parent起点坐标
    create_plot.ax1.annotate(node_text, xy=parent, xycoords='axes fraction', xytext=center, textcoords='axes fraction',
                             va='center', ha='center', bbox=node_type, arrowprops=arrow_args)

#绘制分支
def plot_mid_text(center, parent, txt_string):
    x_mid = (parent[0] - center[0]) / 2. + center[0]
    y_mid = (parent[1] - center[1]) / 2. + center[1]
    create_plot.ax1.text(x_mid, y_mid, txt_string)

#返回叶子结点数
def get_num_leafs(tree):
    num_leafs = 0
    first_str = list(tree.keys())[0]    #根节点的属性->字符串
    second_dict = tree[first_str]   #根节点对应的子树->字典
    # 若子节点是字典类型，则该节点也是一个非叶节点，递归返回子节点的叶子节点数
    for key in second_dict.keys():
        if type(second_dict[key]).__name__ == 'dict':
            num_leafs += get_num_leafs(second_dict[key])
        else:
            num_leafs += 1  #节点数相加

    return num_leafs

#返回树的高度
def get_tree_depth(tree):
    max_depth = 0
    first_str = list(tree.keys())[0]
    second_dict = tree[first_str]
    for key in second_dict.keys():
        #若子节点是字典类型，则该节点也是一个非叶节点，递归返回子节点的最大高度
        if type(second_dict[key]).__name__ == 'dict':
            this_depth = 1 + get_tree_depth(second_dict[key])
        else:
            this_depth = 1
        if this_depth > max_depth:
            max_depth = this_depth  #高度取最大
    return max_depth

#绘制决策树
def plot_tree(tree, parent, node_text):
    num_leafs = get_num_leafs(tree) #计算宽度
    depth = get_tree_depth(tree)    #计算深度
    first_str = list(tree.keys())[0]    #树根
    #parent是一个元祖，表示树根坐标
    center = (plot_tree.x_off + (1.0 + float(num_leafs)) / 2. / plot_tree.total_w, plot_tree.y_off)
    plot_mid_text(center, parent, node_text)
    plot_node(first_str, center, parent, decision_node) #画树根节点
    second_dict = tree[first_str]

    plot_tree.y_off -= 1. / plot_tree.total_d   #自顶向下绘图，依次递减y坐标
    for key in second_dict.keys():
        if type(second_dict[key]).__name__ == 'dict':
            plot_tree(second_dict[key], center, str(key))
        else:#递归到了叶子节点才开始画图
            plot_tree.x_off += 1. / plot_tree.total_w
            plot_node(second_dict[key], (plot_tree.x_off, plot_tree.y_off), center, leaf_node)  #画节点
            plot_mid_text((plot_tree.x_off, plot_tree.y_off), center, str(key)) #画父子节点间的分支
    # 在绘制完所有子节点之后，回到父节点，需要加回y的偏移
    plot_tree.y_off += 1. / plot_tree.total_d

#公有接口
def create_plot(tree):
    figure = plt.figure(1, facecolor='white')
    figure.clf()

    axprops = dict(xticks=[], yticks=[])
    create_plot.ax1 = plt.subplot(111, frameon=False, **axprops)
    plot_tree.total_w = float(get_num_leafs(tree))  #树宽
    plot_tree.total_d = float(get_tree_depth(tree)) #树高
    plot_tree.x_off = -0.5 / plot_tree.total_w  #偏移量，用于计算其他节点的坐标
    plot_tree.y_off = 1.
    plot_tree(tree, (0.5, 1.), '')
    plt.show()

def example():
    watermelon = {'纹理':{'清晰':{'根蒂':{'蜷缩':'好瓜',
                                           '稍蜷':{'色泽':{'青绿':'好瓜',
                                                            '乌黑':{'触感':{'硬滑':'好瓜',
                                                                             '软粘':'坏瓜'}},
                                                            '浅白':'好瓜'}},
                                           '硬挺':'坏瓜'}},
                          '稍糊':{'触感':{'硬滑':'好瓜',
                                           '软粘':'坏瓜'}},
                          '模糊':'坏瓜'}}

    return watermelon

def main():
    f = open(r'C:\Users\linqi\Desktop\decisiontree\jsontree.txt', 'r')
    a = f.read()
    a = eval(a)

    create_plot(a)
    # create_plot(example())

if __name__ == '__main__':
    main()
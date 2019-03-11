# decisiontree
This is my C++ implementation of the id3 decision tree, and I visualized the decision tree in python, mainly using what I learned in data structure.

If we own the training set as follows:

#如何挑选西瓜#  
6  
色泽 根蒂 敲声 纹理 脐部 触感  
17  
青绿 蜷缩 浊响 清晰 凹陷 硬滑 好瓜  
乌黑 蜷缩 沉闷 清晰 凹陷 硬滑 好瓜  
乌黑 蜷缩 浊响 清晰 凹陷 硬滑 好瓜  
青绿 蜷缩 沉闷 清晰 凹陷 硬滑 好瓜  
浅白 蜷缩 浊响 清晰 凹陷 硬滑 好瓜  
青绿 稍蜷 浊响 清晰 稍凹 软粘 好瓜  
乌黑 稍蜷 浊响 稍糊 稍凹 软粘 好瓜  
乌黑 稍蜷 浊响 清晰 稍凹 硬滑 好瓜  
乌黑 稍蜷 沉闷 稍糊 稍凹 硬滑 坏瓜  
青绿 硬挺 清脆 清晰 平坦 软粘 坏瓜  
浅白 硬挺 清脆 模糊 平坦 硬滑 坏瓜  
浅白 蜷缩 浊响 模糊 平坦 软粘 坏瓜  
青绿 稍蜷 浊响 稍糊 凹陷 硬滑 坏瓜  
浅白 稍蜷 沉闷 稍糊 凹陷 硬滑 坏瓜  
乌黑 稍蜷 浊响 清晰 稍凹 软粘 坏瓜  
浅白 蜷缩 浊响 模糊 平坦 硬滑 坏瓜  
青绿 蜷缩 沉闷 稍糊 稍凹 硬滑 坏瓜  

we can using the **main.cpp** for training a decision tree as well as testing.

what's more, you can using the **visualization.py** for visualiza the decision tree easily.
![v](https://github.com/linqinghong/decisiontree/blob/master/tree.png)

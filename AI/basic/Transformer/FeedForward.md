# 0x00. 导读

# 0x01. 简介

前馈神经网络(Feedforward Neural Network, FNN) 是最早发明的简单人工神经网络。在前馈神经网络中，各神经元分别属于不同的层，每一层的神经元可以接收前一层神经元的信号，并产生信号输出到下一层。第0层称为输入层，最后一层称为输出层，其他中间层称为隐藏层。整个网络中无反馈，信号从输入层向输出层单向传播，可用一个有向无环图表示。

# 0x02.

位于 Self-Attention 之后，下一个 Add & Norm 之前。

没有 Feed Forward 会怎样？
- 模型变笨：只能处理线性关系（比如“A导致B”），无法理解复杂逻辑（比如“A导致B，但B只有在C发生时才有效”）。
- 性能下降：实验表明，去掉 Feed Forward 的 Transformer 在某些任务上准确率会降低 10%~20%。

# 0x03. 结构拆解

假设你有一堆杂乱的数据（比如一堆单词），想从中提取关键信息：

1. 第一层（Linear 1）：

    - 作用：把数据压缩到一个更小的“思考空间”（比如从 512 维降到 256 维）。
    - 比喻：像用滤网筛掉无关细节，只保留重要线索（比如从“猫吃鱼”中提取“动物-食物”关系）。
2. ReLU 激活函数：

    - 作用：给数据“打气”，让模型能主动忽略无用信息（类似按开关控制电路通断）。
    - 比喻：如果发现“鱼被吃”的信息没用，直接切断这条路径。
3. 第二层（Linear 2）：

    - 作用：把压缩后的信息“扩展”回原来的维度（比如从 256 维回到 512 维）。
    - 比喻：把筛选出的线索组合成完整的指令（比如“猫→捕食者→鱼→受害者”）。

普通全连接层：  
$$
X \xrightarrow{\text{Linear}} X' \quad (\text{直接输出})
$$

Transformer 的 Feed Forward：  
$$
X \xrightarrow{\text{Linear 1}} X' \xrightarrow{\text{ReLU}} X'' \xrightarrow{\text{Linear 2}} X'''
$$




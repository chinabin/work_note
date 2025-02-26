Transformer:
- embedding
  - input embedding
  - position embedding
- encoder
  - Multi-Head Attention
    - Self-Attention
  - 残差连接 Add
  - 归一化 Norm
  - Feed Forward
- decoder
  - Multi-Head Attention
    - Masked Encoder-Decoder Attention
  - Masked Multi-Head Attention
    - Masked Self-Attention
  - Feed Forward
  - 残差连接 Add
  - 归一化 Norm



![Alt text](../../../pic/AI/OpenAI/transformer3.png)


一个 Encoder 层包含 4 个子层的顺序堆叠：

- Multi-Head Self-Attention 层
- Add & Norm 层
- Feed Forward 层
- Add & Norm 层

一个 Decoder 层包含 6 个子层的顺序堆叠：

- Masked Multi-Head Self-Attention 层
- Add & Norm 层
- Multi-Head Cross-Attention 层
- Add & Norm 层
- Feed Forward 层
- Add & Norm 层


![Alt text](../../../pic/AI/OpenAI/s4.png)

后面基本都是 decoder-only 了。

![Alt text](../../../pic/AI/OpenAI/transformer30.png)
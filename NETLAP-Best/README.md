## NOSEP: Strict pattern matching under non-overlapping condition
***

Youxi Wu, Cong Shen, He Jiang, Xindong Wu. Strict pattern matching under non-overlapping condition. Science China Information Sciences. 2017, 60 (1):012101:1-16.

#### Abstract:

Pattern matching (or string matching) is an essential task in computer science, especially in sequential pattern mining, since pattern matching methods can be used to calculate the support (or the number of occurrences) of a pattern and then to determine whether the pattern is frequent or not. A state-of-the-art sequential pattern mining with gap constraints (or flexible wildcards) uses the number of non-overlapping occurrences to denote the frequency of a pattern. Non-overlapping means that any two occurrences cannot use the same character of the sequence at the same position of the pattern. In this paper, we investigate strict pattern matching under the non-overlapping condition. We show that the problem is in P at first. Then we propose an algorithm, called NETLAP-Best, which uses Nettree structure. NETLAP-Best transforms the pattern matching problem into a Nettree and iterates to find the rightmost root-leaf path, to prune the useless nodes in the Nettree after removing the rightmost root-leaf path. We show that NETLAP-Best is a complete algorithm and analyse the time and space complexities of the algorithm. Extensive experimental results demonstrate the correctness and efficiency of NETLAP-Best.

---

#### 中文摘要：

模式匹配（串匹配）是计算机科学中至关重要的一个任务，特别是在序列模式挖掘中，因为模式匹配方法可以用来计算一个模式在序列中的支持度（出现数），进而判断这个模式是否频繁。一种具有间隙约束（可变长度通配符）的序列模式挖掘算法采用模式的无重叠出现数目来表示这个模式的频度，这里无重叠是指任何两个出现不能共用序列的相同位置的字符。首先理论证明了无重叠条件的严格模式匹配的计算复杂度是P，然后提出了一个基于网树结构的NETLAP-Best算法，该算法将模式匹配问题转换为一颗网树，并在网树上迭代地寻找最右树根-叶子路径，之后剪去这条路径和无用的网树结点。之后理论证明了NETLAP-Best算法的完备性并分析了该算法的时间和空间复杂度。大量实验结果验证了NETLAP-Best算法的正确性和有效性。

---

#### Datasets:
[Dataset1](https://github.com/wuc567/Pattern-Matching/blob/master/NETLAP-Best/dataset.rar)

#### Algorithms:
[NETLAP-Best](https://github.com/wuc567/Pattern-Matching/tree/master/NETLAP-Best)



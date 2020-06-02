## 无重叠条件严格模式匹配的高效求解算法
## Efficient Solving Algorithm for Strict Pattern Matching under Nonoverlapping Condition
***

武优西, 刘茜, 闫文杰, 郭磊, 吴信东. 无重叠条件严格模式匹配的高效求解算法. 软件学报，2020. （已录用）

#### Abstract:

Nonoverlapping conditional pattern mining is a method of gap constrained sequence pattern mining. Compared with similar mining methods, this method is easier to find valuable frequent patterns. The core of the problem is to calculate the support (or the number of occurrences) of a pattern in the sequence, and then determine whether the pattern is frequent. The essence of calculating the support is the pattern matching under nonoverlapping condition. The current studies employ the iterative search to find a nonoverlapping occurrence, and then prune the useless nodes to calculate the support of the pattern. The computational time complexities of these algorithms are O(m\*m\*n\*W), where m, n, and W are the pattern length, sequence length, and maximum gap, respectively. In order to improve the calculation speed of pattern matching under nonoverlapping condition, and effectively reduce sequence pattern mining time, this paper proposes an efficient and effective algorithm, which converts the pattern matching problem into a Nettree, and then starts from the minroot node of the Nettree, and adopts the backtracking strategy to iteratively search the leftmost child to calculate the nonoverlapping minimum occurrence. After pruning the occurrence on the Nettree, the problem can be solved without further searching and pruning invalid nodes. This paper proves the completeness of the algorithm and reduces the time complexity to O(m\*n\*W). On this basis, the paper continues to indicate that there are other three similar solving strategies for this problem, iteratively finds the leftmost parent path from the leftmost leaf, the rightmost child path from the rightmost root, and the rightmost parent path from the rightmost leaf. Extensively experimental results verify the efficiency of the algorithm in this paper, especially, the mining algorithm adopting this method can reduce the mining time.

---

#### Datasets:
[Datasets](https://github.com/wuc567/Pattern-Matching/blob/master/NetBack/datasets.rar)

#### Algorithms:

[NetBack](https://github.com/wuc567/Pattern-Matching/blob/master/NetBack/NetBack.rar)

**中文摘要**：无重叠条件序列模式挖掘是一种间隙约束序列模式挖掘方法,与同类挖掘方法相比,该方法更容易发现有价值的频繁模式,其核心问题是计算给定模式在序列中的支持度或出现数,进而判定该模式的频繁性.而计算模式支持度问题实质是无重叠条件模式匹配.当前研究采用迭代搜索无重叠出现,然后剪枝无用结点的方式计算模式的支持度,其计算时间复杂度为 O(m\*m\*n\*W),其中 m,n 和 W分别为模式长度,序列长度及最大间隙.为了进一步提高无重叠条件模式匹配计算速度,从而有效地降低无重叠条件序列模式挖掘时间,本文提出了一种高效的算法,该算法将模式匹配问题转换为一棵网树,然后从网树的最小树根结点出发,采用回溯策略迭代搜索最左孩子方式计算无重叠最小出现,在网树上剪枝该出现后,无需进一步查找并剪枝无效结点即可实现问题的求解.本文理论证明了该算法的完备性并将该算法的时间复杂度降低为 O(m\*n\*W).在此基础上,本文继续指明,该问题还存在另外三种相似的求解策略,分别是从最左叶子出发迭代查找最左双亲方式、 从最右树根出发迭代查找最右孩子方式和从最右叶子出发迭代查找最右双亲方式.实验结果验证了本文算法的性能,特别是在序列模式挖掘中,应用本文方法的挖掘算法可以降低挖掘时间.

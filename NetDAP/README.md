## NetDAP: (δ, γ)-Approximate Pattern matching with length constraints

***

Youxi Wu, Jinquan Fan, Yan Li, Lei Guo, Xindong Wu. NetDAP: (δ, γ)-Approximate Pattern matching with length constraints[J]. Applied Intelligence, 2020.

#### Abstract:

Pattern matching (PM) with gap constraints has been applied to compute the support of a pattern in a sequence, which is an essential task of the repetitive sequential pattern mining (or sequence pattern mining). Compared with exact PM, approximate PM allows data noise (differences) between the pattern and the matched subsequence. Therefore, more valuable patterns can be found. Approximate PM with gap constraints mainly adopts the Hamming distance to measure the approximation degree which only reflects the number of different characters between two sequences, but ignores the distance between different characters. Hence, this paper addresses (δ, γ) approximate PM with length constraints which employs local-global constraints to improve the accuracy of the PM, namely, the maximal distance between two corresponding characters is not allowed to exceed the local threshold δ, and the sum of all the δ distances must not exceed the global threshold γ. In order to tackle the problem effectively, this paper proposes an effective online algorithm, named NetDAP, which employs a special designed data structure named approximate single-leaf Nettree. An approximate single-leaf Nettree can be created by adopting dynamic programming method to determine the range of leaf, the minimal root, the maximal root, the range of nodes for each level, and the range of parents for each node. To improve the performance, two pruning strategies are proposed to prune the nodes and the parent-child relationships which do not satisfy the γ and δ distance constraints, respectively. Finally, extensive experimental results on real protein data sets and time series verify the performance of the proposed algorithm.

---

#### Datasets:
[Datasets](https://github.com/wuc567/Pattern-Matching/tree/master/NetDAP/dataset)

#### Algorithms:

[NetDAP](https://github.com/wuc567/Pattern-Matching/tree/master/NetDAP/code)

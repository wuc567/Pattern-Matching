## NetDPO: (delta, gamma)-Approximate Pattern Matching with Gap Constraints under One-Off Condition
***
Yan Li, Lei Yu, Jing Liu, Lei Guo, Youxi Wu*, Xindong Wu. NetDPO: (delta, gamma)-Approximate Pattern Matching with Gap Constraints under One-Off Condition

#### Abstract:

Approximate pattern matching is not only more general than exact pattern matching but also allows some data noise. Most of them adopt the Hamming distance to measure similarity, which indicates the number of different characters in two strings, but it cannot reflect the approximation between two characters. This paper addresses the approximate pattern matching with a local distance no larger than $\delta$ and a global distance no larger than $\gamma$, which is named Delta and gamma Pattern matching with gap constraints under One-off condition (DPO). We show that the problem is an NP-Hard problem. Therefore, we construct a heuristic algorithm named approximate Nettree for DPO (NetDPO), which transforms the problem into an approximate Nettree based on $\delta$ distance which is a specially designed data structure. Then, NetDPO calculates the number of paths that reach the roots within $\gamma$ distance. To find the maximal occurrences, we employ the rightmost parent strategy and the optimal parent strategy to select the better occurrence which can minimize the influence after removing the occurrence. Iterate this process until there are no occurrences. Finally, we analyze the time and space complexities of NetDPO. Extensive experimental results verify the superiority of the proposed algorithm. 

---

#### Datasets:
[Dataset](https://github.com/wuc567/Pattern-Matching/blob/master/NetDPO/DataSet.zip)  (All sequences)
#### Algorithms:
[NetDPO and all competitive algorithms](https://github.com/wuc567/Pattern-Matching/blob/master/NetDPO/NetDPO_code.zip)
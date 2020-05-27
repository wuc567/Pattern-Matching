## NetNPG: Nonoverlapping pattern matching with general gap constraints
***

Qiaoshuo Shi, Jinsong Shan, Wenjie Yan, Youxi Wu, Xindong Wu. NetNPG: Nonoverlapping pattern matching with general gap constraints[J]. Applied Intelligence, 2020. DOI: 10.1007/s10489-019-01616-z.

#### Abstract:

Pattern matching (PM) with gap constraints (or flexible wildcards) is one of the essential tasks in repetitive sequential pattern mining (or sequence pattern mining), since it can compute the support of a pattern in a sequence. Nonoverlapping PM (or PM under nonoverlapping condition) which is a kind of PM with gap constraints methods allows the same position character in the sequence to be reused at different locations in the pattern, but is not allowed to be reused in the same position of the pattern. The researches on nonoverlapping are under non-negative gaps which are more restrictive on the order of each character occurring in the sequence. As we know that it is easy to obtain valuable patterns under the nonoverlapping condition in sequence pattern mining. This paper addresses a nonoverlapping PM problem with general gaps which means that the gap can be a negative value. We proposes an effective algorithm which employs Nettree structure to convert the problem into a general gap Nettree at first. In order to find the nonoverlapping occurrences, the algorithm employs a backtracking strategy to find the leftmost full path in each iteration. This paper also analyzes the time and space complexities of the proposed algorithm. Experimental results verify the proposed algorithm has excellent performance and demonstrate that the general gap is more flexible than the non-negative gap.
---

#### Datasets:
[Datasets](https://github.com/wuc567/Pattern-Matching/blob/master/NetNPG/datasets.zip)  (All sequences)

#### Algorithms:

[NetNPG](https://github.com/wuc567/Pattern-Matching/blob/master/NetNPG/NetNPG.zip)

[INSgrow-gen](https://github.com/wuc567/Pattern-Matching/blob/master/NetNPG/INSgrow-gen.zip)

[NETGAP-gen](https://github.com/wuc567/Pattern-Matching/blob/master/NetNPG/NETGAP-gen.zip)

[NETLAP-gen](https://github.com/wuc567/Pattern-Matching/blob/master/NetNPG/NETLAP-gen.zip)

[NetNPG-nb](https://github.com/wuc567/Pattern-Matching/blob/master/NetNPG/NetNPG-nb.zip)

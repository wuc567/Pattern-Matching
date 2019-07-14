## NETASPNO: Approximate strict pattern matching under nonoverlapping condition

***

Abstract: In pattern matching, a gap constraint is a more flexible wildcard than traditional wildcards \"?\" and \"*\". Pattern matching with gap constraints is more difficult to handle and fulfills user’s enquiries more easily. Pattern matching with gap constraints has therefore been carried out in numerous research works such as music information retrieval, finding protein sites, and sequence pattern mining. Strict pattern matching under a nonoverlapping condition, as a type of pattern matching with gap constraints, is a key issue of sequence pattern mining with gap constraints since it can be used to compute the frequency of a pattern. Exact matching limits the flexibility of the match to some extent since it requires that each character match. Therefore, we address Approximate Strict Pattern matching under the NonOverlapping constraints (ASPNO). We propose an effective algorithm, named NETtree for ASPNO (NETASPNO), which first transforms the problem into a Nettree data structure. To find the nonoverlapping occurrences effectively, NETASPNO is iterated to find the rightmost root-leaf-path from the rightmost root and prunes the path. Extensive experimental results demonstrate that NETASPNO has better performance than other competitive algorithms.

#### Codes:

NETROL

NETLAP-Appro

NETASPNO

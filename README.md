# fibonacci_table
Consistent Hashing Using Fibonacci Encoding

This is is essentially a hash table that can grow seamlessly and one by one in O(log(n)). This is achieved using consistent hashing with fibonacci encoding at a computational cost of O(log(n)) for each encode or decode operation. Each operation essentially is O(log(n)) on average. The major improvement with respect to other consistent hashing techniques is that the nodes with keys that need to be redistributed can be computed, taking the O(n) complexity in the addition or expansion step down to O(log(n)).

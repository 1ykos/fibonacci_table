# fibonacci_table
Consistent Hashing Using Fibonacci Encoding

This is is essentially a hash table that can grow seamlessly and one by one in `O(log(n))`. This is achieved using consistent hashing with fibonacci encoding at a computational cost of `O(log(n))` for each encode or decode operation. Each operation essentially is `O(log(n))` amortized. The major improvement with respect to other consistent hashing techniques is that the nodes with keys that need to be redistributed can be computed, taking the `O(n/k+log(n))` complexity in the addition or expansion step down to `O(log(n))`.

At the heart is the function `fibonacci_mapping`
```c++
uint64_t constexpr fibonacci_mapping(uint64_t value,const uint64_t max) {
  uint64_t i= 4660046610375530309ull;
  uint64_t j= 7540113804746346429ull;
  uint64_t k= 1;
  uint64_t l= 1;
  uint64_t map = 0;
  while (j>0) {
    if (value>=j) {
      value-=j;
      if (map+l<max) map+=l;
      else return map;
    }
    i=j-i;
    j=j-i;
    l=l+k;
    k=l-k;
  }
  return map;
}
```
It encodes a 64 bit integer assumed to be less than 12200160415121876738 as a sum of fibonacci numbers using a greedy algorithm. At the same time this code is read in reverse until it would exceed the nodecount. The last number less than the nodecount is then taken to be the mapping. The values for map are evenly distributed in `[0,max)`. The beautiful thing is that the next larger value for map is known, it's just the next fibonacci number away. And even better, this works in reverse too, the value of map that it had when the node count was less than about 1/φ less or less than about φ more can be found by subtracting the largest fibonacci number not greater than map from it.

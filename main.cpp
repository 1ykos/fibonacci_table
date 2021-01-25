#include <chrono>
#include "fibonacci_table.hpp"
#include "wmath_hash.hpp"

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::tuple;
using std::get;
using std::make_tuple;
using std::string;
using std::stoull;
using wmath::pow;
using wmath::long_mul;
using wmath::popcount;

int main(int argc, char** argv) {
  /*uint64_t tmp[256]{};
  for (uint64_t i=1;i!=256;++i) {
    cout << i-gold::largest_fibonacci_less_than(i)-1 << endl;
    for (uint64_t j=0;j!=256;++j) {
      const uint64_t map = gold::fibonacci_mapping(
          get<0>(long_mul(
            uint64_t(12200160415121876738ull),
            uint64_t(j*10897729332731318105ull)
            )),i);
      if (tmp[j]!=map) {
        cout << tmp[j] << " -> " << map << endl;
        tmp[j] = map;
      }
    }
    cout << endl;
  }*/
  auto start = std::clock();
  gold::fibonacci_table test;
  for (size_t i=0;i!=stoull(argv[1]);++i) {
    test.insert(
        get<0>(
          long_mul(
            uint64_t(12200160415121876738ull),
            uint64_t(i*13332737083598475508ull)
            )
          )
        );
    if (wmath::popcount(i)==1) cout << i << " " << (std::clock()-start) << endl;
  }
}

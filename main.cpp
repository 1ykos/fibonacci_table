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
using wmath::pow;
using wmath::long_mul;

int main() {
  gold::fibonacci_table test;
  for (size_t i=0;i!=1ull<<20;++i) {
    cerr << i << endl; 
    test.insert(
        get<0>(
          long_mul(
            uint64_t(12200160415121876738ull),
            uint64_t(i*13332737083598475508ull)
            )
          )
        );
  }
}

#include "wmath.hpp"
#include <vector>
#include <string>
#include <exception>
#include <iostream>

namespace gold{
  using wmath::log2;
  using wmath::pow;
  using std::vector;
  using std::swap;
  using std::to_string;
  using std::cout;
  using std::endl;

  constexpr uint64_t fibonacci_sequence[93] = {
                       1ull,
                       1ull,
                       2ull,
                       3ull,
                       5ull,
                       8ull,
                      13ull,
                      21ull,
                      34ull,
                      55ull,
                      89ull,
                     144ull,
                     233ull,
                     377ull,
                     610ull,
                     987ull,
                    1597ull,
                    2584ull,
                    4181ull,
                    6765ull,
                   10946ull,
                   17711ull,
                   28657ull,
                   46368ull,
                   75025ull,
                  121393ull,
                  196418ull,
                  317811ull,
                  514229ull,
                  832040ull,
                 1346269ull,
                 2178309ull,
                 3524578ull,
                 5702887ull,
                 9227465ull,
                14930352ull,
                24157817ull,
                39088169ull,
                63245986ull,
               102334155ull,
               165580141ull,
               267914296ull,
               433494437ull,
               701408733ull,
              1134903170ull,
              1836311903ull,
              2971215073ull,
              4807526976ull,
              7778742049ull,
             12586269025ull,
             20365011074ull,
             32951280099ull,
             53316291173ull,
             86267571272ull,
            139583862445ull,
            225851433717ull,
            365435296162ull,
            591286729879ull,
            956722026041ull,
           1548008755920ull,
           2504730781961ull,
           4052739537881ull,
           6557470319842ull,
          10610209857723ull,
          17167680177565ull,
          27777890035288ull,
          44945570212853ull,
          72723460248141ull,
         117669030460994ull,
         190392490709135ull,
         308061521170129ull,
         498454011879264ull,
         806515533049393ull,
        1304969544928657ull,
        2111485077978050ull,
        3416454622906707ull,
        5527939700884757ull,
        8944394323791464ull,
       14472334024676221ull,
       23416728348467685ull,
       37889062373143906ull,
       61305790721611591ull,
       99194853094755497ull,
      160500643816367088ull,
      259695496911122585ull,
      420196140727489673ull,
      679891637638612258ull,
     1100087778366101931ull,
     1779979416004714189ull,
     2880067194370816120ull,
     4660046610375530309ull,
     7540113804746346429ull,
    12200160415121876738ull
  };
  // i:1$ while i>(1/2)**64 do (i:i/%phi,print(floor(i*2**64)));
  //constexpr uint64_t iphi_powers[91];
  
  vector<bool> inline fibonacci_encoding(uint64_t value) {
    vector<bool> encoding;
    uint64_t i=1,j=1;
    while ((i+j>i)&&(i+j<value)) {
      j=j+i;
      i=j-i;
    }
    while ((j>0)&&value) {
      if (value<j) {
        encoding.push_back(0);
      } else {
        encoding.push_back(1);
        value-=j;
      }
      i=j-i;
      j=j-i;
    }
    return encoding;
  }
  
  // value assumed to be less than 12200160415121876738
  uint64_t inline fibonacci_mapping(uint64_t value,const uint64_t max) {
    uint64_t i= 4660046610375530309ull;
    uint64_t j= 7540113804746346429ull;
    uint64_t k= 0;
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
  
  struct golden_tree{
    uint64_t num_elem = 0;
    uint64_t num_data = 0;
    uint64_t* data = nullptr;
    uint64_t* mask = nullptr;
    inline bool is_set(const uint64_t& n) const {        
      const uint64_t i = n/64;
      const uint64_t j = n%64;
      return (mask[i]&(uint64_t(1)<<(64-j-1)));
    }
    inline void set(const uint64_t& n) {
      const uint64_t i = n/64;
      const uint64_t j = n%64;
      mask[i]|=uint64_t(1)<<(64-j-1);
      ++num_elem;
    }
    inline void unset(const uint64_t& n) {
      const uint64_t i = n/64;
      const uint64_t j = n%64;
      mask[i]&=((~uint64_t(0))^(uint64_t(1)<<(64-j-1)));
      --num_elem;
    }
    inline uint64_t find_node(const uint64_t& key,const uint64_t& map_key) {
      uint64_t j = 0;
      for (uint64_t i=map_key;
           is_set(i);
           (i=i+1>num_data?0:i+1,++j)) {
        if (data[i]==key) return i;
        if (j>log2(num_elem)) {
          if (fibonacci_mapping(data[i],num_data)<map_key) return num_data;
        }
        if (j==num_data) return num_data;
      }
      return num_data;
    }
    inline uint64_t find_node(const uint64_t& key) {
      return find_node(key,fibonacci_mapping(key,num_data));
    }
    void inline erase(const uint64_t& key,uint64_t position) {
      uint64_t i=position;
      while (true) {
        const uint64_t j=i+1>num_data?0:i+1;
        if (!is_set(j)) break;
        if (i<=fibonacci_mapping(data[j],num_data)) break;
        data[i]=data[j];
        i=j;
      }
      unset(i);
    }
    void inline erase(const uint64_t& key) {
      const uint64_t position = find_node(key);
      if (position<num_data) return;
      erase(key,position);
    }
    inline uint64_t& insert_assuming_space(
        const uint64_t key,
        const uint64_t map_key) {
      uint64_t i;
      for (uint64_t i=0;i!=num_data;++i) {
        cout << (is_set(i)?'x':'o');
      }
      cout << endl;
      for (i=map_key;is_set(i);i=(i+1==num_data)?0:i+1);
      set(i);
      data[i] = key;
      while (true) {
        const uint64_t j = i?i-1:num_data-1;
        if (!is_set(j)) break;
        if (fibonacci_mapping(data[j],num_data)>map_key) swap(data[j],data[i]);
        else break;
      }
      return data[i];
    }
    inline uint64_t& insert_assuming_space(const uint64_t& key) {
      return insert_assuming_space(key,fibonacci_mapping(key,num_data));
    }
    inline void resize(const uint64_t size) {
      cout << "resize to " << size << " (" << num_elem << ")" << endl;
      if (size==num_data) return;
      if (size <num_elem) throw std::domain_error(
            std::string(typeid(*this).name())
            +".resize("+to_string(size)+") :: "
            +"can't resize the table to smaller than the number of elements"
           );
      const uint64_t old_num_data = num_data;
      num_data = size;
      if (size<num_data) {
        for (uint64_t i=size;i!=old_num_data;i=i+1==old_num_data?0:i+1) {
          insert_assuming_space(data[i]);
        }
        for (uint64_t i=0;i!=old_num_data;++i) {
          if (!is_set(i)) break;
          if (fibonacci_mapping(data[i],old_num_data)>=num_data) {
            const auto tmp = data[i];
            erase(data[i],i);
            insert(tmp);
          } else {
            break;
          }
        }
        data = (uint64_t*)realloc(data,num_data*sizeof(uint64_t));
        if (((old_num_data+63)/64)!=((num_data+63)/64)) {
          mask = (uint64_t*)realloc(mask,(num_data+63)/64);
        }
      } else {
        data = (uint64_t*)realloc(data,num_data*sizeof(uint64_t));
        if (((old_num_data+63)/64)!=((num_data+63)/64)) {
          mask = (uint64_t*)realloc(mask,(num_data+63)/64);
          for (uint64_t i=(old_num_data+63)/64;i!=(num_data+63)/64;++i) {
            mask[i] = 0;
          }
        }
        if (old_num_data==0) return;
        uint64_t from = 0;
        {
          uint64_t i=1,j=1;
          while ((i+j>i)&&(i+j<old_num_data)) {
            j=j+i;
            i=j-i;
          }
          from = old_num_data-j;
        }
        cout << "from = " << from << endl;
        for (uint64_t i=from,j=0;
           //(j<old_num_data);
             (j<num_data)&&((j<num_data-old_num_data)||is_set(i));
             ((i=((i+1)==old_num_data)?0:i+1),++j)) {
          if (fibonacci_mapping(data[i],num_data)<old_num_data) continue;
          if (!is_set(i)) continue;
          const auto tmp = data[i];
          erase(data[i],i);
          insert(tmp);
        }
      }
    }
    // load_factor = 2/3
    void inline ensure_size(uint64_t size) {
      cout << "ensure_size(" << size << ")" << endl;
      const uint64_t min_size = 3*size/2;
      if (num_data<min_size) resize(min_size);
    }
    inline uint64_t& insert(const uint64_t& key,const uint64_t& map_key) {
      ensure_size(num_elem+1);
      for (uint64_t i=0;i!=num_data;++i) {
        cout << "|";
        if (is_set(i)) {
          cout<<data[i]<<" "<<fibonacci_mapping(data[i],num_data);
        }
        cout << endl;
      }
      cout << endl;
      return insert_assuming_space(key);
    }
    inline uint64_t& insert(const uint64_t& key) {
      return insert(key,fibonacci_mapping(key,num_data));
    }
    inline uint64_t count(uint64_t& key) {
      return find_node(key)<num_data;
    }
    inline uint64_t& operator[](const uint64_t& key) {
      const uint64_t map_key = fibonacci_mapping(key,num_data);
      const uint64_t i = find_node(key,map_key);
      if (i<num_data) return data[i];
      return insert(key,map_key);
    }
    ~golden_tree(){
      free(data);
      free(mask);
    }
  };
}

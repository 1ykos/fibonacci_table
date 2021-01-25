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
  using std::min;

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

  uint64_t constexpr largest_fibonacci_not_greater_than(uint64_t value) {
    uint64_t i=1,j=0;
    while ((i+j>=i)&&(i+j<=value)) {
      j=j+i;
      i=j-i;
    }
    return j;
  }
  
  uint64_t constexpr largest_fibonacci_less_than(const uint64_t value) {
    uint64_t i=1,j=0;
    while ((i+j>=i)&&(i+j<value)) {
      j=j+i;
      i=j-i;
    }
    return j;
  }

  uint64_t constexpr absolute_distance(const uint64_t a,const uint64_t b) {
    if (a<b) return b-a;
    else     return a-b;
  }
  
  uint64_t constexpr modular_distance(
      const uint64_t a,
      const uint64_t b,
      const uint64_t m)
  {
    if (a<b) {
      return min(b-a,a+m-b);
    } else {
      return min(a-b,b+m-a);
    }
  }
  
  struct fibonacci_table{
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
    inline uint64_t find_node(const uint64_t key) {
      return find_node(key,fibonacci_mapping(key,num_data));
    }
    void inline erase(const uint64_t key,uint64_t i) {
      while (true) {
        const uint64_t j=i+1>num_data?0:i+1;
        if (!is_set(j)) {
          //cout << "encountered empty slot" << endl;
          break;
        }
        const uint64_t map_data_j = fibonacci_mapping(data[j],num_data);
        if (modular_distance(i,map_data_j,num_data)
          >=modular_distance(j,map_data_j,num_data)
            ) {
          //cout << i << " " << j << " seems to be in order" << endl;
          break;
        }
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
      //cout << "insert " << key << " " << map_key << endl;
      uint64_t i;
      for (i=map_key;is_set(i);i=(i+1==num_data)?0:i+1);
      set(i);
      data[i] = key;
      //cout << "insert at " << i << endl;
      while (true) {
        const uint64_t j = i?i-1:num_data-1;
        if (!is_set(j)) {
          //cout << "encountered empty slot" << endl;
          break;
        }
        if (
            modular_distance(i,map_key,                            num_data)
          <=modular_distance(j,fibonacci_mapping(data[j],num_data),num_data)
            ) {
          //cout << i << " " << j << " seems to be in order" << endl;
          break;
        }
        swap(data[j],data[i]);
        i = j;
      }
      //cout << "moved to " << i << endl;
      return data[i];
    }
    inline uint64_t& insert_assuming_space(const uint64_t& key) {
      return insert_assuming_space(key,fibonacci_mapping(key,num_data));
    }
    void print_table() {
      for (uint64_t i=0;i!=num_data;++i) {
        cout << (is_set(i)?'x':'-');
        cout << endl;
      }
      return;
      cout << "table size = " << num_data << endl;
      for (uint64_t i=0;i!=num_data;++i) {
        cout << "|";
        if (is_set(i)) {
          const uint64_t map_data_i = fibonacci_mapping(data[i],num_data);
          cout << data[i] << " " << map_data_i <<" "
               << modular_distance(map_data_i,i,num_data);
        }
        cout << endl;
      }
      cout << endl;
    }
    inline void resize(const uint64_t size) {
      //print_table();
      //cout << "resize to " << size << " (" << num_elem << ")" << endl;
      if (size==num_data) return;
      if (size<=num_elem) throw std::domain_error(
            std::string(typeid(*this).name())
            +".resize("+to_string(size)+") :: "
            +"can't resize the table to smaller than "
            +"or equal the number of elements in the table."
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
        if (((old_num_data+63)/8)!=((num_data+63)/8)) {
          mask = (uint64_t*)realloc(mask,(num_data+63)/8);
        }
      } else {
        //cout << "realloc data " << num_data << " "
        //     << num_data*sizeof(uint64_t) << endl;
        data = (uint64_t*)realloc(data,num_data*sizeof(uint64_t));
        if (((old_num_data+63)/8)!=((num_data+63)/8)) {
          //cout << "realloc mask " << (num_data+63)/8 << endl;
          mask = (uint64_t*)realloc(mask,(num_data+63)/8);
          for (uint64_t i=(old_num_data+63)/64;i!=(num_data+63)/64;++i) {
            mask[i] = 0;
          }
        }
        if (old_num_data==0) return;
        const uint64_t from =
          old_num_data-largest_fibonacci_not_greater_than(old_num_data); 
        //cout << "from " << from << " " << old_num_data << endl;
        uint64_t i=from,j=0;
        for (uint64_t i=from,j=0;j!=num_data;++j) {
          if (!is_set(i)) {
            i=(i+1==old_num_data)?0:i+1;
            if (j<num_data-old_num_data) continue;
            else                         break;
          }
          //cout << "inspecting " << data[i] << " "
          //     << fibonacci_mapping(data[i],num_data) << endl;
          if (fibonacci_mapping(data[i],num_data)<old_num_data){
            i=(i+1==old_num_data)?0:i+1;
            continue;
          }
          const auto tmp = data[i];
          //const uint64_t tmp_num_data = num_data;
          //num_data = old_num_data;
          erase(data[i],i);
          //num_data = tmp_num_data;
          insert_assuming_space(tmp);
          if (data[i]==tmp) {
            i=(i+1==old_num_data)?0:i+1;
          }
        }
        for (uint64_t i=0;i!=num_data;++i) {
          if (!is_set(i)) break;
          const auto tmp = data[i];
          erase(data[i],i);
          insert_assuming_space(tmp);
          if (data[i]==tmp) break;
        }
        /* // this is the stupid way
        for (uint64_t i=0;i!=old_num_data;++i) {
          if (!is_set(i)) continue;
          const auto tmp = data[i];
          erase(data[i],i);
          insert(tmp);
        }
        */
      }
      //print_table();
    }
    // load_factor = 2/3
    void inline ensure_size(uint64_t size) {
      //cout << "ensure_size(" << size << ")" << endl;
      const uint64_t min_size = 2*size;
      //if (num_data<min_size) resize(8*size/5);
      if (num_data<min_size) resize(min_size);
    }
    inline uint64_t& insert(const uint64_t& key,const uint64_t& map_key) {
      ensure_size(num_elem+1);
      return insert_assuming_space(key);
    }
    inline uint64_t& insert(const uint64_t& key) {
      return insert(key,fibonacci_mapping(key,num_data));
    }
    inline uint64_t count(const uint64_t key) {
      return find_node(key)<num_data;
    }
    inline uint64_t& operator[](const uint64_t& key) {
      const uint64_t map_key = fibonacci_mapping(key,num_data);
      const uint64_t i = find_node(key,map_key);
      if (i<num_data) return data[i];
      return insert(key,map_key);
    }
    ~fibonacci_table(){
      free(data);
      free(mask);
    }
  };
}

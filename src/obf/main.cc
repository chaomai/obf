#include <iostream>

#include "basic_bloom_filter.h"
#include "common.h"
#include "ordinal_bloom_filter.h"

using namespace std;
using obf::BasicBloomFilter;
using obf::OrdinalBloomFilter;

int main() {
  int bit_array_size = 500000000;
  int hash_func_num = 7;

  Timer timer;

  BasicBloomFilter<int> bf(bit_array_size, hash_func_num);
  // OrdinalBloomFilter<int> bf(bit_array_size, hash_func_num);

  for (int i = 0; i < 3000000; ++i) {
    bf.add(i);
  }

  timer.print_passed();

  int match_count = 0;
  int false_count = 0;

  for (int i = 0; i < 3000000; ++i) {
    if (bf.contains(i)) {
      ++match_count;
    } else {
      ++false_count;
    }
  }

  timer.print_passed();

  cout << match_count << endl;
  cout << false_count << endl;
}

#include <iostream>
#include <string>

#include "common.h"
#include "gtest/gtest.h"
#include "obf/basic_bloom_filter.h"
#include "obf/bloom_filter.h"
#include "obf/ordinal_bloom_filter.h"

using obf::OrdinalBloomFilter;
using obf::BasicBloomFilter;
using obf::BloomFilter;
using std::cout;
using std::endl;
using std::string;

TEST(BloomFilter, default_constructor) {
  BloomFilter<string>* bf =
      new BasicBloomFilter<string>(false_positive, capacity);

  string vals[] = {"a", "b", "c"};

  for (const string& str : vals) {
    bf->add(str);
  }

  for (const string& str : vals) {
    ASSERT_TRUE(bf->contains(str));
  }
}

#ifndef OBF_BLOOM_FILTER_H_
#define OBF_BLOOM_FILTER_H_

namespace obf {

template <typename T>
class BloomFilter {
 public:
  virtual void clear() = 0;
  virtual void add(const T& elem) = 0;
  virtual bool contains(const T& elem) const = 0;
};
}

#endif  // OBF_BLOOM_FILTER_H_

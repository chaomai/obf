#ifndef OBF_ORDINAL_BLOOM_FILTER_H_
#define OBF_ORDINAL_BLOOM_FILTER_H_

namespace obf {

template <typename T>
class OrdinalBloomFilter {
 public:
  using size_type = std::uint64_t;

  // OrdinalBloomFilter(double false_positive, size_type capacity);
  OrdinalBloomFilter(size_type hash_func_num, size_type array_size);
  OrdinalBloomFilter(const OrdnalBloomFilter& rhs) = delete;
  OrdinalBloomFilter(OrdinalBloomFilter&& rhs) noexcept;
  OrdinalBloomFilter& operator=(const OrdinalBloomFilter& rhs) = delete;
  OrdinalBloomFilter& operator=(OrdinalBloomFilter&& rhs) noexcept;
  ~OrdnalBloomFilter();

  void clear();
  void add(const T& elem);
  bool contains(const T& elem) const;
  void swap(BloomFilter& rhs);

 private:
  size_type _size;
  size_type _hash_func_num;

  inline size_type hash_at_n(const T* p, size_type n) const;
};
}

#endif  // OBF_ORDINAL_BLOOM_FILTER_H_

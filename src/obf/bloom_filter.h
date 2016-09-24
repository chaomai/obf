#ifndef OBF_BLOOM_FILTER_H_
#define OBF_BLOOM_FILTER_H_

#include <cmath>
#include <cstddef>  // for size_t
#include <cstdint>  // for uint8_t
#include <stdexcept>

#include "MurmurHash3.h"

namespace obf {

template <typename T>
class BloomFilter {
  using elem_type = std::uint8_t;

 public:
  using size_type = std::uint64_t;

  BloomFilter(double false_positive, size_type capacity);
  BloomFilter(const BloomFilter& rhs) = delete;
  BloomFilter(BloomFilter&& rhs);
  BloomFilter& operator=(const BloomFilter& rhs) = delete;
  BloomFilter& operator=(BloomFilter&& rhs);
  ~BloomFilter() = default;

  void clear();
  void add(const T& elem);
  bool contains(const T& elem) const;
  void swap(BloomFilter& rhs);

 private:
  std::unique_ptr<elem_type[]> _bits;
  size_type _size;
  size_type _hash_func_num;

  inline size_type hash_at_n(const T* p, size_type n) const;
};

template <typename T>
BloomFilter<T>::BloomFilter(double false_positive, size_type capacity) {
  if (false_positive <= 0 || false_positive >= 1) {
    throw std::range_error("false positive is invalie.");
  }

  double ln2 = std::log(2);
  _size = static_cast<size_type>(
      std::ceil(-(capacity * std::log(false_positive) / (ln2 * ln2))));

  _bits.reset(new elem_type[_size]);

  double fractor = static_cast<double>(_size) / static_cast<double>(capacity);
  _hash_func_num = static_cast<size_type>(std::ceil(fractor * ln2));
}

template <typename T>
BloomFilter<T>::BloomFilter(BloomFilter&& rhs)
    : _bits(rhs._bits.release()),
      _size(rhs._size),
      _hash_func_num(rhs._hash_func_num) {
  rhs._size = 0;
  rhs._hash_func_num = 0;
}

template <typename T>
BloomFilter<T>& BloomFilter<T>::operator=(BloomFilter&& rhs) {
  if (this != &rhs) {
    _bits.reset(rhs._bits.release());
    _size = rhs._size;
    _hash_func_num = rhs._hash_func_num;

    rhs._size = 0;
    rhs._hash_func_num = 0;
  }
  return *this;
}

template <typename T>
void BloomFilter<T>::clear() {
  _bits.reset(new elem_type[_size]);
}

template <typename T>
void BloomFilter<T>::add(const T& elem) {
  for (size_type i = 0; i < _size; ++i) {
    size_type hash_val = hash_at_n(&elem, i);
    _bits[i] |= (1 << hash_val % 8);
  }
}

template <typename T>
bool BloomFilter<T>::contains(const T& elem) const {
  for (size_type i = 0; i < _size; ++i) {
    size_type hash_val = hash_at_n(&elem, i);
    if (!(_bits[i] & (1 << hash_val % 8))) {
      return false;
    }
  }
  return true;
}

template <typename T>
void BloomFilter<T>::swap(BloomFilter& rhs) {
  using std::swap;
  swap(_bits, rhs._bits);
  swap(_size, rhs._size);
  swap(_hash_func_num, rhs._hash_func_num);
}

template <typename T>
typename BloomFilter<T>::size_type BloomFilter<T>::hash_at_n(
    const T* p, size_type n) const {
  size_type hash_val[2];
  MurmurHash3_x64_128(p, sizeof(T), 0, hash_val);

  size_type hash_a = hash_val[0];
  size_type hash_b = hash_val[1];

  return (hash_a + n * hash_b) % _size;
}
}

#endif  // OBF_BLOOM_FILTER_H_

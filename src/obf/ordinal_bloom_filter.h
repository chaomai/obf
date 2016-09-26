#ifndef OBF_ORDINAL_BLOOM_FILTER_H_
#define OBF_ORDINAL_BLOOM_FILTER_H_

#include <cmath>
#include <cstddef>  // for size_t
#include <cstdint>  // for uint8_t
#include <stdexcept>
#include <vector>

#include "MurmurHash3.h"

namespace obf {

template <typename T>
class OrdinalBloomFilter {
  using elem_type = std::uint8_t;

 public:
  using size_type = std::uint64_t;

  OrdinalBloomFilter(size_type bit_array_size, size_type hash_func_num);
  // OrdinalBloomFilter(double false_positive, size_type capacity);
  OrdinalBloomFilter(const OrdinalBloomFilter& rhs) = delete;
  OrdinalBloomFilter(OrdinalBloomFilter&& rhs) noexcept;
  OrdinalBloomFilter& operator=(const OrdinalBloomFilter& rhs) = delete;
  OrdinalBloomFilter& operator=(OrdinalBloomFilter&& rhs) noexcept;
  ~OrdinalBloomFilter() = default;

  void clear();
  void add(const T& elem);
  bool contains(const T& elem) const;
  void swap(OrdinalBloomFilter& rhs);

 private:
  // the max number of hash function is 2 ^ 8, which shouble be big enough.
  std::vector<elem_type> _bits;
  size_type _bit_array_size;
  size_type _hash_func_num;

  inline size_type hash_at_n(const T* p, size_type n) const;
};

template <typename T>
OrdinalBloomFilter<T>::OrdinalBloomFilter(size_type bit_array_size,
                                          size_type hash_func_num)
    : _bit_array_size(bit_array_size), _hash_func_num(hash_func_num) {
  _bits = std::vector<elem_type>(_bit_array_size, false);
}

// template <typename T>
// OrdinalBloomFilter<T>::OrdinalBloomFilter(double false_positive,
// size_type capacity) {
// if (false_positive <= 0 || false_positive >= 1) {
// throw std::range_error("false positive is invalie.");
// }

// double ln2 = std::log(2);
// _bit_array_size = static_cast<size_type>(
// std::ceil(-(capacity * std::log(false_positive) / (ln2 * ln2))));

// _bits = std::vector<elem_type>(_bit_array_size, 0);

// double fractor =
// static_cast<double>(_bit_array_size) / static_cast<double>(capacity);
// _hash_func_num = static_cast<size_type>(std::round(fractor * ln2));
// }

template <typename T>
OrdinalBloomFilter<T>::OrdinalBloomFilter(OrdinalBloomFilter&& rhs) noexcept
    : _bits(std::move(rhs._bits)),
      _bit_array_size(rhs._bit_array_size),
      _hash_func_num(rhs._hash_func_num) {
  rhs._bit_array_size = 0;
  rhs._hash_func_num = 0;
}

template <typename T>
OrdinalBloomFilter<T>& OrdinalBloomFilter<T>::operator=(
    OrdinalBloomFilter&& rhs) noexcept {
  if (this != &rhs) {
    _bits = std::move(rhs._bits);
    _bit_array_size = rhs._bit_array_size;
    _hash_func_num = rhs._hash_func_num;

    rhs._bit_array_size = 0;
    rhs._hash_func_num = 0;
  }
  return *this;
}

template <typename T>
void OrdinalBloomFilter<T>::clear() {
  _bits.clear();
  _bits = std::vector<elem_type>(_bit_array_size, 0);
}

template <typename T>
void OrdinalBloomFilter<T>::add(const T& elem) {
  for (size_type i = 1; i <= _hash_func_num; ++i) {
    size_type hash_val = hash_at_n(&elem, i);

    if (_bits[hash_val] < i) {
      _bits[hash_val] = static_cast<elem_type>(i);
    }
  }
}

template <typename T>
bool OrdinalBloomFilter<T>::contains(const T& elem) const {
  for (size_type i = 1; i <= _hash_func_num; ++i) {
    size_type hash_val = hash_at_n(&elem, i);

    if (_bits[hash_val] < i) {
      return false;
    }
  }
  return true;
}

template <typename T>
void OrdinalBloomFilter<T>::swap(OrdinalBloomFilter& rhs) {
  using std::swap;
  swap(_bits, rhs._bits);
  swap(_bit_array_size, rhs._bit_array_size);
  swap(_hash_func_num, rhs._hash_func_num);
}

template <typename T>
typename OrdinalBloomFilter<T>::size_type OrdinalBloomFilter<T>::hash_at_n(
    const T* p, size_type n) const {
  size_type hash_val[2];
  MurmurHash3_x64_128(p, sizeof(T), 0, hash_val);

  size_type hash_a = hash_val[0];
  size_type hash_b = hash_val[1];

  return (hash_a + n * hash_b) % _bit_array_size;
}
}

#endif  // OBF_ORDINAL_BLOOM_FILTER_H_

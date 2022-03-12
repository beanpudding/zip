/*
** $Id: zip_iterables.hpp $
** zip iterables
** Li Wanbing<lwbcn@163.com>
** See Copyright Notice at the end of this file
*/
#pragma once

#include <algorithm>   // std::min
#include <tuple>

namespace miju {

template <class AI, class... ZI>
class zipped_iterator {
  std::tuple<AI, ZI...> pi_;
  using Indices = std::index_sequence_for<AI, ZI...>;

 public:
  using iterator_category = typename std::common_type<
      typename std::iterator_traits<AI>::iterator_category,
      typename std::iterator_traits<ZI>::iterator_category...>::type;
  using value_type =
      std::tuple<typename std::iterator_traits<AI>::reference,
                 typename std::iterator_traits<ZI>::reference...>;
  using difference_type = typename std::common_type<
      typename std::iterator_traits<AI>::difference_type,
      typename std::iterator_traits<ZI>::difference_type...>::type;
  using pointer = value_type*;
  using reference = value_type&;

 public:
  zipped_iterator() = default;
  zipped_iterator(const zipped_iterator&) = default;
  zipped_iterator(zipped_iterator&&) = default;
  zipped_iterator(const AI& ai, const ZI&... zi) : pi_(ai, zi...) {}

  zipped_iterator& operator=(const zipped_iterator& rhs) = default;
  zipped_iterator& operator=(zipped_iterator&& rhs) = default;

  zipped_iterator& operator+=(const difference_type d) {
    return operator_prefix_increment_impl(d, Indices{});
  }
  zipped_iterator& operator-=(const difference_type d) {
    return operator+=(-d);
  }

  zipped_iterator& operator++() { return operator+=(1); }
  zipped_iterator operator++(int) {
    zipped_iterator tmp{*this};
    operator++();
    return tmp;
  }
  bool operator==(const zipped_iterator& rhs) const {
    return pi_ == rhs.pi_;
  }
  bool operator!=(const zipped_iterator& rhs) const {
    return pi_ != rhs.pi_;
  }
  typename zipped_iterator::value_type operator*() {
    return operator_dereference_impl(Indices{});
  }
  difference_type operator-(const zipped_iterator& rhs) const {
    return std::distance(std::get<0>(rhs.pi_), std::get<0>(pi_));
  }
  zipped_iterator operator+(const difference_type d) const {
    zipped_iterator tmp{*this};
    tmp += d;
    return tmp;
  }
  zipped_iterator operator-(const difference_type d) const {
    zipped_iterator tmp{*this};
    tmp -= d;
    return tmp;
  }

  bool operator<(const zipped_iterator& rhs) const { return pi_ < rhs.pi_; }
  bool operator>(const zipped_iterator& rhs) const { return pi_ > rhs.pi_; }
  bool operator<=(const zipped_iterator& rhs) const { return pi_ <= rhs.pi_; }
  bool operator>=(const zipped_iterator& rhs) const { return pi_ >= rhs.pi_; }

 private:
  template <std::size_t... I>
  zipped_iterator& operator_prefix_increment_impl(
      const difference_type d,
      std::index_sequence<I...>) {
    int _[]{(std::advance(std::get<I>(pi_), d), 0)...};
    (void)_;
    return *this;
  }
  template <std::size_t... I>
  typename zipped_iterator::value_type operator_dereference_impl(
      std::index_sequence<I...>) {
    return std::forward_as_tuple(*std::get<I>(pi_)...);
  }
};

template <class A, class... Z>
class zipped_view {
  std::tuple<A&, Z&...> p_;
  const std::size_t sz_;
  using Indices = std::index_sequence_for<A, Z...>;

 public:
  using iterator = zipped_iterator<
      decltype(std::begin(
          std::declval<typename std::add_lvalue_reference<A>::type>())),
      decltype(std::begin(
          std::declval<typename std::add_lvalue_reference<Z>::type>()))...>;
  using const_iterator = zipped_iterator<
      decltype(std::cbegin(
          std::declval<typename std::add_lvalue_reference<A>::type>())),
      decltype(std::cbegin(
          std::declval<typename std::add_lvalue_reference<Z>::type>()))...>;
  using reverse_iterator = zipped_iterator<
      decltype(std::rbegin(
          std::declval<typename std::add_lvalue_reference<A>::type>())),
      decltype(std::rbegin(
          std::declval<typename std::add_lvalue_reference<Z>::type>()))...>;
  using const_reverse_iterator = zipped_iterator<
      decltype(std::crbegin(
          std::declval<typename std::add_lvalue_reference<A>::type>())),
      decltype(std::crbegin(
          std::declval<typename std::add_lvalue_reference<Z>::type>()))...>;

  using value_type = typename iterator::value_type;
  using size_type = std::size_t;
  using difference_type = typename iterator::difference_type;
  using pointer = typename iterator::pointer;
  using const_pointer = const pointer;
  using reference = typename iterator::reference;
  using const_reference = const reference;

 public:
  zipped_view() = delete;
  zipped_view(const zipped_view&) = default;
  zipped_view(zipped_view&&) = default;
  zipped_view(A& a, Z&... z) : p_(a, z...), sz_(size_impl(Indices{})) {}

  iterator begin() { return begin_impl(Indices{}); }
  const_iterator begin() const { return cbegin_impl(Indices{}); }
  iterator end() { return end_impl(Indices{}); }
  const_iterator end() const { return cend_impl(Indices{}); }
  reverse_iterator rbegin() { return rbegin_impl(Indices{}); }
  const_reverse_iterator rbegin() const { return crbegin_impl(Indices{}); }
  reverse_iterator rend() { return rend_impl(Indices{}); }
  const_reverse_iterator rend() const { return crend_impl(Indices{}); }
  const_iterator cbegin() const { return begin(); }
  const_iterator cend() const { return end(); }
  const_reverse_iterator crbegin() const { return rbegin(); }
  const_reverse_iterator crend() const { return rend(); }

  size_type size() const { return sz_; }

 private:
  template <std::size_t... I>
  iterator begin_impl(std::index_sequence<I...>) {
    return iterator(std::begin(std::get<I>(p_))...);
  }
  template <std::size_t... I>
  iterator end_impl(std::index_sequence<I...>) {
    return iterator(std::next(std::begin(std::get<I>(p_)), sz_)...);
  }
  template <std::size_t... I>
  reverse_iterator rbegin_impl(std::index_sequence<I...>) {
    return reverse_iterator(std::rbegin(std::get<I>(p_))...);
  }
  template <std::size_t... I>
  reverse_iterator rend_impl(std::index_sequence<I...>) {
    return reverse_iterator(std::next(std::rbegin(std::get<I>(p_)), sz_)...);
  }
  template <std::size_t... I>
  const_iterator cbegin_impl(std::index_sequence<I...>) const {
    return const_iterator(std::cbegin(std::get<I>(p_))...);
  }
  template <std::size_t... I>
  const_iterator cend_impl(std::index_sequence<I...>) const {
    return const_iterator(std::next(std::cbegin(std::get<I>(p_)), sz_)...);
  }
  template <std::size_t... I>
  const_reverse_iterator crbegin_impl(std::index_sequence<I...>) const {
    return const_reverse_iterator(std::crbegin(std::get<I>(p_))...);
  }
  template <std::size_t... I>
  const_reverse_iterator crend_impl(std::index_sequence<I...>) const {
    return const_reverse_iterator(
        std::next(std::crbegin(std::get<I>(p_)), sz_)...);
  }

  template <std::size_t... I>
  size_type size_impl(std::index_sequence<I...>) const {
    return std::min({std::distance(std::begin(std::get<I>(p_)),
                                   std::end(std::get<I>(p_)))...});
  }
};

template <class A, class... Z>
constexpr zipped_view<typename std::remove_reference<A>::type,
                      typename std::remove_reference<Z>::type...>
make_zipped_view(A&& a, Z&&... z) {
  using t = zipped_view<typename std::remove_reference<A>::type,
                        typename std::remove_reference<Z>::type...>;
  return t(std::forward<A>(a), std::forward<Z>(z)...);
}

template <class A, class... Z>
constexpr auto zip(A&& a, Z&&... z) {
  return make_zipped_view(std::forward<A>(a), std::forward<Z>(z)...);
}

}  // namespace miju

/******************************************************************************
 * Copyright (C) 2022-present, Li Wanbing<lwbcn@163.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *****************************************************************************/

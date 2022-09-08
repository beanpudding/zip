## 关于
C++中实现类似Python库中的zip函数。

zip函数用于将两个或多个可迭代对象作为参数，将对象中对应的元素打包成一个个元组，然后返回由这些元组组成的代理容器。

如果各个可迭代对象的元素个数不一致，则返回的代理容器长度与最短的对象相同。


## 如何使用
包含头文件"[zip_iterables.hpp](./zip_iterables.hpp)"即可

需支持C++14（或以上）的编译器

以下编译器经过测试
* msvc(vs2019), 
* gcc(9.3.0)
* clang(7.1.0)

### 调用方式
```
auto zipped = zip([iterable, ...]);
```
### 返回值

返回一个zipped_view代理容器，可通过泛型for或常规begin/end等函数方式迭代遍历获得每个参数容器元素的引用组成的tuple。

zipped_view代理容器可通过begin、end等函数获取其迭代器，其迭代器支持以下操作
* operator++
* operator!=
* operator*
* ...
***
### 注意
zipped_view的用途是为了遍历打包的容器。和通常的遍历原则一致，遍历过程不得添加或移除容器元素，因为这样做会使得遍历过程不可预知。
故而使用zip函数返回的zipped_view对象也得注意
- 遍历zipped_view对象的过程中不得添加或移除任何容器元素；
- 任何容器新增了元素或移除了已有元素，需通过zip函数重新获取一个zipped_view对象。


## 示例
```
#include "zip_iterables.hpp"
#include <algorithm>
#include <array>
#include <iostream>
#include <list>
#include <vector>

using namespace miju;

int main() {
  {
    int a[5]{1, 3, 5, 7, 9};
    int b[5]{2, 4, 6, 8, 10};
    std::cout << "array:";
    for (auto&& c : zip(a, b)) {
      std::cout << " (" << std::get<0>(c) << "," << std::get<1>(c) << ")";
    }
    std::cout << std::endl;
  }

  {
    int a[5]{1, 3, 5, 7, 9};
    std::string b{"abcd"};
    std::cout << "array and std::string:";
    for (auto&& c : zip(a, b)) {
      std::cout << " (" << std::get<0>(c) << "," << std::get<1>(c) << ")";
    }
    std::cout << std::endl;
  }

  {
    std::array<int, 5> a{1, 3, 5, 7, 9};
    auto il = {10, 20, 30};
    std::cout << "std::array and initializer_list: ";
    for (auto&& c : zip(a, il)) {
      std::cout << " (" << std::get<0>(c) << "," << std::get<1>(c) << ")";
    }
    std::cout << std::endl;
  }

  #if 0 // 不支持直接使用初始化列表
  {
    std::array<int, 5> a{1, 3, 5, 7, 9};
    std::cout << "std::array and initializer_list: ";
    for (auto&& c : zip(a, {10, 20, 30})) {
      std::cout << " (" << std::get<0>(c) << "," << std::get<1>(c) << ")";
    }
    std::cout << std::endl;
  }
  #endif

  {
    std::array<int, 5> a{1, 3, 5, 7, 9};
    std::array<int, 5> b{2, 4, 6, 8, 10};
    std::cout << "std::array: ";
    for (auto&& c : zip(a, b)) {
      std::cout << " (" << std::get<0>(c) << "," << std::get<1>(c) << ")";
    }
    std::cout << std::endl;
  }

  {
    std::vector<int> a{1, 3, 5, 7, 9};
    std::vector<int> b{2, 4, 6, 8, 10};
    std::cout << "std::vector: ";
    for (auto&& c : zip(a, b)) {
      std::cout << " (" << std::get<0>(c) << "," << std::get<1>(c) << ")";
    }
    std::cout << std::endl;
  }

  {
    std::vector<int> a{1, 3, 5, 7, 9};
    std::list<int> b{2, 4, 6, 8, 10};
    std::cout << "mix std::vector and std::list: ";
    for (auto&& c : zip(a, b)) {
      std::cout << " (" << std::get<0>(c) << "," << std::get<1>(c) << ")";
    }
    std::cout << std::endl;
  }

  {
    std::array<int, 5> a{1, 3, 5, 7, 9};
    std::vector<int> b{2, 4, 6, 8, 10};
    std::cout << "mix std::array and std::vector: ";
    for (auto&& c : zip(a, b)) {
      std::cout << " (" << std::get<0>(c) << "," << std::get<1>(c) << ")";
    }
    std::cout << std::endl;
  }

  {
    int a[]{1, 3, 5, 7, 9, 11};
    int b[]{2, 4, 6, 8, 10};
    std::cout << "array(diff size):";
    auto ab = zip(a, b);
    for (auto&& c : ab) {
      std::cout << " (" << std::get<0>(c) << "," << std::get<1>(c) << ")";
    }
    std::cout << std::endl;
    auto ab_it = ab.begin();
    auto ab0 = *ab_it++;
    std::cout << "index0: (" << std::get<0>(ab0) << ", " << std::get<1>(ab0)
              << ")\n";
    std::cout << "index1: (" << std::get<0>(*ab_it) << ", "
              << std::get<1>(*ab_it) << ")\n";
    ++ab_it;
    std::cout << "index2: (" << std::get<0>(*ab_it) << ", "
              << std::get<1>(*ab_it) << ")\n";
    std::cout << "find(" << std::get<0>(*ab_it) << ", " << std::get<1>(*ab_it)
              << "):";
    auto found = std::find(ab.begin(), ab.end(), *ab_it);
    std::cout << (found != ab.end() ? " found" : " not found") << std::endl;
    int aa = 5, bb = 7;
    std::cout << "find(" << aa << ", " << bb << "):";
    found = std::find_if(ab.begin(), ab.end(), [&aa, &bb](auto&& c) {
      return (std::get<0>(c) == aa) && (std::get<1>(c) == bb);
    });
    std::cout << (found == ab.end() ? " not found" : " found") << std::endl;
  }

  {
    int a[]{1, 3, 5, 7, 9};
    int b[]{2, 4, 6, 8, 10};
    char c[]{'a', 'b', 'c'/*, 'd', 'e'*/};
    auto abc = zip(a, b, c);
    std::cout << "3 array:";
    for (auto&& c : abc) {
      std::cout << " (" << std::get<0>(c) << "," << std::get<1>(c) << ","
                << std::get<2>(c) << ")";
    }
    std::cout << std::endl;
  }

  {
    int a[]{1, 3, 5, 7, 9};
    std::cout << "1 array:";
    for (auto&& c : zip(a)) {
      std::cout << " (" << std::get<0>(c) << ")";
    }
    std::cout << std::endl;
  }
  
  {
    int a[]{1, 3, 5, 7, 9};
    int b[]{2, 4, 6, 8/*, 10*/};
    std::cout << "array(reverse):";
    auto ab = zip(a, b);
    std::for_each(ab.rbegin(), ab.rend(), [](auto&& c) {
      std::cout << " (" << std::get<0>(c) << "," << std::get<1>(c) << ")";
    });
    std::cout << std::endl;
  }

  {
    int a[]{1, 3, 5, 7, 9};
    std::array<int, 5> b{2, 4, 6, 8, 10};
    std::vector<char> c{'a', 'b', 'c' /*, 'd', 'e'*/};
    std::map<int, int> d{{1, 1}, {2, 2}, {3, 3}};
    std::cout << "nest:";
    for (auto&& c : zip(zip(a, b), c, d)) {
      std::cout << " (" << std::get<0>(std::get<0>(c)) << ","
                << std::get<1>(std::get<0>(c)) << "," << std::get<1>(c) << ",{"
                << std::get<2>(c).first << "," << std::get<2>(c).second << "})";
    }
    std::cout << std::endl;
  }
}
```
输出
```
array: (1,2) (3,4) (5,6) (7,8) (9,10)
array and std::string: (1,a) (3,b) (5,c) (7,d)
std::array and initializer_list:  (1,10) (3,20) (5,30)
std::array:  (1,2) (3,4) (5,6) (7,8) (9,10)
std::vector:  (1,2) (3,4) (5,6) (7,8) (9,10)
mix std::vector and std::list:  (1,2) (3,4) (5,6) (7,8) (9,10)
mix std::array and std::vector:  (1,2) (3,4) (5,6) (7,8) (9,10)
array(diff size): (1,2) (3,4) (5,6) (7,8) (9,10)
index0: (1, 2)
index1: (3, 4)
index2: (5, 6)
find(5, 6): found
find(5, 7): not found
3 array: (1,2,a) (3,4,b) (5,6,c)
1 array: (1) (3) (5) (7) (9)
array(reverse): (9,8) (7,6) (5,4) (3,2)
nest: (1,2,a,{1,1}) (3,4,b,{2,2}) (5,6,c,{3,3})
```


## License
Open sourced under [MIT license](http://opensource.org/licenses/MIT), the terms of which can be read here - [LICENSE](./LICENSE).

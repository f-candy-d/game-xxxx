#### template `<unsigned int D, typename T>` class
# vec
n次元ベクトクラス。尚、2次元ベクトルを使用する場合は`DLib::vec2`の方が断然速い。  
テンプレート引数として、次元`D`、成分の型`T`を指定する。成分の値を`std::vector`の要素として管理する。

#### 名前空間
```c++
namespace DLib
{
	template <unsigned int D, typename T> class vec;
}
```

#### using , typedef
```c++
using iterator = typename std::vector<T>::iterator;
using const_iterator = typename std::vector<T>::const_iterator;
```
`std::vector`クラスのイテレータをこのクラスのイテレータのように扱う。

#### protected メンバ変数
```c++
std::vector<T> mComponents;
```

`std::vector<T> mComponents`
>ベクトルの成分。

#### public メンバ関数
```c++
vec();
vec(T dflt);
unsigned int dim();
template < typename... Rest> void set(const T& first, const Rest&... rest);
iterator begin();
iterator end();
const_iterator cbegin() const;
const_iterator cend() const;
T& operator[](std::size_t index);
const T& operator[](std::size_t index) const;
vec<D, T>& operator+() const;
vec<D, T> operator+(const vec<D, T>& other) const;
vec<D, T> operator-();
vec<D, T> operator-(const vec<D, T>& other) const;
```

`vec()`
>デフォルトコンストラクタ。

`vec(T dflt)`
>**@param dflt :** デフォルトの成分の値  

>デフォルトの成分の値を指定するコンストラクタ。ベクトルの全ての成分を`dflt`の値に設定する。

`unsigned int dim()`
>次元を返す。

`template < typename... Rest> void set(const T& first, const Rest&... rest)`
>**@param first :** 第1成分  
>**@param rest :** 第2成分以降（可変引数）  

>成分の値を設定する。 (次元) > (`first`を含めた引数の数) の場合、第1成分から順に引数の値を設定していく。  
>(次元) < (`first`を含めた引数の数) の場合、動作は未定義。

`iterator begin()`
>`mComponents`の`begin()`を内部で呼ぶ。

`iterator end()`
>`mComponents`の`end()`を内部で呼ぶ。

`const_iterator cbegin() const`
>`mComponents`の`cbegin()`を内部で呼ぶ。

`const_iterator cend() const`
>`mComponents`の`cend()`を内部で呼ぶ。

`T& operator[](std::size_t index)`
>**@param index :** ex.)第1成分 <=> index = 0  

>ベクトルの第`index+1`成分にアクセスする。 (次元) <= (index) の場合、動作は未定義。

`const T& operator[](std::size_t index) const`
>**@param index :** ex.)第1成分 <=> index = 0  

>ベクトルの第`index+1`成分にアクセスする。 (次元) <= (index) の場合、動作は未定義。

`vec<D, T>& operator+() const`
>なにもしない。。。

`vec<D, T> operator+(const vec<D, T>& other) const`
>**@param other :** `DLib::vec`オブジェクト  

>自分自身　+ `other` の演算をし、新しいオブジェクトとして返却。

`vec<D, T> operator-()`
>全ての成分の正負を逆転させる。

`vec<D, T> operator-(const vec<D, T>& other) const`
>**@param other :** `DLib::vec`オブジェクト  

>自分自身 - `other` の演算をし、新しいオブジェクトとして返却。

#### private メンバ関数
```c++
void set_recur(std::size_t index);
template < typename... Rest> void set_recur(std::size_t index, const T& first, const Rest&... rest);
```

`void set_recur(std::size_t index)`
>下の`set_recur()`関数のダミー関数で、特に何もしない。再帰の最後に呼ばれる。

`template < typename... Rest> void set_recur(std::size_t index, const T& first, const Rest&... rest)`
>**@param index :** ex.)第1成分 <=> index = 0  
>**@param first :** n階目の再帰で操作する引数の値（ベクトルの成分値）  
>**@param rest :** n+1階目以降の再帰で操作される引数の値（複数）  

>`public: set()`関数の実体。可変引数テンプレートを用いた可変引数関数。引数に、ベクトルの第1成分、第2成分...と順に渡していく。  
>n階目の再帰で、`first`の値をベクトルの第(index+1)成分として設定する。  
>(次元) < (index + 1) の場合、動作は未定義。  
>再帰の最後に、ダミー関数である上の`set_recur()`を呼ぶ。

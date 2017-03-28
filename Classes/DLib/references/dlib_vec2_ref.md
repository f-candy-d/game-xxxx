#### template `<typename T>` struct
# vec2
2次元ベクトルクラス。尚、2次元程度ならば`DLib::vec`よりもこちらの方が**高速**。
テンプレート引数として、要素の型`T`を渡す。

#### 名前空間
```c++
namespace DLib
{
	template <typename T> struct vec2;
}
```

#### public メンバ変数
```c++
T x;
T y;
```

`T x`
>X成分。

`T y`
>Y成分。

#### public メンバ関数
```c++
vec2();
vec2(T p, T q);
vec2<T>& operator+() const;
vec2<T>& operator-();
vec2<T> operator+(const vec2<T>& other) const;
vec2<T> operator-(const vec2<T>& other) const;
void set(T p, T q);
```

`vec2()`
>デフォルトコンストラクタ。`(0,0)`で初期化する。

`vec2(T p, T q)`
>**@param p :** X成分  
>**@param q :** Y成分  

>デフォルトの成分値を指定するコンストラクタ。

`vec2<T>& operator+() const`
>特に何もしない。。。

`vec2<T>& operator-()`
>全ての成分の正負を逆転する。

`vec2<T> operator+(const vec2<T>& other) const`
>**@param other :** 他の`vec2`オブジェクト  

>自分自身 + `other` の演算をし、新しいオブジェクトとして返す。

`vec2<T> operator-(const vec2<T>& other) const`
>**@param other :**  

>自分自身 - `other` の演算をし、新しいオブジェクトとして返す。

`void set(T p, T q)`
>**@param p :** X成分  
>**@param q :** Y成分  

>新しい成分値を設定する。

#### global メンバ関数
```c++
template <typename T> std::ostream& operator<<(std::ostream& os, const vec2<T>& v2);
```

`template <typename T> std::ostream& operator<<(std::ostream& os, const vec2<T>& v2)`
>**@param os :** outputストリーム  
>**@param v2 :** `vec2`オブジェクト  

>`std::cout << vec2_object << std::endl;`を可能にするためにこれが必要。

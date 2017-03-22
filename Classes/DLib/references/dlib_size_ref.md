##### template `<typename T>` struct
# size
T型（値型）のwidth値、height値を持つ。負値も扱えるが、その場合、動作は未定義。  
ある2つのsizeオブジェクトa、ｂについて、`a.width <(>) b.width`かつ`a.height <(>) b.height`のときのみ、`a <(>) b`が成り立つ。2つの条件の内**どちらか1つのみが真**の場合は、aとｂは**比較不可能**とする。  
また、等号`==`について、a,bが**図形的に合同であるとき**のみ真とする(`a.width == b.width`かつ`a.height == b.height`)。面積が等しいかどうかを判定する場合は、代わりに`area()`関数の結果を利用する。
#### 名前空間
```c++
namespace DLib
{
	template <typename T> struct size;
}
```
#### public メンバ変数
```c++
T width;
T height;
```

`T width`  
>横幅。

`T height`  
>縦幅。

#### *public* メンバ関数
```c++
size();
size(T w, T h);
T area();
```

`size()`  
>デフォルトコンストラクタ。特に何もしない。

`size(T w, T h)`  
> @param w : 横幅  
> @param h : 縦幅  
>
>引数付きコンストラクタ。メンバの初期化のみ。

`T area()`  
>面積（*width* × *height*）を返す。

#### global 関数
```c++
namespace DLib
{
	template <typename T>
	bool operator<(const size<T>& a, const size<T>& b);

	template <typename T>
	bool operator>(const size<T>& a, const size<T>& b);

	template <typename T>
	bool operator==(const size<T>& a, const size<T>& b);

	template <typename T>
	bool operator!=(const size<T>& a, const size<T>& b);

	template <typename T>
	bool operator<=(const size<T>& a, const size<T>& b);

	template <typename T>
	bool operator>=(const size<T>& a, const size<T>& b);

	template <typename T>
	std::ostream& operator<<(std::ostream& os, const size<T>& psize);
}
```

`template <typename T> bool operator<(const size<T>& a, const size<T>& b)`  
> @param a : 比較対象A  
> @param b : 比較対象B  
>
>`a < b`の判定。

`template <typename T> bool operator>(const size<T>& a, const size<T>& b)`  
> @param a : 比較対象A  
> @param b : 比較対象B  
>
>`a > b`の判定。

`template <typename T> bool operator==(const size<T>& a, const size<T>& b)`  
> @param a : 比較対象A  
> @param b : 比較対象B  
>
>`a == b`の判定。

`template <typename T> bool operator!=(const size<T>& a, const size<T>& b)`  
> @param a : 比較対象A  
> @param b : 比較対象B  
>
>`a != b`の判定。

`template <typename T> bool operator<=(const size<T>& a, const size<T>& b)`  
> @param a : 比較対象A  
> @param b : 比較対象B  
>
>`a <= b`の判定。

`template <typename T> bool operator>=(const size<T>& a, const size<T>& b)`  
> @param a : 比較対象A  
> @param b : 比較対象B  
>
>`a >= b`の判定。

`template <typename T> std::ostream& operator<<(std::ostream& os, const size<T>& psize)`  
> @param os    : outputストリーム  
> @param psize : 表示するsizeオブジェクト  
>
>`std::cout << size_object << std::endl;`を可能にするためにこれが必要。

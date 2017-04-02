#### struct
# grid_size
グリッド座標系でのサイズ(width x height)を管理する。グリッドの単位となる格子のサイズをDLib::size<float>で指定する。  
`grid_size`同士の比較については、`DLib::size`と同様の基準で判定する。

#### 名前空間
```c++
namespace DLib
{
	struct grid_size;
}
```

#### public メンバ変数
```c++
size_t width;
size_t height;
const DLib::size<float> unit;
```

`size_t width`
>横幅。x軸方向の格子の数に相当。

`size_t height`
>縦幅。ｙ軸方向の格子の数に相当。

`const DLib::size<float> unit`
>単位となる格子のサイズ。

#### public メンバ関数
```c++
grid_size(DLib::size<float> u);
grid_size(size_t w, size_t h, DLib::size<float> u);
DLib::size<float> comv_to_px();
size_t area();
```

`grid_size(DLib::size<float> u)`
>**@param u :** 単位となる格子のサイズ  

>コンストラクタ。必ず単位となる格子のサイズをここで指定する。

`grid_size(size_t w, size_t h, DLib::size<float> u)`
>**@param w :** 横幅のデフォルト値  
>**@param h :** 縦幅のデフォルト値  
>**@param u :** 単位となる格子のサイズ  

>width、heightのデフォルト値を指定するコンストラクタ。

`DLib::size<float> comv_to_px()`
>グリッド座標系でのサイズ（縦横の格子の数）を実数座標系でのサイズ(px)に変換する。

`size_t area()`
>面積（四角形の中にある格子の数）を返す。単位はpxでないので注意。

#### global メンバ関数
```c++
bool operator<(const grid_size& a, const grid_size& b);
bool operator>(const grid_size& a, const grid_size& b);
bool operator==(const grid_size& a, const grid_size& b);
bool operator!=(const grid_size& a, const grid_size& b);
bool operator<=(const grid_size& a, const grid_size& b);
bool operator>=(const grid_size& a, const grid_size& b);
std::ostream& operator<<(std::ostream& os, const DLib::grid_size& gd_size);
```

`bool operator<(const grid_size& a, const grid_size& b)`
>**@param a :** 比較対象A  
>**@param b :** 比較対象B  

>`a < b`の比較

`bool operator>(const grid_size& a, const grid_size& b)`
>**@param a :** 比較対象A  
>**@param b :** 比較対象B  

>`a > b`の比較

`bool operator==(const grid_size& a, const grid_size& b)`
>**@param a :** 比較対象A  
>**@param b :** 比較対象B  

>`a == b`の比較

`bool operator!=(const grid_size& a, const grid_size& b)`
>**@param a :** 比較対象A  
>**@param b :** 比較対象B  

>`a != b`の比較

`bool operator<=(const grid_size& a, const grid_size& b)`
>**@param a :** 比較対象A  
>**@param b :** 比較対象B  

>`a <= b`の比較

`bool operator>=(const grid_size& a, const grid_size& b)`
>**@param a :** 比較対象A  
>**@param b :** 比較対象B  

>`a >= b`の比較

`std::ostream& operator<<(std::ostream& os, const DLib::grid_size& gd_size)`
>**@param os :** outputストリーム  
>**@param gd_size :** `grid_size`オブジェクト  

>`std::cout << grid_size_object << std::endl;`を可能にするためにこれが必要。

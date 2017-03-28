#### struct
# grid_point
グリッド座標系での座標（0 <= の整数）を管理する。グリッドの単位（1つの格子）はDLib::size<float>で指定する。  
グリッド座標系から実数座標系に変換する場合、グリッド座標系でのある格子（=ある座標）内の１点（代表点）に、その格子の全ての面積が集中すると考え、その位置を実数座標系に変換する。
代表点の位置は、その格子の左下を原点として、x軸、y軸方向それぞれを0.0 ~ 1.0のスケール値で指定する。

#### 名前空間
```c++
namespace DLib
{
	struct grid_point;
}

```

#### enum
```c++
enum class RepresentativePoint
{
	LEFT_BOTTOM,
	RIGHT_BOTTOM,
	LEFT_TOP,
	RIGHT_TOP,
	CENTER,
};
```
代表的な**代表点**の位置。上記のもの以外の位置に設定したい場合は、`set_representative_point()`関数を使用して直接、実数値で指定する。  
尚、上の列挙定数は、`set_representative_point()`関数で、上から順に、`(0,0)`,`(1.0,0)`,`(0,1.0)`,`(1.0,1.0)`,`(0.5,0.5)`を指定した場合と同じ。

#### public メンバ変数
```c++
unsigned int x;
unsigned int y;
const DLib::size<float> unit;
```

`unsigned int x`
>グリッド座標系でのx座標

`unsigned int y`
>グリッド座標系でのy座標

`const DLib::size<float> unit`
>グリッドの単位となる格子のサイズ。コンストラクタで初期化し、変更不可。

#### private メンバ変数
```c++
float rep_point_scale_x;
float rep_point_scale_y;
```

`float rep_point_scale_x`
>代表点のx軸方向の位置を指定するスケール値。格子の左側を0、右側を1として、その間の値でx軸方向の位置を指定する。

`float rep_point_scale_y`
>代表点のｙ軸方向の位置を指定するスケール値。格子の下側を0、上側を1として、その間の値でｙ軸方向の位置を指定する。

#### public メンバ関数
```c++
grid_point(DLib::size<float> unit);
grid_point(unsigned int p, unsigned int q, DLib::size<float> unit);
void set_representative_point(RepresentativePoint type);
void set_representative_point(float rep_scal_x, float rep_scal_y);
DLib::vec2<float> comv_to_px() const;
```

`grid_point(DLib::size<float> unit)`
>**@param unit :** 単位格子のサイズ  

>コンストラクタ。必ず単位となる格子のサイズをここで設定する。デフォルトの座標は原点。

`grid_point(unsigned int p, unsigned int q, DLib::size<float> unit)`
>**@param p :** デフォルトのx座標  
>**@param q :** デフォルトのy座標  
>**@param unit :** 単位格子のサイズ  

>デフォルトの座標を指定するコンストラクタ。

`void set_representative_point(RepresentativePoint type)`
>**@param type :** 代表的な代表点の位置を指定する列挙型  

>代表点の位置を表すスケール値を列挙型で指定する。

`void set_representative_point(float rep_scal_x, float rep_scal_y)`
>**@param rep_scal_x :** x軸方向のスケール値  
>**@param rep_scal_y :** y軸方向のスケール値  

>代表点の位置を表すスケール値を実数値でそれぞれ指定する。0.0以下の値は0.0に、1.0以上の値は1.0に丸める。

`DLib::vec2<float> comv_to_px() const`
>グリッド座標系から実数座標系に変換する。

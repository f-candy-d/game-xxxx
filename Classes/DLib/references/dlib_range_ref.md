#### class
# range
数直線上での”範囲”（始点と終点）を管理する。範囲は可変だが、始点は必ず**終点の左側**に在らなければならない。

#### 名前空間
```c++
namespace DLib {
	class range;
}
```

#### private メンバ変数
```c++
int mFrom;
int mTo;
```

`int mFrom`
>始点。

`int mTo`
>終点。

#### public メンバ関数
```c++
range(int from = 0, int to = 0);
void set(int from, int to);
int from() const;
void from(int from);
int to() const;
void to(int to);
int diff() const;
int distance() const;
void move(int delta);
bool contains(int p) const;
range merge(const range& other) const;
```

`range(int from = 0, int to = 0)`
>**@param from :** 始点のデフォルト値  
>**@param to :** 終点のデフォルト値  

>デフォルトコンストラクタ。始点>終点の場合は`assert`でエラー。

`void set(int from, int to)`
>**@param from :** 始点  
>**@param to :** 終点  

>範囲を変更する。始点>終点の場合は`assert`でエラー。

`int from() const`
>始点のgetter関数。

`void from(int from)`
>**@param from :** 始点  

>始点のsetter関数。

`int to() const`
>終点のgetter関数。

`void to(int to)`
>**@param to :** 終点  

>終点のsetter関数。

`int diff() const`
>始点と終点の差分。最小（始点と終点が重なり合っている場合）の距離は0。 => `from - to`

`int distance() const`
>始点と終点を含めた、２点間の距離。最小（始点と終点が重なり合っている場合）の距離は1。 => `diff() + 1`

`void move(int delta)`
>**@param delta :** 移動距離  

>範囲を数直線上で平行移動させる。

`bool contains(int p) const`
>**@param p :** 対象となる数直線上での位置  

>位置`p`が範囲内にあるかどうかを判定。あれば`true`、なければ`false`を返す。

`range merge(const range& other) const`
>**@param other :** 自分自身とマージする他の`range`オブジェクト  

>自分自身と`other`を包括する(最小の)、新しい`range`オブジェクトを返す。  

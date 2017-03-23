#### template `<typename T>` struct
# size
[class summary here...]

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
>description here...

`T height`
>description here...

#### public メンバ関数
```c++
size();
size(T w, T h);
T area();
```

`size()`
>description here...

`size(T w, T h)`
>**@param w :**  
>**@param h :**  
>

`T area()`
>description here...

#### global メンバ関数
```c++
template <typename T> bool operator<(const size<T>& a, const size<T>& b);
template <typename T> bool operator>(const size<T>& a, const size<T>& b);
template <typename T> bool operator==(const size<T>& a, const size<T>& b);
template <typename T> bool operator!=(const size<T>& a, const size<T>& b);
template <typename T> bool operator<=(const size<T>& a, const size<T>& b);
template <typename T> bool operator>=(const size<T>& a, const size<T>& b);
template <typename T> std::ostream& operator<<(std::ostream& os, const size<T>& psize);
```

`template <typename T> bool operator<(const size<T>& a, const size<T>& b)`
>**@param a :**  
>**@param b :**  
>

`template <typename T> bool operator>(const size<T>& a, const size<T>& b)`
>**@param a :**  
>**@param b :**  
>

`template <typename T> bool operator==(const size<T>& a, const size<T>& b)`
>**@param a :**  
>**@param b :**  
>

`template <typename T> bool operator!=(const size<T>& a, const size<T>& b)`
>**@param a :**  
>**@param b :**  
>

`template <typename T> bool operator<=(const size<T>& a, const size<T>& b)`
>**@param a :**  
>**@param b :**  
>

`template <typename T> bool operator>=(const size<T>& a, const size<T>& b)`
>**@param a :**  
>**@param b :**  
>

`template <typename T> std::ostream& operator<<(std::ostream& os, const size<T>& psize)`
>**@param os :**  
>**@param psize :**  
>

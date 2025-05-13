#pragma once
#include <iostream>
#include <math.h>

using namespace std;

template <class T>
class TVectorIterator;

template <class T>
class TVector
{
protected:
    T* vector;
    int len;
    bool isNew;
public:
    TVector();
    TVector(int len_);
    TVector(const TVector& obj);
    TVector(TVector&& obj);
    ~TVector();

    virtual int GetLen();
    virtual void GetVector(T** vector_);

    virtual void SetLen(int len_);
    virtual void SetVector(T* vector_, int len_);

    virtual TVector operator+(const TVector<T>& obj);
    virtual TVector operator-(const TVector<T>& obj);
    virtual T operator*(const TVector<T>& obj);
    virtual TVector operator*(const T& mul);

    TVector<T> operator/(const TVector<T>& div) const;

    virtual TVector& operator=(const TVector<T>& obj);
    virtual TVector& operator=(TVector<T>&& obj);
    virtual bool operator==(const TVector<T>& obj);

    virtual T& operator[](int index) const;

    template <class T1>
    friend ostream& operator<<(ostream& o, TVector<T1>& v);
    template <class T1>
    friend istream& operator>>(istream& i, TVector<T1>& v);

    virtual void SortBubble();
    virtual void SortQuick();
    virtual void SortInsertion();
    virtual int Partition(int low, int high);
    virtual void QuickSortHelper(int low, int high);



    virtual T Normalization();
    virtual T FirstNorm();
    virtual T SecondNorm();
    virtual T InfinityNorm();

    virtual TVectorIterator<T> begin();
    virtual TVectorIterator<T> end();

    virtual void Rand();
};


template<class T>
class TVectorIterator
{
protected:
    TVector<T>& p;
    int index;
public:
    TVectorIterator(TVector<T>& vector, int index_ = 0);
    T& operator*();
    TVectorIterator<T>& operator++();
    TVectorIterator<T>& operator++(int);

    bool operator != (const TVectorIterator<T>& p);
};

template<class T>
inline TVector<T>::TVector()
{
  vector = nullptr;
  isNew = true;
  len = 0;
}

template<class T>
inline TVector<T>::TVector(int len_)
{
  if (len_ < 0) throw(0);
  else if (len_ == 0)
  {
    len = 0;
    vector = nullptr;
    return;
  }
  else len = len_;
  vector = new T[len]{ 0 };
  isNew = true;
}

template<class T>
inline TVector<T>::TVector(const TVector& obj):TVector<T>::TVector(obj.len)
{
  for (int i = 0; i < len; i++) vector[i] = obj.vector[i];
  isNew = true;
}

template<class T>
inline TVector<T>::TVector(TVector&& obj)
{
  vector = obj.vector;
  len = obj.len;
  isNew = obj.isNew;
  obj.len = 0;
  obj.vector = nullptr;
  obj.isNew = false;
}

template<class T>
inline TVector<T>::~TVector()
{
  if (vector != nullptr)
    if (isNew == true)
      delete[] vector;
  vector = nullptr;
  len = 0;
}

template<class T>
inline int TVector<T>::GetLen()
{
  return len;
}

template<class T>
inline void TVector<T>::GetVector(T** vector_)
{
  *vector_ = vector;
}

template<class T>
inline void TVector<T>::SetLen(int len_)
{
  if (len == len_) return;
  if (len_ < 0) throw -1;
  else if (len_ == 0)
  {
    if (vector != nullptr) delete[] vector;
    vector = nullptr;
    len = 0;
    return;
  }

  T* newVector = new T[len_];
  for (int i = 0; i < std::min(len, len_); i++) newVector[i] = vector[i];
  if (vector != nullptr) delete[] vector;

  vector = newVector;
  len = len_;
}

template<class T>
inline void TVector<T>::SetVector(T* vector_, int len_)
{
  if (len_ < 0) throw -1;
  if (vector != nullptr) delete[] vector;
  isNew = false;
  vector = vector_;
  len = len_;
}



template<class T>
inline TVector<T> TVector<T>::operator+(const TVector<T>& obj)
{
  if (len != obj.len) throw -1;

  TVector<T> res = len;
  for (int i = 0; i < len; i++)
  {
    res[i] = (*this)[i] + obj[i];
  }
  return res;
}

template<class T>
inline TVector<T> TVector<T>::operator-(const TVector<T>& obj)
{
  if (len != obj.len) throw -1;

  TVector<T> res = len;
  for (int i = 0; i < len; i++)
  {
    res[i] = (*this)[i] - obj[i];
  }
  return res;
}

template<class T>
inline T TVector<T>::operator*(const TVector<T>& obj) {
  if (len != obj.len) throw -1;
  T res = 0;
  for (int i = 0; i < len; i++)
  {
  res = res + (*this)[i] * obj[i];
  }
  return res;
}

template<class T>
inline TVector<T> TVector<T>::operator*(const T& mul)
{
  TVector<T> res = len;
  for (int i = 0; i < len; i++)
  {
    res[i] = res[i] * mul;
  }
  return res;
}

template <class T>
inline TVector<T> TVector<T>::operator/(const TVector<T>& div) const {
  if (len != div.len)
  {
    throw std::invalid_argument("Length is different");
  }
  TVector<T> res(len);
  for (int i = 0; i < len; ++i)
  {
    res.vector[i] = this->vector[i] / div.vector[i];
  }
  return res;
}

template<class T>
inline TVector<T>& TVector<T>::operator=(const TVector<T>& obj)
{
  if (this == &obj) return *this;
  if (len != obj.len)
  {
    delete[] vector;
    vector = new T[obj.len];
  }
  len = obj.len;
  for (int i = 0; i < len; i++)
    vector[i] = obj.vector[i];
  return *this;
}

template<class T>
inline TVector<T>& TVector<T>::operator=(TVector<T>&& obj)
{
  if (this == &obj) return *this;
  if (vector != nullptr) delete[] vector;
  vector = obj.vector;
  len = obj.len;
  obj.vector = nullptr;
  obj.len = 0;
  return *this;
}

template<class T>
inline bool TVector<T>::operator==(const TVector<T>& obj)
{
  if (len != obj.len) return false;
  for (int i = 0; i < len; ++i)
  {
    if (vector[i] != obj.vector[i])
    {
      return false;
    }
  }
  return true;
}

template<class T>
inline T& TVector<T>::operator[](int index) const
{
  if (vector == nullptr) throw -1;
  if (index < 0 || index > len) throw -1;
  return vector[index];
}

template<class T>
inline void TVector<T>::SortBubble()
{
  int temp;
  for (int s = 0; s < len; ++s)
  {
    for (int j = 0; j < len - 1; ++j)
    {
      if (vector[j] > vector[j + 1])
      {
        temp = vector[j];
        vector[j] = vector[j + 1];
        vector[j + 1] = temp;
      }
    }
  }
}

template<class T>
inline void TVector<T>::SortQuick()
{
  if (len <= 1) return;
  QuickSortHelper(0, len - 1);
}

template<class T>
inline void TVector<T>::SortInsertion() {
  for (int i = 1; i < len; ++i) {
    T key = vector[i];
    int j = i - 1;

    while (j >= 0 && vector[j] > key) {
      vector[j + 1] = vector[j];
      j--;
    }
    vector[j + 1] = key;

  }
}
template<class T>
inline void TVector<T>::QuickSortHelper(int low, int high)
{
  if (low < high) {
    int pi = Partition(low, high);
    QuickSortHelper(low, pi - 1);
    QuickSortHelper(pi + 1, high);
  }
}

template<class T>
inline int TVector<T>::Partition(int low, int high) {
  T pivot = vector[high];
  int i = low - 1;

  for (int j = low; j <= high - 1; j++) {
    if (vector[j] <= pivot) {
      i++;
      std::swap(vector[i], vector[j]);
    }
  }
  std::swap(vector[i + 1], vector[high]);
  return i + 1;
}


template<class T>
inline T TVector<T>::Normalization()
{
  double norm = 0.0;
  for (int i = 0; i < len; ++i)
  {
    norm += pow(vector[i], 2);
  }
  double newNorm = sqrt(norm);
  if (newNorm == 0.0)
  {
    printf("Can't normalize vector! Second norm is 0!");
    return 1;
  }
  for (int i = 0; i < len; ++i){
    vector[i] /= norm;
  }
  return 0;
}

template<class T>
inline T TVector<T>::FirstNorm()
{
  int norm = 0;
  for (int i = 0; i < len; ++i)
  {
    norm += abs(vector[i]);
  }
  return norm;
}

template<class T>
inline T TVector<T>::SecondNorm()
{
  double norm = 0.0;
  for (int i = 0; i < len; ++i)
  {
    norm += pow(vector[i], 2);
  }
  return sqrt(norm);
}

template<class T>
inline T TVector<T>::InfinityNorm()
{
  int x = 0;
  for (int i = 0; i < len; ++i)
    if (abs(vector[i]) > x)
      x = abs(vector[i]);
  return x;
}

template<class T>
inline TVectorIterator<T> TVector<T>::begin()
{
  return TVectorIterator<T>(*this, 0);
}

template<class T>
inline TVectorIterator<T> TVector<T>::end()
{
  return TVectorIterator<T>(*this, len);
}

template<class T>
inline void TVector<T>::Rand()
{
  for (int i = 0; i < len; i++) vector[i] = rand();
}

template<class T1>
inline ostream& operator<<(ostream& o, TVector<T1>& v)
{
  for (int i = 0; i < v.GetLen(); i++)
    o << v[i] << "\t";
  return o;
}

template<class T1>
inline istream& operator>>(istream& i, TVector<T1>& v)
{
  return i;
}

template<class T>
inline TVectorIterator<T>::TVectorIterator(TVector<T>& vector, int index_):
        p(vector), index(index_)
{
}

template<class T>
inline T& TVectorIterator<T>::operator*()
{
  return p[index];
}

template<class T>
inline TVectorIterator<T>& TVectorIterator<T>::operator++()
{
  index++;
  return *this;
}

template<class T>
inline TVectorIterator<T>& TVectorIterator<T>::operator++(int)
{
  index++;
  return* this;
}

template<class T>
inline bool TVectorIterator<T>::operator!=(const TVectorIterator<T>& p)
{
  return index != p.index;
}
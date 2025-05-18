#pragma once
#include "VectorClass.h"
#include <math.h>
#include <fstream>

#define _CRT_SECURE_NO_WARNINGS


using namespace std;
template<class T>
class TMatrix : public TVector<TVector<T>>
{
public:
  TMatrix();
  TMatrix(int len_);
  TMatrix(int l, int h);
  TMatrix(const TMatrix& obj);
  TMatrix(TMatrix&& obj);
  ~TMatrix();

  int GetRows() const;
  int GetColumns() const;

  TMatrix<T> operator+(const TMatrix<T>& obj);
  TMatrix<T> operator-(const TMatrix<T>& obj);
  TMatrix<T> operator*(const TMatrix<T>& obj);
  TMatrix<T> operator*(const T mul);
  TMatrix<T> operator/(const T div);

  TMatrix<T>& operator=(const TMatrix<T>& obj);
  TMatrix<T>& operator=(TMatrix<T>&& obj);
  bool operator==(const TMatrix<T>& obj);

  template <class O>
  friend ostream& operator<<(ostream& o, TMatrix<O>& v);
  template <class I>
  friend istream& operator>>(istream& i, TMatrix<I>& v);

  void RandMatrix();
  virtual void SaveToFile(const char* path = "./data.txt");
  virtual void ReadFromFile(const char* path = "./data.txt");

  void PrintSystem(TVector<double> & rpart);
  void Gauss(TVector<double> &equals);

  int ValueCount(const T looking_for); // Количество вхождений доп номер 2
  TMatrix AllOccurrences(const T value); // Все вхождения значения доп номер 3
};

template<class T>
inline TMatrix<T>::TMatrix() : TVector<TVector<T>>::TVector() {}

template<class T>
inline TMatrix<T>::TMatrix(int rows, int columns) : TVector<TVector<T>>::TVector(rows)
{
  for (int i = 0; i < rows; ++i)
  {
    (*this)[i] = TVector<T>(columns);
  }
}

template<class T>
TMatrix<T>::TMatrix(const TMatrix<T> &obj) : TVector<TVector<T>>::TVector(obj.GetLen())
{
  int rows = obj.GetLen();
  int columns = obj.GetColumns();
  for (int i = 0; i < rows; i++)
  {
    this->vector[i] = TVector<T>(columns);
    for (int j = 0; j < columns; j++)
    {
      this->vector[i][j] = obj.vector[i][j];
    }
  }
}

template<class T>
TMatrix<T>::TMatrix(TMatrix<T> &&obj)
{
  this->len = obj.len;
  this->vector = obj.vector;
  this->isNew = obj.isNew;
  obj.len = 0;
  obj.vector = nullptr;
  obj.isNew = false;
}

template<class T>
inline TMatrix<T>::~TMatrix() {}

template<class T>
inline int TMatrix<T>::GetRows() const
{
  return (*this).GetLen();
}

template<class T>
inline int TMatrix<T>::GetColumns() const
{
  if (GetRows() == 0) return 0;
  return (*this)[0].GetLen();
}

template<class T>
inline TMatrix<T> TMatrix<T>::operator+(const TMatrix& obj)
{
  int row = GetRows();
  int col = GetColumns();
  if (row != obj.GetRows() || col != obj.GetColumns()) throw "Can't plus";
  TMatrix<T> res(row, col);
  for (int i = 0; i < row; ++i)
    for (int j = 0; j < col; ++j) res.vector[i][j] = (*this)[i][j] + obj[i][j];
  return res;
}

template<class T>
inline TMatrix<T> TMatrix<T>::operator-(const TMatrix& obj)
{
  int row = GetRows();
  int col = GetColumns();
  if (row != obj.GetRows() || col != obj.GetColumns()) throw "Can't minus";
  TMatrix<T> res(row, col);
  for (int i = 0; i < row; ++i)
    for (int j = 0; j < col; ++j) res.vector[i][j] = (*this)[i][j] - obj[i][j];
  return res;
}

// Умножение на число
template<class T>
inline TMatrix<T> TMatrix<T>::operator*(const T mul)
{
  int row = GetRows();
  int col = GetColumns();
  TMatrix<T> res(row, col);
  if (mul == 0) return res;
  for (int i = 0; i < row; ++i)
    for (int j = 0; j < col; ++j) res.vector[i][j] = (*this)[i][j] * mul;
  return res;
}

// Деление на число
template<class T>
inline TMatrix<T> TMatrix<T>::operator/(const T div)
{
  int row = GetRows();
  int col = GetColumns();
  if (div == 0) throw "Cannot divide by zero!!!";
  TMatrix<T> res(row, col);
  for (int i = 0; i < row; ++i)
    for (int j = 0; j < col; ++j) res.vector[i][j] = (*this)[i][j] / div;
  return res;
}

// Умножение матриц
template<class T>
inline TMatrix<T> TMatrix<T>::operator*(const TMatrix& obj)
{
  int row = GetRows();
  int col = GetColumns();
  if (row != obj.GetColumns()) throw "Cannot multiply";
  TMatrix<T> res(row, col);
  for (int i = 0; i < row; ++i)
    for (int j = 0; j < obj.GetColumns(); ++j)
    {
      res.vector[i][j] = 0;
      for (int k = 0; k < col; ++k) res.vector[i][j] += (*this)[i][k] * obj[k][j];
    }
  return res;
}

// Присваивает только матрицы одинакогого размера
template<class T>
inline TMatrix<T>& TMatrix<T>::operator=(const TMatrix<T>& obj)
{
  int row = GetRows();
  int col = GetColumns();
  if (row != obj.GetRows() || col != obj.GetColumns()) throw "Cannot equal those";
  for (int i = 0; i < row; ++i)
    for (int j = 0; j < col; ++j) (*this)[i][j] = obj[i][j];
  return *this;
}
// Посмотреть где отличия
template<class T>
inline TMatrix<T>& TMatrix<T>::operator=(TMatrix<T>&& obj)
{
  *this = obj;
  return *this;
}

template<class T>
inline bool TMatrix<T>::operator==(const TMatrix<T>& obj)
{
  int row = GetRows();
  int col = GetColumns();
  if (row != obj.GetRows() || col != obj.GetColumns()) throw "Cannot equal";
  bool flag = true;
  for (int i = 0; i < row; ++i)
    for (int j = 0; j < col; ++j) if ((*this)[i][j] != obj[i][j]) flag = false;
  return flag;
}
// Output
template <class O>
inline ostream& operator<<(ostream& o, TMatrix<O>& p)
{
  int row = p.GetRows();
  int col = p.GetColumns();
  for (int i = 0; i < row; ++i)
  {
    for (int j = 0; j < col; ++j)
    {
      o << p[i][j] << " ";
    }
    o << endl;
  }
  return o;
}
// Input
template <class I>
inline istream& operator>>(istream& is, TMatrix<I>& p)
{
  int row = p.GetRows();
  int col = p.GetColumns();
  for (int i = 0; i < row; i++)
  {
    for (int j = 0; j < col; j++)
    {
      cout <<"a[" << i << "][" << j << "]:  ";
      is >> p[i][j];
      cout << endl;
    }
  }
  return is;
}

template<class T>
inline void TMatrix<T>::RandMatrix()
{
  int size1 = (*this).GetRows();
  int size2 = (*this).GetColumns();
  for (int i = 0; i < size1; ++i)
  {
    for (int j = 0; i < size2; ++j)
      (*this)[i][j] = rand();
  }
}

template<class T>
inline void TMatrix<T>::SaveToFile(const char* path)
{
  ofstream FileLoc(path);
  if (!FileLoc.is_open()) throw "File dosen't work";
  if (FileLoc.is_open())
  {
    FileLoc << GetRows() << "\n";
    FileLoc << GetColumns() << "\n";
    for (int i = 0; i < GetRows(); ++i)
    {
      for (int j = 0; j < GetColumns(); ++j)
        FileLoc << (*this)[i][j] << "\n";
    }
  }
  FileLoc.close();
}

template<class T>
inline void TMatrix<T>::ReadFromFile(const char* path)
{
  ifstream FileLoc(path);
  if (!FileLoc.is_open()) throw "File dosen't work";
  if (FileLoc.is_open())
  {
    int row = 0, col = 0;
    FileLoc >> row;
    FileLoc >> col;
    if (row != GetRows() || col != GetColumns()) throw "Cannot read, the size is different";
    for (int i = 0; i < row; ++i)
      for (int j = 0; j < col; ++j)
        FileLoc >> (*this)[i][j];
  }
  FileLoc.close();
}

template <class T>
inline int TMatrix<T>::ValueCount(const T looking_for)
{
  int row = GetRows();
  int col = GetColumns();
  int ans = 0;
  for (int i = 0; i < row; ++i)
    for (int j = 0; j < col; ++j) if ((*this)[i][j] == looking_for) ans++;
  return ans;
}

template <class T>
inline TMatrix<T> TMatrix<T>::AllOccurrences(const T value)
{
  int row = GetRows();
  int col = GetColumns();
  int count = ValueCount(value);
  TMatrix<T> ans(count, 2);
  int n = 0;
  for (int i = 0; i < row; ++i)
    for (int j = 0; j < col; ++j)
      if ((*this)[i][j] == value)
      {
        ans[n][0] = i;
        ans[n++][1] = j;
      }
  return ans;
}

template <class T>
inline void TMatrix<T>::PrintSystem(TVector<double> & rpart)
{
  int n = rpart.GetLen();
  if (n != GetRows()) throw "Length doesn't match!";
  for (int i = 0; i < n; ++i)
  {
    for (int j = 0; j < n; ++j)
    {
      cout << (*this)[i][j] << "*x" << j+1;
      if (j < n - 1) cout << " + ";
    }
    cout << " = " << rpart[i] << endl;
    }
}

template <class T>
inline void TMatrix<T>::Gauss(TVector<double> &equals)
{
  int n = equals.GetLen();
  TVector<double> x(n);
  double max;
  int index;
  const double accuracy = pow(10,-5);
  double temp;
  int k = 0;
  while (k < n) // к - идет по диагонали матрицы слева сверху
  {
    // Поиск строки с максимальным элементом в нынешнем столбце
    max = abs((*this)[k][k]);
    index = k;
    for (int i = k + 1; i < n; i++) if (abs((*this)[i][k]) > max)
      {
        max = abs((*this)[i][k]);
        index = i; // индекс строки с макс. эелеметном
      }

    // Перестановка строк
    if (max < accuracy)
    {
      // нулевой столбец
      cout << "No solution, det(A) = 0, zero column index: " << index << endl;
    }

    // перенос строки с самым большим элементом наверх
    for (int j = 0; j < n; j++)
    {
      temp = (*this)[k][j];
      (*this)[k][j] = (*this)[index][j];
      (*this)[index][j] = temp;
    }
    // перенос значений:
    temp = equals[k];
    equals[k] = equals[index];
    equals[index] = temp;

    // Решение матрицы
    for (int i = k; i < n; i++)
    {
      temp = (*this)[i][k];
      if (abs(temp) < accuracy) continue; // упустить погрешность
      for (int j = k; j < n; j++) (*this)[i][j] /= temp;
      equals[i] /= temp;
      if (i == k)  continue; // пропустить себя же
      for (int j = 0; j < n; j++) (*this)[i][j] -= (*this)[k][j];
      equals[i] -= equals[k];
    }
    k++;
  }

  // обратная подстановка
  for (k = n - 1; k > -1; k--)
  {
    x[k] = equals[k];
    for (int i = 0; i < k; i++)
      equals[i] -= (*this)[i][k] * x[k];
  }
}
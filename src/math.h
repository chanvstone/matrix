#ifndef _MATRIX_
#define _MATRIX_ 1

#include <vector>
#include <assert.h>
#include <initializer_list>
#include <functional>

namespace math
{
    using elem_type = double;

    class matrix
    {
    public:
        using elem_type = double;
        using size_type = unsigned long;

    public:
        // 构造函数
        matrix(size_type row,size_type column);
        matrix(size_type row, size_type column, elem_type value);
        matrix(std::initializer_list<std::initializer_list<elem_type>> init);
        matrix(const std::vector<std::vector<elem_type>> &v);
        matrix(const matrix &m);
        matrix(matrix &&m);
        matrix &operator=(const matrix &m);
        ~matrix();

        // 接口
        size_type size() const;
        size_type rows() const;
        size_type columns() const;
        elem_type at(size_type x, size_type y) const;
        elem_type &at(size_type x, size_type y);
        std::vector<elem_type> row(size_type x) const;
        std::vector<elem_type> column(size_type y) const;

        // 计算 返回右值
        matrix operator+(const matrix &m) const;
        matrix operator-(const matrix &m) const;
        matrix operator*(const elem_type &e) const;
        matrix operator/(const elem_type &e) const;
        matrix operator*(const matrix &m) const;

        // 计算 直接修改
        void operator+=(const matrix &m);
        void operator-=(const matrix &m);
        void operator*=(const elem_type &e);
        void operator/=(const elem_type &e);
        void operator*=(const matrix &m);

        // 比较
        bool operator==(const matrix &m) const;
        bool can_plus(const matrix &m) const;
        bool can_multiply(const matrix &m) const;

        // 遍历
        void traverse(std::function<void(size_type x, size_type y, const elem_type &e)> operate) const;
        void traverse(std::function<void(size_type x, size_type y, elem_type &e)> operate);

    private:
        elem_type *_data;
        size_type _row, _column;
    };
}

#endif // _MATRIX_
#include "math.h"
namespace math
{
    // 构造函数
    matrix::matrix(size_type row, size_type column) : _row(row), _column(column), _data(new elem_type[row * column])
    {
    }
    matrix::matrix(size_type row, size_type column, elem_type value) : _row(row), _column(column), _data(new elem_type[row * column])
    {
        traverse([value](size_type x, size_type y, elem_type &e) -> void
                 { e = value; });
    }
    matrix::matrix(std::initializer_list<std::initializer_list<elem_type>> init) : _row(0), _column(0)
    {
        _row = init.size();
        _column = _row > 0 ? init.begin()->size() : 0;
        assert(_row > 0 && _column > 0);
        _data = new elem_type[_row * _column];
        size_type index = 0;
        for (auto x = init.begin(); x != init.end(); ++x)
        {
            assert(x->size() == _column);
            for (auto y = x->begin(); y != x->end(); ++y)
            {
                _data[index++] = *y;
            }
        }
    }
    matrix::matrix(const std::vector<std::vector<elem_type>> &v) : _row(v.size()), _column(v.size() > 0 ? v.front().size() : 0)
    {
        _data = new elem_type[_row * _column];
        traverse([&v](size_type x, size_type y, elem_type &e) -> void
                 { e = v[x][y]; });
    }
    matrix::matrix(const matrix &m) : _row(m._row), _column(m._column), _data(new elem_type[m._row * m._column])
    {
        traverse([&m](size_type x, size_type y, elem_type &e) -> void
                 { e = m.at(x, y); });
    }
    matrix::matrix(matrix &&m) : _row(m._row), _column(_column), _data(m._data)
    {
        m._data = nullptr;
    }
    matrix &matrix::operator=(const matrix &m)
    {
        delete[] _data;
        _row = m._row;
        _column = m._column;
        _data = new elem_type[_row * _column];
        traverse([&m](size_type x, size_type y, elem_type &e) -> void
                 { e = m.at(x, y); });
        return *this;
    }
    matrix::~matrix()
    {
        delete[] _data;
    }

    // 接口
    matrix::size_type matrix::size() const
    {
        return _row * _column;
    }
    matrix::size_type matrix::rows() const
    {
        return _row;
    }
    matrix::size_type matrix::columns() const
    {
        return _column;
    }
    elem_type matrix::at(size_type x, size_type y) const
    {
        return _data[_row * x + y];
    }
    elem_type &matrix::at(size_type x, size_type y)
    {
        return _data[_row * x + y];
    }
    std::vector<elem_type> matrix::row(size_type x) const
    {
        std::vector<elem_type> v;
        for (int y = 0; y < _column; ++y)
        {
            v.push_back(at(x, y));
        }
        return v;
    }
    std::vector<elem_type> matrix::column(size_type y) const
    {
        std::vector<elem_type> v;
        for (int x = 0; x < _row; ++x)
        {
            v.push_back(at(x, y));
        }
        return v;
    }

    // 计算
    matrix matrix::operator+(const matrix &m) const
    {
        assert(can_plus(m));
        matrix sum(_row, _column);
        sum.traverse([this, &m](size_type x, size_type y, elem_type &e) -> void
                     { e = this->at(x, y) + m.at(x, y); });
        return sum;
    }
    matrix matrix::operator-(const matrix &m) const
    {
        assert(can_plus(m));
        matrix difference(_row, _column);
        difference.traverse([this, &m](size_type x, size_type y, elem_type &e) -> void
                            { e = this->at(x, y) - m.at(x, y); });
        return difference;
    }
    matrix matrix::operator*(const elem_type &e) const
    {
        matrix product(_row, _column);
        product.traverse([this, &e](size_type x, size_type y, elem_type &pe) -> void
                         { pe = this->at(x, y) * e; });
        return product;
    }
    matrix matrix::operator/(const elem_type &e) const
    {
        matrix quotient(_row, _column);
        quotient.traverse([this, e](size_type x, size_type y, elem_type &qe) -> void
                          { qe = this->at(x, y) / e; });
        return quotient;
    }
    matrix matrix::operator*(const matrix &m) const
    {
        can_multiply(m);
        matrix product(_row, m._column);

        auto sum = [](const std::vector<elem_type> &v1, const std::vector<elem_type> &v2) -> elem_type
        {
            elem_type result = 0;
            for (auto i1 = v1.cbegin(), i2 = v2.cbegin(); i1 != v1.cend() && i2 != v2.cend(); ++i1, ++i2)
            {
                result += ((*i1) * (*i2));
            }
            return result;
        };
        for (int x = 0; x < product._row; ++x)
        {
            for (int y = 0; y < product._column; ++y)
            {
                product.at(x, y) = sum(row(x), m.column(y));
            }
        }
        return product;
    }

    // 计算
    void matrix::operator+=(const matrix &m)
    {
        can_plus(m);
        traverse([&m](size_type x, size_type y, elem_type &e) -> void
                 { e += m.at(x, y); });
    }
    void matrix::operator-=(const matrix &m)
    {
        can_plus(m);
        traverse([&m](size_type x, size_type y, elem_type &e) -> void
                 { e -= m.at(x, y); });
    }
    void matrix::operator*=(const elem_type &e)
    {
        traverse([&e](size_type x, size_type y, elem_type &te) -> void
                 { te *= e; });
    }
    void matrix::operator/=(const elem_type &e)
    {
        traverse([&e](size_type x, size_type y, elem_type &te) -> void
                 { te /= e; });
    }
    void matrix::operator*=(const matrix &m)
    {
        matrix product = (*this) * m;
        *this = product;
    }

    // 比较
    bool
    matrix::operator==(const matrix &m) const
    {
        if (!can_plus(m))
        {
            return false;
        }
        bool result = true;
        traverse([&result, &m](size_type x, size_type y, const elem_type &e) -> void
                 {
            if(e!=m.at(x,y)){result=false;} });
        return result;
    }
    bool matrix::can_plus(const matrix &m) const
    {
        return _row == m._row && _column == m._column;
    }
    bool matrix::can_multiply(const matrix &m) const
    {
        return _column == m._row;
    }

    // 遍历
    void matrix::traverse(std::function<void(size_type x, size_type y, const elem_type &e)> operate) const
    {
        for (size_type x = 0; x < _row; ++x)
            for (size_type y = 0; y < _column; ++y)
            {
                operate(x, y, at(x, y));
            }
    }
    void matrix::traverse(std::function<void(size_type x, size_type y, elem_type &e)> operate)
    {
        for (size_type x = 0; x < _row; ++x)
            for (size_type y = 0; y < _column; ++y)
            {
                operate(x, y, at(x, y));
            }
    }
}
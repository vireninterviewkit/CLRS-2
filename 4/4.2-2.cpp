#include <cassert>
#include <chrono>
#include <initializer_list>
#include <iostream>
#include <numeric>
#include <type_traits>
#include <valarray>

template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <Arithmetic T, size_t R, size_t C>
struct Matrix;

template <Arithmetic T, size_t R, size_t C>
struct MatrixView {
    std::gslice_array<T> data_view;
    explicit MatrixView(std::gslice_array<T> data_view) : data_view {data_view} {}

    friend struct Matrix<T, R, C>;

    template <Arithmetic U>
    MatrixView& operator=(const Matrix<U, R, C>&);

    template <size_t RV, size_t CV>
    MatrixView<T, RV, CV> submatrix(size_t r1, size_t c1, size_t r2, size_t c2) {
        assert(r1 <= r2 && c1 <= c2 && r2 < R && c2 < C);
        assert(RV == r2 - r1 + 1 && CV == c2 - c1 + 1);
        std::valarray<T> data(data_view);
        return MatrixView<T, RV, CV>(data[std::gslice(r1 * C + c1,{r2 - r1 + 1, c2 - c1 + 1}, {C, 1})]);
    }
};

template <Arithmetic T, size_t R, size_t C>
struct Matrix {
    std::valarray<T> data;
    Matrix() : data(R * C) {}
    Matrix(std::initializer_list<T> il) : data(il) {
        assert(il.size() == R * C);
    }

    template <Arithmetic U>
    Matrix (const MatrixView<U, R, C>&);

    template <Arithmetic U>
    Matrix& operator=(const MatrixView<U, R, C>&);

    auto begin() { return std::begin(data);}
    auto begin() const { return std::begin(data);}
    auto end() { return std::end(data);}
    auto end() const { return std::end(data);}

    T& operator()(size_t r, size_t c) {return data[r * C + c];}
    const T& operator()(size_t r, size_t c) const {return data[r * C + c];}

    Matrix& operator+=(const T& val);
    Matrix& operator-=(const T& val);
    Matrix& operator*=(const T& val);
    Matrix& operator/=(const T& val);
    Matrix& operator%=(const T& val);

    template <Arithmetic U>
    Matrix<T, R, C>& operator+=(const Matrix<U, R, C>& rhs);
    template <Arithmetic U>
    Matrix<T, R, C>& operator+=(const MatrixView<U, R, C>& rhs);
    template <Arithmetic U>
    Matrix<T, R, C>& operator-=(const Matrix<U, R, C>& rhs);
    template <Arithmetic U>
    Matrix<T, R, C>& operator-=(const MatrixView<U, R, C>& rhs);

    template <size_t RV, size_t CV>
    MatrixView<T, RV, CV> submatrix(size_t r1, size_t c1, size_t r2, size_t c2) {
        assert(r1 <= r2 && c1 <= c2 && r2 < R && c2 < C);
        assert(RV == r2 - r1 + 1 && CV == c2 - c1 + 1);
        return MatrixView<T, RV, CV>(data[std::gslice(r1 * C + c1,{r2 - r1 + 1, c2 - c1 + 1}, {C, 1})]);
    }
};

template <Arithmetic T, size_t R, size_t C>
template <Arithmetic U>
MatrixView<T, R, C>& MatrixView<T, R, C>::operator=(const Matrix<U, R, C>& mat) {
    data_view = mat.data[std::gslice(0, {R, C}, {C, 1})];
    return *this;
}

template <Arithmetic T, size_t R, size_t C>
template <Arithmetic U>
Matrix<T, R, C>::Matrix(const MatrixView<U, R, C>& view) : data (view.data_view) {
}

template <Arithmetic T, size_t R, size_t C>
template <Arithmetic U>
Matrix<T, R, C>& Matrix<T, R, C>::operator=(const MatrixView<U, R, C>& view) {
    data = std::valarray<T>(view.data_view);
    return *this;
}

template <Arithmetic T, size_t R, size_t C>
Matrix<T, R, C>& Matrix<T, R, C>::operator+=(const T& val) {
    data += val;
    return *this;
}

template <Arithmetic T, size_t R, size_t C>
Matrix<T, R, C> operator+(const Matrix<T, R, C>& m, const T& val) {
    Matrix<T, R, C> res = m;
    res += val;
    return res;
}

template <Arithmetic T, size_t R, size_t C>
Matrix<T, R, C> operator+(const MatrixView<T, R, C>& m, const T& val) {
    Matrix<T, R, C> res = m;
    res += val;
    return res;
}

template <Arithmetic T, size_t R, size_t C>
Matrix<T, R, C>& Matrix<T, R, C>::operator-=(const T& val) {
    data -= val;
    return *this;
}

template <Arithmetic T, size_t R, size_t C>
Matrix<T, R, C> operator-(const Matrix<T, R, C>& m, const T& val) {
    Matrix<T, R, C> res = m;
    res -= val;
    return res;
}

template <Arithmetic T, size_t R, size_t C>
Matrix<T, R, C> operator-(const MatrixView<T, R, C>& m, const T& val) {
    Matrix<T, R, C> res = m;
    res -= val;
    return res;
}

template <Arithmetic T, size_t R, size_t C>
Matrix<T, R, C>& Matrix<T, R, C>::operator*=(const T& val) {
    data *= val;
    return *this;
}

template <Arithmetic T, size_t R, size_t C>
Matrix<T, R, C> operator*(const Matrix<T, R, C>& m, const T& val) {
    Matrix<T, R, C> res = m;
    res *= val;
    return res;
}

template <Arithmetic T, size_t R, size_t C>
Matrix<T, R, C> operator*(const MatrixView<T, R, C>& m, const T& val) {
    Matrix<T, R, C> res = m;
    res *= val;
    return res;
}

template <Arithmetic T, size_t R, size_t C>
Matrix<T, R, C>& Matrix<T, R, C>::operator/=(const T& val) {
    data /= val;
    return *this;
}

template <Arithmetic T, size_t R, size_t C>
Matrix<T, R, C> operator/(const Matrix<T, R, C>& m, const T& val) {
    Matrix<T, R, C> res = m;
    res /= val;
    return res;
}

template <Arithmetic T, size_t R, size_t C>
Matrix<T, R, C> operator/(const MatrixView<T, R, C>& m, const T& val) {
    Matrix<T, R, C> res = m;
    res /= val;
    return res;
}

template <Arithmetic T, size_t R, size_t C>
Matrix<T, R, C>& Matrix<T, R, C>::operator%=(const T& val) {
    data %= val;
    return *this;
}

template <Arithmetic T, size_t R, size_t C>
Matrix<T, R, C> operator%(const Matrix<T, R, C>& m, const T& val) {
    Matrix<T, R, C> res = m;
    res %= val;
    return res;
}

template <Arithmetic T, size_t R, size_t C>
Matrix<T, R, C> operator%(const MatrixView<T, R, C>& m, const T& val) {
    Matrix<T, R, C> res = m;
    res %= val;
    return res;
}

template <Arithmetic T, size_t R, size_t C>
template <Arithmetic U>
Matrix<T, R, C>& Matrix<T, R, C>::operator+=(const Matrix<U, R, C>& rhs) {
    data += rhs.data;
    return *this;
}

template <Arithmetic T, size_t R, size_t C>
template <Arithmetic U>
Matrix<T, R, C>& Matrix<T, R, C>::operator+=(const MatrixView<U, R, C>& rhs) {
    data += std::valarray<T>(rhs.data_view);
    return *this;
}

template <Arithmetic T1, Arithmetic T2, size_t R, size_t C, Arithmetic T3 = std::common_type_t<T1, T2>>
Matrix<T3, R, C> operator+(const Matrix<T1, R, C>& m1, const Matrix<T2, R, C>& m2) {
    Matrix<T3, R, C> res = m1;
    res += m2;
    return res;
}

template <Arithmetic T1, Arithmetic T2, size_t R, size_t C, Arithmetic T3 = std::common_type_t<T1, T2>>
Matrix<T3, R, C> operator+(const Matrix<T1, R, C>& m1, const MatrixView<T2, R, C>& m2) {
    Matrix<T3, R, C> res = m1;
    res += m2;
    return res;
}

template <Arithmetic T1, Arithmetic T2, size_t R, size_t C, Arithmetic T3 = std::common_type_t<T1, T2>>
Matrix<T3, R, C> operator+(const MatrixView<T1, R, C>& m1, const Matrix<T2, R, C>& m2) {
    Matrix<T3, R, C> res = m1;
    res += m2;
    return res;
}

template <Arithmetic T1, Arithmetic T2, size_t R, size_t C, Arithmetic T3 = std::common_type_t<T1, T2>>
Matrix<T3, R, C> operator+(const MatrixView<T1, R, C>& m1, const MatrixView<T2, R, C>& m2) {
    Matrix<T3, R, C> res = m1;
    res += m2;
    return res;
}

template <Arithmetic T, size_t R, size_t C>
template <Arithmetic U>
Matrix<T, R, C>& Matrix<T, R, C>::operator-=(const Matrix<U, R, C>& rhs) {
    data -= rhs.data;
    return *this;
}

template <Arithmetic T, size_t R, size_t C>
template <Arithmetic U>
Matrix<T, R, C>& Matrix<T, R, C>::operator-=(const MatrixView<U, R, C>& rhs) {
    data -= std::valarray<T>(rhs.data_view);
    return *this;
}

template <Arithmetic T1, Arithmetic T2, size_t R, size_t C, Arithmetic T3 = std::common_type_t<T1, T2>>
Matrix<T3, R, C> operator-(const Matrix<T1, R, C>& m1, const Matrix<T2, R, C>& m2) {
    Matrix<T3, R, C> res = m1;
    res -= m2;
    return res;
}

template <Arithmetic T1, Arithmetic T2, size_t R, size_t C, Arithmetic T3 = std::common_type_t<T1, T2>>
Matrix<T3, R, C> operator-(const MatrixView<T1, R, C>& m1, const Matrix<T2, R, C>& m2) {
    Matrix<T3, R, C> res = m1;
    res -= m2;
    return res;
}

template <Arithmetic T1, Arithmetic T2, size_t R, size_t C, Arithmetic T3 = std::common_type_t<T1, T2>>
Matrix<T3, R, C> operator-(const Matrix<T1, R, C>& m1, const MatrixView<T2, R, C>& m2) {
    Matrix<T3, R, C> res = m1;
    res -= m2;
    return res;
}

template <Arithmetic T1, Arithmetic T2, size_t R, size_t C, Arithmetic T3 = std::common_type_t<T1, T2>>
Matrix<T3, R, C> operator-(const MatrixView<T1, R, C>& m1, const MatrixView<T2, R, C>& m2) {
    Matrix<T3, R, C> res = m1;
    res -= m2;
    return res;
}

template <Arithmetic T1, Arithmetic T2, size_t M, size_t K, size_t N, Arithmetic T3 = std::common_type_t<T1, T2>>
Matrix<T3, M, N> operator*(const Matrix<T1, M, K>& m1, const Matrix<T2, K, N>& m2) {
    Matrix<T3, M, N> m3;
    for (size_t m = 0; m < M; m++) {
        for (size_t n = 0; n < N; n++) {
            for (size_t k = 0; k < K; k++) {
                m3(m, n) += m1(m, k) * m2(k, n);
            }
        }
    }
    return m3;
}

template <Arithmetic T, size_t R, size_t C>
std::ostream& operator<<(std::ostream& os, const Matrix<T, R, C>& m) {
    for (size_t r = 0; r < R; r++) {
        for (size_t c = 0; c < C; c++) {
            os << m(r, c) << ' ';
        }
        os << '\n';
    }
    return os;
}

template <Arithmetic T, size_t N>
Matrix<T, N, N> Strassen(Matrix<T, N, N>& A, Matrix<T, N, N>& B) {
    static_assert((N & (N - 1)) == 0);
    Matrix<T, N, N> C;
    constexpr size_t H = N / 2;
    Matrix<T, H, H> A11 = A.template submatrix<H, H>(0, 0, H - 1, H - 1);
    auto A12 = A.template submatrix<H, H>(0, H, H - 1, N - 1);
    auto A21 = A.template submatrix<H, H>(H, 0, N - 1, H - 1);
    Matrix<T, H, H> A22 = A.template submatrix<H, H>(H, H, N - 1, N - 1);
    Matrix<T, H, H> B11 = B.template submatrix<H, H>(0, 0, H - 1, H - 1);
    auto B12 = B.template submatrix<H, H>(0, H, H - 1, N - 1);
    auto B21 = B.template submatrix<H, H>(H, 0, N - 1, H - 1);
    Matrix<T, H, H> B22 = B.template submatrix<H, H>(H, H, N - 1, N - 1);
    auto C11 = C.template submatrix<H, H>(0, 0, H - 1, H - 1);
    auto C12 = C.template submatrix<H, H>(0, H, H - 1, N - 1);
    auto C21 = C.template submatrix<H, H>(H, 0, N - 1, H - 1);
    auto C22 = C.template submatrix<H, H>(H, H, N - 1, N - 1);
    auto S1 = B12 - B22;
    auto S2 = A11 + A12;
    auto S3 = A21 + A22;
    auto S4 = B21 - B11;
    auto S5 = A11 + A22;
    auto S6 = B11 + B22;
    auto S7 = A12 - A22;
    auto S8 = B21 + B22;
    auto S9 = A11 - A21;
    auto S10 = B11 + B12;
    auto P1 = Strassen(A11, S1);
    auto P2 = Strassen(S2, B22);
    auto P3 = Strassen(S3, B11);
    auto P4 = Strassen(A22, S4);
    auto P5 = Strassen(S5, S6);
    auto P6 = Strassen(S7, S8);
    auto P7 = Strassen(S9, S10);
    C11 = P5 + P4 - P2 + P6;
    C12 = P1 + P2;
    C21 = P3 + P4;
    C22 = P5 + P1 - P3 - P7;
    return C;
}

template <Arithmetic T>
Matrix<T, 1, 1> Strassen(Matrix<T, 1, 1>& A, Matrix<T, 1, 1>& B) {
    return A * B;
}

int main() {
    constexpr size_t N = 1u << 6u;
    Matrix<int, N, N> m1, m2;
    std::iota(m1.begin(), m1.end(), 0);
    std::iota(m2.begin(), m2.end(), 0);

    auto t1 = std::chrono::steady_clock::now();
    auto m3 = m1 * m2;
    auto t2 = std::chrono::steady_clock::now();
    auto dt1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout << m3;
    std::cout << "Elapsed time: " << dt1.count() << "ms\n";

    auto t3 = std::chrono::steady_clock::now();
    auto m4 = Strassen(m1, m2);
    auto t4 = std::chrono::steady_clock::now();
    auto dt2 = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3);
    std::cout << m4;
    std::cout << "Elapsed time: " << dt2.count() << "ms\n";
}
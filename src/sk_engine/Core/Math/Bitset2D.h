#include <vector>
#include <stdexcept>
#include <cstdint>
#include <string>
#include <sstream>
#include <iostream>
#include <boost/dynamic_bitset.hpp>
#include <bitset>

namespace sk_math {
    class DynamicBitset2D {
        private:
        void check_bounds(size_t row, size_t col) const {
            if (row >= m || col >= n)
                throw std::out_of_range("Bitset2D index out of range");
        }

        size_t m, n;
        boost::dynamic_bitset<> bits;
        public:
        DynamicBitset2D(size_t rows, size_t cols, bool init = false)
            : m(rows), n(cols), bits(rows* cols, init) {
        }

        bool operator()(size_t row, size_t col) const {
            check_bounds(row, col);
            return bits[row * n + col];
        }

        void set(size_t row, size_t col, bool value = true) {
            check_bounds(row, col);
            bits[row * n + col] = value;
        }

        void reset() {
            bits.reset();
        }

        void flip() {
            bits.flip();
        }

        size_t count() const {
            return bits.count();
        }

        size_t size() const {
            return bits.size();
        }

        bool any() const {
            return bits.any();
        }

        bool none() const {
            return bits.none();
        }

        std::string to_string() const {
            std::ostringstream oss;
            for (size_t i = 0; i < m; ++i) {
                for (size_t j = 0; j < n; ++j)
                    oss << (bits[i * n + j] ? '1' : '0');
                oss << '\n';
            }
            return oss.str();
        }

        size_t rows() const { return m; }
        size_t cols() const { return n; }
    };


    typedef DynamicBitset2D Bitset2D;
}
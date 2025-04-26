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

    class DynamicBitset2D_with_xy_coordinate {
        private:
        void check_bounds(size_t x, size_t y) const {
            if (x >= width || y >= height)
                throw std::out_of_range("Bitset2D index out of range");
        }

        size_t width, height;
        boost::dynamic_bitset<> bits;
        public:
        DynamicBitset2D_with_xy_coordinate(size_t width, size_t height, bool init = false)
            : width(width), height(height), bits(width* height, init) {
        }

        bool operator()(size_t x, size_t y) const {
            check_bounds(x, y);
            return bits[y * width + x];
        }

        void set(size_t x, size_t y, bool value = true) {
            check_bounds(x, y);
            bits[y * width + x] = value;
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
            for (size_t i = 0; i < height; ++i) {
                for (size_t j = 0; j < width; ++j)
                    oss << (bits[i * width + j] ? '1' : '0');
                oss << '\n';
            }
            return oss.str();
        }

        size_t rows() const { return height; }
        size_t cols() const { return width; }
    };

    /// @brief bitset2D use y-x coordinate (rows first)
    typedef DynamicBitset2D Bitset2D;
    /// @brief bitmask2D use x-y coordinate (columns first, compatible to world coordinate, tilemap2d, etc)
    /// @note for faster caching, access it with by increasing x first
    typedef DynamicBitset2D_with_xy_coordinate Bitmask2D;
}
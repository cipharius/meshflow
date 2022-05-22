#ifndef BLOCK_H
#define BLOCK_H

#include <ostream>
#include <vector>
#include <array>
#include <cmath>

class Vec3 {
  public:
    constexpr Vec3() : _x(0), _y(0), _z(0) {}
    constexpr Vec3(float x, float y, float z) : _x(x), _y(y), _z(z) {}

    constexpr float x() const { return _x; }
    constexpr float y() const { return _y; }
    constexpr float z() const { return _z; }

    constexpr float length() const {
      return std::sqrt(_x*_x + _y*_y + _z*_z);
    }

    constexpr Vec3 normalise() const {
      return *this / length();
    }

    constexpr float dot(Vec3 const &other) const {
      return _x * other._x + _y * other._y + _z * other._z;
    }

    constexpr Vec3 lerp(Vec3 const &other, float alpha) const {
      return Vec3(
        (1 - alpha) * _x + alpha * other._x,
        (1 - alpha) * _y + alpha * other._y,
        (1 - alpha) * _z + alpha * other._z
      );
    }

    constexpr friend Vec3 operator+(Vec3 const &lhs, Vec3 const &rhs) {
      return Vec3(lhs._x + rhs._x, lhs._y + rhs._y, lhs._z + rhs._z);
    }

    constexpr friend Vec3 operator-(Vec3 const &lhs, Vec3 const &rhs) {
      return Vec3(lhs._x - rhs._x, lhs._y - rhs._y, lhs._z - rhs._z);
    }

    constexpr friend Vec3 operator*(Vec3 const &lhs, float rhs) {
      return Vec3(lhs._x * rhs, lhs._y * rhs, lhs._z * rhs);
    }

    constexpr friend Vec3 operator*(float lhs, Vec3 const &rhs) {
      return Vec3(lhs * rhs._x, lhs * rhs._y, lhs * rhs._z);
    }

    constexpr friend Vec3 operator/(Vec3 const &lhs, float rhs) {
      return Vec3(lhs._x / rhs, lhs._y / rhs, lhs._z / rhs);
    }

    inline friend std::ostream & operator<<(std::ostream &out, Vec3 const &vec) {
      return out << '(' << vec._x << ',' << vec._y << ',' << vec._z << ')';
    }

  private:
    float _x, _y, _z;
};

class Polyline {
  public:
    Polyline();
    explicit Polyline(std::vector<Vec3> const & points);
    explicit Polyline(std::vector<Vec3> &&points);
    Polyline(std::initializer_list<Vec3> points);

    constexpr float length() const { return _length; }
    constexpr std::vector<Vec3> const & points() const { return _points; }

    Vec3 front() const;
    Vec3 back() const;
    Vec3 lerp(float alpha) const;

  private:
    void recalculate();

    std::vector<Vec3> _points;
    std::vector<float> _bounds;
    float _length;
};

class Block {
  public:
    Block() = delete;
    Block(Polyline const & top, Polyline const & right, Polyline const & bottom, Polyline const & left);

    constexpr Polyline const & top() const { return _top; }
    constexpr Polyline const & right() const { return _right; }
    constexpr Polyline const & bottom() const { return _bottom; }
    constexpr Polyline const & left() const { return _left; }

    inline Vec3 top_left() const { return _top.front(); }
    inline Vec3 top_right() const { return _top.back(); }
    inline Vec3 bottom_right() const { return _bottom.back(); }
    inline Vec3 bottom_left() const { return _bottom.front(); }

  private:
    Polyline _top;
    Polyline _right;
    Polyline _bottom;
    Polyline _left;
};

struct QuadMesh {
  std::vector<std::array<size_t,4>> faces;
  std::vector<Vec3> vertices;
};

#endif

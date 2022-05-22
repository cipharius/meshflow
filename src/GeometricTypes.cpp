#include "GeometricTypes.h"

// Polyline
//

Polyline::Polyline() : _length(0) {}

Polyline::Polyline(std::vector<Vec3> const & points) : _points(points), _length(0) {
  this->recalculate();
}

Polyline::Polyline(std::vector<Vec3> &&points) : _points(std::move(points)), _length(0) {
  this->recalculate();
}

Polyline::Polyline(std::initializer_list<Vec3> points) : _points(points), _length(0) {
  this->recalculate();
}

Vec3 Polyline::front() const {
  if (_points.size() == 0) return Vec3();
  return _points.front();
}

Vec3 Polyline::back() const {
  if (_points.size() == 0) return Vec3();
  return _points.back();
}

Vec3 Polyline::lerp(float alpha) const {
  if (_points.size() <= 1) return this->front();
  if (alpha <= 0) return this->front();
  if (alpha >= 1) return this->back();

  for (unsigned int i = 0; i < _bounds.size(); i++) {
    if (alpha > _bounds[i]) continue;

    float beta = (alpha - _bounds[i - 1]) / (_bounds[i] - _bounds[i - 1]);
    return (1 - beta) * _points[i - 1] + beta * _points[i];
  }

  return Vec3();
}

void Polyline::recalculate() {
  _bounds.clear();
  if (_points.size() == 0) return;

  _bounds.push_back(0);

  for (unsigned int i = 1; i < _points.size(); i++) {
    _length += (_points[i] - _points[i - 1]).length();
    _bounds.push_back(_length);
  }

  for (unsigned int i = 0; i < _bounds.size(); i++) {
    _bounds[i] /= _length;
  }
}

// Block
//

Block::Block(Polyline const & top, Polyline const & right,
             Polyline const & bottom, Polyline const & left) {
  Vec3 top_left = (top.front() + left.back()) / 2;
  Vec3 top_right = (top.back() + right.back()) / 2;
  Vec3 bottom_right = (bottom.back() + right.front()) / 2;
  Vec3 bottom_left = (bottom.front() + left.front()) / 2;

  std::vector<Vec3> top_points = top.points();
  std::vector<Vec3> right_points = right.points();
  std::vector<Vec3> bottom_points = bottom.points();
  std::vector<Vec3> left_points = left.points();

  top_points.front() = top_left;
  left_points.back() = top_left;

  top_points.back() = top_right;
  right_points.back() = top_right;

  bottom_points.back() = bottom_right;
  right_points.front() = bottom_right;

  bottom_points.front() = bottom_left;
  left_points.front() = bottom_left;

  _top = Polyline(std::move(top_points));
  _right = Polyline(std::move(right_points));
  _bottom = Polyline(std::move(bottom_points));
  _left = Polyline(std::move(left_points));
}

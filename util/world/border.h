#ifndef ZYLANN_BORDER_H
#define ZYLANN_BORDER_H

#include "core/core_bind.h"


struct PointResult {
  Vector2 index;
  Vector2 location;
  float distance;
  float weight;

  PointResult() {}

  PointResult(Vector2 _index, Vector2 _location, float _distance, float _weight) {
    index = _index;
    location = _location;
    distance = _distance;
    weight = _weight;
  }
};

struct ClosestPointsResult {
  PointResult closest;
  List<PointResult> under_threshold;

  ClosestPointsResult() {
	closest = PointResult(Vector2(0, 0), Vector2(0, 0), 100000.0, 0.0);
    under_threshold = {};
  }

  ClosestPointsResult(PointResult _closest, List<PointResult> _under_threshold) {
    closest = _closest;
    under_threshold = _under_threshold;
  }
};

namespace zylann {
class CellNoise {
public:
	CellNoise();

	ClosestPointsResult get_closest_result(Vector2 location);

private:
	Map<Vector2, List<Vector2>> world_grid = {};
	int scale = 100;
	int border_threshold = 150;

	Vector2 get_grid_index(Vector2 location);
	List<Vector2> get_neighboors(Vector2 index);
	float get_distance(Vector2 point_a, Vector2 point_b);
	void generate_points(Vector2 index);
};

} // namespace zylann

#endif // ZYLANN_BORDER_H
#include "border.h"
#include "core/math/math_funcs.h"

namespace zylann {
CellNoise::CellNoise() {}

ClosestPointsResult CellNoise::get_closest_result(Vector2 location) {
	ClosestPointsResult result;
	float total_weight = 0;

	Vector2 index = get_grid_index(location);
	List<Vector2> neighboors = get_neighboors(index);

	for (const Vector2 &cell_index : neighboors) {
		// first encounter with this cell
		if (!world_grid.has(cell_index)) {
			generate_points(cell_index);
		}

		List<Vector2> points = world_grid[cell_index];
		for (const Vector2 &point : points) {
			float distance = get_distance(location, point);
			float normalzed_distance = sqrt(distance);

			if (normalzed_distance < result.closest.distance) {
				result.closest = PointResult(cell_index, point, normalzed_distance, 1.0);
			}

			if (distance < border_threshold) {
				PointResult res = PointResult(cell_index, point, normalzed_distance, pow(border_threshold - distance, 2.0));
				result.under_threshold.push_back(res);
				total_weight += res.weight; 
			}
		}
	}

	if (result.under_threshold.size() > 1) {
		for (PointResult &point : result.under_threshold) {
			point.weight /= total_weight;
		}
	}

	return result;
}

Vector2 CellNoise::get_grid_index(Vector2 location) {
	return Vector2(floor(location.x / scale), floor(location.y / scale));
}

List<Vector2> CellNoise::get_neighboors(Vector2 index) {
	List<Vector2> result = {};

	for (int x = -1; x < 2; x++) {
		for (int y = -1; y < 2; y++) {
			result.push_back(Vector2(index.x + x, index.y + y));
		}
	}

	return result;
}

float CellNoise::get_distance(Vector2 point_a, Vector2 point_b) {
	return pow(point_a.x - point_b.x, 2.0) + pow(point_a.y - point_b.y, 2.0);
}

void CellNoise::generate_points(Vector2 index) {
	List<Vector2> points = {};

	int min_x = index.x * scale;
	int max_x = (index.x + 1) * scale - 1;
	int min_y = index.y * scale;
	int max_y = (index.y + 1) * scale - 1;

	int x = rand() % (max_x - min_x + 1) + min_x;
	int y = rand() % (max_y - min_y + 1) + min_y;

	points.push_back(Vector2(x, y));

	world_grid[index] = points;
}
} //namespace zylann
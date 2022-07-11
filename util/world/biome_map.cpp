#include "biome_map.h"
#include "core/math/math_funcs.h"

namespace zylann {
BiomeMap::BiomeMap() {
}
BiomeMap::~BiomeMap() {}


void BiomeMap::set_temperature_noise(Ref<FastNoiseLite> _temperature_noise) {
    temperature_noise = _temperature_noise;
}
void BiomeMap::set_moisture_noise(Ref<FastNoiseLite> _moisture_noise) {
    moisture_noise = _moisture_noise;
}
void BiomeMap::set_offset(int _offset) {
    offset = _offset;
}
void BiomeMap::set_biomes(Array biomes) {
	biomes = _biomes;
}

float BiomeMap::normalize_noise_2d(FastNoiseLite noise, int x, int y, int offest) {
    return 0.5 + 0.5 * noise.get_noise_2d(x / offest, y / offest);
}

List<WeightedBiomeInstance> BiomeMap::get_closest_biomes(Vector2 location) {
    List<WeightedBiomeInstance> closest_biomes = {};
    float total_weight = 0;

    Vector2 index = get_grid_index(location);
	List<Vector2> neighboors = get_neighboors(index);

    for (const Vector2 &cell_index : neighboors) {
		// first encounter with this cell
		if (!biome_map_grid.has(cell_index)) {
			generate_biomes(cell_index);
		}

		List<BiomeInstance> biome_instances = world_grid[cell_index];
		for (const BiomeInstance &biome_instance : biome_instances) {
			float distance = get_distance(location, biome_instance->location);
			float normalized_distance = sqrt(distance);

			if (distance < border_threshold) {
				WeightedBiomeInstance res = WeightedBiomeInstance(biome_instance, pow(border_threshold - distance, 2.0));
				closest_biomes.push_back(res);
				total_weight += res.weight;
			}
		}
	}

    for (WeightedBiomeInstance &weighted_biome_instance : closest_biomes) {
        weighted_biome_instance.weight /= total_weight;
    }

    return closest_biomes;
}


Vector2 BiomeMap::get_grid_index(Vector2 location) {
	return Vector2(floor(location.x / scale), floor(location.y / scale));
}

List<Vector2> BiomeMap::get_neighboors(Vector2 index) {
	List<Vector2> result = {};

	for (int x = -1; x < 2; x++) {
		for (int y = -1; y < 2; y++) {
			result.push_back(Vector2(index.x + x, index.y + y));
		}
	}

	return result;
}

float BiomeMap::get_distance(Vector2 point_a, Vector2 point_b) {
	return pow(point_a.x - point_b.x, 2.0) + pow(point_a.y - point_b.y, 2.0);
}

void BiomeMap::generate_points(Vector2 index) {
	List<BiomeInstance> biome_instances = {};

    float temperature = normalize_noise_2d(temperature_noise, x, y, 1);
    float moisture = normalize_noise_2d(moisture_noise, x, y, 1);

	int min_x = index.x * scale;
	int max_x = (index.x + 1) * scale - 1;
	int min_y = index.y * scale;
	int max_y = (index.y + 1) * scale - 1;

	int x = temperature * (max_x - min_x + 1) + min_x;
	int y = moisture * (max_y - min_y + 1) + min_y;

    BiomeInstance new_biome_instance = BiomeInstance(get_biome_by(temperature, moisture), Vector2(x, y));

	biome_instances.push_back(new_biome_instance);

	biome_map_grid[index] = biome_instances;
}

Ref<Biome> BiomeMap::get_biome_by(float temperature, float moisture) {
	// fallback to default if no biome meets the requirements
	Ref<Biome> biome_ref { Object::cast_to<Biome>(biomes[0]) };

	for (int i = 0; i < biomes.size(); ++i) {
		Ref<Biome> biome_new_ref{ Object::cast_to<Biome>(biomes[i]) };
		if (biome_new_ref->is_in_range(temperature, moisture)) {
			biome_ref = biome_new_ref;
			break;
		}
	}

	return biome_ref;
}
} //namespace zylann

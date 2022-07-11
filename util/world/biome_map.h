#ifndef ZYLANN_BIOME_MAP_H
#define ZYLANN_BIOME_MAP_H

#include "core/core_bind.h"
#include <modules/voxel/util/world/biome.h>
#include <map>

struct BiomeInstance {
  Biome biome;
  Vector2i location;

  BiomeInstance() {}

  BiomeInstance(Biome _biome, Vector2i _location) {
    biome = _biome;
    location = _location;
  }
};

struct WeightedBiomeInstance {
  BiomeInstance biome_instance;
  float weight;

  WeightedBiomeInstance() {}

  WeightedBiomeInstance(BiomeInstance _biome_instance, float _weight) {
    biome_instance = _biome_instance;
    weight = _weight;
  }
};

namespace zylann {
class BiomeMap {
public:
	BiomeMap();
    ~BiomeMap();

    void set_temperature_noise(Ref<FastNoiseLite> _temperature_noise);
	void set_moisture_noise(Ref<FastNoiseLite> _moisture_noise);
	void set_offset(int _offset);
    void set_biomes(HashMap<Humidity, HashMap<Temperature, List<Biome>>> _biomes);

    List<WeightedBiomeInstance> get_closest_biomes(Vector2 location);

private:
    Ref<FastNoiseLite> temperature_noise;
	Ref<FastNoiseLite> moisture_noise;
    HashMap<Humidity, HashMap<Temperature, List<Biome>> biomes;
    int offset = 1;
    int closest_biome_threshold = 1000000;

    HashMap<Vector2, List<WeightedBiomeInstance>> biome_map_grid = {};

    float normalize_noise_2d(FastNoiseLite noise, int x, int y, int offest);
    Vector2 get_grid_index(Vector2 location);
	List<Vector2> get_neighboors(Vector2 index);
	float get_distance(Vector2 point_a, Vector2 point_b);
	void generate_points(Vector2 index);
};

} // namespace zylann

#endif // ZYLANN_BIOME_MAP_H

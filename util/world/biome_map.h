#ifndef ZYLANN_BIOME_MAP_H
#define ZYLANN_BIOME_MAP_H

#include "core/core_bind.h"
#include <map>
#include <modules/noise/fastnoise_lite.h>
#include <modules/voxel/util/world/biome.h>

namespace zylann::voxel {

struct BiomeInstance {
  Ref<Biome> biome;
  Vector2 location;

  BiomeInstance() {
	  biome = nullptr;
	  location = Vector2(0, 0);
  }

  BiomeInstance(Ref<Biome> _biome, Vector2 _location) {
    biome = _biome;
    location = _location;
  }
};

struct WeightedBiomeInstance {
  BiomeInstance *biome_instance;
  float weight;

  WeightedBiomeInstance() {
	  biome_instance = nullptr;
	  weight = .0;
  }

  WeightedBiomeInstance(BiomeInstance *_biome_instance, float _weight) {
    biome_instance = _biome_instance;
    weight = _weight;
  }
};

class BiomeMap {
public:
	BiomeMap();
    ~BiomeMap();

    int get_offset() const;
    void set_offset(int new_offset);
    int get_closest_biome_threshold() const;
    void set_closest_biome_threshold(int new_closest_biome_threshold);
    int get_scale() const;
    void set_scale(int new_scale);

    void set_temperature_noise(Ref<FastNoiseLite> _temperature_noise);
	void set_moisture_noise(Ref<FastNoiseLite> _moisture_noise);
    void set_biomes(HashMap<Biome::Humidity, HashMap<Biome::Temperature, List<Ref<Biome>>>> _biomes);

    List<WeightedBiomeInstance> get_closest_biomes(Vector2 location);
	Ref<Biome> get_biome_by_position(Vector2 location);

private:
    Ref<FastNoiseLite> temperature_noise;
	Ref<FastNoiseLite> moisture_noise;
	HashMap <Biome::Humidity, HashMap<Biome::Temperature, List<Ref<Biome>>>> biomes;

    int offset = 1;
    int closest_biome_threshold = 1000000;
	int scale = 1000;

    HashMap<Vector2, List<BiomeInstance>> biome_map_grid = {};

    float normalize_noise_2d(Ref<FastNoiseLite> noise, int x, int y, int offest);
    Vector2 get_grid_index(Vector2 location);
	List<Vector2> get_neighboors(Vector2 index);
	float get_distance(Vector2 point_a, Vector2 point_b);
	void generate_biomes(Vector2 index);
	Ref<Biome> get_biome_by(float temperature, float moisture);
};

} // namespace zylann

#endif // ZYLANN_BIOME_MAP_H

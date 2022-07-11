#ifndef VOXEL_GENERATOR_WORLD_H
#define VOXEL_GENERATOR_WORLD_H

#include "voxel_generator_heightmap.h"
#include <scene/resources/curve.h>
#include <modules/noise/fastnoise_lite.h>
#include <modules/voxel/util/world/biome.h>
#include <modules/voxel/util/world/biome_map.h>

class Curve;
class Noise;

namespace zylann::voxel {

class VoxelGeneratorWorld : public VoxelGenerator {
	GDCLASS(VoxelGeneratorWorld, VoxelGenerator)

public:
	VoxelGeneratorWorld();
	~VoxelGeneratorWorld();

    static const int MIN_HEIGHT = 100;
	static const int MAX_HEIGHT = 1000;
    int get_height() const;
	void set_height(int new_height);

    static const int MIN_WATER_LEVEL = 0;
	static const int MAX_WATER_LEVEL = MAX_HEIGHT;
    int get_water_level() const;
    void set_water_level(int new_water_level);

    static const int MIN_OFFSET = 0;
	static const int MAX_OFFEST = 1000;
    int get_offset() const;
    void set_offset(int new_offset);

	void set_temperature_noise(Ref<FastNoiseLite> temperature_noise);
	Ref<FastNoiseLite> get_temperature_noise() const;
	void set_moisture_noise(Ref<FastNoiseLite> moisture_noise);
	Ref<FastNoiseLite> get_moisture_noise() const;
	void set_continentalness_noise(Ref<FastNoiseLite> continentalness_noise);
	Ref<FastNoiseLite> get_continentalness_noise() const;
	void set_peaks_and_valleys_noise(Ref<FastNoiseLite> peaks_and_valleys_noise);
	Ref<FastNoiseLite> get_peaks_and_valleys_noise() const;
	void set_erosion_noise(Ref<FastNoiseLite> erosion_noise);
	Ref<FastNoiseLite> get_erosion_noise() const;


	void set_biomes(Array biomes);
	Array get_biomes() const;

	VoxelBufferInternal::ChannelId VoxelGeneratorWorld::get_channel() const;

	Result generate_block(VoxelGenerator::VoxelQueryData &input) override;

private:
	Ref<Biome> get_biome_by(float temperature, float moisture);

	void _on_temperature_noise_changed();
	void _on_moisture_noise_changed();
	void _on_continentalness_noise_changed();
	void _on_peaks_and_valleys_noise_changed();
	void _on_erosion_noise_changed();
	static void _bind_methods();

private:
	VoxelBufferInternal::ChannelId channel = VoxelBufferInternal::CHANNEL_COLOR;

	Array _biomes;
	BiomeMap biome_map;

    int height = 200;
	int water_level = 50;

	const uint64_t ground = Color8(255, 255, 255, 255).to_u16();
	const uint64_t water = Color8(255, 255, 255, 150).to_u16();


	// biome selection
	Ref<FastNoiseLite> _temperature_noise;
	Ref<FastNoiseLite> _moisture_noise;
	// terrain shape
	Ref<FastNoiseLite> _continentalness_noise;
	Ref<FastNoiseLite> _peaks_and_valleys_noise;
	Ref<FastNoiseLite> _erosion_noise;

	struct Parameters {
		Ref<FastNoiseLite> temperature_noise;
		Ref<FastNoiseLite> moisture_noise;
		Ref<FastNoiseLite> continentalness_noise;
		Ref<FastNoiseLite> peaks_and_valleys_noise;
		Ref<FastNoiseLite> erosion_noise;
	};

	Parameters _parameters;
	RWLock _parameters_lock;

    float normalize_noise_2d(FastNoiseLite noise, int x, int y, int offest);
};

} // namespace zylann::voxel

#endif // VOXEL_GENERATOR_WORLD_H

#ifndef VOXEL_GENERATOR_WORLD_H
#define VOXEL_GENERATOR_WORLD_H

#include "voxel_generator_heightmap.h"
#include <scene/resources/curve.h>
#include <modules/noise/fastnoise_lite.h>
#include <modules/voxel/util/world/biome.h>
#include <modules/voxel/util/world/border.h>

class Curve;
class Noise;

namespace zylann::voxel {

class VoxelGeneratorWorld : public VoxelGenerator {
	GDCLASS(VoxelGeneratorWorld, VoxelGenerator)

public:
	VoxelGeneratorWorld();
	~VoxelGeneratorWorld();

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
	CellNoise border_delimeter;

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
};

} // namespace zylann::voxel

#endif // VOXEL_GENERATOR_WORLD_H

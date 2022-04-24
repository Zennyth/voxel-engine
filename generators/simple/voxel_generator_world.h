#ifndef VOXEL_GENERATOR_WORLD_H
#define VOXEL_GENERATOR_WORLD_H

#include "voxel_generator_heightmap.h"
#include <modules/noise/fastnoise_lite.h>

class Curve;
class Noise;

namespace zylann::voxel {

class VoxelGeneratorWorld : public VoxelGenerator {
	GDCLASS(VoxelGeneratorWorld, VoxelGenerator)

public:
	VoxelGeneratorWorld();
	~VoxelGeneratorWorld();

	void set_continentalness_curve(Ref<Curve> curve);
	Ref<Curve> get_continentalness_curve() const;
	void set_peaks_and_valleys_curve(Ref<Curve> curve);
	Ref<Curve> get_peaks_and_valleys_curve() const;
	void set_erosion_curve(Ref<Curve> curve);
	Ref<Curve> get_erosion_curve() const;

	VoxelBufferInternal::ChannelId VoxelGeneratorWorld::get_channel() const;

	Result generate_block(VoxelGenerator::VoxelQueryData &input) override;

private:
	void _on_continentalness_curve_changed();
	void _on_peaks_and_valleys_curve_changed();
	void _on_erosion_curve_changed();
	static void _bind_methods();

private:
	FastNoiseLite continentalness_noise;
	FastNoiseLite peaks_and_valleys_noise;
	FastNoiseLite erosion_noise;

	VoxelBufferInternal::ChannelId channel = VoxelBufferInternal::CHANNEL_COLOR;

	Ref<Curve> _continentalness_curve;
	Ref<Curve> _peaks_and_valleys_curve;
	Ref<Curve> _erosion_curve;

	struct Parameters {
		Ref<Curve> continentalness_curve;
		Ref<Curve> peaks_and_valleys_curve;
		Ref<Curve> erosion_curve;
	};

	Parameters _parameters;
	RWLock _parameters_lock;
};

} // namespace zylann::voxel

#endif // VOXEL_GENERATOR_WORLD_H

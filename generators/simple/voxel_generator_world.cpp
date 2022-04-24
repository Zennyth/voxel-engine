#include "voxel_generator_world.h"
#include <core/config/engine.h>
#include <core/core_string_names.h>
#include <modules/noise/fastnoise_lite.h>
#include <scene/resources/curve.h>
#include <modules/voxel/util/math/color8.h>

#include <scene\resources\gradient.h>

namespace zylann::voxel {
VoxelGeneratorWorld::VoxelGeneratorWorld() {}

VoxelGeneratorWorld::~VoxelGeneratorWorld() {}

/* continentalness_curve */
void VoxelGeneratorWorld::set_continentalness_curve(Ref<Curve> curve) {
	if (_continentalness_curve == curve) {
		return;
	}
	if (_continentalness_curve.is_valid()) {
		_continentalness_curve->disconnect(CoreStringNames::get_singleton()->changed,
				callable_mp(this, &VoxelGeneratorWorld::_on_continentalness_curve_changed));
	}
	_continentalness_curve = curve;
	RWLockWrite wlock(_parameters_lock);
	if (_continentalness_curve.is_valid()) {
		_continentalness_curve->connect(CoreStringNames::get_singleton()->changed,
				callable_mp(this, &VoxelGeneratorWorld::_on_continentalness_curve_changed));
		// The Curve resource is not thread-safe so we make a copy of it for use in threads
		_parameters.continentalness_curve = _continentalness_curve->duplicate();
		_parameters.continentalness_curve->bake();
	} else {
		_parameters.continentalness_curve.unref();
	}
}
Ref<Curve> VoxelGeneratorWorld::get_continentalness_curve() const {
	return _continentalness_curve;
}
void VoxelGeneratorWorld::_on_continentalness_curve_changed() {
	ERR_FAIL_COND(_continentalness_curve.is_null());
	RWLockWrite wlock(_parameters_lock);
	_parameters.continentalness_curve = _continentalness_curve->duplicate();
	_parameters.continentalness_curve->bake();
}

/* peaks_and_valleys_curve */
void VoxelGeneratorWorld::set_peaks_and_valleys_curve(Ref<Curve> curve) {
	if (_peaks_and_valleys_curve == curve) {
		return;
	}
	if (_peaks_and_valleys_curve.is_valid()) {
		_peaks_and_valleys_curve->disconnect(CoreStringNames::get_singleton()->changed,
				callable_mp(this, &VoxelGeneratorWorld::_on_peaks_and_valleys_curve_changed));
	}
	_peaks_and_valleys_curve = curve;
	RWLockWrite wlock(_parameters_lock);
	if (_peaks_and_valleys_curve.is_valid()) {
		_peaks_and_valleys_curve->connect(CoreStringNames::get_singleton()->changed,
				callable_mp(this, &VoxelGeneratorWorld::_on_peaks_and_valleys_curve_changed));
		// The Curve resource is not thread-safe so we make a copy of it for use in threads
		_parameters.peaks_and_valleys_curve = _peaks_and_valleys_curve->duplicate();
		_parameters.peaks_and_valleys_curve->bake();
	} else {
		_parameters.peaks_and_valleys_curve.unref();
	}
}
Ref<Curve> VoxelGeneratorWorld::get_peaks_and_valleys_curve() const {
	return _peaks_and_valleys_curve;
}
void VoxelGeneratorWorld::_on_peaks_and_valleys_curve_changed() {
	ERR_FAIL_COND(_peaks_and_valleys_curve.is_null());
	RWLockWrite wlock(_parameters_lock);
	_parameters.peaks_and_valleys_curve = _peaks_and_valleys_curve->duplicate();
	_parameters.peaks_and_valleys_curve->bake();
}

/* erosion_curve */
void VoxelGeneratorWorld::set_erosion_curve(Ref<Curve> curve) {
	if (_erosion_curve == curve) {
		return;
	}
	if (_erosion_curve.is_valid()) {
		_erosion_curve->disconnect(CoreStringNames::get_singleton()->changed,
				callable_mp(this, &VoxelGeneratorWorld::_on_erosion_curve_changed));
	}
	_erosion_curve = curve;
	RWLockWrite wlock(_parameters_lock);
	if (_erosion_curve.is_valid()) {
		_erosion_curve->connect(CoreStringNames::get_singleton()->changed,
				callable_mp(this, &VoxelGeneratorWorld::_on_erosion_curve_changed));
		// The Curve resource is not thread-safe so we make a copy of it for use in threads
		_parameters.erosion_curve = _erosion_curve->duplicate();
		_parameters.erosion_curve->bake();
	} else {
		_parameters.erosion_curve.unref();
	}
}
Ref<Curve> VoxelGeneratorWorld::get_erosion_curve() const {
	return _erosion_curve;
}
void VoxelGeneratorWorld::_on_erosion_curve_changed() {
	ERR_FAIL_COND(_erosion_curve.is_null());
	RWLockWrite wlock(_parameters_lock);
	_parameters.erosion_curve = _erosion_curve->duplicate();
	_parameters.erosion_curve->bake();
}

VoxelGenerator::Result VoxelGeneratorWorld::generate_block(VoxelGenerator::VoxelQueryData &input) {
	Parameters params;
	{
		RWLockRead rlock(_parameters_lock);
		params = _parameters;
	}

	Result result;

	ERR_FAIL_COND_V(params.continentalness_curve.is_null(), result);
	ERR_FAIL_COND_V(params.peaks_and_valleys_curve.is_null(), result);
	ERR_FAIL_COND_V(params.erosion_curve.is_null(), result);

	VoxelBufferInternal &out_buffer = input.voxel_buffer;
	out_buffer.set_channel_depth(channel, VoxelBufferInternal::DEPTH_16_BIT);
	const Vector3i origin = input.origin_in_voxels;
	const Vector3i bs = out_buffer.get_size();
	const float margin = 1 << input.lod;
	const int lod = input.lod;
	const int offest = 4;

	const int height = 200;
	const Color grass_up = Color(0.219, 0.588, 0.01, 1.0);
	const Color grass_down = Color(0.549, 0.698, 0.325, 1.0);
	Gradient &grass_gradient = Gradient();
	grass_gradient.add_point(0, Color(0.219, 0.588, 0.01, 1.0));
	grass_gradient.add_point(1, Color(0.549, 0.698, 0.325, 1.0));

	Curve &continentalness_curve 	= **params.continentalness_curve;
	Curve &peaks_and_valleys_curve 	= **params.peaks_and_valleys_curve;
	Curve &erosion_curve 			= **params.erosion_curve;

	if (origin.y > height + margin) {
		// The bottom of the block is above the highest ground can go (default is air)
		result.max_lod_hint = true;
		return result;
	}
	if (origin.y + (bs.y << lod) < 0) {
		// The top of the block is below the lowest ground can go
		out_buffer.clear_channel(channel, Color8(grass_up).to_u16());
		result.max_lod_hint = true;
		return result;
	}

	const int stride = 1 << lod;

	// Blocky
	int gz = origin.z;

	for (int z = 0; z < bs.z; ++z, gz += stride) {
		int gx = origin.x;

		for (int x = 0; x < bs.x; ++x, gx += stride) {
			float noise_continentalness = 0.5 + 0.5 * continentalness_noise.get_noise_2d(gx/offest, gz/offest);
			float noise_peaks_and_valleys = 0.5 + 0.5 * peaks_and_valleys_noise.get_noise_2d(gx/offest, gz/offest);
			float noise_erosion = 0.5 + 0.5 * erosion_noise.get_noise_2d(gx/offest, gz/offest);

			// Output is blocky, so we can go for just one sample
			float normalized_height_map = continentalness_curve.interpolate_baked(noise_continentalness) * .33 + peaks_and_valleys_curve.interpolate_baked(noise_peaks_and_valleys) * .33 + erosion_curve.interpolate_baked(noise_erosion) * .33;
			float h = normalized_height_map * height;
			h -= origin.y;
			int ih = int(h) >> lod;
			if (ih > 0) {
				if (ih > bs.y) {
					ih = bs.y;
				}
				const Color color = grass_gradient.get_color_at_offset(normalized_height_map);
				out_buffer.fill_area(Color8(color).to_u16(), Vector3i(x, 0, z), Vector3i(x + 1, ih, z + 1), channel);
			}

		} // for x
	} // for z

	out_buffer.compress_uniform_channels();
	return result;
}

void VoxelGeneratorWorld::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_continentalness_curve", "curve"), &VoxelGeneratorWorld::set_continentalness_curve);
	ClassDB::bind_method(D_METHOD("get_continentalness_curve"), &VoxelGeneratorWorld::get_continentalness_curve);
	ClassDB::bind_method(D_METHOD("set_peaks_and_valleys_curve", "curve"), &VoxelGeneratorWorld::set_peaks_and_valleys_curve);
	ClassDB::bind_method(D_METHOD("get_peaks_and_valleys_curve"), &VoxelGeneratorWorld::get_peaks_and_valleys_curve);
	ClassDB::bind_method(D_METHOD("set_erosion_curve", "curve"), &VoxelGeneratorWorld::set_erosion_curve);
	ClassDB::bind_method(D_METHOD("get_erosion_curve"), &VoxelGeneratorWorld::get_erosion_curve);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "continentalness_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve"), "set_continentalness_curve", "get_continentalness_curve");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "peaks_and_valleys_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve"), "set_peaks_and_valleys_curve", "get_peaks_and_valleys_curve");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "erosion_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve"), "set_erosion_curve", "get_erosion_curve");
}

VoxelBufferInternal::ChannelId VoxelGeneratorWorld::get_channel() const {
	return channel;
}

} // namespace zylann::voxel

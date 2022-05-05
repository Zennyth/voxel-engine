#ifndef ZYLANN_BIOME_H
#define ZYLANN_BIOME_H

#include <core/io/resource.h>
#include <scene/resources/gradient.h>
#include "height_map.h"

namespace zylann {

class Biome : public Resource {
	GDCLASS(Biome, Resource)

public:
	Biome();

	uint16_t get_color_at(float offset);
	bool is_in_range(float temperature, float moisture);

	void set_continentalness(Ref<HeightMap> continentalness);
	Ref<HeightMap> get_continentalness() const;
	void set_erosion(Ref<HeightMap> erosion);
	Ref<HeightMap> get_erosion() const;
	void set_peaks_and_valleys(Ref<HeightMap> peaks_and_valleys);
	Ref<HeightMap> get_peaks_and_valleys() const;

	void set_gradient(Ref<Gradient> gradient);
	Ref<Gradient> get_gradient() const;

	void set_min_temperature(float min_temperature);
	float get_min_temperature() const;
	void set_max_temperature(float max_temperature);
	float get_max_temperature() const;
	void set_min_moisture(float min_moisture);
	float get_min_moisture() const;
	void set_max_moisture(float max_moisture);
	float get_max_moisture() const;
	void set_biome_name(String biome_name);
	String get_biome_name() const;


private:
	static void _bind_methods();

	Ref<HeightMap> _continentalness;
	Ref<HeightMap> _erosion;
	Ref<HeightMap> _peaks_and_valleys;
	Ref<Gradient> _gradient;

	float _min_temperature = 0;
	float _max_temperature = 1;
	float _min_moisture = 0;
	float _max_moisture = 1;
	String _biome_name = "";

    struct Parameters {
		Ref<HeightMap> continentalness;
		Ref<HeightMap> erosion;
		Ref<HeightMap> peaks_and_valleys;
		Ref<Gradient> gradient;
	};

	Parameters _parameters;
	RWLock _parameters_lock;
};

} // namespace zylann

#endif // ZYLANN_BIOME_H

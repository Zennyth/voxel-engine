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

    enum Humidity {
        HUMIDITY_STICKY,
		HUMIDITY_HUMID,
        HUMIDITY_PLEASANT,
        HUMIDITY_DRY,
        HUMIDITY_COUNT,
	};
    Humidity get_humidity() const;
	void set_humidity(Humidity _humidity);

    enum Temperature {
        TEMPERATURE_FREEZING,
		TEMPERATURE_COLD,
        TEMPERATURE_TEMPERATE,
        TEMPERATURE_HOT,
        TEMPERATURE_BURNING,
        TEMPERATURE_COUNT,
	};
    Temperature get_temperature() const;
	void set_temperature(Temperature _temperature);

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

	String get_biome_name() const;


private:
	static void _bind_methods();

    Temperature temperature;
    Humidity humidity;

	Ref<HeightMap> _continentalness;
	Ref<HeightMap> _erosion;
	Ref<HeightMap> _peaks_and_valleys;
	Ref<Gradient> _gradient;

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

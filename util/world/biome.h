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
//        HUMIDITY_STICKY = 0,
		HUMIDITY_HUMID = 0,
//        HUMIDITY_PLEASANT,
        HUMIDITY_DRY,
        HUMIDITY_COUNT,
	};
    Humidity get_humidity() const;
	void set_humidity(Humidity _humidity);

    enum Temperature {
        TEMPERATURE_FREEZING = 0,
//		TEMPERATURE_COLD,
//        TEMPERATURE_TEMPERATE,
        TEMPERATURE_HOT,
//        TEMPERATURE_BURNING,
        TEMPERATURE_COUNT,
	};
    Temperature get_temperature() const;
	void set_temperature(Temperature _temperature);

	void set_continentalness(Ref<HeightMap> continentalness);
	Ref<HeightMap> get_continentalness() const;
	void set_erosion(Ref<HeightMap> erosion);
	Ref<HeightMap> get_erosion() const;
	void set_peaks_and_valleys(Ref<HeightMap> peaks_and_valleys);
	Ref<HeightMap> get_peaks_and_valleys() const;

    int get_surface_threshold() const;
	void set_surface_threshold(int _surface_threshold);
	void set_underground_gradient(Ref<Gradient> gradient);
	Ref<Gradient> get_underground_gradient() const;
	void set_surface_gradient(Ref<Gradient> gradient);
	Ref<Gradient> get_surface_gradient() const;

	String get_biome_name() const;
	void set_biome_name(String _biome_name);


    uint16_t get_underground_color_at(float offset);
    uint16_t get_surface_color_at(float offset);

private:
	static void _bind_methods();
    uint16_t get_color_at(Ref<Gradient> gradient, float offset);

    Humidity humidity = HUMIDITY_HUMID;
	Temperature temperature = TEMPERATURE_FREEZING;

	Ref<HeightMap> _continentalness;
	Ref<HeightMap> _erosion;
	Ref<HeightMap> _peaks_and_valleys;

    int surface_threshold = 3;
	Ref<Gradient> _underground_gradient;
	Ref<Gradient> _surface_gradient;

	String _biome_name = "";

    struct Parameters {
		Ref<HeightMap> continentalness;
		Ref<HeightMap> erosion;
		Ref<HeightMap> peaks_and_valleys;
		Ref<Gradient> underground_gradient;
	    Ref<Gradient> surface_gradient;
	};

	Parameters _parameters;
	RWLock _parameters_lock;
};

} // namespace zylann

VARIANT_ENUM_CAST(zylann::Biome::Humidity);
VARIANT_ENUM_CAST(zylann::Biome::Temperature);

#endif // ZYLANN_BIOME_H

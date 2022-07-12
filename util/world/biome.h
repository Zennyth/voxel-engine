#ifndef ZYLANN_BIOME_H
#define ZYLANN_BIOME_H

#include <core/io/resource.h>
#include <scene/resources/gradient.h>
#include "height_filter.h"

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

	uint16_t get_color_at(float offset);

	void set_continentalness(Ref<HeightFilter> continentalness);
	Ref<HeightFilter> get_continentalness() const;
	void set_erosion(Ref<HeightFilter> erosion);
	Ref<HeightFilter> get_erosion() const;
	void set_peaks_and_valleys(Ref<HeightFilter> peaks_and_valleys);
	Ref<HeightFilter> get_peaks_and_valleys() const;

	void set_gradient(Ref<Gradient> gradient);
	Ref<Gradient> get_gradient() const;

	String get_biome_name() const;
	void set_biome_name(String _biome_name);


private:
	static void _bind_methods();

    Humidity humidity = HUMIDITY_HUMID;
	Temperature temperature = TEMPERATURE_FREEZING;

	Ref<HeightFilter> _continentalness;
	Ref<HeightFilter> _erosion;
	Ref<HeightFilter> _peaks_and_valleys;
	Ref<Gradient> _gradient;

	String _biome_name = "";

    struct Parameters {
		Ref<HeightFilter> continentalness;
		Ref<HeightFilter> erosion;
		Ref<HeightFilter> peaks_and_valleys;
		Ref<Gradient> gradient;
	};

	Parameters _parameters;
	RWLock _parameters_lock;
};

} // namespace zylann

VARIANT_ENUM_CAST(zylann::Biome::Humidity);
VARIANT_ENUM_CAST(zylann::Biome::Temperature);

#endif // ZYLANN_BIOME_H

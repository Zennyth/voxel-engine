#ifndef ZYLANN_BIOME_H
#define ZYLANN_BIOME_H

#include <core/io/resource.h>
#include "height_map.h"

namespace zylann {

class Biome : public Resource {
	GDCLASS(Biome, Resource)

public:
	Biome();

	void set_continentalness(Ref<HeightMap> continentalness);
	Ref<HeightMap> get_continentalness() const;

private:
	static void _bind_methods();

	Ref<HeightMap> _continentalness;

    struct Parameters {
		Ref<HeightMap> continentalness;
	};

	Parameters _parameters;
	RWLock _parameters_lock;
};

} // namespace zylann

#endif // ZYLANN_BIOME_H

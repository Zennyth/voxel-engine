#ifndef ZYLANN_HEIGHT_FILTER_H
#define ZYLANN_HEIGHT_FILTER_H

#include <core/io/resource.h>

class Curve;

namespace zylann::voxel {

class HeightFilter : public Resource {
	GDCLASS(HeightFilter, Resource)

public:
	HeightFilter();

	real_t get_height_at(real_t noise);


    void set_weight(float weight);
	float get_weight() const;

    void set_curve(Ref<Curve> curve);
	Ref<Curve> get_curve() const;

private:
    void _on_curve_changed();
	static void _bind_methods();

    float _weight = 0;
    Ref<Curve> _curve;

    struct Parameters {
		Ref<Curve> curve;
	};

	Parameters _parameters;
	RWLock _parameters_lock;
};

} // namespace zylann

#endif // ZYLANN_HEIGHT_FILTER_H

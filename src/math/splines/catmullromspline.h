#ifndef CATMULLROMSPLINE_H
#define CATMULLROMSPLINE_H

#include "./spline.h"
#include "./cardinalspline.h"

namespace math {

    class catmullrom_spline : public cardinal_spline {
    public:

        catmullrom_spline(std::vector<vec> inControlPoints)
        : cardinal_spline{inControlPoints, 0.5 }
        { }
    
    };
};

#endif
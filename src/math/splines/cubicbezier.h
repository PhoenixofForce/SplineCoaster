#ifndef CUBICBEZIER_H
#define CUBICBEZIER_H

#include "./spline.h"

namespace math {

    class cubic_bezier : public spline {
    public:

        cubic_bezier(vec c1, vec c2, vec c3, vec c4)
        : spline{{ c1, c2, c3, c4 }}
        { }

        //t means values in [0, 1]
        vec get(double t) const { 
            if(t < 0) return get(0);
            if(t > getSegmentCount()) return get(getSegmentCount());

            const vec& c1{ controlPoints.at(0) };
            const vec& c2{ controlPoints.at(1) };
            const vec& c3{ controlPoints.at(2) };
            const vec& c4{ controlPoints.at(3) };

            return (c1 + 
                    t  * (-3 * c1 + 3 * c2) +
                    t*t * (3 * c1 - 6 * c2 + 3 * c3) + 
                    t*t*t * ( -c1 + 3 * c2 - 3 * c3 + c4));
        }

        vec getDerivate(double t) const { 
            if(t < 0) return getDerivate(0);
            if(t > getSegmentCount()) return getDerivate(getSegmentCount());

            const vec& c1{ controlPoints.at(0) };
            const vec& c2{ controlPoints.at(1) };
            const vec& c3{ controlPoints.at(2) };
            const vec& c4{ controlPoints.at(3) };

            return (1  * (-3 * c1 + 3 * c2) +
                    2*t * (3 * c1 - 6 * c2 + 3 * c3) + 
                    3*t*t * ( -c1 + 3 * c2 - 3 * c3 + c4)).normalize();
        }

        int getSegmentCount() const {
            return 1;
        }

    private:
        
    };
};

#endif
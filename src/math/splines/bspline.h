#ifndef BSPLINE_H
#define BSPLINE_H

#include "./spline.h"

namespace math {

    class b_spline : public spline {
    public:

        b_spline(std::vector<vec> inControlPoints)
        : spline{inControlPoints}
        { }

        vec get(double u) const { 
            if(u < 0) return get(0);
            if(u > getSegmentCount()) return get(getSegmentCount());

            int startIndex = (int) std::floor(u) + 1;
            
            vec p1 = controlPoints.at(startIndex - 1);
            vec p2 = controlPoints.at(startIndex + 0);
            vec p3 = controlPoints.at(startIndex + 1);
            vec p4 = controlPoints.at(startIndex + 2);

            double t = u;
            while(t > 1) t -= 1; 

            return (1  * (p1 + 4 * p2 + p3)/6.0 + 
                    t   * (-3 * p1 + 3 * p3)/6.0 +
                    t*t  * (3 * p1 - 6 * p2 + 3 * p3)/6.0 + 
                    t*t*t * (-p1 + 3 * p2 - 3 * p3 + p4)/6.0);

        }

        vec getDerivate(double u) const {
            if(u < 0) return getDerivate(0);
            if(u > getSegmentCount()) return getDerivate(getSegmentCount());

            int startIndex = (int) std::floor(u) + 1;
            
            vec p1 = controlPoints.at(startIndex - 1);
            vec p2 = controlPoints.at(startIndex + 0);
            vec p3 = controlPoints.at(startIndex + 1);
            vec p4 = controlPoints.at(startIndex + 2);

            double t = u;
            while(t > 1) t -= 1; 

            return (1   * (-3 * p1 + 3 * p3)/6.0 +
                    2*t  * (3 * p1 - 6 * p2 + 3 * p3)/6.0 + 
                    3*t*t * (-p1 + 3 * p2 - 3 * p3 + p4)/6.0).normalize();
            
        }

         int getSegmentCount() const {
            return spline::getSegmentCount() - 2;
        }
    };
};

#endif
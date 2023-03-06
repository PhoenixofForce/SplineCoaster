#ifndef HERMITSPLINE_H
#define HERMITSPLINE_H

#include "./spline.h"

namespace math {

    class hermit_spline : public spline {
    public:

        hermit_spline(std::vector<vec> inControlPoints, std::vector<vec> inVelocities)
        : spline{inControlPoints}, velocities{ std::move(inVelocities) }
        { }

        vec get(double u) const { 
            if(u < 0) return get(0);
            if(u >= getSegmentCount()) return controlPoints.at(controlPoints.size() - 1);

            int startIndex = (int) std::floor(u); 
		
            const vec& startPosition{ controlPoints.at(startIndex) };
            const vec& startVelocity{ velocities.at(startIndex) };
            const vec& endPosition{ controlPoints.at(startIndex + 1) };
            const vec& endVelocity{ velocities.at(startIndex + 1) };

            double t = u;
            while(t > 1) t -= 1; 

            return (1  * (startPosition) + 
                    t   * (startVelocity) +
                    t*t  * (-3 * startPosition - 2 * startVelocity + 3 * endPosition - 1 * endVelocity) + 
                    t*t*t * (2 * startPosition + startVelocity - 2 * endPosition + endVelocity));

        }

        vec getDerivate(double u) const { 
            if(u < 0) return getDerivate(0);
            if(u > getSegmentCount()) return getDerivate(getSegmentCount());

            int startIndex = (int) std::floor(u);
		
            const vec& startPosition{ controlPoints.at(startIndex) };
            const vec& startVelocity{ velocities.at(startIndex) };
            const vec& endPosition{ controlPoints.at(startIndex + 1) };
            const vec& endVelocity{ velocities.at(startIndex + 1) };

            double t = u;
            while(t > 1) t -= 1; 

            return (1   * (startVelocity) +
                    2*t  * (-3 * startPosition - 2 * startVelocity + 3 * endPosition - 1 * endVelocity) + 
                    3*t*t * (2 * startPosition + startVelocity - 2 * endPosition + endVelocity)).normalize();
        }

    protected:
        std::vector<vec> velocities;

        void setVelocities(std::vector<vec> inVelocities) {
            velocities = std::move(inVelocities);
        }
    };
};

#endif
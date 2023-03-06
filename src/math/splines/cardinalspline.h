#ifndef CARDINALSPLINE_H
#define CARDINALSPLINE_H

#include "./spline.h"
#include "./hermitspline.h"

namespace math {

    class cardinal_spline : public hermit_spline {
    public:

        cardinal_spline(std::vector<vec> inControlPoints, double scale = 1, bool loop = false)
        : hermit_spline{inControlPoints, {} }
        { 
            std::vector<vec> velocities{};
            int lastIndex = controlPoints.size() - 1;

            velocities.push_back((controlPoints.at(1) - controlPoints.at(0)) * 2);
            for(int i = 1; i < lastIndex; i++) velocities.push_back(controlPoints.at(i + 1) - controlPoints.at(i - 1));
            velocities.push_back((controlPoints.at(lastIndex) - controlPoints.at(lastIndex - 1)) * 2);
            
            if(!loop) {
                velocities.push_back(velocities.at(0));
                controlPoints.push_back(controlPoints.at(0));
            }

            setVelocities(velocities);
        }
    
    };
};

#endif
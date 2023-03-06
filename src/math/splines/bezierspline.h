#ifndef BEZIERSPLINE_H
#define BEZIERSPLINE_H

#include "./spline.h"
#include "./cubicbezier.h"

namespace math {

    class bezier_spline : public spline {
    public:

        bezier_spline(std::vector<vec> controlPoints)
        : spline{controlPoints}
        { }

        vec get(double u) const { 
            if(u < 0) return get(0);
            if(u > getSegmentCount()) return get(getSegmentCount());
            if(u == getSegmentCount()) return controlPoints.at(controlPoints.size() - 1);

            int startIndex = std::floor(u) * 3; // 4 points form a cubic bezier curve

            double t = u;
            while(t > 1) t -= 1; 
            
            return cubic_bezier{ controlPoints.at(startIndex), controlPoints.at(startIndex + 1), controlPoints.at(startIndex + 2), controlPoints.at(startIndex + 3) }
                    .get(t);
        }

        vec getDerivate(double u) const {
            if(u < 0) return getDerivate(0);
            if(u > getSegmentCount()) return getDerivate(getSegmentCount());
            if(u == getSegmentCount()) return getDerivate(getSegmentCount() - 0.000001);

            // 4 points form a cubic bezier curve
            // 0 - 1 - 2 - 3 - 4 - 5 - 6
            // ^           ^           ^
            // so multiplying by 3 gets the control points that lay on the curve
            int startIndex = std::floor(u) * 3;
            
            double t = u;
            while(t > 1) t -= 1; 
            
            return cubic_bezier{ controlPoints.at(startIndex), controlPoints.at(startIndex + 1), controlPoints.at(startIndex + 2), controlPoints.at(startIndex + 3) }
                    .getDerivate(t);
        }

        int getSegmentCount() const {
            return std::floor(spline::getSegmentCount() / 3);
        }

    private:
        
    };
};

#endif
#ifndef BEZIER_H
#define BEZIER_H

#include "./spline.h"

namespace math {

    class bezier : public spline {
    public:

        bezier(std::vector<vec> inControlPoints)
        : spline{ inControlPoints}
        { }

        vec getPositionByPercentage(double t) {
            return get(t * getSegmentCount());
        }

        vec get(double u) const { 
            if(u < 0) return get(0);
            if(u > getSegmentCount()) return get(getSegmentCount());

            double t = u / getSegmentCount();
            std::vector<vec> casteljauInput{ controlPoints };

            while(casteljauInput.size() > 1) {
                std::vector<vec> casteljauOutput{};

                for(int i = 0; i < casteljauInput.size() - 1; i++) {
                    casteljauOutput.push_back(lerp(casteljauInput.at(i), t, casteljauInput.at(i + 1)));
                }

                casteljauInput = casteljauOutput;
            }

            return casteljauInput.at(0);
        }

        vec getDerivate(double u) const { 
            if(u < 0) return get(0);
            if(u > getSegmentCount()) return get(getSegmentCount());

            double t = u / getSegmentCount();
            std::vector<vec> casteljauInput{ controlPoints };

            while(casteljauInput.size() > 2) {
                std::vector<vec> casteljauOutput{};

                for(int i = 0; i < casteljauInput.size() - 1; i++) {
                    casteljauOutput.push_back(lerp(casteljauInput.at(i), t, casteljauInput.at(i + 1)));
                }

                casteljauInput = casteljauOutput;
            }

            return casteljauInput.at(1) - casteljauInput.at(2);
        }

    private:

        vec lerp(vec start, double t, vec end) const {
            return vec{ start} * (1 - t) + vec{ end } * t;
        }
        
    };
};

#endif
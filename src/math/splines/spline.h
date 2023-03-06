#ifndef SPLINE_H
#define SPLINE_H

#include <math.h>
#include <vector>
#include <algorithm>

#include "raylib.h"
#include "../vector.h"
#include "../../point.h"

namespace math {

    /*
     * Given a series of n points, that form a path
     *
     *  p0-----p1
     *         /
     *        / 
     *       /
     *      p2-----p3
     * 
     * Then a spline will calculate for each u in [0, n] the point that lays on a curve that
     * runs (ideally)  through the given points.
     * 
     * Also u values between [i, i+1] result in points that lay on curve 
     * between the given i-th and (i+1)-th point.
     * 
     * The path from one point to the next is called a segment. Ex p1-p2
     * t-values denote the progress on a segment.
     */
    class spline {
    public:

        spline(std::vector<vec> inControlPoints) 
        :controlPoints{ std::move(inControlPoints) }
        { }

        oriented_point getOrientedPoint(double u) const {
            vec point{ get(u) };

            vec tangent{ getDerivate(u) };
            vec binormal{ tangent.cross(vec::vec3d(0, 1, 0)).normalize() };
            vec normal{ tangent.cross(binormal).normalize() };

            matrix orientation{ lookRotation(tangent, normal) };
            return oriented_point{ point, orientation };
        }

        virtual vec get(double u) const { 
            if(u < 0) return get(0);
            if(u >= getSegmentCount()) return get(getSegmentCount());

            int startIndex = (int) std::floor(u);
            vec start = controlPoints.at(startIndex);
            vec end = controlPoints.at(startIndex + 1);
            
            double t = u;
            while(t > 1) t -= 1;

            return vec{ start}  + (end - start) * t;
        }

        virtual vec getDerivate(double u) const { 
            if(u < 0) return getDerivate(0);
            if(u > getSegmentCount()) return getDerivate(getSegmentCount());

            int startIndex = (int) std::floor(u);
            vec start = controlPoints.at(startIndex);
            vec end = controlPoints.at(startIndex + 1);
            
            double t = u;
            while(t > 1) t -= 1;

            return ((end - start)).normalize();
        }

        vec operator()(double u) const {
            return get(u);
        }

        virtual int getSegmentCount() const {
            return controlPoints.size() - 1;
        }

        double estimateLength(double sampleRate = 0.05, double lastU = -1) const {   
            if(lastU < 0) lastU = getSegmentCount();
            double length = 0;

            vec last{ get(0) };
            for(double u = sampleRate; u < lastU; u++) {
                vec current{ get(u) };
                length += current.distanceTo(last);
            }
            length += last.distanceTo(get(lastU));

            return length;
        }

    protected:
        std::vector<vec> controlPoints{};

        void setControlPoints(std::vector<vec> inControlPoints) {
            controlPoints = std::move(inControlPoints);
        }

    private:

    };
}

#endif
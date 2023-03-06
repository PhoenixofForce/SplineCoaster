#ifndef POINT_H
#define POINT_H

#include "math/matrix.h"
#include "math/vector.h"

struct oriented_point {
    math::vec position;
    math::matrix rotation;

    math::vec localToWorld(math::vec point) {
        return position + rotation * point;
    }

    math::vec worldToLocal(math::vec point) {
        return rotation.inverse() * (point - position);
    }

    math::vec worldToLocalDirection(math::vec point) {
        return rotation * point;
    }
};

#endif
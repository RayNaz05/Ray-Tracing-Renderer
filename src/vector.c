#include "vector.h"
#include <math.h>

Vec3 add(Vec3 v1, Vec3 v2) {
    return (Vec3){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

Vec3 subtract(Vec3 v1, Vec3 v2) {
    return (Vec3){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

Vec3 scalarMultiply(float s, Vec3 v) {
    return (Vec3){s * v.x, s * v.y, s * v.z};
}

Vec3 scalarDivide(Vec3 v, float d) {
    return (d != 0) ? (Vec3){v.x / d, v.y / d, v.z / d} : v;
}

Vec3 normalize(Vec3 v) {
    float len = length(v);  
    if (len != 0) {
        return scalarDivide(v, len);
    } else {

        Vec3 zero = {0, 0, 0};
        return zero;
    }
}

float dot(Vec3 v1, Vec3 v2) {
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

float length2(Vec3 v) {
    return dot(v, v);
}

float length(Vec3 v) {
    return sqrt(length2(v));
}
float distance2(Vec3 v1, Vec3 v2) {
    float dx = v1.x - v2.x;
    float dy = v1.y - v2.y;
    float dz = v1.z - v2.z;
    return dx * dx + dy * dy + dz * dz;
}

float distance(Vec3 v1, Vec3 v2) {
    float dx = v1.x - v2.x;
    float dy = v1.y - v2.y;
    float dz = v1.z - v2.z;
    return sqrt(dx * dx + dy * dy + dz * dz);
}
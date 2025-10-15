#include "spheres.h"
#include <stdlib.h>
#include <math.h>

void worldInit(World *world) {
    world->size = 0;
    world->capacity = 1;
    world->spheres = malloc(sizeof(Sphere*));
}

void freeWorld(World *world) {
    for (int i = 0; i < world->size; i++) {
        free(world->spheres[i]);
    }
    free(world->spheres);
}

void addSphere(World *world, Sphere *sphere) {
    if (world->size >= world->capacity) {
        world->capacity +=1;
        Sphere **newArray = realloc(world->spheres, world->capacity * sizeof(Sphere*));
        world->spheres = newArray;
    }
    world->spheres[world->size++] = sphere;
}

Sphere *createSphere(float radius, Vec3 position, Vec3 color) {
    Sphere *sphere = malloc(sizeof(Sphere));
    sphere->r = radius;
    sphere->pos = position;
    sphere->color = color;
    return sphere;
}

int doesIntersect(const Sphere *sphere, Vec3 rayPos, Vec3 rayDir, float *t) {
    Vec3 oc = subtract(rayPos, sphere->pos);
    float a = dot(rayDir, rayDir);          
    float b = 2.0f * dot(oc, rayDir);       
    float c = dot(oc, oc) - sphere->r * sphere->r; 

    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return 0;
    }
    float sqrtDiscriminant = sqrtf(discriminant);
    float t1 = (-b - sqrtDiscriminant) / (2.0f * a);
    float t2 = (-b + sqrtDiscriminant) / (2.0f * a);

    if (t1 > 0.001f && t2 > 0.001f) {
        *t = fminf(t1, t2); 
        return 1;
    } else if (t1 > 0.001f) {
        *t = t1; 
        return 1;
    } else if (t2 > 0.001f) {
        *t = t2; 
        return 1;
    }
    return 0;
}
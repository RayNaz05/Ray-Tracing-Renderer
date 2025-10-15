#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "spheres.h"
#include <math.h>
#include <string.h>
#include "color.h"

Vec3 cameraPos={.x=0,.y=0,.z=0};
float focalLength;

typedef struct {
    float width;
    float height;
} Viewport;

void initViewport(Viewport *viewport, float height, float imgWidth, float imgHeight) {
    float aspectRatio = imgWidth/ imgHeight;
    viewport->height = height;
    viewport->width = height*aspectRatio;
}

typedef struct {
    Vec3 position;
    float brightness;
}Light;
Light lightSource;

Vec3 backgroundColor;
int imgWidth, imgHeight;
float vportHeight;
Viewport viewport;
int noColours, bgcolor, nospheres;

Vec3 calculate_ray_direction(int pixel_x, int pixel_y, int image_width, int image_height) {
    float aspectRatio = (float)image_width / (float)image_height;
    float scale = viewport.height / 2.0;
    float x = (1 - 2 * ((pixel_x + 0.5) / image_height)) * scale;
    float y = (2 * ((pixel_y + 0.5) / image_width) - 1) * aspectRatio * scale;
    Vec3 rayDir = {y, x, -focalLength};
    return normalize(rayDir);
}

Vec3 calculate_lighting(Vec3 point, Vec3 normal, Vec3 sphereColor, Vec3 light_pos, float brightness) {
    Vec3 lightDir = subtract(light_pos, point);
    lightDir = normalize(lightDir);

    float dotProduct = fmaxf(0.0f, dot(lightDir, normal));  
    float distanceSquared = distance2(light_pos, point);
    float intensity = brightness * dotProduct / distanceSquared;

    intensity = fminf(intensity, 1.0f);
    Vec3 color = scalarMultiply(intensity, sphereColor);
    return color;
}

void MS_2(Sphere *spheres, int nospheres,FILE *f2) {
    fprintf(f2, "P3\n%d %d\n255\n", imgWidth, imgHeight);

    for (int x = 0; x < imgHeight; x++) {
        for (int y = 0; y < imgWidth; y++) {
            Vec3 fresh = {0.0f, 0.0f, 0.0f};
            for (int a = 0; a < 3; a++) {
                for (int b = 0; b < 3; b++) {
                    float r1 = (x + (a - 1) + 0.5f) / imgWidth; 
                    float r2 = (y + (b - 1) + 0.5f) / imgHeight;

                    Vec3 rayDir = calculate_ray_direction(r1 * imgWidth, r2 * imgHeight, imgWidth, imgHeight);
                    Vec3 rayPos = cameraPos;
                    float closestT = INFINITY;
                    int closestSphere = -1;

                    for (int i = 0; i < nospheres; i++) {
                        float t;
                        if (doesIntersect(&spheres[i], rayPos, rayDir, &t)&& t<closestT) {
                            closestT = t;
                            closestSphere = i;
                        }
                    }

                    Vec3 pixel_color = backgroundColor;
                    if (closestSphere!=-1) {
                        Vec3 intersectionPoint = add(rayPos, scalarMultiply(closestT, rayDir));
                        Vec3 normal = normalize(subtract(intersectionPoint, spheres[closestSphere].pos));
                        pixel_color = calculate_lighting(intersectionPoint, normal, spheres[closestSphere].color, lightSource.position, lightSource.brightness);

                        Vec3 lightDir = normalize(subtract(lightSource.position, intersectionPoint));

                        int inShadow = 0;

                        Vec3 shadowRayPos = add(intersectionPoint, scalarMultiply(0.001f, lightDir));
                        float lightDistance = length(subtract(lightSource.position, intersectionPoint)); 
                        float t;

                        for (int i = 0; i < nospheres; i++) {
                            if (doesIntersect(&spheres[i], shadowRayPos, lightDir, &t)) {
 
                                if (t > 0 && t < lightDistance) {
                                    inShadow = 1; 
                                    break;
                                }
                            }
                        }
                        if (inShadow) {
                            pixel_color = scalarMultiply(0.1f, pixel_color);
                        }

                    }
                    fresh = add(pixel_color,fresh);
                }
            }
            
            fresh = scalarMultiply(1.0f/9.0f, fresh);
            int r = (int)(fminf(255.0f, fmaxf(0.0f, fresh.x * 255)));
            int g = (int)(fminf(255.0f, fmaxf(0.0f, fresh.y * 255)));
            int b = (int)(fminf(255.0f, fmaxf(0.0f, fresh.z * 255)));
            fprintf(f2, "%d %d %d\n", r, g, b);
        }
        fprintf(f2, "\n");
    }
    fclose(f2);
}

int main(int argc,char *argv[]) {
    (void)argc;
    FILE *f = fopen(argv[1], "r");
    fscanf(f, "%d %d",&imgWidth, &imgHeight);
    fscanf(f, "%f", &vportHeight);
    fscanf(f, "%f", &focalLength);
    fscanf(f, "%f %f %f %f", &lightSource.position.x, &lightSource.position.y, &lightSource.position.z, &lightSource.brightness);

    initViewport(&viewport, vportHeight, imgWidth, imgHeight);

    fscanf(f, "%d", &noColours);
    int *colors = malloc(noColours * sizeof(int));
    Vec3 *colorRGB = malloc(noColours * sizeof(Vec3));
    for (int i = 0; i < noColours; i++) {
        fscanf(f, "%x", &colors[i]); 
    }

    qsort(colors, noColours, sizeof(int), compareColor);
    for (int i = 0; i < noColours; i++) {
        colorRGB[i] = unpackRGB(colors[i]); 
    }

    fscanf(f, "%d", &bgcolor);
    fscanf(f, "%d", &nospheres);

    Sphere *spheres = malloc(nospheres * sizeof(Sphere));
    int *sphereColorIndices = malloc(nospheres * sizeof(int)); 

    for (int i = 0; i < nospheres; i++) {
        int colorIndex;
        fscanf(f, "%f %f %f %f %d", &spheres[i].pos.x, &spheres[i].pos.y, &spheres[i].pos.z, &spheres[i].r, &colorIndex);
        sphereColorIndices[i] = colorIndex; 
        spheres[i].color = (Vec3){1.0f, 1.0f, 1.0f}; 
    }

    #ifdef FS
    for (int i = 0; i < nospheres; i++) {
        spheres[i].color = colorRGB[sphereColorIndices[i]]; 
    }
    backgroundColor = colorRGB[bgcolor];
    #endif

    fclose(f);
    free(sphereColorIndices); 

    #ifdef MS1
    Vec3 spherecolor = {.x = 1, .y = 1, .z = 1};
    FILE *f1 = fopen(argv[2], "w");
    Vec3 resultadd = add(backgroundColor, lightSource.position);
    Vec3 resultsubtract = subtract(backgroundColor, lightSource.position);
    Vec3 resultmultiply = scalarMultiply(viewport.width, lightSource.position);
    Vec3 resultnormalize = normalize(lightSource.position);

    typedef struct {
        Vec3 rdiv;
        float rdot,rlen,rdis;
    } SphereResults;
    SphereResults *result = malloc(nospheres * sizeof(SphereResults));

    for (int i = 0; i < nospheres; i++) {
        result[i].rdiv = scalarDivide(spherecolor, spheres[i].r);
        result[i].rdot = dot(lightSource.position, spheres[i].pos);
        result[i].rdis = distance(lightSource.position, spheres[i].pos);
        result[i].rlen = length(spheres[i].pos);
    }
    fprintf(f1, "(0.0, 0.0, 0.0) + (%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",lightSource.position.x, lightSource.position.y, lightSource.position.z, resultadd.x, resultadd.y, resultadd.z);
    fprintf(f1, "(0.0, 0.0, 0.0) - (%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",lightSource.position.x, lightSource.position.y, lightSource.position.z, resultsubtract.x, resultsubtract.y, resultsubtract.z);
    fprintf(f1, "%.1f * (%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",viewport.width, lightSource.position.x, lightSource.position.y, lightSource.position.z, resultmultiply.x, resultmultiply.y, resultmultiply.z);
    fprintf(f1, "normalize(%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",lightSource.position.x, lightSource.position.y, lightSource.position.z, resultnormalize.x, resultnormalize.y, resultnormalize.z);

    for (int i = 0; i < nospheres; i++) {
        fprintf(f1, "\n(1.0, 1.0, 1.0) / %.1f = (%.1f, %.1f, %.1f)\n",spheres[i].r, result[i].rdiv.x, result[i].rdiv.y, result[i].rdiv.z);
        fprintf(f1, "dot((%.1f, %.1f, %.1f), (%.1f, %.1f, %.1f)) = %.1f\n",lightSource.position.x, lightSource.position.y, lightSource.position.z, spheres[i].pos.x, spheres[i].pos.y, spheres[i].pos.z, result[i].rdot);
        fprintf(f1, "distance((%.1f, %.1f, %.1f), (%.1f, %.1f, %.1f)) = %.1f\n",lightSource.position.x, lightSource.position.y, lightSource.position.z, spheres[i].pos.x, spheres[i].pos.y, spheres[i].pos.z, result[i].rdis);
        fprintf(f1, "length(%.1f, %.1f, %.1f) = %.1f\n",spheres[i].pos.x, spheres[i].pos.y, spheres[i].pos.z, result[i].rlen);
    }
    fclose(f1);
    free(result);
    #endif

    #ifdef MS2
    FILE *f2 = fopen(argv[2], "w");
    MS_2(spheres, nospheres, f2);
    #endif

    #ifdef FS
    FILE *f2 = fopen(argv[2], "w");
    MS_2(spheres, nospheres, f2);
    #endif

    free(colors);
    free(spheres);
    free(colorRGB);
    return 0;
}
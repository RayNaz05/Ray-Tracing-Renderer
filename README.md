# 🌌 Ray Tracing Renderer (C)

A lightweight **CPU-based Ray Tracing Renderer** written in **C**, built as part of a university graphics programming course.  
The project demonstrates the fundamentals of **computer graphics**, including vectors, rays, lighting, shading, and anti-aliasing — all implemented from scratch without external graphics libraries.

---

## 🧭 Concept Overview

Ray tracing simulates how light interacts with objects by tracing rays from a virtual **camera** through a **viewport** into a 3D scene.  
Each ray checks for intersections with scene geometry (e.g., spheres), determines lighting and shading at the hit point, and outputs a pixel color.

<p align="center">
   <img src="images/image_2025-10-14_223237277.png" width="85%" alt="Ray tracing concept diagram showing camera rays hitting a scene through a viewport" />
</p>

---

## 🧩 Features

- Implements **ray–sphere intersection detection** and **diffuse (Lambertian) lighting**
- Supports **shadow rendering** from one or more light sources
- Includes **anti-aliasing** via multi-sample pixel averaging
- Modular structure with **vector**, **color**, and **geometry** components
- Renders final images in **PPM format** for easy viewing and debugging

---

## 🧠 Technologies Used

- **Language:** C  
- **Concepts:** Ray tracing, linear algebra, shading models, anti-aliasing  
- **Tools:** GCC / Clang, Makefile for build automation  

---

## 👤 My Contribution

- Implemented the **vector math library** and **ray–sphere intersection** logic  
- Added **lighting and shadow calculations** using Lambertian reflection  
- Developed **anti-aliasing** through stochastic pixel sampling  
- Integrated all modules into a working ray tracer that generates `.ppm` image outputs  

---

## 🎨 Rendered Output

<p align="center">
  <img src="images/image_2025-10-14_223325068.png" width="85%" alt="Final rendered image showing multiple colored spheres and shadows on a green plane" />
</p>

The image above demonstrates ray–sphere intersections, diffuse shading, and shadow rendering using a single light source.

---

## ⚙️ 2. Build the Program

If you have GCC installed, compile using:

``` bash
gcc src/*.c -lm -DMS2 -o raytracer.exe
```

💡 The -DMS2 flag enables the Milestone 2 rendering mode.

If you prefer using make:

make

## 🖼️ 3. Run the Renderer
``` bash
raytracer.exe scene.txt output.ppm
```

scene.txt → defines the camera, lights, colors, and spheres in the scene.

output.ppm → is the rendered image output written in PPM format.

After running, you’ll see a new file named output.ppm in the project folder.

## 🌈 4. View the Output

You can open output.ppm in several ways:

-Windows: GIMP
 -or Paint.NET

-macOS/Linux:

-open output.ppm


-VS Code: install the Image Preview extension and open the file directly.

-Convert to PNG (optional):

-magick convert output.ppm output.png

## 📄 Scene File (scene.txt)

scene.txt defines the scene configuration used by the renderer — including image size, camera setup, light position, and objects.
It acts as an input script for the ray tracer, allowing different renders without changing the source code.

Example:
``` bash
400 225
2.0
1.0
2.0 5.0 -3.0 800.0
4
FF0000
00FF00
0000FF
FFFFFF
3
3
0.0 0.0 -5.0 1.0 0
2.0 0.5 -6.0 0.8 1
-1.5 0.3 -4.0 0.6 2
```
## ⚖️ Academic Note

This project was developed as part of a University of Alberta CMPUT course.
Certain starter code and structure were provided by the teaching staff.
This repository reflects my individual work and modifications on top of that foundation.

## 📫 Author

Rayhan nazeel
🎓 University of Alberta — Computing Science
📧 Rayhannazeel@outlook.com


---

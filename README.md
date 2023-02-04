# RenderToy
RenderToy is an experimental path tracing rendering library for academic purposes.

## We implement:
* Fullly multi-threaded path-traced GI.
    * Direct Light Sampling (DLS).
        * It also produces fast-GI results.
    * Disney PBR BSDF.
    * Multiple importance sampling (MIS).
* Triangulated mesh system.
    * Bounding Volume Hierarchy (BVH) acceleration structure.
* Physically based perspective camera.
* Multi-pass ray-casting renderer, including normal pass, albedo pass & depth pass.
    * Can be linked with denoising library such as OIDN & Nvidia OptiX.
* Physically based principled BSDF material system.
* A compositor for post-processing.
    * Gaussian blur.
    * Convolution with customized kernels.
* File IO.
    * Importing wavefront OBJ model file.
    * Exporting PPM & BMP image file.

## Our plan:
* Procedural texture and mesh module.
* A richer compositor.
* Importing SDR/HDR texture image.
* Exporting HDR image.
* Physical sky.
* Code optimization.

# Graphics-on-GPU
This is the implementation of computer graphics on a GPU using Vulkan written for CS 311 Computer Graphics, taught By Josh Davis.
More than running computer graphics on the GPU, this project also contains shaders in the OpenGL language, scene-wide and body specific uniforms, scene graphs, lighting using the Phong Illumination Model.

To run  on mac just type
```
glslc 590shader.vert -o 590vert.spv
glslc 590shader.frag -o 590frag.spv
clang 590mainDiffuse.c -lglfw -lvulkan
```
Or on Linux 
```
/mnt/c/VulkanSDK/1.3.216.0/Bin/glslc.exe 590shader.vert -o 590vert.spv
/mnt/c/VulkanSDK/1.3.216.0/Bin/glslc.exe 590shader.frag -o 590frag.spv
clang 590mainDiffuse.c -lglfw -lvulkan -lm
```
You might need to change the 1.3.216 to the version of Vulkan running on your computer. To move the character, use W,A,S,D

The files were originally written as homework exercises to be submitted before every class. The file naming convention of the class was quite strict, files which start with a larger number were written later in the term. Some files were the updated/upgraded version of homework assignments delivered earlier. For example, 080triangle is the same as 060triangle but with additional features (as it was done later). Files where the name start with main can be executed, and were used to test/debug the code throughout the term. Feel free to compile/run them. 

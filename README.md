# Mesh Simplification
## Introduction
An OBJ file is a standard 3D image format. And the model depicted in such kind of file is made of various kinds of triangle meshes.  
![avatar](doc/Dolphin.png)
There is a need to simplify such models to reduce the calculation workload and the effect won't be affected when looked from a relatively far view point.  
Edge collapse method based on quadric error matrix which is posed by Garland M., Heckbert P S works very well.
## OBJ file Format
OBJ files we use has the following format:
```
v 1  0 1
v 1  1 1
v 0  1 1
f 1 2 6
f 1 6 5
f 2 3 7
f 2 7 6
```
v means vertex and the following is the coordinate of the vertex.  
f means face and the following are the indexes of three vertexes made of the triangle mesh.
## Edge Collapse Algorithm
The main idea of edge collapse is to merge two vertexes or an Edge to one vertex.  
![avatar](doc/introduction.png)
For any vertex in a model, it has may adjacent planes.  
![avatar](doc/plane.png)
The error of moving one vertex to another place can be measured using the following equation.  
![avatar](doc/err.png)
![avatar](doc/Kp.png)
We can get the coordinate of the merged vertex by solving the following equations.  
![avatar](doc/ECv.png)
## Compile and Run

## Effect

## Author :
Cassiopée Gossin - M2 CMI informatique <br>
2026 - 2027

### Fork :
This project is forked from [PointCloudCurvCNC](https://github.com/JacquesOlivierLachaud/PointCloudCurvCNC). 

# Filter and reconstruction project 
## About :
This is a course project about 3D reconstruction.  
The goal is to find ways to filter or move points from a point cloud to remove outliers and reconstruct the 3D model as accurately as possible.  

## Reading progress :
To learn more about various reconstructions technics, I'm reading many scientific papers, thesis and publications.  
This is a summary of what I read or what I plan to read.  
| Name | To read | Reading | Read |
| ------------------ | ------------------ | ------------------ | ------------------ | 
| Advancing Front Surface Reconstruction | X |  |  |
| Algebraic Point Set Surface |  |  | X |
| Cocone | X |  |  |
| Dual Contouring |  |  | X |
| Efficient Surface Normal Estimation for LiDAR Point Clouds |  | X |  |
| LiDAR Aquisition (fr) | X |  |  |
| Marching Cube |  |  | X |
| Poisson Reconstruction |  |  | X |
| Robust Cocone | X |  |  |
| Super Cocone | X |  |  |
| Scale-Space Surface Reconstruction | X |  |  |
| Tight Cocone | X |  |  |
| Winding Number |  |  | X |

## Code progress : 
### Naive Filter :
First I tried a naive version to filter points. This function create, for each point, an average point from many points around it. If the point we are looking at is too far from the average point, it's errased. The number of point taken into account to create the average point and the maximum accepted distance are decided by the user.  
The goal of this first filter is to erase isolated outliers. 

<u>**Results :**</u>  

|![before](images/readme/naiveFilterBefore.png) | ![after1](images/readme/naiveFilter10-5.png) | ![after2](images/readme/naiveFilter100-05.png) |
| ---- | ---- | ---- |
| Before (no changes) | After : 10 neighbour and 5 max distance | After : 100 neighbour and 0.5 max distance |  

<u>**Pros :**</u>
- we can see some results 

<u>**Cons :**</u>
- too naive and not optimised
- can erase no-outliers points
- inefficient finding neighbour technique

<u>**Conclusion :**</u>  
This version is not robust enough to give it serious consideration. 

### Other methods in progress ... 

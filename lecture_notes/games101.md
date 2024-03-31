# ComputerGraphic

计算机图形学-渲染
现代计算机图形学入门(闫令琪)
Overview of Computer Graphics
资源
[课程ppt]( https://games-cn.org/graphics-intro-ppt-video/)
[在线平台](https://games-cn.org/) 

### Applications

游戏，电影，动画，设计，可视化，虚拟现实(VR-AR)，数字插画Digital llustration，模拟simulation，图形UI，字体typography
Fundamental Intellectual Challenges
创建真实的虚拟世界并与之交互
需要了解物理世界的各个方面
新的计算方法、显示、技术

### Technical Challenges

- (透视)投影、曲线、曲面的数学

- 照明和遮阳物理学
- 用3D表示/操作形状
- 动画/仿真
- 3D绘图软件编程和硬件

### Rasterization光栅化

Project geometry primitives (3D triangles / polygons) onto the screen
投影几何原语(三维三角形/多边形)到屏幕上
Break projected primitives into fragments (pixels)
将投影原语分解为片段(像素)
Gold standard in Video Games (Real-time Applications) 实时显示，30fps
电子游戏 (实时应用)
Curves and Meshes曲线和网格
How to represent geometry in Computer Graphics
如何在计算机图形学中表示几何
Ray Tracing射线追踪
Shoot rays from the camera though each pixel
通过每个像素从照相机上拍摄光线
Calculate intersection and shading
计算交点和阴影

-Continue to bounce the rays till they hit light sources
-继续反射光线，直到它们击中光源
Gold standard in Animations / Movies (Offline Applications)
动画/电影中的金本位(离线应用)
//tradeoff
Animation / Simulation动画/仿真
Key frame Animation
关键帧动画
Mass-spring System
质量弹簧系统

Using OpenGL / DirectX / Vulcan
The syntax of Shaders

Computer Vision / Deep Learning

![image-20230325222439660](https://raw.githubusercontent.com/akingse/my-picbed/main/image-20230325222439660.png)



---

[课程](http://www.cs.ucsb.edu/~lingqi/teaching/games101.html)

### 课程大纲：

## 【1】：计算机图形学概述

[官网](https://learnopengl-cn.github.io/) 



## 【2】：向量与线性代数 Review of Linear Algebra

基础数学
线性代数、计算、数据处理
基础物理 ：光学、力学
信号处理、数值分析、美学
More dependent on Linear Algebra
更多依赖于线性代数
Vectors (dot products, cross products,)
Matrices (matrix-matrix, matrix-vector mult., ...)

#### 基础概念

默认列向量，模长，单位向量，加法
点乘 Dot (scalar) Product
性质：交换律、结合律、分配律
Find angle between two vectors寻找两个向量间的角度
Finding projection of one vector on another求一个向量在另一个向量上的投影
Measure how close测量有多近
Decompose a vector分解向量 （perp垂直向量）
Determine forward /backward 确定前进/倒向（光追判定用）
叉乘
Cross product is orthogonal to two initial vectors交叉积与两个初始向量正交
Direction determined by right-hand rule由右手法则决定的方向
Useful in constructing coordinate systems (later)在构造坐标系统方面很有用(稍后)
叉乘设定：x*y=z右手系，x*y=-z左手系
unity左手系，direct3D左手系，OpenGL右手系
Determine left / right确定左/右
Determine inside / outside确定内/外（点在三角形内部）
判断点在三角形内部，单边法则（corner case）
坐标系正交基 Orthonormal bases and coordinate frames
矩阵
数乘，矩阵乘矩阵 rows columns行列
无交换律，有结合律，分配律，转置，逆矩阵； 
叉乘矩阵

![image-20230325222706284](https://raw.githubusercontent.com/akingse/my-picbed/main/image-20230325222706284.png)

## 【3】：基础变换（二维）

Why study transformation .为什么要学习转变。
modeling模型变换，view视图变换，坐标系运动学变换，投影成像
2D transformations: rotation, scale, shear 2D变换：旋转、缩放、剪切
scale
reflect反射
shear切变
horizontal 水平
vertical 竖直
Homogeneous coordinates齐次坐标
Why homogeneous coordinates为什么齐次坐标
tradeoff，为了表示平移引入齐次坐标；

![image-20230325222905142](https://raw.githubusercontent.com/akingse/my-picbed/main/image-20230325222905142.png)

 齐次坐标系下的增加维度，齐次坐标系下的点相加，得到这两个点的中电； 

- Affine transformation仿射变换
- Composing transforms复合变换
- 3D transformations三维变换
- projection 投影



## 【4】：基础变换（三维）

Transformation Cont 续
正交矩阵（inverse=transpose）
公式 Rodrigues’ Rotation Formula
视图和投影变换 View/Projection
MVP变换（model-view-projection）
Viewing (观测) transformation

View (视图) / Camera transformation 
相机有一个默认设置，朝向z负方向
$$
M_{view}=R_{view} T_{view}
$$

相机变换公式，模型由相对位置，应用同一个矩阵

![image-20230325223352632](https://raw.githubusercontent.com/akingse/my-picbed/main/image-20230325223352632.png)



## 【5】：三维到二维变换（模型、视图、投影）

#### Projection (投影) transformation 

三维到二维的投影

**Orthographic projection**   (正交投影) 

camera在origin，看向-z，

canonical cube标准立方体，[-1, 1]^3（包围盒？），右手系，左右lr，上下bt，远近nf

![image-20230725232511115](../../../../../AppData/Roaming/Typora/typora-user-images/image-20230725232511115.png)

 **Perspective** (透视) projection，近大远小，更接近人眼，应用更广泛；

 Frustum 截锥，这个n是相机距离near clip plane的距离，还是透视投影的参考点距离；

![image-20231005192156361](../../../../../AppData/Roaming/Typora/typora-user-images/image-20231005192156361.png)












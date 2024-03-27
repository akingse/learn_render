## 【13】：光线追踪（基本原理）GAMES101_Lecture_13

 光栅化无法解决全局效果 global effects  （软阴影，glossy反射，间接光照）

光线追踪，非常慢，适合离线渲染；

光线三公理：1光沿直线传播，2光线不会碰撞collide，3从光源出发到相机过程光路可逆 reciprocity

pinhole小孔相机模型

设定：eye是一个点；光源是点光源；光线完美折射和反射；

做法：从像素投射光线，与场景相交得到交点，交点和光源连线，判定可见性和可见度，计算着色写回像素值；

![image-20240216234359150](../../../../../AppData/Roaming/Typora/typora-user-images/image-20240216234359150.png)

whitted风格

- primary ray  主射光线，
- secondary rays  弹射光线，
- shadow rays  投影光线

渲染：visible判断

几何：inside判断

DLSS深度学习超采样

RTXGI 实时全局光照ray tracing execute

**Uniform grids 网格化** 

**Spatial partitions  空间划分**

空间划分：Oct-tree，KD-tree，BSP-tree

KD-tree的问题，1同一个mesh可能会被多个node包含，2三角形与包围盒相交判断较难；

Bounding Volume Hierarchy (BVH)  层次包围体，主流做法

原来我之前写的kd-tree就是BVH，尴了个尬；

优化：选择最长的轴进行二分，查找中位数（快速选择算法，o(n)）



## 【14】：光线追踪（加速结构）

 

**Basic radiometry** (辐射度量学)  

Radiant flux辐射通量, 单位时间内的光量，lm流明
$$
Q[J=Joule] \\
\phi=dQ/dt \quad [W=watt]
$$

Radiant intensity光强, 光源向各个方向辐射能量，方向性的能量

intersity单位立体角上的能量；= 能量每单位立体角 solid angle
$$
I(\omega)=d\Phi/d\omega \\
[W/sr] [lm/sr=cd=candela] \\
\Omega=A/r^2
$$


![image-20240226231228772](../../../../AppData/Roaming/Typora/typora-user-images/image-20240226231228772.png)

irradiance辐射照度, 物体表面接收到的能量；

irradiance是单位面积的能量，垂直方向投影；cos(theta)

能量衰减，E=phi/4pi，E'=E/r^2，光源在传播过程中，intersity不变，irradiance在衰减；
$$
E(x)=d\Phi(x)/dA \\
[W/m^2]\quad [lm/m^2=lux]
$$
radiance  辐射亮度，光线在传播中如何度量能量；

能量-单位立体角，单位面积；
$$
L(p,\omega)=d^2\Phi(p,\omega)/d\omega dAcos\theta \\
[W/sr \  m^2]\quad [cd/m^2=lm/sr\ m^2=nit]
$$

- incident radiance 入射

- exiting radiance 出射


E=irradiance：dA区域收到的全部能量（integral积分，energy能量）

L=radiance：dA区域在dw方向接受的能量(light)

**BRDF 双向反射分布函数**

反射（漫/镜面）现象：光线发出，打到物体上，被反射出去，这个过程的函数（物理量）描述；

假设已知入射光和受光点 求反射光分布的函数 反之亦然 所以叫双向分布函数；

物体表面一个点在接收一束光照射之后会将这束光反射到空间半球中的任何一个可能的方向（取决于物体表面材质），BRDF描述的就是向某一个方向反射出去的光的“量”与这束入射光总的“量”的比值；

$$
f_r(\omega_i \rightarrow \omega_r)=dL_r(\omega_r)/dE_i(\omega_i)
$$

递归方程：物体表面不仅被光源照亮，还被其他物体的反射光线照亮；

渲染方程 RenderEquation，现代图形学的基础

![image-20240303224050126](../../../../../AppData/Roaming/Typora/typora-user-images/image-20240303224050126.png)
$$
L_o(p,\omega_o)= \\
L_e：自发光，e=Emission 发光体 \\
\Omega^+=H^2,表示半球，平面上的点只接收半球范围内的radiance \\
cos\theta=n\cdot \omega_i \\

L_i=Light \  incident
$$

解渲染方程，L = E + KL  算子分解

![image-20240306225245053](../../../../../AppData/Roaming/Typora/typora-user-images/image-20240306225245053.png)

发光体+直接光源+间接光照+二次反射

路径追踪， resolve render equation

概率论回顾

- X随机变量
- X~p(x) 概率分布
- 数学期望：E(X)=Σxipi
- 连续函数：E(X)=∫xp(x)dx 概率密度函数 PDF
- 随机变量函数的，数学期望 E(Y)=F[fx]=∫f(x)p(x)dx

通过概率的方法，去解渲染方程；

**蒙特卡洛积分**

积分域(a,b)内N等分

![image-20240304231859541](../../../../../AppData/Roaming/Typora/typora-user-images/image-20240304231859541.png)

**路径追踪 path tracing**

- diffuse漫反射
- glossy 哑光
- specular 镜面反射

幂级数问题，N=1的蒙特卡洛积分，叫路径追踪；（N!=1分布式光线追踪）

无限递归问题，俄罗斯轮盘赌概率停递归止，

**光源采样**

改变积分域，dw->dA

![image-20240312230852870](../../../../../AppData/Roaming/Typora/typora-user-images/image-20240312230852870.png)

- 1，对于光源部分，直接积分
- 2，其他反射，间接RR轮盘赌

Side Notes 旁注

路径追踪 indeed difficult

Blinn-Phong 光栅化

ray tracing==whitted style的光线追踪

path tracing

敬畏科学，这仅仅是个入门介绍；



- 【15】：路径追踪与光的传播理论

 

- 【16】：复杂外观建模与光的传播、实时光线追踪（前沿动态）
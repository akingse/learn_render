 

## 图形学中的数学-games001

仿真模拟应用

几何形状+物理规律+运动控制

 

Part I: 几何与代数

1 线性代数基础

向量空间：带有向量加法和标量乘法的非空集合(V)；

线性组合：Σa_i*u_i=0，互相表示；

图形学维度：低维（物理空间，欧氏空间，闵氏空间，颜色空间rgb）高维（图像像素，自由度向量）

线性映射：f:V->W

矩阵matrix，对线性映射的一种表示

单目运算：转置，行列式，迹，逆，伴随；

特征值eigen：λu=Au，一个向量u在特定线性变换A下的，作用是数乘；

矩阵多项式的特征值，判定矩阵的收敛性；

范数normed

内积空间

幺正空间unitary

齐次坐标

 

 

2 计算几何

3 旋转变换

4 主成分分析与奇异值分解

Part II: 数值方法

5 插值、拟合与采样

从离散数据到连续数据的途径；

 

6 谱分析与傅里叶变换

降维

 

7 概率论 (I)

8 概率论 (II)

光追渲染

 

Part III: 微分方程求解

for物理模拟

 9 场论初步

10 古典微分几何

11 微分方程

12 线性系统

Part IV: 优化与拓扑

虚实结合，人工智能相关

13 最优化

14 机器学习 (I)

15 机器学习 (II)

16 拓扑

## 图形学中的数学

### 点与多边形

凸多边形：面积法，绕序法；

任意多边形：光线投射法（射线法），回转法（winding）

点到三角形距离

三角形之间的距离

二维情形：定理：其中一个三角形必然位于顶点上，即，18组点边距离；

三维情形：6组点面+9组边边；

三角形碰撞检测

离散DCD

连续CCD

德劳内-三角剖分

凸包-凸多边形-包围盒，三维凸包（增量法O(n^2)）

平面最近点对2D

最小圆覆盖2D，最小包围圆，增量构造O(n)，AABB用于光线追踪，实时碰撞检测-球碰撞；

反演变换 inversion

三点共线，|p-o||p'-o|=r^2

 

### 奇异值分解与主成分分析SVD & PCA

矩阵背后更深层的含义

（物理）中间轴定理

 

### 旋转矩阵

镜像矩阵也是正交矩阵，但无法通过旋转得到；

旋转矩阵是特征值是+1的正交矩阵；

旋转可逆，满足结合律，不满足交换律；

流形manifold

局部展开是低维图形；

 

二维移动->三维旋转

旋转插值

旋转表示=流形参数化

四元数yyds，转换成轴角更容易理解；

 

图形学研发基础工具

windows开发环境

CMD与powershell，功能更发杂，更像一种高级语言；

 
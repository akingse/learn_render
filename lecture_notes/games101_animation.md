

## 【21】：动画与模拟（基本概念、逆运动学、质点弹簧系统）

 动画，让画面动起来；

虚拟现实，双目90fps；

Keyframe Animation 关键帧，中间部分插值得出

Physical Simulation  物理仿真

Mass Spring System  质点弹簧系统
$$
f_{a->b}=k_s(b-a)
$$
布料模拟

有限元

粒子系统，各种作用力，建模，解描述的作用力

正运动学，艺术家们不喜欢；

rigging 控制点，之间插值

动作捕捉





## 【21】：物质点法（前沿动态）

### Single particle simulation 单粒子模拟

Ordinary Differential Equation (ODE)  常微分方程



• Explicit Euler method 欧拉方法，前量欧拉，显式欧拉
$$
dx/dt=x'=v(x,t)
$$
数值方法解方程，会有误差和不稳定性问题；

• Instability and improvements 稳定性改进

- 1中点法；二次项优化；
- 2自适应步长
- 3隐式欧拉法 implicit

数值分析（还有一门重要的课，信号处理），龙格库塔，RK4

基于位置的，不基于物理，快速简单；



### Rigid body simulation 刚体模拟

$$
d/dt
$$

位置，速度，角度，角速度



### Fluid simulation 流体模拟

先模拟后渲染

- 刚体小球
- 假设不可压缩
- 给定时刻，根据密度修正，小球的位置（机器学习，梯度下降）

### 完结撒花





---

### 图形学

光栅化，手写渲染器

realtime rendering

学习一些着色器shader

几何，深厚的数学基础，微分几何，离散几何，拓扑流形；

光线传播

动画仿真



其他课程：实时渲染，复杂光追

以假乱真，无限月读；










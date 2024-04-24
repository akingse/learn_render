#pragma once
#include <vector>
#include <map>
#include <set>
#include <vector>
#include <optional>
#include <array>
#include <algorithm>
#include <math.h>
#include <iostream>
#include <stdexcept>
// D:\Alluser\akingse\TPL\vcpkg\packages\eigen3_x64-windows\include\Eigen
#include <Eigen/Dense> // .\vcpkg.exe install eigen3:x64-windows
// D:\Alluser\akingse\TPL\vcpkg\packages\opencv4_x64-windows\include\opencv2
// D:\Alluser\akingse\TPL\vcpkg\packages\opencv4_x64-windows\lib
#include <opencv2/opencv.hpp> // \.vcpkg.exe install opencv:x64-windows
#include "OBJ_Loader.h"

#include "calculateTransform.h" //eigen calculate
#include "Texture.hpp" //opencv read image
#include "Triangle.hpp" //triangle with render info
#include "Shader.hpp" //shader method
#include "rasterizer.hpp"

#ifndef M_PI
#define M_PI       3.14159265358979323846   // pi
#endif // !M_PI
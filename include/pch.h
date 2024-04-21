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
#include <Eigen/Dense> 
#include <opencv2/opencv.hpp>
#include "OBJ_Loader.h"

#include "calculateTransform.h" //eigen calculate
#include "Texture.hpp" //opencv read image
#include "Triangle.hpp" //triangle with render info
#include "Shader.hpp" //shader method
#include "rasterizer.hpp"

#ifndef M_PI
#define M_PI       3.14159265358979323846   // pi
#endif // !M_PI
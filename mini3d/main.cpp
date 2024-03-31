#include "pch.h"
#include <iostream>
#include <Eigen/Dense> //$(SolutionDir)..\TPL\eigen-3.4.0
#include <windows.h>
#include <tchar.h>
#ifdef _MSC_VER
#pragma comment(lib, "gdi32.lib") //GDI
#pragma comment(lib, "user32.lib")
#endif

/*
项目解析
https://blog.csdn.net/needmorecode/article/details/82846619

tiny-renderer项目
https://www.bilibili.com/video/BV1CP411U7D3/?spm_id_from=333.337.search-card.all.click&vd_source=dcb8b113123affd8262c0bfb1bbcbac3
https://blog.csdn.net/weixin_53459056/article/details/127284975?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522170220691916800211597484%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=170220691916800211597484&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduend~default-3-127284975-null-null.142^v96^control&utm_term=tinyrenderer&spm=1018.2226.3001.4187

*/

// using c++ style
using namespace std;
using namespace Eigen;
using namespace eigen;
using namespace mini3d;


// 设备初始化，fb为外部帧缓存，非 NULL 将引用外部帧缓存（每行 4字节对齐）
void device_init(Device* device, int width, int height, void* fb) {
	int need = sizeof(void*) * (height * 2 + 1024) + width * height * 8;
	char* ptr = (char*)malloc(need + 64);
	char* framebuf, * zbuf;
	int j;
	assert(ptr);
	device->framebuffer = (uint32_t**)ptr;
	device->zbuffer = (double**)(ptr + sizeof(void*) * height);
	ptr += sizeof(void*) * height * 2;
	device->texture = (uint32_t**)ptr;
	ptr += sizeof(void*) * 1024;
	framebuf = (char*)ptr;
	zbuf = (char*)ptr + width * height * 4;
	ptr += width * height * 8;
	if (fb != NULL) framebuf = (char*)fb;
	for (j = 0; j < height; j++) {
		device->framebuffer[j] = (uint32_t*)(framebuf + width * 4 * j);
		device->zbuffer[j] = (double*)(zbuf + width * 4 * j);
	}
	device->texture[0] = (uint32_t*)ptr;
	device->texture[1] = (uint32_t*)(ptr + 16);
	memset(device->texture[0], 0, 64);
	device->tex_width = 2;
	device->tex_height = 2;
	device->max_u = 1.0f;
	device->max_v = 1.0f;
	device->width = width;
	device->height = height;
	device->background = 0xc0c0c0;
	device->foreground = 0;
	transform_init(&device->transform, width, height);
	device->render_state = RENDER_STATE_WIREFRAME;
}

// 删除设备
void device_destroy(Device* device) {
	if (device->framebuffer)
		free(device->framebuffer);
	device->framebuffer = NULL;
	device->zbuffer = NULL;
	device->texture = NULL;
}

// 设置当前纹理
void device_set_texture(Device* device, void* bits, long pitch, int w, int h) {
	char* ptr = (char*)bits;
	int j;
	assert(w <= 1024 && h <= 1024);
	for (j = 0; j < h; ptr += pitch, j++) 	// 重新计算每行纹理的指针
		device->texture[j] = (uint32_t*)ptr;
	device->tex_width = w;
	device->tex_height = h;
	device->max_u = (double)(w - 1);
	device->max_v = (double)(h - 1);
}

// 清空 framebuffer 和 zbuffer
void device_clear(Device* device, int mode) {
	int y, x, height = device->height;
	for (y = 0; y < device->height; y++) {
		uint32_t* dst = device->framebuffer[y];
		uint32_t cc = (height - 1 - y) * 230 / (height - 1);
		cc = (cc << 16) | (cc << 8) | cc;
		if (mode == 0) cc = device->background;
		for (x = device->width; x > 0; dst++, x--) dst[0] = cc;
	}
	for (y = 0; y < device->height; y++) {
		double* dst = device->zbuffer[y];
		for (x = device->width; x > 0; dst++, x--) dst[0] = 0.0f;
	}
}

// 画点
void device_pixel(Device* device, int x, int y, uint32_t color) 
{
	if (((uint32_t)x) < (uint32_t)device->width && ((uint32_t)y) < (uint32_t)device->height) {
		device->framebuffer[y][x] = color;
	}
}

// 绘制线段
void device_draw_line(Device* device, int x1, int y1, int x2, int y2, uint32_t c) {
	int x, y, rem = 0;
	if (x1 == x2 && y1 == y2) {
		device_pixel(device, x1, y1, c);
	}
	else if (x1 == x2) {
		int inc = (y1 <= y2) ? 1 : -1;
		for (y = y1; y != y2; y += inc) device_pixel(device, x1, y, c);
		device_pixel(device, x2, y2, c);
	}
	else if (y1 == y2) {
		int inc = (x1 <= x2) ? 1 : -1;
		for (x = x1; x != x2; x += inc) device_pixel(device, x, y1, c);
		device_pixel(device, x2, y2, c);
	}
	else {
		int dx = (x1 < x2) ? x2 - x1 : x1 - x2;
		int dy = (y1 < y2) ? y2 - y1 : y1 - y2;
		if (dx >= dy) {
			if (x2 < x1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
			for (x = x1, y = y1; x <= x2; x++) {
				device_pixel(device, x, y, c);
				rem += dy;
				if (rem >= dx) {
					rem -= dx;
					y += (y2 >= y1) ? 1 : -1;
					device_pixel(device, x, y, c);
				}
			}
			device_pixel(device, x2, y2, c);
		}
		else {
			if (y2 < y1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
			for (x = x1, y = y1; y <= y2; y++) {
				device_pixel(device, x, y, c);
				rem += dx;
				if (rem >= dy) {
					rem -= dy;
					x += (x2 >= x1) ? 1 : -1;
					device_pixel(device, x, y, c);
				}
			}
			device_pixel(device, x2, y2, c);
		}
	}
}

// 根据坐标读取纹理
uint32_t device_texture_read(const Device* device, double u, double v) {
	int x, y;
	u = u * device->max_u;
	v = v * device->max_v;
	x = (int)(u + 0.5f);
	y = (int)(v + 0.5f);
	x = middle(x, 0, device->tex_width - 1);
	y = middle(y, 0, device->tex_height - 1);
	return device->texture[y][x];
}

//=====================================================================
// 渲染实现
//=====================================================================

// 绘制扫描线
void device_draw_scanline(Device* device, scanline_t* scanline) {
	uint32_t* framebuffer = device->framebuffer[scanline->y];
	double* zbuffer = device->zbuffer[scanline->y];
	int x = scanline->x;
	int w = scanline->w;
	int width = device->width;
	int render_state = device->render_state;
	for (; w > 0; x++, w--) {
		if (x >= 0 && x < width) {
			double rhw = scanline->v.rhw;
			if (rhw >= zbuffer[x]) {
				double w = 1.0f / rhw;
				zbuffer[x] = rhw;
				if (render_state & RENDER_STATE_COLOR) {
					double r = scanline->v.color.r * w;
					double g = scanline->v.color.g * w;
					double b = scanline->v.color.b * w;
					int R = (int)(r * 255.0f);
					int G = (int)(g * 255.0f);
					int B = (int)(b * 255.0f);
					R = middle(R, 0, 255);
					G = middle(G, 0, 255);
					B = middle(B, 0, 255);
					framebuffer[x] = (R << 16) | (G << 8) | (B);
				}
				if (render_state & RENDER_STATE_TEXTURE) {
					double u = scanline->v.tc.u * w;
					double v = scanline->v.tc.v * w;
					uint32_t cc = device_texture_read(device, u, v);
					framebuffer[x] = cc;
				}
			}
		}
		vertex_add(&scanline->v, &scanline->step);
		if (x >= width) break;
	}
}

// 主渲染函数
void device_render_trap(Device* device, trapezoid_t* trap) {
	scanline_t scanline;
	int j, top, bottom;
	top = (int)(trap->top + 0.5f);
	bottom = (int)(trap->bottom + 0.5f);
	for (j = top; j < bottom; j++) {
		if (j >= 0 && j < device->height) {
			trapezoid_edge_interp(trap, (double)j + 0.5f);
			trapezoid_init_scan_line(trap, &scanline, j);
			device_draw_scanline(device, &scanline);
		}
		if (j >= device->height) break;
	}
}

// 根据 render_state 绘制原始三角形
void device_draw_primitive(Device* device, const Vertex* v1, const Vertex* v2, const Vertex* v3) {
	Vector4d p1, p2, p3, c1, c2, c3;
	int render_state = device->render_state;

	// 按照 Transform 变化
	transform_apply(&device->transform, &c1, &v1->pos);
	transform_apply(&device->transform, &c2, &v2->pos);
	transform_apply(&device->transform, &c3, &v3->pos);

	// 裁剪，注意此处可以完善为具体判断几个点在 cvv内以及同cvv相交平面的坐标比例
	// 进行进一步精细裁剪，将一个分解为几个完全处在 cvv内的三角形
	if (transform_check_cvv(&c1) != 0) return;
	if (transform_check_cvv(&c2) != 0) return;
	if (transform_check_cvv(&c3) != 0) return;

	// 归一化
	transform_homogenize(&device->transform, &p1, &c1);
	transform_homogenize(&device->transform, &p2, &c2);
	transform_homogenize(&device->transform, &p3, &c3);

	// 纹理或者色彩绘制
	if (render_state & (RENDER_STATE_TEXTURE | RENDER_STATE_COLOR)) {
		Vertex t1 = *v1, t2 = *v2, t3 = *v3;
		trapezoid_t traps[2];
		t1.pos = p1;
		t2.pos = p2;
		t3.pos = p3;
		t1.pos.w() = c1.w();
		t2.pos.w() = c2.w();
		t3.pos.w() = c3.w();
		vertex_rhw_init(&t1);	// 初始化 w
		vertex_rhw_init(&t2);	// 初始化 w
		vertex_rhw_init(&t3);	// 初始化 w

		// 拆分三角形为0-2个梯形，并且返回可用梯形数量
		int n = trapezoid_init_triangle(traps, &t1, &t2, &t3);

		if (n >= 1) device_render_trap(device, &traps[0]);
		if (n >= 2) device_render_trap(device, &traps[1]);
	}

	if (render_state & RENDER_STATE_WIREFRAME)
	{		// 线框绘制
		device_draw_line(device, (int)p1.x(), (int)p1.y(), (int)p2.x(), (int)p2.y(), device->foreground);
		device_draw_line(device, (int)p1.x(), (int)p1.y(), (int)p3.x(), (int)p3.y(), device->foreground);
		device_draw_line(device, (int)p3.x(), (int)p3.y(), (int)p2.x(), (int)p2.y(), device->foreground);
	}
}

// win32
int screen_w, screen_h, screen_exit = 0;
int screen_mx = 0, screen_my = 0, screen_mb = 0;
int screen_keys[512];	// 当前键盘按下状态
static HWND screen_handle = NULL;		// 主窗口 HWND
static HDC screen_dc = NULL;			// 配套的 HDC
static HBITMAP screen_hb = NULL;		// DIB
static HBITMAP screen_ob = NULL;		// 老的 BITMAP
unsigned char* screen_fb = NULL;		// frame buffer
long screen_pitch = 0;

int screen_init(int w, int h, const TCHAR* title);	// 屏幕初始化
int screen_close(void);								// 关闭屏幕
void screen_dispatch(void);							// 处理消息
void screen_update(void);							// 显示 FrameBuffer
LRESULT screen_events(HWND, UINT, WPARAM, LPARAM);  // 屏幕事件

int screen_init(int w, int h, const TCHAR* title) {
	WNDCLASS wc = { CS_BYTEALIGNCLIENT, (WNDPROC)screen_events, 0, 0, 0,
		NULL, NULL, NULL, NULL, _T("SCREEN3.1415926") };
	BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB,
		w * h * 4, 0, 0, 0, 0 } };
	RECT rect = { 0, 0, w, h };
	int wx, wy, sx, sy;
	LPVOID ptr;
	HDC hDC;

	screen_close();

	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	if (!RegisterClass(&wc)) return -1;

	screen_handle = CreateWindow(_T("SCREEN3.1415926"), title,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);
	if (screen_handle == NULL) return -2;

	screen_exit = 0;
	hDC = GetDC(screen_handle);
	screen_dc = CreateCompatibleDC(hDC);
	ReleaseDC(screen_handle, hDC);

	screen_hb = CreateDIBSection(screen_dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
	if (screen_hb == NULL) return -3;

	screen_ob = (HBITMAP)SelectObject(screen_dc, screen_hb);
	screen_fb = (unsigned char*)ptr;
	screen_w = w;
	screen_h = h;
	screen_pitch = w * 4;

	AdjustWindowRect(&rect, GetWindowLong(screen_handle, GWL_STYLE), 0);
	wx = rect.right - rect.left;
	wy = rect.bottom - rect.top;
	sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
	sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
	if (sy < 0) sy = 0;
	SetWindowPos(screen_handle, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	SetForegroundWindow(screen_handle);

	ShowWindow(screen_handle, SW_NORMAL);
	screen_dispatch();

	memset(screen_keys, 0, sizeof(int) * 512);
	memset(screen_fb, 0, w * h * 4);

	return 0;
}

int screen_close(void) {
	if (screen_dc) {
		if (screen_ob) {
			SelectObject(screen_dc, screen_ob);
			screen_ob = NULL;
		}
		DeleteDC(screen_dc);
		screen_dc = NULL;
	}
	if (screen_hb) {
		DeleteObject(screen_hb);
		screen_hb = NULL;
	}
	if (screen_handle) {
		CloseWindow(screen_handle);
		screen_handle = NULL;
	}
	return 0;
}

LRESULT screen_events(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CLOSE: screen_exit = 1; break;
	case WM_KEYDOWN: screen_keys[wParam & 511] = 1; break;
	case WM_KEYUP: screen_keys[wParam & 511] = 0; break;
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

void screen_dispatch(void) {
	MSG msg;
	while (1) {
		if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
		if (!GetMessage(&msg, NULL, 0, 0)) break;
		DispatchMessage(&msg);
	}
}

void screen_update(void) {
	HDC hDC = GetDC(screen_handle);
	BitBlt(hDC, 0, 0, screen_w, screen_h, screen_dc, 0, 0, SRCCOPY);
	ReleaseDC(screen_handle, hDC);
	screen_dispatch();
}

// main
array<Vertex, 8> mesh = { {
	{ { -1, -1,  1, 1 }, { 0, 0 }, { 1.0, 0.2, 0.2 }, 1 },
	{ {  1, -1,  1, 1 }, { 0, 1 }, { 0.2, 1.0, 0.2 }, 1 },
	{ {  1,  1,  1, 1 }, { 1, 1 }, { 0.2, 0.2, 1.0 }, 1 },
	{ { -1,  1,  1, 1 }, { 1, 0 }, { 1.0, 0.2, 1.0 }, 1 },
	{ { -1, -1, -1, 1 }, { 0, 0 }, { 1.0, 1.0, 0.2 }, 1 },
	{ {  1, -1, -1, 1 }, { 0, 1 }, { 0.2, 1.0, 1.0 }, 1 },
	{ {  1,  1, -1, 1 }, { 1, 1 }, { 1.0, 0.3, 0.3 }, 1 },
	{ { -1,  1, -1, 1 }, { 1, 0 }, { 0.2, 1.0, 0.3 }, 1 },
} };

void draw_plane(Device* device, int a, int b, int c, int d)
{
	Vertex p1 = mesh[a], p2 = mesh[b], p3 = mesh[c], p4 = mesh[d];
	p1.tc.u = 0, p1.tc.v = 0, p2.tc.u = 0, p2.tc.v = 1;
	p3.tc.u = 1, p3.tc.v = 1, p4.tc.u = 1, p4.tc.v = 0;
	device_draw_primitive(device, &p1, &p2, &p3);
	device_draw_primitive(device, &p3, &p4, &p1);
}

void draw_box(Device* device, double theta)
{
	Matrix4d m;
	//matrix_set_rotate(&m, -1, -0.5, 1, theta);
	device->transform.model = m;
	transform_update(&device->transform);
	draw_plane(device, 0, 1, 2, 3);
	draw_plane(device, 7, 6, 5, 4);
	draw_plane(device, 0, 4, 5, 1);
	draw_plane(device, 1, 5, 6, 2);
	draw_plane(device, 2, 6, 7, 3);
	draw_plane(device, 3, 7, 4, 0);
}

void camera_at_zero(Device* device, double x, double y, double z)
{
	Vector4d eye = { x, y, z, 1 }, at = { 0, 0, 0, 1 }, up = { 0, 0, 1, 1 };
	matrix_set_lookat(&device->transform.view, eye, at, up);
	transform_update(&device->transform);
}

void init_texture(Device* device)
{
	static uint32_t texture[256][256];
	int i, j;
	for (j = 0; j < 256; j++) {
		for (i = 0; i < 256; i++) {
			int x = i / 32, y = j / 32;
			texture[j][i] = ((x + y) & 1) ? 0xffffff : 0x3fbcef;
		}
	}
	device_set_texture(device, texture, 256 * 4, 256, 256);
}

#ifndef USING_ORIGIN_MINI_C
int main(void)
{
	Device device;
	int states[] = { RENDER_STATE_TEXTURE, RENDER_STATE_COLOR, RENDER_STATE_WIREFRAME };
	int indicator = 0;
	int kbhit = 0;
	double alpha = 1;
	double pos = 3.5;

	const TCHAR* title = _T("Mini3d (software render tutorial) - Left/Right: rotation, Up/Down: forward/backward, Space: switch state");

	if (screen_init(800, 600, title))
		return -1;

	device_init(&device, 800, 600, screen_fb);
	camera_at_zero(&device, 3, 0, 0);

	init_texture(&device);
	device.render_state = RENDER_STATE_TEXTURE;

	while (screen_exit == 0 && screen_keys[VK_ESCAPE] == 0)
	{
		screen_dispatch();
		device_clear(&device, 1);
		camera_at_zero(&device, pos, 0, 0);

		if (screen_keys[VK_UP]) pos -= 0.01f;
		if (screen_keys[VK_DOWN]) pos += 0.01f;
		if (screen_keys[VK_LEFT]) alpha += 0.01f;
		if (screen_keys[VK_RIGHT]) alpha -= 0.01f;

		if (screen_keys[VK_SPACE]) {
			if (kbhit == 0) {
				kbhit = 1;
				if (++indicator >= 3) indicator = 0;
				device.render_state = states[indicator];
			}
		}
		else {
			kbhit = 0;
		}

		draw_box(&device, alpha);
		screen_update();
		Sleep(1);
	}
	return 0;
}
#endif // USING_ORIGIN_MINI_C

#define _USE_MATH_DEFINES
#include <math.h>
static void test0()
{
	Vector4d vec = { 1,1,1,1 };
	double n = vec.hnormalized().norm();
	Vector3d vec1 = vec.hnormalized().normalized();
	Vector4d vec2 = vec.hnormalized().normalized().homogeneous();

	Matrix4d mat;
	double theta;
	theta = 0;
	mat = rotate({ -1, -0.5, 1 }, theta);
	theta = 1;				  
	mat = rotate({ -1, -0.5, 1 }, theta);
	theta = M_PI / 2;		  
	mat = rotate({ -1, -0.5, 1 }, theta);


	cout << "main" << endl;
	return;
}

static int _enrol = []()->int
	{
		test0();
		return 0;
	}();

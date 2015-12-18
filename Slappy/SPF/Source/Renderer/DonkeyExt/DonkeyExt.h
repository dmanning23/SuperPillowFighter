//////////////////////////////////////////////////////////////////////////////////////////
// File Name:      DonkeyExt.h
//
// Author:         Kevin Cecelski
//
// Date Created:   Friday, August 27, 2004
//
// Purpose:        This file declares the Opengl extension prototypes that the donkey will
//				   use.
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef _DONKEY_EXT_H_
#define	_DONKEY_EXT_H_
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

// Masks used to store the results of loading the extensions.
#define VERTEX_BUF	0x00000001
#define PIXEL_BUF	0x00000002
#define PIX_FMT		0x00000004
#define RND_TEX		0x00000008

// After calling loadExtensions, use this with the masks above
// to find which extensions are supported by this machine.
extern unsigned int uExtSupport;	

//////////////////////////////////////////////////////////////////////////////////////////
// Function:       loadExtensions()
//
// Purpose:        Get the extensions the donkey needs.
//
// Last Modified:  Friday, August 27, 2004
//
// Input:          None.
//
// Output:         None.
//
// Returns:        Return the results of loading the extensions.
//
//////////////////////////////////////////////////////////////////////////////////////////
void loadExtensions(HDC hDc);

// ** The extension functions supported. **
// Functions for vertex buffers.
extern void (APIENTRY * BindBufferARB)(unsigned int target, unsigned int buffer);
extern void	(APIENTRY * DeleteBuffersARB)(int n, const GLuint *buffers);
extern void	(APIENTRY * GenBuffersARB)(int n, GLuint *buffers);
extern void (APIENTRY * BufferDataARB)(unsigned int target, int size, const void *data,
                       unsigned int usage);

// Other extension functions the donkey will not use(currently).
//WINGDIAPI GLboolean APIENTRY IsBufferARB(GLuint buffer);
//WINGDIAPI void APIENTRY BufferSubDataARB(enum target, intptrARB offset, sizeiptrARB size,
//                          const void *data);
//WINGDIAPI void APIENTRY GetBufferSubDataARB(enum target, intptrARB offset,
//                             sizeiptrARB size, void *data);
//WINGDIAPI void APIENTRY *MapBufferARB(enum target, enum access);
//WINGDIAPI GLboolean APIENTRYUnmapBufferARB(enum target);
//WINGDIAPI void APIENTRY GetBufferParameterivARB(enum target, enum pname, int *params);
//WINGDIAPI void APIENTRY GetBufferPointervARB(enum target, enum pname, void **params);

// Functions for wgl extensions.

// Functions for the pixel buffer extension.
DECLARE_HANDLE(HPBUFFERARB);
extern HPBUFFERARB (APIENTRY * wglCreatePbufferARB)(HDC hDC, int iPixelFormat, int iWidth, int iHeight,
				const int *piAttribList);
extern HDC (APIENTRY * wglGetPbufferDCARB)(HPBUFFERARB hPbuffer);
extern int (APIENTRY * wglReleasePbufferDCARB)(HPBUFFERARB hPbuffer, HDC hDC);
extern BOOL (APIENTRY * wglDestroyPbufferARB)(HPBUFFERARB hPbuffer);
extern BOOL (APIENTRY * wglQueryPbufferARB)(HPBUFFERARB hPbuffer, int iAttribute, int *piValue);

// WGL get string extension function.
extern const char *(APIENTRY * wglGetExtensionsStringARB)(HDC hdc);

// Functions for the pixel format extension.
extern BOOL (APIENTRY * wglGetPixelFormatAttribivARB)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes,
								  const int *piAttributes, int *piValues);
extern BOOL (APIENTRY * wglGetPixelFormatAttribfvARB)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes,
                                  const int *piAttributes, FLOAT *pfValues);
extern BOOL (APIENTRY * wglChoosePixelFormatARB)(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats,
                             int *piFormats, UINT *nNumFormats);

// Functions for the render to texture extension.
extern BOOL (APIENTRY * wglBindTexImageARB)(HPBUFFERARB hPbuffer, int iBuffer);
extern BOOL (APIENTRY * wglReleaseTexImageARB)(HPBUFFERARB hPbuffer, int iBuffer); 
extern BOOL (APIENTRY * wglSetPbufferAttribARB)(HPBUFFERARB hPbuffer, const int *piAttribList);

// ** Enumerants for the extensions. **
// Enumerants for vertex buffers.
#define ARRAY_BUFFER_ARB							0x8892
#define ELEMENT_ARRAY_BUFFER_ARB                    0x8893
#define ARRAY_BUFFER_BINDING_ARB                    0x8894
#define ELEMENT_ARRAY_BUFFER_BINDING_ARB            0x8895
#define VERTEX_ARRAY_BUFFER_BINDING_ARB             0x8896
#define NORMAL_ARRAY_BUFFER_BINDING_ARB             0x8897
#define COLOR_ARRAY_BUFFER_BINDING_ARB              0x8898
#define INDEX_ARRAY_BUFFER_BINDING_ARB              0x8899
#define TEXTURE_COORD_ARRAY_BUFFER_BINDING_ARB      0x889A
#define EDGE_FLAG_ARRAY_BUFFER_BINDING_ARB          0x889B
#define SECONDARY_COLOR_ARRAY_BUFFER_BINDING_ARB    0x889C
#define FOG_COORDINATE_ARRAY_BUFFER_BINDING_ARB     0x889D
#define WEIGHT_ARRAY_BUFFER_BINDING_ARB             0x889E
#define VERTEX_ATTRIB_ARRAY_BUFFER_BINDING_ARB      0x889F
#define STREAM_DRAW_ARB                             0x88E0
#define STREAM_READ_ARB                             0x88E1
#define STREAM_COPY_ARB                             0x88E2
#define STATIC_DRAW_ARB                             0x88E4
#define STATIC_READ_ARB                             0x88E5
#define STATIC_COPY_ARB                             0x88E6
#define DYNAMIC_DRAW_ARB                            0x88E8
#define DYNAMIC_READ_ARB                            0x88E9
#define DYNAMIC_COPY_ARB                            0x88EA
#define READ_ONLY_ARB                               0x88B8
#define WRITE_ONLY_ARB                              0x88B9
#define READ_WRITE_ARB                              0x88BA
#define BUFFER_SIZE_ARB                             0x8764
#define BUFFER_USAGE_ARB                            0x8765
#define BUFFER_ACCESS_ARB                           0x88BB
#define BUFFER_MAPPED_ARB                           0x88BC
#define BUFFER_MAP_POINTER_ARB                      0x88BD

// Enumerants for pixel buffers.
#define WGL_DRAW_TO_PBUFFER_ARB						0x202D
#define WGL_DRAW_TO_PBUFFER_ARB						0x202D
#define WGL_MAX_PBUFFER_PIXELS_ARB					0x202E
#define WGL_MAX_PBUFFER_WIDTH_ARB					0x202F
#define WGL_MAX_PBUFFER_HEIGHT_ARB					0x2030
#define WGL_PBUFFER_LARGEST_ARB						0x2033
#define WGL_PBUFFER_WIDTH_ARB						0x2034
#define WGL_PBUFFER_HEIGHT_ARB						0x2035
#define WGL_PBUFFER_LOST_ARB						0x2036

// Enumerants for the pixel format extension.
#define WGL_NUMBER_PIXEL_FORMATS_ARB				0x2000
#define WGL_DRAW_TO_WINDOW_ARB						0x2001
#define WGL_DRAW_TO_BITMAP_ARB						0x2002
#define WGL_ACCELERATION_ARB						0x2003
#define WGL_NEED_PALETTE_ARB						0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB					0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB					0x2006
#define WGL_SWAP_METHOD_ARB							0x2007
#define WGL_NUMBER_OVERLAYS_ARB						0x2008
#define WGL_NUMBER_UNDERLAYS_ARB					0x2009
#define WGL_TRANSPARENT_ARB							0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB				0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB				0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB				0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB				0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB				0x203B
#define WGL_SHARE_DEPTH_ARB							0x200C
#define WGL_SHARE_STENCIL_ARB						0x200D
#define WGL_SHARE_ACCUM_ARB							0x200E
#define WGL_SUPPORT_GDI_ARB							0x200F
#define WGL_SUPPORT_OPENGL_ARB						0x2010
#define WGL_DOUBLE_BUFFER_ARB						0x2011
#define WGL_STEREO_ARB								0x2012
#define WGL_PIXEL_TYPE_ARB							0x2013
#define WGL_COLOR_BITS_ARB							0x2014
#define WGL_RED_BITS_ARB							0x2015
#define WGL_RED_SHIFT_ARB							0x2016
#define WGL_GREEN_BITS_ARB							0x2017
#define WGL_GREEN_SHIFT_ARB							0x2018
#define WGL_BLUE_BITS_ARB							0x2019
#define WGL_BLUE_SHIFT_ARB							0x201A
#define WGL_ALPHA_BITS_ARB							0x201B
#define WGL_ALPHA_SHIFT_ARB							0x201C
#define WGL_ACCUM_BITS_ARB							0x201D
#define WGL_ACCUM_RED_BITS_ARB						0x201E
#define WGL_ACCUM_GREEN_BITS_ARB					0x201F
#define WGL_ACCUM_BLUE_BITS_ARB						0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB					0x2021
#define WGL_DEPTH_BITS_ARB							0x2022
#define WGL_STENCIL_BITS_ARB						0x2023
#define WGL_AUX_BUFFERS_ARB							0x2024
#define WGL_NO_ACCELERATION_ARB						0x2025
#define WGL_GENERIC_ACCELERATION_ARB				0x2026
#define WGL_FULL_ACCELERATION_ARB					0x2027
#define WGL_SWAP_EXCHANGE_ARB						0x2028
#define WGL_SWAP_COPY_ARB							0x2029
#define WGL_SWAP_UNDEFINED_ARB						0x202A
#define WGL_TYPE_RGBA_ARB							0x202B
#define WGL_TYPE_COLORINDEX_ARB						0x202C

// Enumerants for the render to texture extension.
#define WGL_BIND_TO_TEXTURE_RGB_ARB					0x2070
#define WGL_BIND_TO_TEXTURE_RGBA_ARB			    0x2071
#define WGL_TEXTURE_FORMAT_ARB					    0x2072
#define WGL_TEXTURE_TARGET_ARB						0x2073
#define WGL_MIPMAP_TEXTURE_ARB						0x2074
#define WGL_TEXTURE_RGB_ARB							0x2075
#define WGL_TEXTURE_RGBA_ARB						0x2076
#define WGL_NO_TEXTURE_ARB						    0x2077
#define WGL_TEXTURE_CUBE_MAP_ARB				    0x2078
#define WGL_TEXTURE_1D_ARB						    0x2079
#define WGL_TEXTURE_2D_ARB						    0x207A
#define WGL_NO_TEXTURE_ARB							0x2077
#define WGL_MIPMAP_LEVEL_ARB						0x207B
#define WGL_CUBE_MAP_FACE_ARB						0x207C
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB			0x207D
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB			0x207E
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB			0x207F
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB			0x2080
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB			0x2081 
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB			0x2082
#define WGL_FRONT_LEFT_ARB						    0x2083
#define WGL_FRONT_RIGHT_ARB					        0x2084
#define WGL_BACK_LEFT_ARB						    0x2085
#define WGL_BACK_RIGHT_ARB						    0x2086
#define WGL_AUX0_ARB						        0x2087 
#define WGL_AUX1_ARB						        0x2088 
#define WGL_AUX2_ARB						        0x2089 
#define WGL_AUX3_ARB						        0x208A 
#define WGL_AUX4_ARB						        0x208B 
#define WGL_AUX5_ARB							    0x208C 
#define WGL_AUX6_ARB							    0x208D
#define WGL_AUX7_ARB						        0x208E 
#define WGL_AUX8_ARB						        0x208F 
#define WGL_AUX9_ARB							    0x2090

#endif

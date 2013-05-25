/*
 *	Copyright (C) 2011-2013 Gregory hainaut
 *	Copyright (C) 2007-2009 Gabest
 *
 *  This file was generated by glsl2h.pl script
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GNU Make; see the file COPYING.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#pragma once

#include "stdafx.h"

static const char* convert_glsl =
	"//#version 420 // Keep it for editor detection\n"
	"\n"
	"struct vertex_basic\n"
	"{\n"
	"    vec4 p;\n"
	"    vec2 t;\n"
	"};\n"
	"\n"
	"\n"
	"#ifdef VERTEX_SHADER\n"
	"\n"
	"#if __VERSION__ > 140\n"
	"out gl_PerVertex {\n"
	"    vec4 gl_Position;\n"
	"    float gl_PointSize;\n"
	"    float gl_ClipDistance[];\n"
	"};\n"
	"#endif\n"
	"\n"
	"layout(location = 0) in vec4 POSITION;\n"
	"layout(location = 1) in vec2 TEXCOORD0;\n"
	"\n"
	"// FIXME set the interpolation (don't know what dx do)\n"
	"// flat means that there is no interpolation. The value given to the fragment shader is based on the provoking vertex conventions.\n"
	"//\n"
	"// noperspective means that there will be linear interpolation in window-space. This is usually not what you want, but it can have its uses.\n"
	"//\n"
	"// smooth, the default, means to do perspective-correct interpolation.\n"
	"//\n"
	"// The centroid qualifier only matters when multisampling. If this qualifier is not present, then the value is interpolated to the pixel's center, anywhere in the pixel, or to one of the pixel's samples. This sample may lie outside of the actual primitive being rendered, since a primitive can cover only part of a pixel's area. The centroid qualifier is used to prevent this; the interpolation point must fall within both the pixel's area and the primitive's area.\n"
	"layout(location = 0) out vertex_basic VSout;\n"
	"\n"
	"void vs_main()\n"
	"{\n"
	"    VSout.p = POSITION;\n"
	"    VSout.t = TEXCOORD0;\n"
	"    gl_Position = POSITION; // NOTE I don't know if it is possible to merge POSITION_OUT and gl_Position\n"
	"}\n"
	"\n"
	"#endif\n"
	"\n"
	"#ifdef FRAGMENT_SHADER\n"
	"// NOTE: pixel can be clip with \"discard\"\n"
	"\n"
	"layout(location = 0) in vertex_basic PSin;\n"
	"\n"
	"layout(location = 0) out vec4 SV_Target0;\n"
	"layout(location = 1) out uint SV_Target1;\n"
	"\n"
	"#ifdef DISABLE_GL42\n"
	"uniform sampler2D TextureSampler;\n"
	"#else\n"
	"layout(binding = 0) uniform sampler2D TextureSampler;\n"
	"#endif\n"
	"\n"
	"vec4 sample_c()\n"
	"{\n"
	"    return texture(TextureSampler, PSin.t );\n"
	"}\n"
	"\n"
	"uniform vec4 mask[4] = vec4[4]\n"
	"(\n"
	"		vec4(1, 0, 0, 0),\n"
	"		vec4(0, 1, 0, 0),\n"
	"		vec4(0, 0, 1, 0),\n"
	"		vec4(1, 1, 1, 0)\n"
	");\n"
	"\n"
	"\n"
	"vec4 ps_crt(uint i)\n"
	"{\n"
	"	return sample_c() * clamp((mask[i] + 0.5f), 0.0f, 1.0f);\n"
	"}\n"
	"\n"
	"void ps_main0()\n"
	"{\n"
	"    SV_Target0 = sample_c();\n"
	"}\n"
	"\n"
	"void ps_main1()\n"
	"{\n"
	"    vec4 c = sample_c();\n"
	"\n"
	"	c.a *= 256.0f / 127.0f; // hm, 0.5 won't give us 1.0 if we just multiply with 2\n"
	"\n"
	"	highp uvec4 i = uvec4(c * vec4(uint(0x001f), uint(0x03e0), uint(0x7c00), uint(0x8000)));\n"
	"\n"
	"    SV_Target1 = (i.x & uint(0x001f)) | (i.y & uint(0x03e0)) | (i.z & uint(0x7c00)) | (i.w & uint(0x8000));\n"
	"}\n"
	"\n"
	"void ps_main7()\n"
	"{\n"
	"    vec4 c = sample_c();\n"
	"\n"
	"	c.a = dot(c.rgb, vec3(0.299, 0.587, 0.114));\n"
	"\n"
	"    SV_Target0 = c;\n"
	"}\n"
	"\n"
	"void ps_main5() // triangular\n"
	"{\n"
	"	highp uvec4 p = uvec4(PSin.p);\n"
	"\n"
	"	vec4 c = ps_crt(((p.x + ((p.y >> 1u) & 1u) * 3u) >> 1u) \% 3u);\n"
	"\n"
	"    SV_Target0 = c;\n"
	"}\n"
	"\n"
	"void ps_main6() // diagonal\n"
	"{\n"
	"	uvec4 p = uvec4(PSin.p);\n"
	"\n"
	"	vec4 c = ps_crt((p.x + (p.y \% 3u)) \% 3u);\n"
	"\n"
	"    SV_Target0 = c;\n"
	"}\n"
	"\n"
	"void ps_main2()\n"
	"{\n"
	"    if((sample_c().a - 128.0f / 255) < 0) // >= 0x80 pass\n"
	"        discard;\n"
	"\n"
	"    SV_Target0 = vec4(0.0f, 0.0f, 0.0f, 0.0f);\n"
	"}\n"
	"void ps_main3()\n"
	"{\n"
	"    if((127.95f / 255 - sample_c().a) <0) // < 0x80 pass (== 0x80 should not pass)\n"
	"        discard;\n"
	"\n"
	"    SV_Target0 = vec4(0.0f, 0.0f, 0.0f, 0.0f);\n"
	"}\n"
	"void ps_main4()\n"
	"{\n"
	"    // FIXME mod and fmod are different when value are negative\n"
	"    // 	output.c = fmod(sample_c(input.t) * 255 + 0.5f, 256) / 255;\n"
	"    vec4 c = mod(sample_c() * 255 + 0.5f, 256) / 255;\n"
	"\n"
	"    SV_Target0 = c;\n"
	"}\n"
	"\n"
	"#endif\n"
	;

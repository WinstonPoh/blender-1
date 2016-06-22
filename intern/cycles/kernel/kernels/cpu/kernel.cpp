/*
 * Copyright 2011-2013 Blender Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* CPU kernel entry points */

/* On x86-64, we can assume SSE2, so avoid the extra kernel and compile this
 * one with SSE2 intrinsics.
 */
#if defined(__x86_64__) || defined(_M_X64)
#  define __KERNEL_SSE2__
#endif

/* When building kernel for native machine detect kernel features from the flags
 * set by compiler.
 */
#ifdef WITH_KERNEL_NATIVE
#  ifdef __SSE2__
#    ifndef __KERNEL_SSE2__
#      define __KERNEL_SSE2__
#    endif
#  endif
#  ifdef __SSE3__
#    define __KERNEL_SSE3__
#  endif
#  ifdef __SSSE3__
#    define __KERNEL_SSSE3__
#  endif
#  ifdef __SSE4_1__
#    define __KERNEL_SSE41__
#  endif
#  ifdef __AVX__
#    define __KERNEL_AVX__
#  endif
#  ifdef __AVX2__
#    define __KERNEL_AVX2__
#  endif
#endif

/* quiet unused define warnings */
#if defined(__KERNEL_SSE2__)
    /* do nothing */
#endif

#include "kernel.h"
#define KERNEL_ARCH cpu
#include "kernel_cpu_impl.h"

CCL_NAMESPACE_BEGIN

/* Memory Copy */

void kernel_const_copy(KernelGlobals *kg, const char *name, void *host, size_t size)
{
	if(strcmp(name, "__data") == 0)
		memcpy(&kg->__data, host, size);
	else if(strcmp(name, "__float_volume") == 0)
		kg->float_volumes[size] = (float_volume *)host;
	else if(strcmp(name, "__float3_volume") == 0)
		kg->float3_volumes[size] = (float3_volume *)host;
	else
		assert(0);
}

void kernel_tex_copy(KernelGlobals *kg,
                     const char *name,
                     device_ptr mem,
                     size_t width,
                     size_t height,
                     size_t depth,
                     InterpolationType interpolation,
                     ExtensionType extension)
{
	if(0) {
	}

#define KERNEL_TEX(type, ttype, tname) \
	else if(strcmp(name, #tname) == 0) { \
		kg->tname.data = (type*)mem; \
		kg->tname.width = width; \
	}
#define KERNEL_IMAGE_TEX(type, ttype, tname)
#include "kernel_textures.h"

	else if(strstr(name, "__tex_image_float4")) {
		texture_image_float4 *tex = NULL;
		int id = atoi(name + strlen("__tex_image_float4_"));
		int array_index = id;

		if(array_index >= 0 && array_index < TEX_NUM_FLOAT4_CPU) {
			tex = &kg->texture_float4_images[array_index];
		}

		if(tex) {
			tex->data = (float4*)mem;
			tex->dimensions_set(width, height, depth);
			tex->interpolation = interpolation;
			tex->extension = extension;
		}
	}
	else if(strstr(name, "__tex_image_float")) {
		texture_image_float *tex = NULL;
		int id = atoi(name + strlen("__tex_image_float_"));
		int array_index = id - TEX_START_FLOAT_CPU;

		if(array_index >= 0 && array_index < TEX_NUM_FLOAT_CPU) {
			tex = &kg->texture_float_images[array_index];
		}

		if(tex) {
			tex->data = (float*)mem;
			tex->dimensions_set(width, height, depth);
			tex->interpolation = interpolation;
			tex->extension = extension;
		}
	}
	else if(strstr(name, "__tex_image_byte4")) {
		texture_image_uchar4 *tex = NULL;
		int id = atoi(name + strlen("__tex_image_byte4_"));
		int array_index = id - TEX_START_BYTE4_CPU;

		if(array_index >= 0 && array_index < TEX_NUM_BYTE4_CPU) {
			tex = &kg->texture_byte4_images[array_index];
		}

		if(tex) {
			tex->data = (uchar4*)mem;
			tex->dimensions_set(width, height, depth);
			tex->interpolation = interpolation;
			tex->extension = extension;
		}
	}
	else if(strstr(name, "__tex_image_byte")) {
		texture_image_uchar *tex = NULL;
		int id = atoi(name + strlen("__tex_image_byte_"));
		int array_index = id - TEX_START_BYTE_CPU;

		if(array_index >= 0 && array_index < TEX_NUM_BYTE_CPU) {
			tex = &kg->texture_byte_images[array_index];
		}

		if(tex) {
			tex->data = (uchar*)mem;
			tex->dimensions_set(width, height, depth);
			tex->interpolation = interpolation;
			tex->extension = extension;
		}
	}
	else if(strstr(name, "__tex_image_half4")) {
		texture_image_half4 *tex = NULL;
		int id = atoi(name + strlen("__tex_image_half4_"));
		int array_index = id - TEX_START_HALF4_CPU;

		if(array_index >= 0 && array_index < TEX_NUM_HALF4_CPU) {
			tex = &kg->texture_half4_images[array_index];
		}

		if(tex) {
			tex->data = (half4*)mem;
			tex->dimensions_set(width, height, depth);
			tex->interpolation = interpolation;
			tex->extension = extension;
		}
	}
	else if(strstr(name, "__tex_image_half")) {
		texture_image_half *tex = NULL;
		int id = atoi(name + strlen("__tex_image_half_"));
		int array_index = id - TEX_START_HALF_CPU;

		if(array_index >= 0 && array_index < TEX_NUM_HALF_CPU) {
			tex = &kg->texture_half_images[array_index];
		}

		if(tex) {
			tex->data = (half*)mem;
			tex->dimensions_set(width, height, depth);
			tex->interpolation = interpolation;
			tex->extension = extension;
		}
	}
	else
		assert(0);
}

CCL_NAMESPACE_END

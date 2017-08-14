// Copyright 2016 The SwiftShader Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef vk_Device_hpp
#define vk_Device_hpp

#include "Renderer/Renderer.hpp"
#include "Renderer/Surface.hpp"

namespace vulkan
{
	class Texture;

	struct Viewport
	{
		int x0;
		int y0;
		unsigned int width;
		unsigned int height;
		float minZ;
		float maxZ;

		Viewport() : x0(0), y0(0), width(0), height(0), minZ(0), maxZ(0) {}

		Viewport(int xi, int yi, unsigned int width_, unsigned int height_, float minZ_, float maxZ_) :
			x0(xi), y0(yi), width(width_), height(height_), minZ(minZ_), maxZ(maxZ_) {}
	};

	class SwDevice : public sw::Renderer
	{
	public:
		enum : unsigned char
		{
			USE_FILTER = 0x01,
			COLOR_BUFFER = 0x02,
			DEPTH_BUFFER = 0x04,
			STENCIL_BUFFER = 0x08,
			ALL_BUFFERS = COLOR_BUFFER | DEPTH_BUFFER | STENCIL_BUFFER,
		};

		explicit SwDevice(sw::Context *context);

		virtual ~SwDevice();

		void *operator new(size_t size);
		void operator delete(void * mem);

		void clearColor(float red, float green, float blue, float alpha, unsigned int rgbaMask);
		void clearDepth(float z);
		void clearStencil(unsigned int stencil, unsigned int mask);
		sw::Surface *createDepthStencilSurface(unsigned int width, unsigned int height, sw::Format format, int multiSampleDepth, bool discard);
		sw::Surface *createRenderTarget(unsigned int width, unsigned int height, sw::Format format, int multiSampleDepth, bool lockable);
		void drawIndexedPrimitive(sw::DrawType type, unsigned int indexOffset, unsigned int primitiveCount);
		void drawPrimitive(sw::DrawType type, unsigned int primiveCount);
		void setPixelShader(sw::PixelShader *shader);
		void setPixelShaderConstantF(unsigned int startRegister, const float *constantData, unsigned int count);
		void setScissorEnable(bool enable);
		void setRenderTarget(int index, sw::Surface *renderTarget);
		void setDepthBuffer(sw::Surface *depthBuffer);
		void setStencilBuffer(sw::Surface *stencilBuffer);
		void setScissorRect(const sw::Rect &rect);
		void setVertexShader(sw::VertexShader *shader);
		void setVertexShaderConstantF(unsigned int startRegister, const float *constantData, unsigned int count);
		void setViewport(const Viewport &viewport);

		bool stretchRect(sw::Surface *sourceSurface, const sw::SliceRect *sourceRect, sw::Surface *destSurface, const sw::SliceRect *destRect, unsigned char flags);
		bool stretchCube(sw::Surface *sourceSurface, sw::Surface *destSurface);
		void finish();
		void setup();

	private:
		sw::Context *const context;

		bool bindResources();
		void bindShaderConstants();
		bool bindViewport();   // Also adjusts for scissoring

		bool validRectangle(const sw::Rect *rect, sw::Surface *surface);

		void copyBuffer(sw::byte *sourceBuffer, sw::byte *destBuffer, unsigned int width, unsigned int height, unsigned int sourcePitch, unsigned int destPitch, unsigned int bytes, bool flipX, bool flipY);

		Viewport viewport;
		sw::Rect scissorRect;
		bool scissorEnable;

		sw::PixelShader *pixelShader;
		sw::VertexShader *vertexShader;

		bool pixelShaderDirty;
		unsigned int pixelShaderConstantsFDirty;
		bool vertexShaderDirty;
		unsigned int vertexShaderConstantsFDirty;

		float pixelShaderConstantF[sw::FRAGMENT_UNIFORM_VECTORS][4];
		float vertexShaderConstantF[sw::VERTEX_UNIFORM_VECTORS][4];

		sw::Surface *renderTarget[sw::RENDERTARGETS];
		sw::Surface *depthBuffer;
		sw::Surface *stencilBuffer;
	};
}

#endif   // gl_Device_hpp

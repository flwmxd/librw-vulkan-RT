
#pragma once

#ifdef RW_VULKAN

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>

namespace maple
{
	enum class TextureFormat : int32_t;
	enum class DrawType : int32_t;
	class Texture;
	class Texture2D;
	class VertexBuffer;
	class IndexBuffer;
	class DescriptorSet;
	class Shader;
	class Pipeline;
	class BatchTask;
	class Texture;
	class TextureCube;
	class UniformBuffer;
	struct PipelineInfo;
}

namespace rw
{
	constexpr static int32_t MAX_OBJECT_SIZE = 4096;
	constexpr static int32_t MAX_LIGHTS = 32;

	struct EngineOpenParams
	{
		GLFWwindow** window;
		int          width, height;
		const char* windowtitle;
	};

	struct LightData;

	namespace vulkan
	{
		void registerPlatformPlugins(void);

		extern Device renderdevice;

		// arguments to glVertexAttribPointer basically
		struct AttribDesc
		{
			uint32 index;
			int32  type;
			int32  size;
			uint32 stride;
			uint32 offset;
		};

		enum AttribIndices
		{
			ATTRIB_POS = 0,
			ATTRIB_NORMAL,
			ATTRIB_COLOR,
			ATTRIB_WEIGHTS,
			ATTRIB_INDICES,
			ATTRIB_TEXCOORDS0,
			ATTRIB_TEXCOORDS1,
			ATTRIB_TEXCOORDS2,
			ATTRIB_TEXCOORDS3,
			ATTRIB_TEXCOORDS4,
			ATTRIB_TEXCOORDS5,
			ATTRIB_TEXCOORDS6,
			ATTRIB_TEXCOORDS7,
		};

		// default uniform indices
		extern int32 u_matColor;
		extern int32 u_surfProps;

		struct InstanceData
		{
			uint32    numIndex;
			uint32    minVert;            // not used for rendering
			int32     numVertices;        //
			Material* material;
			bool32    vertexAlpha;
			uint32    program;
			uint32    offset;
		};

		struct InstanceDataHeader : rw::InstanceDataHeader
		{
			uint32      serialNumber;
			uint32      numMeshes;
			uint16* indexBuffer;
			maple::DrawType primType;
			uint8* vertexBuffer;
			int32       numAttribs;
			AttribDesc* attribDesc;
			uint32      totalNumIndex;
			uint32      totalNumVertex;

			maple::VertexBuffer* vertexBufferGPU;
			maple::IndexBuffer* indexBufferGPU;

			InstanceData* inst;
			uint32    meshId;
			uint32	  blasId = -1;
		};

		struct Shader;
		extern std::shared_ptr<maple::UniformBuffer> cameraUniform;
		extern Shader* defaultShader, * defaultShader_noAT;
		extern Shader* defaultShader_fullLight, * defaultShader_fullLight_noAT;
		extern std::shared_ptr<maple::TextureCube> skyBox;
		extern std::shared_ptr<maple::Texture2D> skyBoxBuffer;

		struct Im3DVertex
		{
			V3d     position;
			V3d     normal;        // librw extension
			float32   r, g, b, a;
			float32 u, v;

			void setX(float32 x)
			{
				this->position.x = x;
			}
			void setY(float32 y)
			{
				this->position.y = y;
			}
			void setZ(float32 z)
			{
				this->position.z = z;
			}
			void setNormalX(float32 x)
			{
				this->normal.x = x;
			}
			void setNormalY(float32 y)
			{
				this->normal.y = y;
			}
			void setNormalZ(float32 z)
			{
				this->normal.z = z;
			}
			void setColor(uint8 r, uint8 g, uint8 b, uint8 a)
			{
				this->r = r / 255.f;
				this->g = g / 255.f;
				this->b = b / 255.f;
				this->a = a / 255.f;
			}
			void setU(float32 u)
			{
				this->u = u;
			}
			void setV(float32 v)
			{
				this->v = v;
			}

			float getX(void)
			{
				return this->position.x;
			}
			float getY(void)
			{
				return this->position.y;
			}
			float getZ(void)
			{
				return this->position.z;
			}
			float getNormalX(void)
			{
				return this->normal.x;
			}
			float getNormalY(void)
			{
				return this->normal.y;
			}
			float getNormalZ(void)
			{
				return this->normal.z;
			}
			RGBA getColor(void)
			{
				return makeRGBA(uint8_t(this->r * 255), uint8_t(this->g * 255), uint8_t(this->b * 255), uint8_t(this->a * 255));
			}
			float getU(void)
			{
				return this->u;
			}
			float getV(void)
			{
				return this->v;
			}
		};
		extern RGBA              im3dMaterialColor;
		extern SurfaceProperties im3dSurfaceProps;

		struct Im2DVertex
		{
			float32 x, y, z, w;
			float32 r, g, b, a;
			float32 u, v;

			void setScreenX(float32 x)
			{
				this->x = x;
			}
			void setScreenY(float32 y)
			{
				this->y = y;
			}
			void setScreenZ(float32 z)
			{
				this->z = z;
			}
			// This is a bit unefficient but we have to counteract GL's divide, so multiply
			void setCameraZ(float32 z)
			{
				this->w = z;
			}
			void setRecipCameraZ(float32 recipz)
			{
				this->w = 1.0f / recipz;
			}
			void setColor(uint8 r, uint8 g, uint8 b, uint8 a)
			{
				this->r = r / 255.f;
				this->g = g / 255.f;
				this->b = b / 255.f;
				this->a = a / 255.f;
			}
			void setU(float32 u, float recipz)
			{
				this->u = u;
			}
			void setV(float32 v, float recipz)
			{
				this->v = v;
			}

			float getScreenX(void)
			{
				return this->x;
			}
			float getScreenY(void)
			{
				return this->y;
			}
			float getScreenZ(void)
			{
				return this->z;
			}
			float getCameraZ(void)
			{
				return this->w;
			}
			float getRecipCameraZ(void)
			{
				return 1.0f / this->w;
			}
			RGBA getColor(void)
			{
				return makeRGBA( uint8_t(this->r * 255), uint8_t(this->g * 255), uint8_t(this->b * 255), uint8_t(this->a * 255));
			}
			float getU(void)
			{
				return this->u;
			}
			float getV(void)
			{
				return this->v;
			}
		};

		// Render state

		// Vertex shader bits
		enum
		{
			// These should be low so they could be used as indices
			VSLIGHT_DIRECT = 1,
			VSLIGHT_POINT = 2,
			VSLIGHT_SPOT = 4,
			VSLIGHT_MASK = 7,        // all the above
			// less critical
			VSLIGHT_AMBIENT = 8,
		};

		extern Shader* im2dOverrideShader;
		//Cache
		void registerVkCache();

		// per Scene
		void setProjectionMatrix(float32*);
		void setViewMatrix(float32*);

		// per Object
		void  setWorldMatrix(Matrix*);
		int32 setLights(WorldLights* lightData, const std::vector<LightData> & lights);


		// per Mesh
		void setTexture(std::shared_ptr<maple::DescriptorSet> sets, int32 n, Texture* tex);
		void setTextureBlend(Texture* tex);
	
		void setMaterial(const RGBA& color, const SurfaceProperties& surfaceprops, 
			int32 textureId = -1, 
			int32 instanceId = 0, 
			int32 indexOffset = 0,
			int32 objectId = 0,
			float extraSurfProp = 0.0f);

		inline void setMaterial(uint32 flags, const RGBA& color, const SurfaceProperties& surfaceprops,
			int32 textureId, 
			int32 instanceId = 0, 
			int32 indexOffset = 0, 
			int32 objectId = 0,
			float extraSurfProp = 0.0f)
		{
			static RGBA white = { 255, 255, 255, 255 };
			if (flags & Geometry::MODULATE)
				setMaterial( color, surfaceprops, textureId, instanceId, indexOffset, objectId, extraSurfProp);
			else
				setMaterial( white, surfaceprops, textureId, instanceId, indexOffset, objectId, extraSurfProp);
		}

		std::shared_ptr<maple::Texture> getTexture(int32_t textureId);
		std::shared_ptr<maple::DescriptorSet> getTextureDescriptorSet(Material* material);
		std::shared_ptr<maple::DescriptorSet> getTextureDescriptorSet(uint32_t textureId);
		std::shared_ptr<maple::DescriptorSet> getDefaultDescriptorSet();
		std::vector<std::shared_ptr<maple::Texture>>& getTextures();
		void resetCache();

		void   setAlphaBlend(bool32 enable);
		bool32 getAlphaBlend(void);

		uint32_t getDepthFunc();

		bool32 getAlphaTest(void);

		void   bindFramebuffer(uint32 fbo);

		void flushCache(std::shared_ptr<maple::Shader> shader, uint32_t objectIndex, int32_t meshId);

		void initPipelineInfo(maple::PipelineInfo & info);

		struct FogData {
			float32 alphaRefLow;
			float32 alphaRefHigh;
			int32 objectId;
			int32 meshId;

			float32 fogStart;
			float32 fogEnd;
			float32 fogRange;
			float32 fogDisable;

			RGBAf fogColor;
		};

		const FogData &flushFog();

		class ObjPipeline : public rw::ObjPipeline
		{
		public:
			void                init(void);
			static ObjPipeline* create(void);

			void (*instanceCB)(Geometry* geo, InstanceDataHeader* header, bool32 reinstance);
			void (*uninstanceCB)(Geometry* geo, InstanceDataHeader* header);
			void (*renderCB)(Atomic* atomic, InstanceDataHeader* header);
			void (*beginUpdate)(Atomic* atomic, InstanceDataHeader* header);
		};

		void  defaultInstanceCB(Geometry* geo, InstanceDataHeader* header, bool32 reinstance);
		void  defaultUninstanceCB(Geometry* geo, InstanceDataHeader* header);
		void  defaultRenderCB(Atomic* atomic, InstanceDataHeader* header);
		void  defaultUpdateCB(Atomic* atomic, InstanceDataHeader* header);
		int32 lightingCB(Atomic* atomic);
		int32 lightingCB(void);

		void drawInst_simple(InstanceDataHeader* header, InstanceData* inst, std::shared_ptr<maple::DescriptorSet> matfx = nullptr, std::shared_ptr<maple::DescriptorSet> uniform = nullptr, int32_t objectId = -1, int32_t instanceId = -1);
		// Emulate PS2 GS alpha test FB_ONLY case: failed alpha writes to frame- but not to depth buffer
		void drawInst_GSemu(InstanceDataHeader* header, InstanceData* inst, std::shared_ptr<maple::DescriptorSet> matfx = nullptr, std::shared_ptr<maple::DescriptorSet> uniform = nullptr, int32_t objectId = -1, int32_t instanceId = -1);
		// This one switches between the above two depending on render state;
		void drawInst(InstanceDataHeader *header, InstanceData *inst, std::shared_ptr<maple::DescriptorSet> matfx = nullptr, std::shared_ptr<maple::DescriptorSet> uniform = nullptr, int32_t objectId = -1, int32_t instanceId = -1);

		void endLastPipeline();

		void* destroyNativeData(void* object, int32, int32);

		void runRenderDevice();

		ObjPipeline* makeDefaultPipeline(void);

		std::shared_ptr<maple::Pipeline> getPipeline(maple::DrawType drawType, uint32_t shaderId);
		void setPipeline(std::shared_ptr<maple::Pipeline>);
		void compareAndBind(std::shared_ptr<maple::Pipeline>);
		// Native Texture and Raster

		struct VulkanRaster
		{
			// arguments to glTexImage2D
			maple::TextureFormat internalFormat;
			int32 bpp;        // bytes per pixel
			// texture object
			bool isCompressed;//in theory, vulkan support compress. but now, we don't use them.
			bool hasAlpha;
			bool autogenMipmap;
			int8 numLevels;
			int32_t textureId = -1;

			//cache
			uint8 filterMode;
			uint8 addressU;
			uint8 addressV;
			int32 maxAnisotropy;
		};

		struct VulkanStats 
		{
			uint32_t numOfObjects = 0;
			uint32_t textureSets = 0;
			uint32_t currentUsedSets = 0;
		};

		extern VulkanStats vulkanStats;
		
		void allocateDXT(Raster* raster, int32 dxt, int32 numLevels, bool32 hasAlpha);

		Texture* readNativeTexture(Stream* stream);
		void     writeNativeTexture(Texture* tex, Stream* stream);
		uint32   getSizeNativeTexture(Texture* tex);

		extern int32 nativeRasterOffset;
		void         registerNativeRaster(void);

#	define GET_VULKAN_RASTEREXT(raster) PLUGINOFFSET(VulkanRaster, raster, rw::vulkan::nativeRasterOffset)
	}        // namespace vulkan
}        // namespace rw

#else

namespace rw
{
	namespace vulkan
	{
		void registerPlatformPlugins(void);

		Texture* readNativeTexture(Stream* stream);

		uint32   getSizeNativeTexture(Texture* tex);

		void     writeNativeTexture(Texture* tex, Stream* stream);
	}
}

#endif // RW_VULKAN

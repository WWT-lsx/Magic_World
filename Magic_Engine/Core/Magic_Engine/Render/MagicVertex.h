#ifndef _MAGICVERTEX_H_
#define _MAGICVERTEX_H_

#include "Define/Magic_Macro.h"

namespace Magic
{
	class DLL_MAGIC_ENGINE_OUTPUT_INPUT VERTEX_BUFFER
	{
	public:
		enum DRAW_USAGE
		{
			DYNAMIC_DRAW = 0,
			STREAM_DRAW,
			STATIC_DRAW
		};
	private:
		struct Buffer_Attribute
		{
			unsigned int Buffer_MaxSize;
			DRAW_USAGE usage;
			Buffer_Attribute();
		};

	public:
		VERTEX_BUFFER();
		virtual ~VERTEX_BUFFER();

		void Shutdown();

		/*
		功能:创建缓存区域
		参数:
		_number = 创建缓存区域数量
		返回值:空
		*/
		void CreateBuffer(unsigned char _number);

		/*
		功能:设置缓存区格式
		参数:
		_BufferPos = 设置第几个缓存区域
		_usage = DRAW_USAGE(渲染属性)
		_number = 排列对象数量
		_byteSize = 每个对象占的多少个Float(4字节)
		_divisor = 开始渲染一个新的实例的时候更新顶点属性的内容(0为每个顶点对应一个对象)
		返回值:空
		*/
		void SetBuffer(unsigned int _BufferPos, DRAW_USAGE _usage, unsigned int _number, unsigned int* _4byteSize, unsigned int _divisor = 0);

		/*
		功能:设置间接渲染那缓存区
		参数:
		_BufferPos = 设置第几个缓存区域
		_usage = DRAW_USAGE(渲染属性)
		返回值:空
		*/
		void SetDrawIndirectBuffer(unsigned int _BufferPos, DRAW_USAGE _usage);

		/*
		功能:设置索引缓存区
		参数:
		_BufferPos = 设置第几个缓存区域
		_usage = DRAW_USAGE(渲染属性)
		返回值:空
		*/
		void SetIndexBuffer(unsigned char _BufferPos, DRAW_USAGE _usage);

		/*
		功能:动态写入数据
		(写入GPU的数据大小将自动缓存最大的一次大小)
		参数:
		_BufferPos = 第几个缓存区域
		_size = 写入大小
		_data = 需要写入的数据
		返回值:空
		*/
		void DynamicWrite(unsigned int _BufferPos, unsigned int _size, void* _data);

		/*
		功能:静态写入数据
		(写入GPU的缓存大小不会发生改变所以尽量避免,不超过GPU的最大缓存数据)
		参数:
		_BufferPos = 第几个缓存区域
		_Offset = 写入的偏移地址
		_size = 写入大小
		_data = 需要写入的数据
		返回值:空
		*/
		void StaticWrite(unsigned int _BufferPos, unsigned int _Offset, unsigned int _size, void* _data);

		/*
		功能:映射缓存区
		参数:
		_BufferPos = 第几个缓存区域
		返回值:
		void* = 指向映射区域的指针
		*/
		void* StartUpdataVertex(unsigned char _BufferPos);

		/*
		功能:关闭映射缓存区
		参数:
		_BufferPos = 第几个缓存区域
		返回值:空
		*/
		void EndUpdataVertex(unsigned char _BufferPos);

		/*
		功能:绑定缓存区
		参数:
		_BufferPos = 第几个缓存区域
		返回值:空
		*/
		void BindBuffer(unsigned char _BufferPos);

		/*
		功能:关闭绑定缓存区
		参数:
		_BufferPos = 第几个缓存区域
		返回值:空
		*/
		void UnBindBuffer(unsigned char _BufferPos);

		/*
		功能:绑定主缓存区
		参数:空
		返回值:空
		*/
		void Bind();

		/*
		功能:关闭绑定主缓存区
		参数:空
		返回值:空
		*/
		void UnBind();

	private:
		char m_Index_Pos;
		char m_Draw_Indirect;
		char m_Buffer_Number;
		char m_Max_CardSlot;
		Buffer_Attribute* pBuffer_Attribute;
		unsigned int m_VAO;
		unsigned int* pVBO;
	};
}

class DLL_MAGIC_ENGINE_OUTPUT_INPUT MagicVertex
{
public:
	MagicVertex();
	virtual ~MagicVertex();

	void CreateVBO(const unsigned char &_number = 1, bool _ARRAYBuffer = false);
	void BindCreate(const unsigned char &_number, unsigned char _size, unsigned int _VertexNumber, void* _data);
	void Delete();

	void* StartUpdataVertex(const unsigned char &_number);
	void EndUpdataVertex();

	void* StartUpdataIndex();
	void EndUpdataIndex();

	unsigned int GetVAO() { return vaoHandle; }
	inline unsigned int GetVertexNumber() { return m_VertexNumber; }
	inline unsigned int GetIndexNumber() { return m_IndexNumber; }
private:
	unsigned char m_vboNumber;
	unsigned int m_VertexNumber, m_IndexNumber;
	unsigned int*vboHandles;
	unsigned int vaoHandle;//vertex array object 
};

#endif
#pragma once

#include "../SECore.h"
#include "../System/SEObject.h"

#include <SCFImaging.h>

namespace Resources
{
	class SECORE_API CTexture : public System::CSEObject
	{
	public:
		System::Category Category() _GET { return System::Category::Textures; }

	public:
		CString XmlName() _GET { return STRING("Texture"); }
		CString ToString() _GET { return STRING("{Texture}"); }

	public:
		CTexture(_IN ENUM eTarget);
		CTexture(_IN SCFImaging::CImage& rImage, _IN ENUM eTarget, _IN bool bMipmaps);
		CTexture();
		virtual ~CTexture();

	public:
		inline UINT GLID()    _GET { return m_uiID; }
		inline ENUM Target()  _GET { return m_eTarget; }
		inline bool Mipmaps() _GET { return m_bMipmaps; }

	public:
		inline void Minification(_IN ENUM eMinFilter) _SET { m_eMinification = eMinFilter; }
		inline ENUM Minification()                    _GET { return m_eMinification; }

		inline void Magnification(_IN ENUM eMagFilter) _SET { m_eMagnification = eMagFilter; }
		inline ENUM Magnification()                    _GET { return m_eMagnification; }

		inline void MaxAnisotropy(_IN UINT uiMaxAnisotropy) _SET { m_uiMaxAnisotropy = uiMaxAnisotropy; }
		inline UINT MaxAnisotropy()                         _GET { return m_uiMaxAnisotropy; }
 
		inline void CoordClamp(_IN ENUM eCoordClamp) _SET { m_eCoordClamp = eCoordClamp; }
		inline ENUM CoordClamp()                     _GET { return m_eCoordClamp; }

	public:
		inline void  Size(_IN int iWidth, _IN int iHeight) _SET { m_Size[0] = iWidth; m_Size[1] = iHeight; }
		inline void  Size(_IN Int2& rSize)                 _SET { CopyVector2(m_Size, rSize); }
		inline Int2& Size()                                _GET { return (Int2&)m_Size; }

		inline void Format(_IN ENUM eFormat) _SET { m_eFormat = eFormat; }
		inline ENUM Format()                 _GET { return m_eFormat; }
		
		inline void FormatInternal(_IN ENUM eFormat) _SET { m_eFormatInternal = eFormat; }
		inline ENUM FormatInternal()                 _GET { return m_eFormatInternal; }
		
		inline void DataType(_IN ENUM eDataType) _SET { m_eDataType = eDataType; }
		inline ENUM DataType()                   _GET { return m_eDataType; }

	public:
		void UpdateProperties();
		void UpdateData(_IN void* vpData);

	protected:
		//Load an image & store it as a texture	
		bool __fastcall LoadImage2D(_IN SCFImaging::CImage& rImage);
		bool __fastcall LoadImage3D(_IN BYTE* bpData, _IN UINT uiWidth, _IN UINT uiHeight, _IN UINT uiDepth, _IN GLenum eFormat);

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	protected:
		void Construct(_IN _OPT SCFImaging::CImage* pImage, _IN ENUM eTarget, _IN bool bMipmaps);

	protected:
		CString m_FileName;

	protected:
		//The texture target or type (1D, 2D, 3D, CubeMap, ..)
		ENUM m_eTarget;
		UINT m_uiID;

	protected:
		//Texture properties
		ENUM m_eCoordClamp;
		UINT m_uiMaxAnisotropy;

		ENUM m_eMinification;
		ENUM m_eMagnification;

	protected:
		ENUM m_eFormat;
		ENUM m_eFormatInternal;
		ENUM m_eDataType;

	protected:
		Int2 m_Size;
		bool m_bMipmaps;
	};
};

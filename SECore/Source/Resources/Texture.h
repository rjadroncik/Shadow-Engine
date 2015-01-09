#pragma once

#include "../SECore.h"
#include "../System/SEObject.h"

#include <SCFImaging.h>

namespace Resources
{
	class SECORE_API CTexture : public System::CSEObject
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassTexture; }
		CString   ToString() _GET { return STRING("{Texture}"); }

	public:
		CTexture(_IN SCF::ENUM eTarget);
		CTexture(_IN SCFImaging::CImage& rImage, _IN SCF::ENUM eTarget, _IN bool bMipmaps);
		CTexture();
		virtual ~CTexture();

	public:
		inline SCF::UINT GLID()    _GET { return m_uiID; }
		inline SCF::ENUM Target()  _GET { return m_eTarget; }
		inline bool      Mipmaps() _GET { return m_bMipmaps; }

	public:
		inline void      Minification(_IN SCF::ENUM eMinFilter) _SET { m_eMinification = eMinFilter; }
		inline SCF::ENUM Minification()                         _GET { return m_eMinification; }

		inline void      Magnification(_IN SCF::ENUM eMagFilter) _SET { m_eMagnification = eMagFilter; }
		inline SCF::ENUM Magnification()                         _GET { return m_eMagnification; }

		inline void      MaxAnisotropy(_IN SCF::UINT uiMaxAnisotropy) _SET { m_uiMaxAnisotropy = uiMaxAnisotropy; }
		inline SCF::UINT MaxAnisotropy()                              _GET { return m_uiMaxAnisotropy; }

		inline void      CoordClamp(_IN SCF::ENUM eCoordClamp) _SET { m_eCoordClamp = eCoordClamp; }
		inline SCF::ENUM CoordClamp()                          _GET { return m_eCoordClamp; }

	public:
		inline void  Size(_IN int iWidth, _IN int iHeight) _SET { m_Size[0] = iWidth; m_Size[1] = iHeight; }
		inline void  Size(_IN Int2& rSize)                 _SET { CopyVector2(m_Size, rSize); }
		inline Int2& Size()                                _GET { return (Int2&)m_Size; }

		inline void      Format(_IN SCF::ENUM eFormat) _SET { m_eFormat = eFormat; }
		inline SCF::ENUM Format()                      _GET { return m_eFormat; }
		
		inline void      FormatInternal(_IN SCF::ENUM eFormat) _SET { m_eFormatInternal = eFormat; }
		inline SCF::ENUM FormatInternal()                      _GET { return m_eFormatInternal; }
		
		inline void      DataType(_IN SCF::ENUM eDataType) _SET { m_eDataType = eDataType; }
		inline SCF::ENUM DataType()                        _GET { return m_eDataType; }

	public:
		void UpdateProperties();
		void UpdateData(_IN void* vpData);

	protected:
		//Load an image & store it as a texture	
		bool __fastcall LoadImage2D(_IN SCFImaging::CImage& rImage);
		bool __fastcall LoadImage3D(_IN SCF::BYTE* bpData, _IN SCF::UINT uiWidth, _IN SCF::UINT uiHeight, _IN SCF::UINT uiDepth, _IN GLenum eFormat);

	public:
		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	protected:
		void Construct(_IN _OPT SCFImaging::CImage* pImage, _IN SCF::ENUM eTarget, _IN bool bMipmaps);

	protected:
		CString m_FileName;

	protected:
		//The texture target or type (1D, 2D, 3D, CubeMap, ..)
		SCF::ENUM m_eTarget;
		SCF::UINT m_uiID;

	protected:
		//Texture properties
		SCF::ENUM m_eCoordClamp;
		SCF::UINT m_uiMaxAnisotropy;

		SCF::ENUM m_eMinification;
		SCF::ENUM m_eMagnification;

	protected:
		SCF::ENUM m_eFormat;
		SCF::ENUM m_eFormatInternal;
		SCF::ENUM m_eDataType;

	protected:
		Int2 m_Size;
		bool m_bMipmaps;
	};
};

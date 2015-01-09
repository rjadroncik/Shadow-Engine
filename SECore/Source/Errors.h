#pragma once

enum CoreErrors
{
	ErrorHardwareUnsupported = 0x1000,
	
	ErrorSettingLoadFailed,
	ErrorGUILoadFailed,

	ErrorDisplaySettingsEnumFailed,
	ErrorDisplayModeSetFailed,

	ErrorPixelFormatChooseFailed,
	ErrorPixelFormatSetFailed,


	ErrorWindowClassRegisterFailed,
	ErrorWindowClassUnregisterFailed,

	ErrorWindowDCGetFailed,
	ErrorWindowDCReleaseFailed,

	ErrorWindowCreateFailed,
	ErrorWindowDestroyFailed,

	
	ErrorGLContextCreateFailed,
	ErrorGLContextMakeCurrentFailed,
	ErrorGLContextDeleteFailed,

	ErrorGLGPUProgramLoadFailed,


	ErrorDIObjectMainCreationFailed,
	ErrorDIObjectMouseCreationFailed,
	ErrorDIObjectKeyboardCreationFailed,


	ErrorAccessorPropertyUnrecognized,
	ErrorAccessorPropertyChangeDenied,
	ErrorAccessorPropertyTypeMismatch,
};


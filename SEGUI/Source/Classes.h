#pragma once

#ifdef SEGUI_EXPORTS
#define SEGUI_API __declspec(dllexport)
#else
#define SEGUI_API __declspec(dllimport)
#endif

enum GUIClasses
{
	ClassText = 0x2000,
	ClassLabel,
	ClassEditField,
	ClassWindow,
	ClassModule,
	ClassButtonClick,
	ClassButtonCheck,
	ClassButtonPush,
	ClassButtonRadio,
	ClassFont,
	ClassCursor,
	ClassCarret,

	ClassLayoutTable = 0x20F0,
	ClassLayoutSplit,
	ClassWindowManager,
};

namespace GUI
{
	enum Actions
	{
		//Button actions
		ActionClicked = 1,
		ActionActivated,
		ActionDeactivated,

		//Window actions
		ActionClosed,
	};

	#define AligmentMaskHorizontal 0x0F
	#define AligmentMaskVertical   0xF0

	enum Alignments
	{
		AlignCenter = 0x00, //The default alignment

		AlignLeft   = 0x01,
		AlignRight  = 0x02,
		AlignTop    = 0x10,
		AlignBottom = 0x20,
	};
};

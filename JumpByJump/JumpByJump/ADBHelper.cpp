#include "stdafx.h"
#include "ADBHelper.h"
#include "include\opencv\highgui.h"
#include <stdlib.h>

void ADBHelper::CapScreen()
{
	system("adb shell screencap -p /sdcard/JumpByJump.png");
	system("adb pull /sdcard/JumpByJump.png");
}

void ADBHelper::jump(int pressTime)
{
	char cmd[256] = { 0 };
	sprintf_s(cmd, "adb shell input swipe 320 410 320 410 %d", pressTime);
	system(cmd);
}

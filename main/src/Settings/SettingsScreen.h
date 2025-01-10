#ifndef CLOCKSTAR_FIRMWARE_SETTINGSSCREEN_H
#define CLOCKSTAR_FIRMWARE_SETTINGSSCREEN_H

#include "LV_Interface/LVScreen.h"
#include "LV_Interface/LVModal.h"

class SettingsScreen : public LVScreen {
public:
	SettingsScreen();

private:
	virtual void onStarting() override;
	virtual void onStart() override;
	virtual void onStop() override;

	void buildUI();

	lv_obj_t* bg = nullptr;
	class DiscreteSliderElement* sleepSlider = nullptr;
	class SliderElement* brightnessSlider = nullptr;
	class BoolElement* audioSwitch = nullptr;
	class LabelElement* factoryReset = nullptr;
	class LabelElement* saveAndExit = nullptr;

	class Prompt* factoryResetPrompt = nullptr;

	static constexpr const char* BgPaths[6] = {
			"S:/Bg/Level1.bin",
			"S:/Bg/Level2.bin",
			"S:/Bg/Level3.bin",
			"S:/Bg/Level4.bin",
			"S:/Bg/Level5.bin",
			"S:/Bg/Level6.bin"
	};
};


#endif //CLOCKSTAR_FIRMWARE_SETTINGSSCREEN_H

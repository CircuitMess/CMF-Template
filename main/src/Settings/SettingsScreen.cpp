#include "SettingsScreen.h"
#include "BoolElement.h"
#include "SliderElement.h"
#include "LabelElement.h"
#include "DiscreteSliderElement.h"
#include "LV_Interface/LVGL.h"
#include "LV_Interface/InputLVGL.h"
#include "LV_Interface/FSLVGL.h"
#include "nvs_flash.h"
#include "Util/stdafx.h"

static constexpr uint8_t SleepSteps = 5;
static constexpr const uint32_t SleepSeconds[SleepSteps] = { 0, 30, 60, 2 * 60, 5 * 60 };
static constexpr const char* SleepText[SleepSteps] = { "OFF", "30 sec", "1 min", "2 min", "5 min" };


SettingsScreen::SettingsScreen() {
	buildUI();
}

void SettingsScreen::onStop(){

}

void SettingsScreen::onStarting(){

}

void SettingsScreen::onStart(){
}

void SettingsScreen::buildUI(){
	lv_obj_set_size(*this, 128, 128);

	bg = lv_obj_create(*this);
	lv_obj_add_flag(bg, LV_OBJ_FLAG_FLOATING);
	lv_obj_set_size(bg, 128, 128);
	lv_obj_set_pos(bg, 0, 0);
	lv_obj_set_style_bg_color(bg, lv_color_black(), 0);

	const auto lvl = std::clamp((int) 5 - 1, 0, 5);

	lv_obj_set_style_bg_image_src(bg, BgPaths[lvl], 0);
	lv_obj_set_style_bg_image_opa(bg, LV_OPA_30, 0);
	lv_obj_set_style_bg_opa(bg, LV_OPA_COVER, 0);

	lv_obj_set_flex_flow(*this, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(*this, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(*this, 5, 0);
	lv_obj_set_style_pad_hor(*this, 1, 0);
	lv_obj_set_style_pad_bottom(*this, 1, 0);


	auto values = std::vector<const char*>(SleepText, SleepText + SleepSteps);

	sleepSlider = new DiscreteSliderElement(*this, "Sleep time", [this](uint8_t value){
	}, values, 0);
	lv_group_add_obj(inputGroup, *sleepSlider);

	brightnessSlider = new SliderElement(*this, "Brightness", [this](uint8_t value){
	}, 100);
	lv_group_add_obj(inputGroup, *brightnessSlider);

	audioSwitch = new BoolElement(*this, "Sound", [this](bool value){

	}, true);
	lv_group_add_obj(inputGroup, *audioSwitch);

	factoryReset = new LabelElement(*this, "Factory reset", [this](){

	}, LV_ALIGN_LEFT_MID);
	lv_group_add_obj(inputGroup, *factoryReset);


	saveAndExit = new LabelElement(*this, "Save and Exit", [this](){

	}, LV_ALIGN_LEFT_MID);
	lv_group_add_obj(inputGroup, *saveAndExit);


	lv_obj_add_flag(*this, LV_OBJ_FLAG_SCROLLABLE);
}

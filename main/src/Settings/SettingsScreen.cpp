#include "SettingsScreen.h"
#include "LV_Interface/InputLVGL.h"
#include "BoolElement.h"
#include "SliderElement.h"
#include "DiscreteSliderElement.h"

static constexpr uint8_t SleepSteps = 5;
static constexpr const uint32_t SleepSeconds[SleepSteps] = { 0, 30, 60, 2 * 60, 5 * 60 };
static constexpr const char* SleepText[SleepSteps] = { "OFF", "30 sec", "1 min", "2 min", "5 min" };

enum class Theme : uint8_t {
	Theme1,
	Theme2,
	Theme3,
	Theme4,
	COUNT
};

struct SettingsStruct {
	bool sound = true;
	uint8_t screenBrightness = 100;
	uint8_t sleepTime = 3;
	Theme theme = Theme::Theme1;
	uint8_t avatar = 0;
	int8_t pet = -1;
};

SettingsScreen::SettingsScreen(){
	buildUI();
}

void SettingsScreen::onStart(){
	if(bg != nullptr){
		bg->start();
	}

	InputLVGL::getInstance()->setVertNav(true);
}

void SettingsScreen::onStop(){
	if(bg != nullptr){
		bg->stop();
	}

	InputLVGL::getInstance()->setVertNav(false);
}

void SettingsScreen::buildUI(){
	lv_obj_set_flex_flow(*this, LV_FLEX_FLOW_COLUMN);

	bg = new LVGIF(*this, "S:/bg");
	lv_obj_add_flag(*bg, LV_OBJ_FLAG_FLOATING);
	lv_obj_set_pos(*bg, 0, 0);

	auto top = lv_obj_create(*this);
	lv_obj_set_size(top, 128, 32);
	lv_obj_set_style_pad_ver(top, 4, 0);

	auto img = lv_img_create(top);
	lv_img_set_src(img, "S:/Theme1/settings.bin");
	lv_obj_center(img);

	auto rest = lv_obj_create(*this);
	lv_obj_set_size(rest, 128, 96);
	lv_obj_set_flex_flow(rest, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(rest, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_all(rest, 4, 0);

	lv_style_set_width(itemStyle, lv_pct(100));
	lv_style_set_height(itemStyle, 17);
	lv_style_set_border_width(itemStyle, 1);
	lv_style_set_border_color(itemStyle, lv_color_hex(0xd999ba));
	lv_style_set_border_opa(itemStyle, LV_OPA_COVER);
	lv_style_set_radius(itemStyle, 2);

	lv_style_set_bg_color(focusStyle, lv_color_hex(0xd999ba));
	lv_style_set_bg_opa(focusStyle, LV_OPA_70);

	auto initSet = SettingsStruct{};

	audioSwitch = new BoolElement(rest, "Sound", [this](bool value){
	}, initSet.sound);
	lv_group_add_obj(inputGroup, *audioSwitch);
	lv_obj_add_style(*audioSwitch, focusStyle, LV_STATE_FOCUSED);

	blSlider = new SliderElement(rest, "Brightness", [](uint8_t value){

	}, initSet.screenBrightness);
	lv_group_add_obj(inputGroup, *blSlider);
	lv_obj_add_style(*blSlider, focusStyle, LV_STATE_FOCUSED);


	sleepSlider = new DiscreteSliderElement(rest, "Sleep time", [this](uint8_t value){

	}, std::vector<const char*>(SleepText, SleepText + SleepSteps), initSet.sleepTime);
	lv_group_add_obj(inputGroup, *sleepSlider);
	lv_obj_add_style(*sleepSlider, focusStyle, LV_STATE_FOCUSED);

	auto mkBtn = [this, &rest](const char* text){
		auto item = lv_obj_create(rest);
		lv_group_add_obj(inputGroup, item);
		lv_obj_add_style(item, itemStyle, 0);
		lv_obj_add_style(item, focusStyle, LV_STATE_FOCUSED);
		lv_obj_add_flag(item, LV_OBJ_FLAG_CLICKABLE);
		lv_obj_clear_flag(item, LV_OBJ_FLAG_SCROLLABLE);

		auto label = lv_label_create(item);
		lv_label_set_text(label, text);
		lv_obj_set_size(label, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
		lv_obj_center(label);
		lv_obj_set_style_text_color(label, lv_color_hex(0xcfc6b8), 0);

		return item;
	};

	auto save = mkBtn("Save and exit");

}

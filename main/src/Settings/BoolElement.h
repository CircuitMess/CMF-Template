#ifndef CLOCKSTAR_FIRMWARE_BOOLELEMENT_H
#define CLOCKSTAR_FIRMWARE_BOOLELEMENT_H


#include <functional>
#include "LV_Interface/LVObject.h"
#include "LV_Interface/LVStyle.h"

class BoolElement : public LVObject {
public:
	explicit BoolElement(lv_obj_t* parent, const char* name, std::function<void(bool)> cb, bool value = false);

	void setValue(bool value);
	[[nodiscard]] bool getValue() const;

private:
	lv_obj_t* label;
	lv_obj_t* switchElement;
	LVStyle defaultStyle;
	LVStyle labelStyle;
	LVStyle switchStyle;
	LVStyle switchCheckedStyle;
	LVStyle switchKnobStyle;

	static lv_style_transition_dsc_t Transition;
	constexpr static const lv_style_prop_t TransProps[] = {
			LV_STYLE_BG_OPA, LV_STYLE_BG_COLOR,
			LV_STYLE_TRANSFORM_WIDTH, LV_STYLE_TRANSFORM_HEIGHT,
			LV_STYLE_TRANSLATE_Y, LV_STYLE_TRANSLATE_X,
			LV_STYLE_TRANSFORM_SCALE_X, LV_STYLE_TRANSFORM_SCALE_Y, LV_STYLE_TRANSFORM_ROTATION,
			LV_STYLE_COLOR_FILTER_OPA, LV_STYLE_COLOR_FILTER_DSC,
			LV_STYLE_PROP_INV
	};

	bool value;
	std::function<void(bool)> cb;

	static constexpr lv_style_selector_t SelDefault = LV_PART_MAIN | LV_STATE_DEFAULT;

	static constexpr uint8_t SwitchWidth = 24;
	static constexpr uint8_t SwitchHeight = 9;
	static constexpr uint8_t Height = 23;
};

#endif //CLOCKSTAR_FIRMWARE_BOOLELEMENT_H

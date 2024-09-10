#include <Core/EntryPoint.h>
#include "Misc/Enum.h"
#include "Periphery/GPIO.h"
#include "Devices/ButtonInput.h"
#include "Drivers/InputGPIO.h"
#include "Drivers/OutputGPIO.h"
#include "Devices/LED/LED.h"
#include "Devices/LED/LEDBlinkFunction.h"

class TestApp : public Application {
	GENERATED_BODY(TestApp, Application)

	DECLARE_ENUM(ButtonEnum, A);
	enum class SingleLEDs : uint8_t{
		A, B
	};
	enum class RGBLEDs : uint8_t{
		BatteryLED
	};
protected:
	virtual void begin() noexcept override {
		Super::begin();


		esp_log_level_set("LogCMF", ESP_LOG_VERBOSE);

		registerPeriphery<GPIO>();
		std::vector<InputPinDef> inputs = {
				{GPIO_NUM_7, true, PullMode::None}
		};
		registerDriver<InputGPIO>(inputs, getPeriphery<GPIO>());
		registerDevice<ButtonInput>();
		auto bi = getDevice<ButtonInput>();
		bi->reg(ButtonEnum::A, { getDriver<InputGPIO>(), GPIO_NUM_7});

		std::vector<OutputPinDef> outputs = {
				{GPIO_NUM_17, false}
		};
		registerDriver<OutputGPIO>(outputs, getPeriphery<GPIO>());
		registerDevice<LED<SingleLEDs, RGBLEDs>>();
		auto *led = getDevice<LED<SingleLEDs, RGBLEDs>>();
		led->reg(SingleLEDs::A, { getDriver<OutputGPIO>(), GPIO_NUM_17});

		auto func = LEDBlinkFunction<SingleLEDs, float>(0, 0.5f);
		led->set(SingleLEDs::A, std::make_unique<LEDBlinkFunction<SingleLEDs, float>>(0, 0.25f));
	}

	virtual void tick(float deltaTime) noexcept override {
		Super::tick(deltaTime);

		static bool prevState = false;
		auto bi = getDevice<ButtonInput>();
		auto led = getDevice<LED<SingleLEDs, RGBLEDs>>();

		bool state = bi->getState(ButtonEnum::A);
		if(state == prevState) return;

		if(state){
//			led->off(SingleLEDs::A);
			led->set(SingleLEDs::A, std::make_unique<LEDBlinkFunction<SingleLEDs, float>>(1, 1.0f), true);
		}else{
			led->set(SingleLEDs::A, std::make_unique<LEDBlinkFunction<SingleLEDs, float>>(0, 0.25f));
		}
		prevState = state;

	}

	virtual void onDestroy() noexcept override {
		Super::onDestroy();
		TRACE_LOG("");
	}
};

CMF_MAIN(TestApp)
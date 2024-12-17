#include <Core/EntryPoint.h>
#include "Periphery/GPIOPeriph.h"
#include "Drivers/Interface/InputDriver.h"
#include "Drivers/Input/InputGPIO.h"
#include "Services/ButtonInput.h"
#include "Pins.hpp"
#include "Devices/Display.h"
#include "LV_Interface/LVGL.h"
#include "LV_Interface/InputLVGL.h"
#include "LV_Interface/FSLVGL.h"
#include "src/Settings/SettingsScreen.h"
#include "FS/SPIFFS.h"

//HW to test on - Bit v2/v1
class TestApp : public Application {
	GENERATED_BODY(TestApp, Application)

	DECLARE_ENUM(Button, Up, Down, Left, Right, A, B, Menu);

	const std::map<Button, const char*> PinLabels{
			{ Button::Up,    "Up" },
			{ Button::Down,  "Down" },
			{ Button::Left,  "Left" },
			{ Button::Right, "Right" },
			{ Button::A,     "A" },
			{ Button::B,     "B" },
			{ Button::Menu,  "Menu" }
	};

	const std::map<Enum<int>, lv_key_t> LVGL_mappings{
			{ Button::Up,    LV_KEY_UP },
			{ Button::Down,  LV_KEY_DOWN },
			{ Button::Left,  LV_KEY_LEFT },
			{ Button::Right, LV_KEY_RIGHT },
			{ Button::A,     LV_KEY_ENTER },
			{ Button::B,     LV_KEY_ESC }
	};
protected:
	virtual void begin() noexcept override{
		Super::begin();

		std::vector<GPIOPinDef> inputs = {
				{{ BTN_UP,    true }, PullMode::Up },
				{{ BTN_DOWN,  true }, PullMode::Up },
				{{ BTN_LEFT,  true }, PullMode::Up },
				{{ BTN_RIGHT, true }, PullMode::Up },
				{{ BTN_A,     true }, PullMode::Up },
				{{ BTN_B,     true }, PullMode::Up },
				{{ BTN_MENU,  true }, PullMode::Up }
		};

		auto gpio = registerPeriphery<GPIOPeriph>();
		auto inputGPIO = registerDriver<InputGPIO>(inputs, gpio);

		auto bi = registerService<ButtonInput>();
		std::vector<std::pair<Enum<int>, InputPin>> buttons = {
				{ Button::Up,    { inputGPIO, BTN_UP }},
				{ Button::Down,  { inputGPIO, BTN_DOWN }},
				{ Button::Left,  { inputGPIO, BTN_LEFT }},
				{ Button::Right, { inputGPIO, BTN_RIGHT }},
				{ Button::A,     { inputGPIO, BTN_A }},
				{ Button::B,     { inputGPIO, BTN_B }},
				{ Button::Menu,  { inputGPIO, BTN_MENU }}
		};


		bi->reg(buttons);

		auto disp = registerDevice<Display>(
				lgfx::Bus_SPI::config_t{
						.freq_write = 40000000,
						.freq_read = 40000000,
						.pin_sclk = TFT_SCK,
						.pin_miso = -1,
						.pin_mosi = TFT_MOSI,
						.pin_dc = TFT_DC,
						.spi_mode = 0,
						.spi_3wire = false,
						.use_lock = false,
						.dma_channel = LGFX_ESP32_SPI_DMA_CH,
						.spi_host = SPI2_HOST
				},
				lgfx::Panel_Device::config_t{
						.pin_cs = -1,
						.pin_rst = TFT_RST,
						.pin_busy = -1,
						.memory_width = 132,
						.memory_height = 132,
						.panel_width = 128,
						.panel_height = 128,
						.offset_x = 2,
						.offset_y = 1,
						.offset_rotation = 2,
						.readable = false,
						.invert = false,
						.rgb_order = false,
						.dlen_16bit = false,
						.bus_shared = false
				}, [](Sprite& canvas){
					canvas.setColorDepth(lgfx::rgb565_2Byte);
					canvas.createSprite(128, 128);
				});

		printf("lvgl created\n");
		auto lvgl = newObject<LVGL>(this, disp);
		auto lvInput = new InputLVGL(bi, LVGL_mappings);

		if(!SPIFFS::init()) return;

		auto lvFS = new FSLVGL('S');

//		lvgl->startScreen([](){ return std::make_unique<SettingsScreen>(); });


		printf("end of App::begin()\n");


/*		bi->event.bind(this, [this](Enum<int> key, ButtonInput::Action action){
			if(action == ButtonInput::Action::Press){
				printf("Button %d pressed\n", (int) key);
			}else{
				printf("Button %d released\n", (int) key);
			}
		});*/

	}

	virtual void tick(float deltaTime) noexcept override{
	}

	virtual void onDestroy() noexcept override{
		Super::onDestroy();
	}
};

CMF_MAIN(TestApp)
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
#include "themes/lv_theme_private.h"


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

	StrongObjectPtr<LVGL> lvgl;
	StrongObjectPtr<InputLVGL> inputLVGL;
	StrongObjectPtr<FSLVGL> fsLVGL;
	static lv_style_t scr;
	static lv_style_t label;
	lv_theme_t* theme;

	static void applyTheme(lv_theme_t* th, lv_obj_t* obj){
		if(lv_obj_get_parent(obj) == nullptr){
			lv_obj_add_style(obj, &scr, 0);
		}else if(lv_obj_check_type(obj, &lv_label_class)){
			lv_obj_add_style(obj, &label, 0);
		}
	}

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
		disp->getLGFX().setSwapBytes(true);

		bi->event.bind(this, [this](Enum<int> key, ButtonInput::Action action){
			if(action == ButtonInput::Action::Press){
				printf("Button %d pressed\n", (int) key);
			}else{
				printf("Button %d released\n", (int) key);
			}
		});

		LV_FONT_DECLARE(devin);

		lvgl = newObject<LVGL>(this, disp, [this](lv_disp_t* disp) -> lv_theme_t*{
			lv_style_init(&scr);
			lv_style_set_bg_color(&scr, lv_color_black());
			lv_style_set_bg_opa(&scr, LV_OPA_COVER);

			lv_style_init(&label);
			lv_style_set_text_font(&label, &devin);
			lv_style_set_text_color(&label, lv_color_make(207, 198, 184));
			lv_style_set_text_line_space(&label, 2);

			theme = lv_theme_simple_init(disp);
			lv_theme_set_apply_cb(theme, applyTheme);
			theme->disp = disp;
			theme->color_primary = lv_color_make(244, 126, 27);
			theme->color_secondary = lv_color_make(207, 198, 184);
			theme->font_small = &devin;
			theme->font_normal = &devin;
			theme->font_large = &devin;
			return theme;
		});
		inputLVGL = newObject<InputLVGL>(this, bi, LVGL_mappings);

		if(!SPIFFS::init()) return;

		fsLVGL = newObject<FSLVGL>(this, 'S');

		lvgl->startScreen([](){ return std::make_unique<SettingsScreen>(); });

	}

	virtual void tick(float deltaTime) noexcept override{
		Super::tick(deltaTime);
	}

	virtual void onDestroy() noexcept override{
		Super::onDestroy();
	}
};

lv_style_t TestApp::scr = lv_style_t{};
lv_style_t TestApp::label = lv_style_t{};

CMF_MAIN(TestApp)
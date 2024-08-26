#include <Core/EntryPoint.h>
#include "Misc/Enum.h"

class TestApp : public Application {
	GENERATED_BODY(TestApp, Application)

protected:
	virtual void begin() noexcept override {
		Super::begin();
		TRACE_LOG("");
	}

	virtual void tick(float deltaTime) noexcept override {
		Super::tick(deltaTime);
		TRACE_LOG("");
	}

	virtual void onDestroy() noexcept override {
		Super::onDestroy();
		TRACE_LOG("");
	}
};

CMF_MAIN(TestApp)
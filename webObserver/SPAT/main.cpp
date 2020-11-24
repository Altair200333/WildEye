#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "SPATEngine.h"
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "winhttp.lib")

int main()
{
    SPATEngine app;
    app.run();
	
	return 0;
}

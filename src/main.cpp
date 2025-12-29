#include "utils.h"
#include "renderer.h"
#include "debugger.h"

using namespace std;
using namespace CDBugger;

int main(void){
	Renderer renderer = Renderer();
	renderer.init();

	Uint64 prev_time = 0;
	Uint64 now_time = 0;
	bool run = true;
	while(run){
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_EVENT_QUIT)
			{
				run = false;
			}
		}
		now_time = SDL_GetPerformanceCounter();
		renderer.delta_time = (double)(now_time - prev_time) / (double)SDL_GetPerformanceFrequency();
		renderer.FPS = 1.0 / renderer.delta_time;
		prev_time = now_time;
		renderer.time = (double)now_time / (double)SDL_GetPerformanceFrequency();

		renderer.render();
	}

	renderer.deinit();

	return 0;
}

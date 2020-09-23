#include <SDL2/SDL.h>

int main(int argc, char **argv)
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	int running = 1;
	SDL_Event event;
	SDL_Rect rect = {0, 0, 16, 16};
	Uint32 last = -125;
	SDL_Point dir = {-1, 0}, apple = {4, 4}, *head, *new_head;
	int i;
	
	struct {
		SDL_Point segments[256];
		int start;
		int length;
	} snake;
	
	srand(time(0));
	
	snake.segments[0].x = 8;
	snake.segments[0].y = 8;
	snake.start = 0;
	snake.length = 1;
	
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 256, 256, 0);
	renderer = SDL_CreateRenderer(window, -1, 0);
	
	while(running) {
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_LEFT:
							dir.x = -1;
							dir.y = 0;
							break;
						case SDLK_RIGHT:
							dir.x = +1;
							dir.y = 0;
							break;
						case SDLK_UP:
							dir.x = 0;
							dir.y = -1;
							break;
						case SDLK_DOWN:
							dir.x = 0;
							dir.y = +1;
							break;
					}
					
					break;
			}
		}
		
		if(last + 125 < SDL_GetTicks()) {
			last += 125;
			
			head = &snake.segments[snake.start];
			snake.start = (snake.start + 255) % 256;
			new_head = &snake.segments[snake.start];
			new_head->x = (head->x + 16 + dir.x) % 16;
			new_head->y = (head->y + 16 + dir.y) % 16;
			snake.length ++;
			
			if(new_head->x == apple.x && new_head->y == apple.y) {
				
				do {
					apple.x = rand() % 16;
					apple.y = rand() % 16;
					
					for(i = 0; i < snake.length; i ++) {
						SDL_Point *p = &snake.segments[(i + snake.start) % 256];
						
						if(apple.x == p->x && apple.y == p->y) {
							apple.x = -1;
							break;
						}
					}
				} while(apple.x == -1);
			}
			else {
				for(i = 1; i < snake.length; i ++) {
					SDL_Point *p = &snake.segments[(i + snake.start) % 256];
					
					if(new_head->x == p->x && new_head->y == p->y) {
						snake.length = 1;
						break;
					}
				}
				
				if(snake.length > 1) {
					snake.length --;
				}
			}
			
			SDL_SetRenderDrawColor(renderer, 8, 16, 32, 0);
			SDL_RenderClear(renderer);
			
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			rect.x = 16 * apple.x;
			rect.y = 16 * apple.y;
			SDL_RenderFillRect(renderer, &rect);
			
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			
			for(i = 0; i < snake.length; i ++) {
				SDL_Point *p = &snake.segments[(i + snake.start) % 256];
				
				rect.x = 16 * p->x;
				rect.y = 16 * p->y;
				SDL_RenderFillRect(renderer, &rect);
			}
			
			SDL_RenderPresent(renderer);
		}
	}
	
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

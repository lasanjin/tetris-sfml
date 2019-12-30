#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>

using namespace sf;
using namespace std;

string get_path();
bool check();

const int M {20};
const int N {10};
const int offset {18};

int board[M][N] {0};

struct Position {
	int x, y;
} a[4], b[4];

int tetriminos[7][4] {
	{3,5,7,6}, 	// J
	{3,5,4,7}, 	// T
	{3,4,5,6}, 	// Z
	{2,4,5,7}, 	// S
	{2,3,4,5}, 	// O
	{1,3,5,7}, 	// I
	{2,5,3,7} 	// L
};


int main() {
	srand(time(0));
	RenderWindow window(VideoMode(180, 360), "TETRIS");
	window.setKeyRepeatEnabled(false);
	Clock clock;
	Texture texture;
	string file_path {get_path()};
	texture.loadFromFile("tiles.png");
	Sprite tetrimino(texture);

	int dx {0};
	bool rotate {0};
	int color_num {rand() % 7};
	float timer {0};
	float delay {0.3};
	int next = color_num;

	for(unsigned i{0}; i < 4; i++) {
		a[i].x = b[i].x = (tetriminos[next][i] % 2) + 4;
		a[i].y = b[i].y = (tetriminos[next][i] / 2) - 4;
	}

	while(window.isOpen()) {

		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;
		Event event;

		while(window.pollEvent(event)) {

			if(event.type == Event::Closed) {
				window.close();
			}

			if(event.type == Event::KeyPressed) {

				switch(event.key.code) {
				case Keyboard::Up:
					rotate = 1;
					break;
				case Keyboard::Left:
					dx = -1;
					break;
				case Keyboard::Right:
					dx = 1;
					break;
				case Keyboard::Space:
					delay = 0;
					break;
				default:
					break;
				}
			}
		}

		if(Keyboard::isKeyPressed(Keyboard::Down)) {
			delay = 0.05;
		}

		/*
		 * Move
		 */
		for(unsigned i{0}; i < 4; i++) {
			b[i] = a[i]; // Save safe current tetrimino pos
			a[i].x += dx; // Move
		}

		if(!check()) { // If move not allowed
			for(unsigned i{0}; i < 4; i++) {
				a[i] = b[i]; // Set safe tetrimino pos
			}
		}

		/*
		 * Rotate
		 */
		if(next != 4 && rotate) { // Don't rotate O
			/*
			 * Left-handed Cartesian coordinate system,
			 * (x directed to the right but y directed down),
			 * clockwise rotation:
			 * |cosθ -sinθ|
			 * |sinθ  cosθ|
			 *
			 * Counter-clockwise:
			 * |cosθ  sinθ|
			 * |-sinθ cosθ|
			 *
			 * Coordinates (x′,y′) of point (x,y) after rotation are:
			 * x' = xcosθ - ysinθ
			 * y' = xsinθ + ycosθ
			 *
			 * For 90° rotation:
			 * |0 -1|
			 * |1  0|
			 */
			Position p = a[1]; // Center of rotation
			for(int i{0}; i < 4; i++) {
				int dx_ = a[i].y - p.y;	// ysinθ
				int dy_ = a[i].x - p.x;	// xsinθ
				a[i].x = p.x - dx_;
				a[i].y = p.y + dy_;
			}

			if(!check()) {
				for(unsigned i{0}; i < 4; i++) {
					a[i] = b[i];
				}
			}
		}

		/*
		 * Tick
		 */
		if(timer > delay) {
			for(unsigned i{0}; i < 4; i++) {
				b[i] = a[i]; // Save safe current tetrimino position
				a[i].y += 1;
			}

			if(!check()) {
				for(unsigned i{0}; i < 4; i++) {
					board[b[i].y][b[i].x] = color_num;
				}

				color_num = rand() % 7;
				next = color_num;
				for(unsigned i{0}; i < 4; i++) {
					a[i].x = (tetriminos[next][i] % 2) + (N/2) - 1;
					a[i].y = (tetriminos[next][i] / 2) - 4;
				}
			}

			timer = 0;
		}

		/*
		 * Check lines
		 */
		int k {M-1};
		for(int i{M-1}; i > 0; i--) {

			int count {0};
			for(int j=0; j<N; j++) {

				if(board[i][j]) {
					count++;
				}
				board[k][j] = board[i][j];
			}
			if(count < N) {
				k--;
			}
		}

		dx = 0;
		delay = 0.3;
		rotate = 0;

		/*
		 * Draw
		 */
		window.clear(Color::White);

		for(unsigned i{0}; i < M; i++) {
			for(unsigned j{0}; j < N; j++) {

				if(board[i][j] == 0) {
					continue;
				}

				tetrimino.setTextureRect(IntRect(board[i][j]*offset, 0, offset, offset));
				tetrimino.setPosition(j*offset, i*offset);
				window.draw(tetrimino);
			}
		}

		for(unsigned i{0}; i < 4; i++) {
			tetrimino.setTextureRect(IntRect(color_num*offset, 0, offset, offset));
			tetrimino.setPosition(a[i].x*offset, a[i].y*offset);
			window.draw(tetrimino);
		}

		window.display();
	}

	return 0;
}

bool check() {
	for(unsigned i{0}; i < 4; i++) {
		if(a[i].x < 0
		        || a[i].x >= N
		        || a[i].y >= M) {
			return 0;
		}
		else if(a[i].y >= 0 && board[a[i].y][a[i].x]) {
			return 0;
		}
	}
	return 1;
}

string get_path() {
	string main_name {"main.cpp"};
	string image_name {"tiles.png"};	
	string file_path = __FILE__;
	return file_path
	       .substr(0, file_path.size() - main_name.size()).append(image_name);
}

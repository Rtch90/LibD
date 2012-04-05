#pragma once

class Game {
public:
	Game(void);
	~Game(void);
	
	bool Init(void);
	void Prepare(float dt);
	void Render(void);
	void Shutdown(void);
	
	void OnResize(int width, int height);
	
private:
	float _rotationAngle;
};

#include <cheerp/clientlib.h>
#include <cheerp/client.h>

#include <math.h>

void mainLoop();
class [[cheerp::genericjs]] Graphics{
 private:
  static client::HTMLCanvasElement* canvas;
  static client::CanvasRenderingContext2D* canvasCtx;
  static int width;
  static int height;

  static void rafHandler()
	{
		mainLoop();
		client::requestAnimationFrame(cheerp::Callback(rafHandler));
	}

 public:
  static void keyDownHandler(client::KeyboardEvent* e);
  static void initializeCanvas(int w, int h){
    height = h;
    width = w;
    canvas = (client::HTMLCanvasElement*)client::document.getElementById("pongcanvas");
    canvas->set_width(w);
    canvas->set_height(h);
    client::document.get_body()->appendChild(canvas);
    canvasCtx = (client::CanvasRenderingContext2D*)canvas->getContext("2d");

    client::requestAnimationFrame(cheerp::Callback(rafHandler));
		client::document.addEventListener("keydown", cheerp::Callback(keyDownHandler));
  }

  static void debugOutput(const char* str)
  {
    canvasCtx->set_font("24px sans-serif");
    canvasCtx->fillText(str, 0, height-24);
  }

  static void drawRect(int x, int y, int width, int height, int rgb)
  {
    int r = rgb&0xff;
    int g = (rgb>>8)&0xff;
    int b = (rgb>>16)&0xff;
    canvasCtx->set_fillStyle(client::String("").concat("rgb(", r, ",", g, ",", b, ")"));
    canvasCtx->fillRect(x, y, width, height);
  }

  static void drawCircle(int x, int y, int radius, int rgb)
	{
		int r = rgb&0xff;
		int g = (rgb>>8)&0xff;
		int b = (rgb>>16)&0xff;
		canvasCtx->set_fillStyle(client::String("").concat("rgb(", r, ",", g, ",", b, ")"));
		canvasCtx->beginPath();
		canvasCtx->arc(x,y,radius,0,2*M_PI);
		canvasCtx->fill();
	}

};


class Platform {
 private:
  int x;
  int y;
  int width;
  int height;

 public:
  Platform(int x, int y, int width, int height):x(x),y(y),width(width),height(height)
  {
  }

  int getX() const
  {
    return x;
  }

  int getY() const
  {
    return y;
  }

  int getWidth() const
  {
    return width;
  }

  int getHeight() const
  {
    return width;
  }

  void moveLeft()
  {
    x -= 3;
  }

  void moveRight()
  {
    x += 3;
  }

  void render() const
  {
    Graphics::drawRect(x, y, width, height, 0xffffff);
  } 
};

Platform platform(185, 380, 30, 7);

void Graphics::keyDownHandler(client::KeyboardEvent* e)
{
  if(e->get_keyCode() == 37)
    platform.moveLeft();
  else
    platform.moveRight();
}

class Ball
{
private:
    	int x;
    	int y;
    	int vx;
    	int vy;
public:
    	Ball(int x, int y, int vx, int vy):x(x),y(y),vx(vx),vy(vy)
    	{
    	}
    	void update()
    	{
            	x += vx;
            	y += vy;
    	}
    	// Returns true if the ball gets out of the field
    	bool collide(const Platform& platform, int maxX, int maxY)
    	{
            	// If we collided with the bottom side, we lost
            	if(y >= maxY)
                    	return true;
            	// Check left and right side collisions
            	if(x <= 0 || x >= maxX)
                    	vx = -vx;
            	// Check top side collision
            	if(y <= 0)
                    	vy = -vy;
            	// Check collision with the top side of the plaform
            	if(platform.getX() < x && (platform.getX() + platform.getWidth()) > x &&
                    	platform.getY() < y && (platform.getY() + platform.getHeight()) > y)
            	{
                    	vy = -vy;
            	}
            	return false;
    	}
    	void render()
    	{
            	Graphics::drawCircle(x, y, 5, 0xffffff);
    	}
};
Ball ball(200, 200, 2, -2);

void mainLoop()
{
   // Reset the background to black
   Graphics::drawRect(0, 0, 400, 400, 0x000000);
   // Draw the platform
   platform.render();
   // Update the ball state
   ball.update();
   // Check for collisions
   bool hasLost = ball.collide(platform, 400, 400);
   if(hasLost)
     Graphics::debugOutput("You lost!");
   // Render the ball
   ball.render();
}

void webMain() {
  Graphics::initializeCanvas(400, 400);
}

#include <GLFW/glfw3.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#define PI 3.14159265
#define mapSize 32
#define tileSize 40
#define deg 0.0174533
#define fov 80
#define rays 500

typedef struct {
  float distance;
  bool shadow;
  int color;
} RayHit;

int screenWidth, screenHeight;

float px = 200, py = 200, pa = 0;
float speed = 1;
float rotationSpeed = 0.03;

int map[mapSize][mapSize] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 3, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 3, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 3, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 3, 0, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

int getMapTile(int x, int y) {

  if (x >= mapSize) {
    x = mapSize - 1;
  }
  if (x < 0) {
    x = 0;
  }
  if (y >= mapSize) {
    y = mapSize - 1;
  }
  if (y < 0) {
    y = 0;
  }

  return map[x][y];
}

RayHit drawRay(float angle) {
  float x1 = px, y1 = py;
  float x2 = px, y2 = py;
  int xTile = 0, yTile = 0;
  int color1 = 0, color2 = 0;

  if (angle < 0)
    angle += 2 * PI;
  if (angle > 2 * PI)
    angle -= +2 * PI;

  for (int i = 0; i < mapSize; i++) {
    if (angle > PI / 2 && angle < 1.5 * PI) {
      x1 = ((int)x1 / tileSize) * tileSize - 0.0003f;
    } else {
      x1 = ((int)x1 / tileSize) * tileSize + tileSize;
    }
    y1 = (tan(angle) * (x1 - px)) + py;
    if (getMapTile((int)x1 / tileSize, (int)y1 / tileSize)) {
      color1 = getMapTile((int)x1 / tileSize, (int)y1 / tileSize);
      break;
    }
  }

  float rayTan = tanf(angle);
  if (fabsf(rayTan) > 0.0001f) {
    for (int i = 0; i < mapSize; i++) {
      if (angle > PI && angle < 2 * PI) {
        y2 = ((int)y2 / tileSize) * tileSize - 0.0003f;
      } else {
        y2 = ((int)y2 / tileSize) * tileSize + tileSize;
      }
      x2 = (y2 - py) / rayTan + px;
      if (getMapTile((int)x2 / tileSize, (int)y2 / tileSize)) {
        color2 = getMapTile((int)x2 / tileSize, (int)y2 / tileSize);
        break;
      }
    }
  }

  float size1 = INFINITY;
  float size2 = INFINITY;
  if (color1 != 0)
    size1 = sqrt(pow(x1 - px, 2) + pow(y1 - py, 2)) * cos(angle - pa);
  if (color2 != 0)
    size2 = sqrt(pow(x2 - px, 2) + pow(y2 - py, 2)) * cos(angle - pa);

  RayHit hit;

  if (size1 <= size2) {
    hit.distance = size1;
    hit.shadow = true;
    hit.color = color1;
    return hit;
  } else {
    hit.distance = size2;
    hit.shadow = false;
    hit.color = color2;
    return hit;
  }
}

void setWallColor(int tileType, bool shadow) {
  float red, green, blue;
  if (tileType == 1) {
    red = 0.76f, green = 0.72f, blue = 0.64f;
  } else if (tileType == 2) {
    red = 0.58f, green = 0.32f, blue = 0.24f;
  } else if (tileType == 3) {
    red = 0.46f, green = 0.36f, blue = 0.24f;
  } else {
    red = 0.18f, green = 0.18f, blue = 0.17f;
  }

  if (shadow) {
    red *= 0.75f, green *= 0.75f, blue *= 0.75f;
  }

  glColor3f(red, green, blue);
}

void drawColumn(int i, float distance, bool shadow, int color) {
  glBegin(GL_QUADS);
  setWallColor(color, shadow);

  float height = screenHeight / (fmax(distance, 40) / 40);
  float width = (float)screenWidth / rays;
  float positionX = (i + (rays / 2)) * width;
  float positionY = (screenHeight - height) / 2;

  glVertex2i(positionX, positionY);
  glVertex2i(positionX + width + 0.1, positionY);
  glVertex2i(positionX + width + 0.1, positionY + height);
  glVertex2i(positionX, positionY + height);

  glEnd();
}

void drawPlayer() {
  for (int i = -(rays / 2); i < (rays / 2); i++) {
    float angle = pa + (i * (deg / 10));
    RayHit hit = drawRay(angle);
    drawColumn(i, hit.distance, hit.shadow, hit.color);
  }
}

void drawBackground() {
  glBegin(GL_QUADS);
  glColor3f(0.34f, 0.55f, 0.78f);
  glVertex2i(0, 0);
  glVertex2i(screenWidth, 0);
  glColor3f(0.72f, 0.82f, 0.90f);
  glVertex2i(screenWidth, screenHeight / 2);
  glVertex2i(0, screenHeight / 2);
  glEnd();

  glBegin(GL_QUADS);
  glColor3f(0.52f, 0.50f, 0.47f);
  glVertex2i(0, screenHeight / 2);
  glVertex2i(screenWidth, screenHeight / 2);
  glColor3f(0.30f, 0.29f, 0.27f);
  glVertex2i(screenWidth, screenHeight);
  glVertex2i(0, screenHeight);
  glEnd();
}

void drawMap() {
  int xo, yo;

  for (int y = 0; y < mapSize; y++) {
    for (int x = 0; x < mapSize; x++) {
      setWallColor(map[x][y], false);

      xo = x * tileSize;
      yo = y * tileSize;

      glBegin(GL_QUADS);
      glVertex2i(xo, yo);
      glVertex2i(xo, yo + tileSize);
      glVertex2i(xo + tileSize, yo + tileSize);
      glVertex2i(xo + tileSize, yo);
      glEnd();
    }
  }
}

void display() {
  // drawMap();
  drawBackground();
  drawPlayer();
}

void manageKeys(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    py += sin(pa) * speed;
    px += cos(pa) * speed;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    pa -= rotationSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    py += sin(pa) * -speed;
    px += cos(pa) * -speed;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    pa += rotationSpeed;
  }

  if (pa < 0)
    pa += 2 * PI;
  if (pa > 2 * PI)
    pa -= +2 * PI;

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void error_callback(int error, const char *description) {
  fprintf(stderr, "Error: %s\n", description);
}

int main() {
  setvbuf(stdout, NULL, _IONBF, 0);

  if (!glfwInit()) {
    return 0;
  }

  GLFWwindow *window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
  if (!window) {
    return 0;
  }

  glfwMakeContextCurrent(window);

  glfwSetErrorCallback(error_callback);
  glClearColor(0.f, 0.f, 0.f, 1.f); // once, after the context is current

  while (!glfwWindowShouldClose(window)) {
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, screenHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, screenWidth, screenHeight);
    double time = glfwGetTime();
    glfwSwapInterval(1);
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT);

    manageKeys(window);
    display();
    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
}

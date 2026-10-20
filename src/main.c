#include <GLFW/glfw3.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#define PI 3.1415926535
#define mapSize 32
#define tileSize 20
#define deg 0.0174533
#define startY 500
#define fov 80
#define rays 300

typedef struct {
  float distance;
  bool shadow;
} RayHit;

float px = 20, py = 20, pa = 0;
float speed = 3;
float rotationSpeed = 0.04;

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
  int x1 = px, y1 = py;
  float x2 = px, y2 = py;
  int xTile = 0, yTile = 0;

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
    if (getMapTile((int)x1 / tileSize, (int)y1 / tileSize))
      break;
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
      if (getMapTile((int)x2 / tileSize, (int)y2 / tileSize))
        break;
    }
  }

  int size1 = sqrt(pow(x1 - px, 2) + pow(y1 - py, 2));
  int size2 = sqrt(pow(x2 - px, 2) + pow(y2 - py, 2));
  RayHit hit;

  if (size1 <= size2) {
    hit.distance = size1;
    hit.shadow = true;
    return hit;
  } else {
    hit.distance = size2;
    hit.shadow = false;
    return hit;
  }
}

void drawColumn(int i, int distance, bool shadow) {
  glBegin(GL_QUADS);

  if (shadow) {
    glColor3f(0.8, 0.2, 0.2);
  } else {
    glColor3f(0.8, 0.1, 0.1);
  }

  int height = 400 - (distance);
  int width = 1600 / rays;
  int positionX = i * width;
  int positionY = startY + (400 - height) / 2;

  glVertex2i(positionX, positionY);
  glVertex2i(positionX + width, positionY);
  glVertex2i(positionX + width, positionY + height);
  glVertex2i(positionX, positionY + height);

  glEnd();
}

void drawPlayer() {
  glColor3f(1, 0, 0);
  glPointSize(10);
  glBegin(GL_POINTS);
  glVertex2f(px, py);
  glEnd();
  for (int i = -(rays / 2); i < (rays / 2); i++) {
    RayHit hit = drawRay(pa + (i * (fov * deg / rays)));
    drawColumn(i, hit.distance, hit.shadow);
  }
}

void drawBackground() {
  glBegin(GL_QUADS);
  glColor3f(0.1, 0.1, 0.1);
  glVertex2i(0, startY);
  glVertex2i(800, startY);
  glVertex2i(800, startY + 400);
  glVertex2i(0, startY + 400);
  glEnd();
}

void drawMap() {
  int xo, yo;

  for (int y = 0; y < mapSize; y++) {
    for (int x = 0; x < mapSize; x++) {
      int tileType = map[x][y];

      if (tileType == 3) {
        glColor3f(0, 1, 1);
      } else if (tileType == 2) {
        glColor3f(1, 0, 1);
      } else if (tileType == 1) {
        glColor3f(1, 1, 1);
      } else {
        glColor3f(0, 0, 0);
      }

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
  drawMap();
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
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, width, height);
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

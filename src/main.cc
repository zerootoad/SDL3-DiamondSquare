#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

using std::vector;

class DiamondSquare {
public:
  int dimension;
  int h;
  vector<vector<int>> map;

  std::mt19937 gen;
  std::uniform_int_distribution<> dis;

  DiamondSquare(int dimension, int h)
      : dimension(dimension), h(h), gen(std::random_device{}()), dis(-h, h) {
    if (dimension % 2 == 0) {
      std::cout << "Dimension must be an odd number" << std::endl;
      return;
    }

    for (int i = 0; i < dimension; i++) {
      vector<int> row;
      for (int j = 0; j < dimension; j++) {
        row.push_back(0);
      }
      map.push_back(row);
    }
  }

  void set_corners() {
    int h = random(-this->h, this->h);
    map[0][0] = h;
    map[0][dimension - 1] = h;
    map[dimension - 1][0] = h;
    map[dimension - 1][dimension - 1] = h;
  }

  /*
  The diamond step: Taking a square of four points, generate a random value at
  the square midpoint, where the two diagonals meet. The midpoint value is
  calculated by averaging the four corner values, plus a random amount. This
  gives you diamonds when you have multiple squares arranged in a grid.

  The square step: Taking each diamond of four points, generate a random value
  at the center of the diamond. Calculate the midpoint value by averaging the
  corner values, plus a random amount generated in the same range as used for
  the diamond step. This gives you squares again.
  */

  void diamond_step(int x, int y, int step) {
    int tl = map[x][y];
    int tr = map[x][x + step];
    int bl = map[x + step][y];
    int br = map[x + step][y + step];
    int avg = (tl + tr + bl + br) / 4;

    map[x + step / 2][y + step / 2] = avg + random(-h, h);
  }

  void square_step(int x, int y, int step) {
    int avg;

    avg = (map[x][y] + map[x][y + step] + map[x + step / 2][y + step / 2]) / 3;
    map[x][y + step / 2] = avg + random(-h, h);

    avg = (map[x][y] + map[x + step][y] + map[x + step / 2][y + step / 2]) / 3;
    map[x + step / 2][y] = avg + random(-h, h);

    avg = (map[x][y + step] + map[x + step][y + step] +
           map[x + step / 2][y + step / 2]) /
          3;
    map[x + step / 2][y + step] = avg + random(-h, h);

    avg = (map[x + step][y] + map[x + step][y + step] +
           map[x + step / 2][y + step / 2]) /
          3;
    map[x + step][y + step / 2] = avg + random(-h, h);
  }

  int random(int min, int max) { return dis(gen); };

  void generate() {
    set_corners();
    int step = dimension - 1;

    while (step > 1) {
      for (int x = 0; x < dimension - 1; x += step) {
        for (int y = 0; y < dimension - 1; y += step) {
          diamond_step(x, y, step);
        }
      }

      for (int x = 0; x < dimension - 1; x += step) {
        for (int y = 0; y < dimension - 1; y += step) {
          square_step(x, y, step);
        }
      }

      step /= 2;
    }
  }

  void view_height_map() {
    int cell_width = 3;

    std::cout << std::string((cell_width + 3) * dimension + 1, '-')
              << std::endl;

    for (int i = 0; i < dimension; i++) {
      std::cout << "|";
      for (int j = 0; j < dimension; j++) {
        std::cout << " " << std::setw(cell_width) << map[i][j] << " |";
      }
      std::cout << std::endl;

      std::cout << std::string((cell_width + 3) * dimension + 1, '-')
                << std::endl;
    }
  }
};

class DiamondSquareSDL {
public:
  SDL_Window *heightMapWindow = nullptr;
  SDL_Renderer *heightMapRenderer = nullptr;

  SDL_Window *terrainWindow = nullptr;
  SDL_Renderer *terrainRenderer = nullptr;

  float cameraZoom = 1.0f;
  float cameraAngleX = 0.0f;
  float cameraAngleY = 0.0f;

  bool init() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
      SDL_Log("SDL could not initialize! SDL_Error: %s", SDL_GetError());
      return false;
    }

    if (!SDL_CreateWindowAndRenderer("Height Map", 800, 600, 0,
                                     &heightMapWindow, &heightMapRenderer)) {
      SDL_Log("SDL_CreateWindowAndRenderer Error: %s", SDL_GetError());
      return false;
    }

    if (!SDL_CreateWindowAndRenderer("Terrain", 800, 600, 0, &terrainWindow,
                                     &terrainRenderer)) {
      SDL_Log("Terrain Window Error: %s", SDL_GetError());
      return false;
    }

    return true;
  }

  /* Documentation comments were generated using the copilot plugin. */

  void render_height_map(DiamondSquare &ds) {
    // Set the drawing color to black and clear the renderer
    SDL_SetRenderDrawColor(heightMapRenderer, 0, 0, 0, 255);
    SDL_RenderClear(heightMapRenderer);

    // Calculate the width and height of each cell in the grid
    int cell_width = 800 / ds.dimension;
    int cell_height = 600 / ds.dimension;

    // Loop through each cell in the height map
    for (int i = 0; i < ds.dimension; i++) {
      for (int j = 0; j < ds.dimension; j++) {
        // Get the height value of the current cell
        int height = ds.map[i][j];
        // Calculate the grayscale color based on the height value
        int color = 255 * (height + ds.h) / (2 * ds.h);
        SDL_SetRenderDrawColor(heightMapRenderer, color, color, color, 255);

        // Define the rectangle for the current cell
        SDL_FRect rect = {static_cast<float>(j * cell_width),
                          static_cast<float>(i * cell_height),
                          static_cast<float>(cell_width),
                          static_cast<float>(cell_height)};
        // Fill the rectangle with the calculated color
        SDL_RenderFillRect(heightMapRenderer, &rect);
      }
    }

    // Present the rendered height map to the screen
    SDL_RenderPresent(heightMapRenderer);
  }

  void render_terrain(DiamondSquare &ds) {
    // Set the drawing color to black and clear the renderer
    SDL_SetRenderDrawColor(terrainRenderer, 0, 0, 0, 255);
    SDL_RenderClear(terrainRenderer);

    // Set the drawing color to white
    SDL_SetRenderDrawColor(terrainRenderer, 255, 255, 255, 255);

    // Loop through the terrain map and draw the terrain
    for (int i = 0; i < ds.dimension - 1; i++) {
      for (int j = 0; j < ds.dimension - 1; j++) {
        // Get the coordinates and height values for the four corners of the
        // current grid cell
        //
        // Since our heightmap is a 2D array that holds a height value at each
        // point (x; y), we can convert the 2D array to a 3D array by adding a z
        // axis which will represent the height of the terrain at that point.
        //
        // X => length map
        // Y => width map
        // Z => height map

        float x1 = static_cast<float>(j);
        float y1 = static_cast<float>(i);
        float z1 = static_cast<float>(ds.map[i][j]);

        float x2 = static_cast<float>(j + 1);
        float y2 = static_cast<float>(i);
        float z2 = static_cast<float>(ds.map[i][j + 1]);

        float x3 = static_cast<float>(j + 1);
        float y3 = static_cast<float>(i + 1);
        float z3 = static_cast<float>(ds.map[i + 1][j + 1]);

        float x4 = static_cast<float>(j);
        float y4 = static_cast<float>(i + 1);
        float z4 = static_cast<float>(ds.map[i + 1][j]);

        // Calculate the cosine and sine of the camera angles for rotation
        float cosY = cos(cameraAngleY);
        float sinY = sin(cameraAngleY);

        float cosX = cos(cameraAngleX);
        float sinX = sin(cameraAngleX);

        // Rotate points around the Y-axis
        float x1_rotY =
            x1 * cosY - z1 * sinY; // New x-coordinate after rotation
        float z1_rotY =
            x1 * sinY + z1 * cosY; // New z-coordinate after rotation

        float x2_rotY = x2 * cosY - z2 * sinY;
        float z2_rotY = x2 * sinY + z2 * cosY;

        float x3_rotY = x3 * cosY - z3 * sinY;
        float z3_rotY = x3 * sinY + z3 * cosY;

        float x4_rotY = x4 * cosY - z4 * sinY;
        float z4_rotY = x4 * sinY + z4 * cosY;

        // Rotate points around the X-axis
        float y1_rotX = y1 * cosX - z1_rotY * sinX;
        float z1_rotX = y1 * sinX + z1_rotY * cosX;

        float y2_rotX = y2 * cosX - z2_rotY * sinX;
        float z2_rotX = y2 * sinX + z2_rotY * cosX;

        float y3_rotX = y3 * cosX - z3_rotY * sinX;
        float z3_rotX = y3 * sinX + z3_rotY * cosX;

        float y4_rotX = y4 * cosX - z4_rotY * sinX;
        float z4_rotX = y4 * sinX + z4_rotY * cosX;

        // Projection parameters
        float scale = 200.0f;
        float zoomFactor = 1.0f / cameraZoom;

        // Project the rotated points onto a 2D plane
        // The projection formula used here is a simple perspective projection.
        // The points are scaled and translated to fit within a 2D viewport.

        // Project the first point
        float x1_proj =
            (x1_rotY / (z1_rotX + scale)) * scale * zoomFactor +
            320; // Calculate the x-coordinate of the projected point
        float y1_proj =
            (y1_rotX / (z1_rotX + scale)) * scale * zoomFactor +
            240; // Calculate the y-coordinate of the projected point

        float x2_proj =
            (x2_rotY / (z2_rotX + scale)) * scale * zoomFactor + 320;
        float y2_proj =
            (y2_rotX / (z2_rotX + scale)) * scale * zoomFactor + 240;

        float x3_proj =
            (x3_rotY / (z3_rotX + scale)) * scale * zoomFactor + 320;
        float y3_proj =
            (y3_rotX / (z3_rotX + scale)) * scale * zoomFactor + 240;

        float x4_proj =
            (x4_rotY / (z4_rotX + scale)) * scale * zoomFactor + 320;
        float y4_proj =
            (y4_rotX / (z4_rotX + scale)) * scale * zoomFactor + 240;

        SDL_RenderLine(terrainRenderer, x1_proj, y1_proj, x2_proj, y2_proj);
        SDL_RenderLine(terrainRenderer, x2_proj, y2_proj, x3_proj, y3_proj);
        SDL_RenderLine(terrainRenderer, x3_proj, y3_proj, x4_proj, y4_proj);
        SDL_RenderLine(terrainRenderer, x4_proj, y4_proj, x1_proj, y1_proj);
      }
    }

    SDL_RenderPresent(terrainRenderer);
  }

  void handle_input() {
    const Uint8 *keyState =
        reinterpret_cast<const Uint8 *>(SDL_GetKeyboardState(nullptr));

    // Zoom with I and O keys
    if (keyState[SDL_SCANCODE_I]) {
      cameraZoom *= 1.1f;
    }
    if (keyState[SDL_SCANCODE_O]) {
      cameraZoom /= 1.1f;
    }

    /* Camera Rotation

       W
       ^
       |
   A<--|-->D
       |
       v
       S

    */
    if (keyState[SDL_SCANCODE_A]) {
      cameraAngleY -= 0.1f;
    }
    if (keyState[SDL_SCANCODE_D]) {
      cameraAngleY += 0.1f;
    }
    if (keyState[SDL_SCANCODE_W]) {
      cameraAngleX -= 0.1f;
    }
    if (keyState[SDL_SCANCODE_S]) {
      cameraAngleX += 0.1f;
    }
  }

  void cleanup() {
    if (heightMapRenderer) {
      SDL_DestroyRenderer(heightMapRenderer);
    }
    if (heightMapWindow) {
      SDL_DestroyWindow(heightMapWindow);
    }
    if (terrainRenderer) {
      SDL_DestroyRenderer(terrainRenderer);
    }
    if (terrainWindow) {
      SDL_DestroyWindow(terrainWindow);
    }
    SDL_Quit();
  }
};

int main() {
  DiamondSquare ds(65, 1);
  ds.generate();
  // ds.view_height_map();

  DiamondSquareSDL sdl;
  if (!sdl.init()) {
    return 1;
  }

  bool running = true;
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
      }
    }

    sdl.handle_input();
    sdl.render_height_map(ds);
    sdl.render_terrain(ds);

    SDL_Delay(16);
  }

  sdl.cleanup();
  return 0;
}

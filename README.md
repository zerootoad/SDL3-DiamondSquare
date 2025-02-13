# SDL3-DiamondSquare
## What is this?

This is a C++ project that uses the **Diamond-Square Algorithm** to create cool-looking terrain. It then shows the terrain in 2D (like a grayscale map) and in 3D (like a mini mountain range). It uses **SDL3** to draw everything on the screen. You can even move the camera around to see the terrain from different angles.

![image](https://github.com/user-attachments/assets/d07988fa-cd3b-4e74-ac8e-d39421411cd2)


---

## Features

1. **Diamond-Square Algorithm**:
   - Makes a heightmap (basically a grid of numbers that represent how high or low the terrain is).
   - You can change how big the map is and how bumpy the terrain looks.

2. **2D Heightmap**:
   - Shows the heightmap as a grayscale grid. Darker = lower, lighter = higher.

3. **3D Terrain**:
   - Turns the heightmap into a 3D view so it looks like mountains and valleys.
   - You can zoom in/out and rotate the camera to explore.

4. **Controls**:
   - **W, A, S, D**: Rotate the camera.
   - **I, O**: Zoom in and out.

---

## What You Need

- **C++ Compiler**: Like GCC or Clang. Make sure it supports C++17.
- **SDL3**: This is the library used to draw stuff on the screen. You can download it from [here](https://www.libsdl.org/).
- A terminal or command prompt to run the program.

---

## How to Run

1. **Install SDL3**:
   - Download SDL3 from the official website and install it. Make sure your compiler can find the SDL3 files.

2. **Clone the Repository**:
   - Copy the code from the `src/main.cc` file or paste into a terminal:
     ```bash
     git clone https://github.com/zerootoad/SDL3-DiamondSquare.git
     cd SDL3-DiamondSquare
     ```

3. **Compile the Code**:
   - Navigate to the folder where the code is.
   - Run this command to compile:
     ```bash
     g++ -std=c++17 -o terrain src/main.cc -lSDL3
     ```
   - If it doesn’t work, make sure SDL3 is installed correctly.

4. **Run the Program**:
   - After compiling, run the program:
     ```bash
     ./terrain
     ```
   - Two windows will pop up (overlapped): one for the 2D heightmap and one for the 3D terrain.

---

## How to Use

- When the program runs, you’ll see:
  - A **2D heightmap** (looks like a grid of gray squares).
  - A **3D terrain** (looks like mountains or valleys).

- Use these keys to control the 3D view:
  - **W, A, S, D**: Rotate the camera.
  - **I, O**: Zoom in and out.

- Close either window to quit the program.

---

## How It Works

- **DiamondSquare Class**:
  - This is where the terrain is generated. It uses the Diamond-Square Algorithm to make the heightmap.
  - Check out: **https://web.archive.org/web/20170812230846/http://www.gameprogrammer.com/fractal.html**

- **DiamondSquareSDL Class**:
  - This handles all the drawing and camera controls using SDL.

- **Main Function**:
  - This is where everything starts. It creates the terrain, sets up the windows, and runs the program loop.

---

## Customization

- **Change Terrain Size**:
  - In the `main()` function, change the `65` in `DiamondSquare ds(65, 1);` to a different number. It has to be an odd number (like 33, 65, 129, etc.).

- **Change Terrain Roughness**:
  - Change the `1` in `DiamondSquare ds(65, 1);` to a bigger number for more bumpy terrain or a smaller number for smoother terrain.

---

## Example Code

Here’s the main part of the code:

```cpp
int main() {
  DiamondSquare ds(65, 1); // 65x65 grid, roughness = 1
  ds.generate(); // Make the terrain

  DiamondSquareSDL sdl;
  if (!sdl.init()) { // Set up SDL
    return 1;
  }

  bool running = true;
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false; // Quit if the window is closed
      }
    }

    sdl.handle_input(); // Check for key presses
    sdl.render_height_map(ds); // Draw the 2D heightmap
    sdl.render_terrain(ds); // Draw the 3D terrain

    SDL_Delay(16); // Wait a bit to make it run
  }

  sdl.cleanup(); // Clean up the windows
  return 0;
}
```

---

## Why I Made This

I made this project because I thought procedural terrain generation was pretty dope and also to learn how graphics and rendering work.

---

## Feedback

If you have any suggestions or find any bugs, let me know.

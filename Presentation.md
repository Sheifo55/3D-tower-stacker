# 3D Tower Stacker - Presentation Script

## Slide 1: Title Screen
**Visual**: Game start screen with the "3D TOWER STACKER" title.
**Speaker Notes**: "Hello everyone! Welcome to my presentation for the 3D Tower Stacker game. This project is a fully 3D interactive block stacking game built from scratch using C++, OpenGL, and GLFW."

## Slide 2: Core Gameplay
**Visual**: Short gameplay clip showing blocks sliding and being dropped.
**Speaker Notes**: "The goal is simple: stack blocks as high as possible. When you drop a block, any overhanging portion is sliced off and falls away as scrap, making the next block smaller. A perfect drop flashes white and gives you a bonus. This demonstrates robust collision detection and 3D scaling transformations in real-time."

## Slide 3: Graphics & Lighting
**Visual**: Close-up of the blocks showing specular highlights and the stone base.
**Speaker Notes**: "To make the game visually appealing, I implemented a custom shader with the Blinn-Phong lighting model. I animated the light source to circle the scene, which clearly shows off the diffuse shadows and specular highlights. I also integrated `stb_image` to map high-resolution wood and stone textures onto the blocks, and procedurally generated a smooth 3D cylinder for the base foundation."

## Slide 4: The UI Text Engine
**Visual**: Screenshot showing the score in the bottom corner.
**Speaker Notes**: "One of the most interesting challenges was building a custom 2D orthographic text renderer. I used a custom 5x7 bitmap font mapped to a dynamically generated OpenGL texture atlas. I even had to debug a tricky C++ line-continuation bug that was shifting the ASCII values, which is why my text is now rendering perfectly."

## Slide 5: Conclusion
**Visual**: Game Over screen showing the final score.
**Speaker Notes**: "Through this project, I learned how to manage vertex buffers, calculate view matrices with an orbiting camera, write custom fragment shaders, and manage game state. The result is a highly polished, fully functional graphics application. Thank you for watching!"

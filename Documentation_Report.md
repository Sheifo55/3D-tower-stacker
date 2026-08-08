# 3D Tower Stacker Documentation Report

## 1. Cover Page / Title
**Project Title**: 3D Tower Stacker
**Course**: Computer Graphics
**Student**: [Student Name]
**Description**: A 3D block stacking game built with OpenGL, GLFW, and GLM, demonstrating core concepts like 3D transformations, shading, texturing, and user interaction.

## 2. Objectives
The objective of this project is to create an interactive 3D tower stacking game using raw OpenGL. The game demonstrates the following computer graphics concepts:
- **3D Transformations**: Modeling matrices are used to translate blocks back and forth across the X and Z axes. Scaling matrices resize the blocks when placed imperfectly. Rotation transformations are implemented via an orbiting camera and falling scrap pieces.
- **Lighting and Materials**: Blinn-Phong shading is implemented in a custom shader, complete with ambient, diffuse, and specular components. An animated directional light creates moving highlights and shadows to enhance depth perception.
- **Texture Mapping**: External images (`stb_image.h`) are mapped to the 3D geometry with UV coordinates to provide realistic wood and stone textures.
- **User Interface**: A custom 2D orthographic text renderer displays the game state, score, and instructions by mapping ASCII characters from a bitmap sprite sheet.
- **Object Modeling**: Complex non-cube models, such as the procedural 32-segment cylinder for the base foundation, were built mathematically to demonstrate advanced vertex array generation.

## 3. System Design
The system uses an Object-Oriented design to separate rendering logic from gameplay logic:
- **Game Engine**: 
  - `Game`: Manages the state machine, score, tower stack vector, and block collision logic.
  - `Block`: Represents individual game pieces, storing their position, size, and material color.
  - `Camera`: Handles view matrix generation and perspective projection, animating in a smooth orbit around the tower.
  - `InputHandler`: Wraps GLFW keyboard callbacks to provide a clean polling interface for game events.
- **Rendering Pipeline**:
  - `Renderer`: Manages VAOs, VBOs, and EBOs for cubes and cylinders, loads textures with `stb_image`, and issues OpenGL draw calls.
  - `Shader`: Loads, compiles, and links GLSL vertex and fragment shaders.
  - `TextRenderer`: Uses an orthographic projection and a custom font texture to render UI text elements over the 3D scene.

## 4. Code Explanation
### Core Gameplay Loop
The `Game::dropBlock()` method handles the core logic. When the user presses SPACE, it calculates the overlap between the moving block and the top of the tower. If the overlap is greater than zero, the block is sliced: the overlapping portion stays on the tower, while the overhanging portion becomes a `ScrapPiece` that rotates and falls down. A perfect placement triggers a color flash effect.

### Shading and Textures
The `block.frag` shader implements the Blinn-Phong lighting model. 
```glsl
vec3 viewDir = normalize(viewPos - FragPos);
vec3 halfwayDir = normalize(dir + viewDir);
float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
vec3 specular = specularStrength * spec * lightColor;
```
It also samples UV coordinates passed from `block.vert` to sample `texture1`. The `useTexture` uniform determines whether the stone/wood texture is applied, and the texture color is multiplied by the base object color and lighting result.

### Text Rendering Bug Fix
The original font array suffered from a bug where the `\` character (`// 92 \`) acted as a C++ line continuation, commenting out the next character (`]`) and causing an off-by-one error for all subsequent ASCII codes. This was resolved by renaming the comment to `// 92 backslash`.

## 5. Screenshots
*(Placeholder for gameplay screenshots showcasing lighting, textures, and the UI)*
- **Screenshot 1**: Start screen with the base cylinder and orbiting camera.
- **Screenshot 2**: Gameplay mid-stack showing perfect placement flashes and specular highlights.
- **Screenshot 3**: Game Over screen with text correctly rendering the final score.

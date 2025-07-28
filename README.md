# LightningEngine

## About this project

Lightning Engine is a game engine written in C++ that is currently powered by Metal, Apple's Graphics API. The goal of this project is to explore and learn the feature sets Metal provides, while exploring gaming
potential of Apple Silicon. As of right now, this project is being rebuilt to use Premake, a cross platform built system with the long term goal of making this engine multi-platform. For now,
Metal is the main focus since my main development machine is a Macbook. 

> [!NOTE]
> Submit an issue if you find any problems I should look into.
>
> Submit a PR for any code suggestions to make this engine better/more robust 

> [!TIP]
> Here is how to setup Lightning Engine on your local machine
>
> Step 1: Clone the repository in your terminal window using git clone --recurse-submodules <Github clone link>
>
> Step 2: Once the project is sucessfully cloned, in the project root directory, enter './premake5 xcode4' to generate XCode project files using Premake

> [!WARNING]
>  This game engine is currently very primitive and is not intended for commercial use.

## Current Progress

<img width="2025" height="1259" alt="Screenshot 2025-07-27 at 8 08 48 PM (2)" src="https://github.com/user-attachments/assets/4f69872a-0f09-481b-b851-436ba27bbb43" />

## Checklist
- [x] Render a window :tada:
- [x] Render Basic Shapes (Triangle, Quad) :tada:
- [x] Textures :tada:
- [x] DearImGui support :tada:
- [ ] Sprite Animation (WIP)
- [ ] Camera
- [ ] Input System
- [ ] Physics
- [ ] Argument Buffers
- [ ] Native Scripting System
- [ ] ECS

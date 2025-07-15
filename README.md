# ChessBoard

An interactive chess game demo built with **C++** and **ImGui**.  
This project showcases a self-developed GUI chessboard with music playback and real-time FPS display.

🎬 **Demo Video**  
▶️ [Watch on YouTube](https://www.youtube.com/watch?v=EJJQqNtspAE)

> This project was collaboratively developed as part of the **Object-Oriented Programming** course at  
> [**National Taiwan University of Science and Technology (NTUST)** – CSIE Department].
<!--
## My Contribution

This project was developed collaboratively with other team members.
I was mainly responsible for the **backend logic** of the chess system, including:

- Implementing **piece movement rules** in C++ (e.g., valid knight/bishop/queen moves)
- Handling **capture mechanics** (which pieces can capture which)
- Designing **win/loss conditions** (e.g., when checkmate or stalemate occurs)
- Managing **turn control** (e.g., black first vs white first toggle)
- Defining the overall **game state architecture** and rule flow

The logic is designed to be modular and clear, ensuring a smooth gameplay experience with rule consistency.
-->
---

## Screenshot

<p align="center">
  <img src="https://github.com/tongyu0924/ChessBoard/blob/main/demo_image.png?raw=true" width="480" />
</p>

---

## Features

- Chess game logic implemented in C++, following standard chess rules
- Modular OOP design with separate classes for each piece (King.h, Queen.h, etc.)
- Custom GUI interface built with ImGui
- Click-based piece movement with mouse event handling
- Built-in background music playback (multiple tracks supported)

---
## Project Structure
```
chessboard/
├── GUITest/
│   ├── imgui/           # ImGui library for GUI rendering
│   ├── include/         # Header files for core components
│   ├── music/           # Background music files
│   ├── x64/Release/     # Compiled binary output
│   ├── Bishop.h, King.h ... # OOP classes for each chess piece
│   ├── GameManager.h    # Chess game state logic
│   ├── ViewManager.h    # GUI controller logic
│   ├── main.cpp         # Main application entry
├── .vs/                 # Visual Studio metadata
├── x64/                 # Compiled artifacts
├── LICENSE
├── README.md
├── chessGUI.sln         # Visual Studio solution
├── demo.mp4             # Gameplay demo video
├── demo_image.png       # Screenshot for preview
```

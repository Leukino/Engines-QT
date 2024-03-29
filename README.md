# Engines-QT
Engines QT is a simple and small [Game Engine](https://github.com/Leukino/Engines-QT/releases) developed in C++ by [Miquel Quintana](https://github.com/Leukino) and [Òscar Tarrés](https://github.com/oscarta3).

It's a game engine, you can move the camera, add objects in scene, modify their components.

Controls:

- Right-click + wasd to move camera

- Right-click + e and q move camera up and down

- Right-click to rotate camera

- Alt+right click to rotate camera around selected gameobject

Some more features:

- You can select objects in the Hierarchy tab (default Root has child gameobjects), delete and reparent.

- You can see the console messages in the Console tab.

- You can create new objects in the GameObject -- Create empty gameobject tab.

- You can transform (translate, rotate and scale) any GameObject.

- You can add components to any GameObject.

- You can change the texture of the selected object with the Textures tab.

- You can change the frustum camera in the Configuration -- Editor Camera tab.

- You can change render options in the Configuration -- Render tab (specially RenderAABB, easier to see selected object).

- You can see the previous stored actions in the History tab. 

- Undo action Ctrl Z (works only in Transform component for the moment).

- You can pick a GameObject by clicking them on the world.

- You can Pause (which doesn't allow the user to move, rotate and scale GameObjects) and Play.

- You can save and load scenes.

- Loaded scenes will keep components and hierarchy.

- Meshes will be stored in the scene file.

- Serialization of all GameObjects.

- You can store the desired component information, by clicking Store Position. Then, whenever you want, you can Undo the action and revert to the previous stored position. It won't matter whether it's the selected gameobject or not. 

- You can drag and drop models and textures to import them to the engine.

- You can move, scale and rotate GameObjects thanks to the implemented Gizmo (you can change between translate, scale and rotation by pressing "w" "e" "r"). 

The engine executable is located in Engine3D/Game/Engine3D.exe

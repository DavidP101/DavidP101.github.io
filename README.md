# Game Dev Portfolio
--------------------
![Night time scene](/images/nighttower.png)

## About Me
-----------
Hello there. My name's David, I'm 23 years old, and I hail from Ireland.

## Overview
-----------
From a young age I've loved to play RPGs and it's remained my favourite genre of game throughout my life. Being an aspiring Game Dev I naturally gravitated towards this genre and wanted to try my hand at creating some RPG mechanics (listed below). The premise for this project is that the player is situated on an island and is helping the local islanders in preparation for a small fireworks display. The player helps the islanders by completing quests; these quests are then fulfilled by interacting with various actors throughout the world (through dialogue or by standard methods).
I encountered a good few challenges along the way and will these discuss these in my Lessons Learned section. What follows here is a list of the main features in the project and a basic description of each.

## Gameplay Features
--------------------

### Interaction System
----------------------
![Interaction](/images/interaction.png)

#### Setup
* Created custom collision channel in project settings called 'Interactable'.
* Made sphere collision volume on player character that only does checks for the above channel.
* An interface also called 'Interactable' was built with two methods: 'Interact' and 'Exit Interact'.

#### Method
To create an actor that's capable of interaction: I first changed the collision type on one of the actor's static mesh components (if the actor did not have a static mesh component I created a volume instead and applied it to that). Following this, I then implemented the Interactable interface and provided my definitions for Interact and Exit Interact.

### Day-Night Cycle
-------------------
![Day](/images/daytime.png)
### Dialogue System
-------------------

### Quest System
----------------

### Inventory System

## Environment
--------------

### Trees
---------
I created my trees using the methods outlined in the tutorials below. However, to provide a quick summary: The bark/branch shape is created by joining vertices. Skin, subdivide and decimate modifiers are then applied. The leaves of the tree are made by intersecting planes. Each plane has a texture mapped to it. I used a mask I found off a UDK documentation page, which is also linked below.

#### References:
----------------
1. [Bark creation](https://www.youtube.com/watch?v=wB0VgyN8tOw)
1. [Tree creation](https://www.youtube.com/watch?v=_Br6Xq4LZcI)
1. [Leaf mask](https://api.unrealengine.com/udk/Three/rsrc/Three/SoftMasked/GoodLeafMask.jpg)

### Grass
--------------
A mask was created inside MS Paint and then applied to 5-6 planes in blender. These planes were then intersected with one another and imported into UE4.

### Flowers
--------------
The flowers were created much the same way as the grass, except I used textures sourced from a [Udemy Blender Course](https://www.udemy.com/blendertutorial/).

### Landscape
--------------
The landscape was designed using the Landscape Tool. A number of textures were sourced from the [UE4 Starter Content](https://docs.unrealengine.com/en-us/Engine/Content/Packs) including grass, dirt, stone, water, and among others to provide some noise and variation.

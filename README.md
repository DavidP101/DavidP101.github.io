# Game Dev Portfolio
--------------------
![Night time scene](/images/nighttower.png)

## About Me
-----------
Hello there. My name's David, I'm 23 years old, and I hail from Ireland. 

## Gameplay Features
--------------------

### Interaction System
--------------------
#### Setup

* Created custom collision channel in project settings called 'Interactable'.
* Made sphere collision volume on player character that only does checks for the above channel.
* An interface also called 'Interactable' was built with two methods: 'Interact' and 'Exit Interact'.

#### Method

To create an actor that's capable of interaction: I first changed the collision type on one of the actor's static mesh components (if the actor did not have a static mesh component I created a volume instead and applied it to that). Following this, I then implemented the Interactable interface and provided my definitions for Interact and Exit Interact.

### Day-Night Cycle
--------------------

### Dialogue System
--------------------

### Quest System
--------------------

## Environment
--------------

### Trees
--------------
I created my trees using the methods outlined in the tutorials below. However, to provide a quick summary: The bark/branch shape is created by joining vertices. Skin, subdivide and decimate modifiers are then applied. The leaves of the tree are made by intersecting planes. Each plane has a texture mapped to it. I used a mask I found off a UDK documentation page, which is also linked below.

#### References:

1. [Bark creation](https://www.youtube.com/watch?v=wB0VgyN8tOw)
1. [Tree creation](https://www.youtube.com/watch?v=_Br6Xq4LZcI)
1. [Leaf mask](https://api.unrealengine.com/udk/Three/rsrc/Three/SoftMasked/GoodLeafMask.jpg)

### Grass
--------------
A mask was created inside MS Paint and then applied to 5-6 planes in blender. These planes were then intersected with one another and imported into UE4.

### Flowers
--------------

### Landscape
--------------

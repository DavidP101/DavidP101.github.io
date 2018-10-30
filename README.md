# Game Dev Portfolio

![Night time scene](/images/nighttower.png)

## Executable Link

[Here](https://drive.google.com/open?id=1DN9UzSKwuknAhUOBqpwB8aVnljYtBMmg) is a link to a .zip file containing the executable. The code base is available on github to browse through.

## About Me

Hello there. My name's David, I'm 23 years old, and I hail from Ireland. I graduated from a Computer Science degree in 2017, and ever since then I've been learning UE4, dipping my foot into Blender, and working on a portfolio.

## Overview

From a young age I've loved to play RPGs and it's remained my favourite genre of game throughout my life. Being an aspiring game dev I naturally gravitated towards this genre and wanted to try my hand at creating some RPG mechanics. The premise for this project is that the player is situated on an island and is helping the local islanders in preparation for a small fireworks display. The player helps the islanders by completing quests; these quests are fulfilled by interacting with various actors throughout the world.
I encountered challenges along the way and will discuss these in my lessons learned section. What follows here is a list of the main features in the project and a very basic description of each.

## Gameplay Features


### Interaction System

![Interaction](/images/interaction.png)

* Created custom collision channel in project settings called 'Interactable'.
* Made sphere collision volume on player character that only does checks for the above channel.
* An interface also called 'Interactable' was built with two methods: 'Interact' and 'Exit Interact'.

To create an actor that's capable of interaction I first changed the collision type on one of the actor's static mesh components (if the actor did not have a static mesh component I created a volume instead and applied it to that). Following this, I then implemented the Interactable interface and provided my definitions for Interact and Exit Interact.

Whenever the volume around the player comes into contact with an interactable it then adds it to a potential interactables array and the best one is chosen by means of a dot product with the player's camera. I used this tutorial for reference while making this sytem.

### Day-Night Cycle

![Dawn](/images/dawntime.png)
![Day](/images/daytime.png)
![Night](/images/nighttime.png)

An actor was created to house the logic called 'DayNightCycle'. A number of actors are involved in the cycle including the sky sphere, directional light, and sky light. To drive various values associated with each (in particular the directional light) 0-1 curves were created e.g a curve to update the directional light's colour, another curve to update intensity and so on. I then made a timeline to drive an alpha curve where the resulting value would be fed into each of the curves above. Hourly/minute updates are broadcasted by means of a delegate to any actors across the project that are interested (e.g. street lights).

### Dialogue System

![Dialogue01](/images/dialogue01.png)

Any actor that is capable of dialogue has a dialogue component. In here I have variables that store data tables for quest dialogue and non-quest dialogue.
The dialogue system is not true branching dialogue (as it's not implemented with a tree structure) but a more basic data table approach whereby the player and whoever the player is talking to have string arrays of responses. If the player selects an option that appears in let's say, row 0 column 0 of the arra, then on the next row of the data table it will fetch the AI response at that same location. Once the player clicks on the option, I then check a seperate outcomes data table for any outcome that might result. Outcomes are represented by an enum, I have two outcomes in my project 'UpdateObjective' and 'Exit'.

![Dialogue02](/images/casualdialogue.png)

The above picture shows the less advanced dialogue, a string array of responses is housed in the dialogue component, a random string is chosen, sent to the UI, and exposed to the user for a set number of seconds.

### Quest System

![quest02](/images/questui.png)
![quest01](/images/quests.png)

A questline actor exists in the world which stores two arrays: 'ActiveQuests' and 'CompletedQuests'. On my player controller I keep an active quest index variable which remembers which quest I'm following. When a quest is chosen, the objective marker (if there is any) is updated. When we get to the last objective in a quest, it's taken out of the ActiveQuests array and moved to the CompletedQuests array.

The quests of the game are stored inside a data table, a quest structure was made to represent each quest. Every quest has an array of objectives, a stage to represent where we are in the quest, and a name among other UI related variables.
Each objective in the objectives array is also represented by a structure. This structure among the UI related variables also has a stage, and a string for an event name.

After each objective is complete we see if there's any event by this name in the questline actor and execute it if it exists. This way allows us to add items to the player after a quest has finished or provide other custom behaviour idiosyncratic to that quest.


### Inventory System

![inventory](/images/inventory.png)

The inventory follows a slot based approach. A structure was made to represent an inventory item. Any actors that wish to become an inventory item can add an inventory component to their hierarchy where a structure variable is exposed to editing. In here we can add a custom icon, the class, if the actor is equippable, or if the actor is a quest item. A blueprint interface called 'Useable' was made so that actors can provide their own definitions for how they're interacted with inside the inventory. For example, the helmet in the picture will be equipped when clicked on, while the dye (represent by the pink cylinder) will turn the player's shirt to a random colour.

## Environment


### Trees

![daytimetrees](/images/daytimetrees.png)

I created my trees using the methods outlined in the tutorials below. However, to provide a quick summary: The bark/branch shape is created by joining vertices. Skin, subdivide and decimate modifiers are then applied. The leaves of the tree are made by intersecting planes. Each plane has a texture mapped to it. I used a mask I found off a UDK documentation page, which is also linked below.

#### References:

1. [Bark creation](https://www.youtube.com/watch?v=wB0VgyN8tOw)
1. [Tree creation](https://www.youtube.com/watch?v=_Br6Xq4LZcI)
1. [Leaf mask](https://api.unrealengine.com/udk/Three/rsrc/Three/SoftMasked/GoodLeafMask.jpg)

### Grass

A mask was created inside MS Paint and then applied to 5-6 planes in blender. These planes were then intersected with one another and imported into UE4.

### Flowers

The flowers were created much the same way as the grass, except I used textures sourced from a [Udemy Blender Course](https://www.udemy.com/blendertutorial/).

### Landscape

The landscape was designed using the Landscape Tool. A number of textures were sourced from the [UE4 Starter Content](https://docs.unrealengine.com/en-us/Engine/Content/Packs) including grass, dirt, stone, water, and among others to provide some noise and variation.

## Characters

The character meshes and the majority of animations were sourced from [Mixamo](https://www.mixamo.com/#/). All remaining animations were sourced from [UE4 Starter Content](https://docs.unrealengine.com/en-us/Engine/Content/Packs).

## Audio

The majority of the audio was sourced from [BBC Sound Effects](http://bbcsfx.acropolis.org.uk/). Audio for the UI was sourced from the [Content Examples](https://docs.unrealengine.com/en-us/Resources/ContentExamples). The audio for the footsteps was sourced from the [Couch Knights Demo](https://www.unrealengine.com/marketplace/couch-knights).

## Lessons Learned

Many lessons were learned throughout the development of this project. I estimate it took me somewhere in the range of 4-5 months to complete and was a very steep learning curve. All in all, scope was the major challenge. At the beginning I didn't know how big I wanted the project to be or in fact where exactly I wanted to go with it. As I continued development I changed ideas many times over, some by choice, some due to lack of expertise on my part. If I had my time over to do it again, I would have done something much smaller and focused, something in the nature of a few weeks instead of a few months. Another major problem I encountered was best practises, being a novice game developer it was very difficult to know if the solutions I completed were actually done in a logical and somewhat efficient way. In fact, in the earlier stages of the project I would estimate 2-3 weeks was spent on the whiteboard trying to figure out the best solutions to problems. It wasn't until I took a more laissez-faire approach and allowed myself to fail that I actually made progress. I had a self-imposed deadline at the end of July to complete the project, and well, at the time of writing this it is the 28th of October - I realise the importance of organisation now and keeping a tight schedule. Although certain parts of the development were stressful, I still thoroughly enjoyed the process and am looking forward to doing it again.

## Other Notes

The [UE4 Forums](https://forums.unrealengine.com/) were an invaluable resource throughout both for code snippets and resolving bugs. Although I did my best to give the most terse description I could of the major systems of the project there is stil alot I'm leaving out.

# Game Dev Portfolio

![Night time scene](/images/nighttower.png)

## Executable Link

[Here](https://drive.google.com/open?id=1_keTpHczPYsHIKqk5v1NsBZZKpkUjGkT) is a link to a .zip file containing the executable. The code base is available on github to browse through. If you wish to download the project to take a closer look you'll need git-lfs installed. I used SourceTree (which comes with git-lfs) to clone my project. Once it's cloned, double click 'Portfolio.uproject' which will build the project and open the editor. After this, close the editor and right-click the Portfolio.uproject and select 'Generate Visual Studio project files'. Now you should have access to all my project's content. Open Portfolio.uproject to re-enter the editor.

## About Me

Hello there. My name's David, I'm 23 years old, and I hail from Ireland. I graduated with first-class honours in Computer Science and Information Technology in 2017. Ever since then I've been learning UE4, gaining competence in Blender, and working on a portfolio.

## Overview

From a young age I've loved to play RPGs and it's remained my favourite genre of game throughout my life. Being an aspiring game developer I naturally gravitated towards this genre and wanted to try my hand at creating some RPG mechanics. The premise for this project is that the player is situated on an island and is helping the local islanders in preparation for a small fireworks display. The player helps the islanders by completing quests; these quests are fulfilled by interacting with various actors throughout the world. I encountered challenges along the way and will discuss these in my lessons learned section. What follows here is a list of the main features in the project and a very basic description of each.

## Gameplay Features


### Interaction System

![Interaction](/images/interaction.png)

Inside my project settings I created a custom collision channel called 'Interactable'. Next, inside my player character I made a sphere collision volume that only does checks for the above channel. An interface also called 'Interactable' was built with two methods: 'Interact' and 'Exit Interact'.

To create an actor that's capable of interaction I first changed the collision type on one of the actor's static mesh components (if the actor did not have a static mesh component I created a volume instead and applied it to that). Following this, I then implemented the Interactable interface and provided my definitions for Interact and Exit Interact.

Whenever the volume around the player comes into contact with an interactable, it then adds it to a potential interactables array and the best one is chosen by means of a dot product with the player's camera. I used [this](https://www.youtube.com/watch?v=JDrdfCyujJE&index=1&list=PL5ADyRqL3M8HZru3SHTWF_iL28V25WaIp) tutorial for reference while making this sytem.

### Day-Night Cycle

![Dawn](/images/dawntime.png)
![Day](/images/daytime.png)
![Night](/images/nighttime.png)

An actor was created to house the logic called 'DayNightCycle'. A number of actors are involved in the cycle including the sky sphere, directional light, and sky light. To drive various values associated with each (in particular the directional light) 0-1 curves were created e.g a curve to update the directional light's colour, another curve to update sunlight intensity and so on. I then made a timeline to drive an alpha curve where the resulting value would be fed into each of the curves above. Hourly/minute updates are broadcasted by means of a delegate (held inside the game mode) to any actors across the project that are interested (e.g. street lights).

### Dialogue System

![Dialogue01](/images/dialogue01.png)

Any actor that is capable of dialogue has a dialogue component. In here, I have variables that store data tables for quest dialogue and non-quest dialogue.

The dialogue system is not true branching dialogue (as it's not implemented with a tree structure) but a more basic data table approach. Whenever the player engages in quest dialogue, the dialogue component fetches an array of dialogue structs that coincide with the current stage of the quest from the quest actor's data table. Inside these structs are string arrays of player and npc responses.

Once dialogue starts, if the player selects an option that appears in let's say, row 0 column 0 of the player responses, then it will fetch the npc response at that same location in the npc responses. Once the player clicks on the dialogue option, it then checks a seperate outcomes data table for any outcome that might result. Outcomes are represented by an enum. I have two outcomes in my project 'UpdateObjective' and 'Exit'.

![Dialogue02](/images/casualdialogue.png)

The above picture shows the less advanced dialogue, a string array of responses is housed in the dialogue component, a random string is chosen, sent to the UI, and exposed to the user for a set number of seconds.

### Quest System

![quest02](/images/questui.png)
![quest01](/images/quests.png)

A 'Questline' actor exists in the world which stores two arrays: 'ActiveQuests' and 'CompletedQuests'. On my player controller I keep an active quest index variable which remembers which quest I'm following. When a quest is chosen, the objective marker (if there is any) is updated. When we get to the last objective in a quest, it's taken out of the ActiveQuests array and moved to the CompletedQuests array.

The quests of the game are stored inside a data table, a quest structure was made to represent each quest. Every quest has an array of objectives, a stage to represent where we are in the quest, and a name among other UI related variables.
Each objective in the objectives array is also represented by a structure. This structure among the UI related variables also has a stage, and a string for an event name.

After each objective is complete we see if there's any event by this name in the questline actor and execute it if it exists. This way allows us to add items to the player after a quest has finished or provide other custom behaviour idiosyncratic to that quest.


### Inventory System

![inventory](/images/inventory.png)

The inventory follows a slot based approach. A structure was made to represent an inventory item. Any actors that wish to become an inventory item can add an inventory component to their hierarchy, where a structure variable is exposed to editing. In here we can add a custom icon, the class, if the actor is equippable, or if it is a quest item. A blueprint interface called 'Useable' was made so that actors can provide their own definitions for how they're interacted with inside the inventory. For example, the helmet in the picture will be equipped when clicked on, while the dye (represent by the pink cylinder) will turn the player's shirt to a random colour.

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

### Water

Water was created by following parts of [this](https://wiki.unrealengine.com/Water_Shader_Tutorial) tutorial.

### Rocks

I followed [this](https://www.youtube.com/watch?v=BeO7ZZPLeKA) tutorial to make the low-poly rock mesh. I utilised the same sort of work-flow to also create the cave.

## Characters

The character meshes and the majority of animations were sourced from [Mixamo](https://www.mixamo.com/#/). All remaining animations were sourced from the UE4 Starter Content.

## Audio

The majority of the audio was sourced from [BBC Sound Effects](http://bbcsfx.acropolis.org.uk/). Audio for the UI was sourced from the [Content Examples](https://docs.unrealengine.com/en-us/Resources/ContentExamples). The audio for the footsteps was sourced from the [Couch Knights Demo](https://www.unrealengine.com/marketplace/couch-knights).

## Particles & UI

The smoke particle effect was sourced from the UE4 Starter Content, in addition to about half of the materials used in the particle effects aswell. The star texture for the objective marker was sourced from the UE4 Content Examples. Blueprint to draw the objective marker on screen was also sourced from the same location (I later converted this to C++). The colour grade was sourced from [here](https://docs.unrealengine.com/en-us/Engine/Rendering/PostProcessEffects/ColorGrading) and tweaked in various ways to make the game more vibrant and saturated.

## Lessons Learned

Many lessons were learned throughout the development of this project. I estimate it took me somewhere in the range of 4-5 months to complete and was a very steep learning curve. All in all, scope was the major challenge. At the beginning I didn't know how big I wanted the project to be or in fact where exactly I wanted to go with it. As I continued development, I changed ideas many times over; doubts about whether my ideas were good enough plagued me in the early stages of development. If I had my time over to do it again, I would have done something much smaller and focused, something in the nature of a few weeks instead of a few months. Another major problem I encountered was best practices. It was difficult to know if the solutions I completed were actually done in the best way possible. In fact, in the earlier stages of the project I would estimate 2-3 weeks was spent on the whiteboard trying to figure out the most optimal solutions to problems. It wasn't until I took a more laissez-faire approach and allowed myself to fail that I actually made progress. Once I adopted this fail-fast mentality it drastically improved my work rate and relieved alot of self-doubt and pressure. I was constantly prototyping and iterating on my ideas, and because of this I found the ideas that worked quicker, and those that didn't work could be discarded even faster.
I had a self-imposed deadline at the end of July to complete the project, and well, I managed to upload the project on the 1st of November. I now realise the importance of organisation and keeping a tight schedule. This also includes documentation, as without it the implementations and motivations for gameplay systems can easily be forgotten. Although certain parts of the development were stressful, I still thoroughly enjoyed the process and I'm looking forward to doing it again. The entire development only reinforced the notion in my mind that game development is the area where I'd like to pursue a career in.

## Other Notes

The [UE4 Forums](https://forums.unrealengine.com/) were an invaluable resource throughout both for code snippets and resolving bugs. Although I did my best to give the most terse description I could of the major systems of the project there is still alot I'm leaving out. Due to self-imposed time constraints I didn't have enough time to do menu screens and a save system. With that in mind, once there are no more quests in the quest log you can exit the game by just closing the application. I did my upmost to quell any game crashing bugs, and I'm confident you most likely will not run into any. However, a few minor ones may still rear their head having discovered these from my own playtests and asking my friends to playtest too (these are listed below). Upon initial start-up, the game may ask for permissions to run or conflict with anti-virus software presenting a 'fatal error' window in the process. Regardless, it should still run once these windows are closed, or you wait a minute for the anti-virus to finish it's checks. Thanks for reading.

## Gallery
![Map shot 1](/images/mapviewshot1.png)
![Map shot 1](/images/mapviewshot2.png)

## Bugs

* AI characters may walk on water to get to their destination.
* Possible bug in flower fetching quest where the player can complete the quest with 4 flowers intead of 5.
* Turrets may not revert back to their default positions after their fireworks display and remain in an upward facing pose.

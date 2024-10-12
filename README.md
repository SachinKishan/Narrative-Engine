# The Narrative Engine
## A game engine for storytellers
I've seen a lot of storyteller/writer-based tools such as Twine which usually aim at providing easy text editing and story creation for making interactive story content. Unfortunately, this is often limited to text only, and with some coding ability, writers make 2D stories too. This engine is intended to solve that- make a game engine for writers that can easily allow them to create stories in 3D worlds without the hassle of coding. Of course, this comes with many limited features and scope but it has the benefit of having tools targeted solely towards telling a story or narrative.

Features accomplished:
1. Scene loading, editing, saving
2. Scene rendering
3. Point lights
4. Multi-point support
5. Object picking using ray casting(through the use of dynamic box SDFs)
6. Window resizing support
7. Player movement
8. Interactable "event" points that users can customize for different outcomes and conditional events. 
9. Player collectible objects and an inventory system
10. Scene changing system
11. A "game builder" that builds game files and provides a file to execute the game along with an executor for the game
12. An executor application that can execute game files

What's next?
1. The Narrative Engine is opening up for public contributions! Watch this space for updates.

<hr>

## Project Set Up

### Windows
The project requires Microsoft Visual Studio as an IDE. Simply fork the repo and open it up and it should work as intended.

### Linux
Current release of the Narrative Engine requires use of Wine as a compatibility layer.
### Instructions to install Wine:
#### Ubuntu (and Ubuntu-based Distributions)
1. Add 32-bit architecture:
	`sudo dpkg --add-architecture i386`
2. Download and add the repository key:
	`sudo mkdir -pm755 /etc/apt/keyrings`
	`sudo wget -O /etc/apt/keyrings/winehq-archive.key [https://dl.winehq.org/wine-builds/winehq.key](https://dl.winehq.org/wine-builds/winehq.key)`
3. Find Ubuntu Version:
	`cat /etc/os-release`
4. Add the WineHQ sources file w.r.t. your Ubuntu version:
	- Ubuntu 24.04:
	`sudo wget -NP /etc/apt/sources.list.d/ [https://dl.winehq.org/wine-builds/ubuntu/dists/noble/winehq-noble.sources](https://dl.winehq.org/wine-builds/ubuntu/dists/noble/winehq-noble.sources)`
	- Ubuntu 22.04:
	`sudo wget -NP /etc/apt/sources.list.d/ [https://dl.winehq.org/wine-builds/ubuntu/dists/jammy/winehq-jammy.sources](https://dl.winehq.org/wine-builds/ubuntu/dists/jammy/winehq-jammy.sources)`
	- Ubuntu 20.04:
	`sudo wget -NP /etc/apt/sources.list.d/ [https://dl.winehq.org/wine-builds/ubuntu/dists/focal/winehq-focal.sources](https://dl.winehq.org/wine-builds/ubuntu/dists/focal/winehq-focal.sources)`
5. Update package information and install WineHQ Stable:
	`sudo apt update && sudo apt install --install-recommends winehq-stable`
#### Fedora (and Fedora-based Distributions):
1. Add the repository:
	`sudo dnf config-manager --add-repo [https://dl.winehq.org/wine-builds/fedora/$(rpm -E %fedora)/winehq.repo](https://dl.winehq.org/wine-builds/fedora/$(rpm -E %fedora)/winehq.repo)`
2. Install WineHQ Stable:
	`sudo dnf install winehq-stable`

For additional help, visit WineHQ's website for FAQs and general troubleshooting while installing Wine: https://www.winehq.org/

That's it, you should be able to open the Windows executable build for NarrativeEngine using Wine's Application Loader. Be sure to install any additional dependencies Wine prompts while running the NarrativeEngine for the first time. :)
### Mac
The project requires XCode as an IDE. Simply fork the repo and open it up in XCode.
1. Go to Targets -> Build Phases -> Link Binary with Libraries
   	Add the Mac binaries from the lib folder and add the OpenGL.framework
2. Go to Targets -> Build Settings -> Search paths
   	Add library and header search path to "your-location/NarrativeEngine/NarrativeEngine/Libraries/include"

<hr>

## How to make a PR
To make a PR, fork the repo, make your changes and create a Pull Request. Your PR description should preferably have the following format

1. The goal of the PR in the title
2. What changes you made(in brief) to achieve the goal
3. Any test cases you ran to ensure robust functioning (are there edge cases to worry about?)

You can look at possible issues to take up and develop in the Issues tab of the repository.

<hr>

## Current focus
This project is being set up for open source contribution with a few goals in mind

1. Create an MVP for writers and other users to test with
2. Ensure a robust contribution pipeline


<hr>

This engine is meant to be a hobby project. 
If you have any suggestions, let me know at sachinkishan02@gmail.com or Twitter- @SachinKishan9

<img width="414" alt="Screenshot 2023-12-19 125826" src="https://github.com/SachinKishan/Narrative-Engine/assets/33657481/2378ab60-7892-46ab-a1f5-1d1e93fda665">


<img width="600" alt="Screenshot 2024-02-04 172034" src="https://github.com/SachinKishan/Narrative-Engine/assets/33657481/a2a0ddc0-d06c-40ed-a4ea-351686d122ae">

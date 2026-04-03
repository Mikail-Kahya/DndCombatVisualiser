# D&D Campaign Tools
C++20, (Custom Engine)[https://github.com/Mikail-Kahya/custom-engine]

## Overview

This project covers automating Dungeons & Dragons combat mechanics and campaign workflows to streamline the experience for my players. These are **extensible editor-style tools** that automate repetitive combat calculations, dice rolls, initiative tracking, and campaign state management. Currently it is designed for me alone but the goal is one day getting a whole interface together and doing the things that I can't on D&D Beyond.


## Tech Stack
- **C++20** (modern features, RAII)
- **Custom engine** (Own engine)
- **CMake** (modular linking)
- **Git** (version control)

# Design Philosophy
- **DM-first** - Tools serve campaign needs (mostly personal)
- **Expansion** - Easy to add new combat mechanics
- **Session safe** - No data loss mid-campaign
- **Lightweight** - Own engine will result in no extra overhead and fast tools

## Key Tools
### Combat Automation
- Initiative tracking with automated turn order
- Dice roll automation (d20, damage, modifiers) with overrides possible for edge cases
- Attack/Defense resolution engine
- Movement tracking using pathfinding
- Status effect tracking (poison, stun, etc.)

### Campaign State Manager (Not done)
- Persistent save/load for campaign world state
- NPC tracking and relationship systems
- Quest/milestone automation
- Inventory management

### Character Sheet Analysis (Not done)
- Reading character sheets turning them into usable data
- Adding abilities / custom features that adjust stats (homebrew)
- Changing equipment and it reflecting in the final stats

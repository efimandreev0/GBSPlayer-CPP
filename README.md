# GBSPlayer-CPP

**GBSPlayer-CPP** is a .gbs format player written in C++, ported from C#.

## Features
- Lightweight C++ implementation
- Easy to integrate
- Supports GBS file playback

## Usage

Here is a simple example of how to use the player:

```cpp
#include "GbsPlayer.h"

int main() {
    GbsPlayer player;

    // Initialize the player subsystem
    if (player.Init()) {
        // Load your GBS file
        player.LoadFile("YourGBSFile.gbs");
        
        // Select track (usually starts from 0)
        int GBSTrackNumber = 0; 
        player.StartTrack(GBSTrackNumber);
        
        // Start processing thread
        player.StartThread();
        
        // Begin playback
        player.Play();

        // Main loop to keep the application running
        while(true) {
            // Add sleep or logic here to prevent 100% CPU usage
        }
    }
    return 0;
}

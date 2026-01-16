# GBSPlayer-CPP
There is a .gbs-format player written in C++ from C#.
## USAGE:
GbsPlayer player;
if (player.Init()) {
    player.LoadFile("YourGBSFile");
    player.StartTrack(GBSTrackNumber);
    player.StartThread();
    player.Play();
    while(true) {

    }
}

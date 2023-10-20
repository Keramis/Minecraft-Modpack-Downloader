# Minecraft-Modpack-Downloader
A PEEXE (portable executable) that downloads the mods in a manifest.json file without Minecraft having to do the heavy lifing.

# Why do this?
I was very annoyed when I couldn't play my favorite modpacks because they decided to use a `manifest.json` file instead of actually
including the mods in their modpack zip. So, I made a tool to download all of them.

This used to be a simple powershell script, but it wasn't expandable, and I don't like the Powershell language, quite frankly.
I tried using python, but C++ is easier, so you now get a portable executable. If enough people want a Python script version
(for MACOS), maybe I'll make one.

# Usage
So, how do you use this? Just drag-and-drop the `manifest.json` file over the executable, and let it do all the work. It'll dump
all the mods in the folder that it's in. Move those out of your folder and into the `mods/` folder of your modpack.

# Video
Here's a video of it doing its business (I had to cut it short; I don't want all the mods on my computer to take up space):
https://github.com/Keramis/Minecraft-Modpack-Downloader/assets/81401952/8cbc5e86-8d19-4672-a306-2e7dde9e6b9d

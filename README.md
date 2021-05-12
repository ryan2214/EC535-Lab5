# EC535-Lab5
nethack game on bbb

# library
QT, program is ok to run in the image provided in lab 5.

# before run

"qmake", then "make" to compile in QT environment.

put map.txt in /root, the program calls map.txt by /root/map.txt

On the other hand, if you want to run this in other os, search and change this route in mainwindow.cpp line:75 to the position of map.txt.

If the map.txt is ready in /root or the path you changed in code, use ./menus to run.

# command

Press arrow keys to move, press A or arrow keys to attack in facing direction.

For menus bar:

Save is a place holder.

About shows some author information.

# Custom map

in map.txt:

0: dirtground

1: grassGround

2: floor

3: water

4: wall

5: door

6: gold

7: tree

# ascii-physics

![downloads](https://img.shields.io/github/downloads/xyzpw/ascii-physics/total)
![timestamp](https://img.shields.io/date/1741367040?label=repo%20creation)

Ncurses application to simulate physics.


https://github.com/user-attachments/assets/3d38d247-7a0e-471f-9747-e85783d59044

## Usage
Make executable and execute:
```bash
$ make
$ ./physics
```

### Arguments
To view all arguments, use the help arg:
```bash
$ ./physics --help
```

Mass can be customized with args:
```bash
# Set mass to 50 kg.
$ ./physics -m 50
```

The coefficient of restitution can also be customized with the `cor` arg:
```bash
$ ./physics --cor 0.5
```

Changing size of objects:
```bash
# Sets the size of each char to 0.5 meters.
$ ./physics --char-size 0.5
```
> [!NOTE]
> Each char is 1 meter by default.

Changing launch values:
```bash
# Set launch velocity to 5 m/s and angle to 45 degrees.
$ ./physics --lvelocity 5 --langle 45
```

Preset objects can be used, the list can be shown via:
```bash
# List presets.
$ ./physics --list-preset
# Tennis ball.
$ ./physics --preset tennis
```

### Controls
- `q`: quit
- `wasd`: move object
- `space`: drop object
- `r`: reset simulation
- `l`: launch
- `n`: add new object
- `I`: input mode
- `N`: add obstacle
- `S`: slingshot mode
- `z`: undo spawned object
- `b`: open menu panel (clickable)

#### Mouse Controls
Clicking an area on terminal will spawn an object there.<br>
Clicking on an object will allow you to move it, when the left mouse
 is released the object will be placed in that position,
 this can be canceled by pressing right click while left click is still being pressed.

##### Slingshot Mode
When in slingshot mode, click an object, move mouse to a direction and release to launch.

##### Repulsion Click
When enabled, clicking will push back nearby objects.<br>
The closer the object the faster they are pushed away.

#### Changing Values
- `KEY_UP`: Increment selected value
- `KEY_DOWN`: Decrement selected value

##### Object Selection
- `KEY_LEFT`: select previous object
- `KEY_RIGHT`: select next object

###### Resizing obstacles
Use arrow keys to resize selected obstacle.

##### Selecting Values to Change
- `L` Select launch angle
- `V` Select launch velocity
- `M` Select object mass

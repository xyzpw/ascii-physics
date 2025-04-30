# ascii-physics

![downloads](https://img.shields.io/github/downloads/xyzpw/ascii-physics/total)
![timestamp](https://img.shields.io/date/1741367040?label=repo%20creation)

Ncurses application to simulate physics.

![preview-2025-04-03 10-15](https://github.com/user-attachments/assets/d46d67f3-4736-403f-95eb-feb40e543f5a)

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

#### Changing Values
- `KEY_UP`: Increment selected value
- `KEY_DOWN`: Decrement selected value

##### Selecting Values to Change
- `L` Select launch angle
- `V` Select launch velocity

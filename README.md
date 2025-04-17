# ascii-physics

Ncurses application to simulate physics.

![preview-2025-04-03 10-15](https://github.com/user-attachments/assets/d46d67f3-4736-403f-95eb-feb40e543f5a)

## Usage
Make executable and execute:
```bash
$ make
$ ./physics
```

### Arguments
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

### Controls
- `q`: quit
- `wasd`: move object
- `space`: drop object
- `r`: reset simulation
- `l`: launch

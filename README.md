# ascii-physics

Ncurses application to simulate physics.

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

### Controls
- `q`: quit
- `wasd`: move object
- `space`: drop object
- `r`: reset simulation

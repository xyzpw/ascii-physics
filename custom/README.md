# Custom Simulations
Files in this directory can be used to run custom simulations.

## Format
Format for variables `var value`.

Example, changing the default mass:
```conf
mass 0.1
```

Currently, if a variable is a boolean, any value will be set to true.<br>
Example:
```conf
trails 1
```
This will set trails to be shown, but the value used has no effect.

### Events
Events can be used via:
```conf
on start:
    launch 10,0
```
The above code will run these commands when the simulation is started.

### Vars
- `mass`: default mass of objects
- `metersPerChar`: size of each char in meters
- `cor`: default coefficient of restitution
- `trails`: enables trails
- addObstacle &lt;x&gt;,&lt;y&gt;
    - Adds obstacle upon running with a vector position of `(x, y)` meters.

#### Event Commands
- launch &lt;x&gt;,&lt;y&gt;
    - Launches the object with a velocity vector of `(x, y)` m/s.

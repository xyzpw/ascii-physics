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

### Vars
- `mass`: default mass of objects
- `metersPerChar`: size of each char in meters
- `cor`: default coefficient of restitution
- `trails`: enables trails

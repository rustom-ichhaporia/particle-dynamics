# Ideal Gas Visualizer

Some features of the project include: 
- A JSON configuration file through which the user can specify screen size, colors, fonts, particle types, and more. 
- The configuration of multiple particle types through the JSON, where each type can be given a name, particle count, color, and min/max speeds, radii, and masses. 
- A dynamic number of histograms of particle speeds that will match the number of particle types present (e.g. 2 evenly spaced histograms for 2 particle types, or 3 for 3 types). 
- The ability to pause and resume the simulation by pressing "P". 
- The ability to isolate particles from up to 9 different groups, then replace them when desired with the number keys are "R" key. 
- The use of arrow keys to slow down, speed up, enlarge, and shrink all of the particles. 
- A collision checking algorithm with worst-case time complexity of $n\log(n)$, as opposed to the brute force $n^2$. 
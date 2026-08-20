# quake-simd
A quake engine fork of ironwail focusing on SIMD transision and code clean up

# What's this?
A fork of the popular [ironwail](https://github.com/andrei-drexler)

## System requirements

| | Minimum GPU | Recommended GPU |
|:--|:--|:--|
|NVIDIA|GeForce GT 420 ("Fermi" 2010)|GeForce GT 630 or newer ("Kepler" 2012)|
|AMD|Radeon HD 5450 ("TeraScale 2" 2009) |Radeon HD 7700 series or newer ("GCN" 2012)|
|Intel|HD Graphics 4200 ("Haswell" 2012)|HD Graphics 620 ("Kaby Lake" 2016) or newer|

Notes:
1) These requirements might not be 100% accurate since they are based solely on reported OpenGL capabilities. There could still be unforeseen compatibility issues.
2) Mac OS is not supported at this time due to the use of OpenGL 4.3 (for compute shaders), since Apple has deprecated OpenGL after version 4.1.

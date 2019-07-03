## v0.4.0 (Jul 3, 2019)

### :rotating_light: Breaking

- `bs-decode`, `bs-abstract`, and `relude` are now `peerDependencies`, meaning you will need to install them on your own. This allows you to control their versions in a single place if you have multiple dependencies that depend on these libraries (so Bucklescript won't complain about duplicate versions)

## v0.3.1 (Jun 10, 2019)

### :bug: Bug fixes

- Bump `bs-decode` dependency to a version that fixes a stack overflow

## v0.3.0 (Jun 3, 2019)

### :sparkles: New features

- Helpers to get geometry objects (`GeoJSON.getPoints`, `getLines`, `getPolygons`)

## v0.2.0 (May 30, 2019)

### :sparkles: New features

- New `GeoJSON` constructors (`fromGeometry`, `fromGeometries`, `fromFeature`, `fromFeatures`)
- New `Feature` constructor (`fromGeometry`)
- New `Geometry` constructors (`pointFromLatLong`, `polygonFromShape`)

## v0.1.0 (May 29, 2019)

- Initial release with basic encoders and decoders for Geometry, Geometry Collection, Feature, and Feature Collection objects

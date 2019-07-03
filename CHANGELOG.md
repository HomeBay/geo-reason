## v0.4.0 (Jul 3, 2019)

### :rotating_light: Breaking

- `bs-decode`, `bs-abstract`, and `relude` are now `peerDependencies`, meaning you will need to install them on your own. This allows you to control their versions in a single place if you have multiple dependencies that depend on these libraries (so Bucklescript won't complain about duplicate versions)
- The structure of the `Polygon.LinearRing` now enforces that it has at least one shape and at least one hole, which seems consistent with the spec
- Many `ExpectedValidOption` decode errors are now `ExpectedTuple`, which only breaks things if you're matching on the specific type of decode error
- Many `fromArray` functions previously expected a JSON argument. Now they consistently work with only an array of the appropriate type, and instead of returning a result of decode errors, they return options

### :sparkles: New features

- Anywhere that `fromArray` exists, `fromList` should also be present

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

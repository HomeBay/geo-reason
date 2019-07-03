# GeoReason

[![build status](https://img.shields.io/circleci/build/github/HomeBay/geo-reason.svg?style=flat-square)](https://circleci.com/gh/HomeBay/geo-reason)
[![test coverage](https://img.shields.io/coveralls/github/HomeBay/geo-reason.svg?style=flat-square)](https://coveralls.io/github/HomeBay/geo-reason)
[![npm version](https://img.shields.io/npm/v/geo-reason.svg?style=flat-square)](https://www.npmjs.com/package/geo-reason)
[![license](https://img.shields.io/github/license/HomeBay/geo-reason.svg?style=flat-square)](https://github.com/HomeBay/geo-reason/blob/master/LICENSE)

GeoReason provides ReasonML types for working with [RFC-7946-compliant GeoJSON data](https://tools.ietf.org/html/rfc7946). This includes decoders to safely parse JSON data into structured types, encoders to convert Reason data into valid GeoJSON, and a variety of utility functions for working with points, polygons, and more.

## Installation

Get the library from npm:

`npm install --save geo-reason`

Then update your `bsconfig.json`:

```json
"bs-dependencies": [
  "geo-reason"
],
```

Note that this library has some peerDependencies. If you don't already have the following libraries, you should also: `npm install --save bs-abstract bs-decode relude`. While this is a little more work up front, it helps you avoid duplicate versions of these packages which can cause issues while compiling.

## Purpose and Usage

If you've worked with GeoJSON in its JSON form, you've probably noticed that it's very compact, but hard to make sense of (and error-prone to produce). For example:

```json
[
  [
    [100.0, 0.0],
    [101.0, 0.0],
    [101.0, 1.0],
    [100.0, 1.0],
    [100.0, 0.0]
  ]
]
```

These are valid coordinates for a polygon in GeoJSON... three layers of array (and it gets worse for multi-polygons), and a bunch of unlabeled numbers. By contrast, the `GeoReason` version is a bit more verbose, but much harder to get wrong:

```reason
let polygon =
  GeoJSON.Geometry.(
    Polygon(
      Polygon.makeShape(
        ~startEnd=Position.makeLabels(~longitude=100.0, ~latitude=0.0, ()),
        ~second=Position.makeLabels(~longitude=101.0, ~latitude=0.0, ()),
        ~third=Position.makeLabels(~longitude=101.0, ~latitude=1.0, ()),
        ~rest=[Position.makeLabels(~longitude=100.0, ~latitude=1.0, ())],
        (),
      ),
    )
  );
```

The GeoReason version ensures that:

- You won't accidentally switch the latitude and longitude (which is easy to do, since GeoJSON puts longitude first)
- Your start and end point are guaranteed to be the same, which is required by the spec
- Your polygon has at least three points, which is also required by the spec

Note that there are some additional rules in the spec (e.g. for polygons, exterior rings are specified in counter-clockwise order, inner rings are clockwise) that aren't easy to enforce with the compiler. So it's still possible to produce invalid GeoJSON, but it's definitely harder.

## Contributing

- fork and clone [this repository](https://github.com/HomeBay/geo-reason)
- if you're adding utility functions, make sure they're fully tested
- if you're fixing encoders/decoders, please use samples from [the spec](https://tools.ietf.org/html/rfc7946) in tests

## License

Released under the [MIT license](https://github.com/HomeBay/geo-reason/blob/master/LICENSE).

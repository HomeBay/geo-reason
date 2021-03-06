/**
 * The following sample data is taken from the official samples in the geojson
 * spec: https://tools.ietf.org/html/rfc7946#appendix-A
 */

let pointJson: Js.Json.t = [%raw
  {|
  {
    "type": "Point",
    "coordinates": [100.0, 0.0]
  }
  |}
];

let latLong =
  GeoJSON.Geometry.Position.LatLong.makeLabels(
    ~longitude=100.2,
    ~latitude=10.8,
  );

let pointPosition =
  GeoJSON.Geometry.Position.makeLabels(~longitude=100.0, ~latitude=0.0, ());

let pointGeometry = GeoJSON.Geometry.Point(pointPosition);

let pointGeoJson = GeoJSON.fromGeometry(pointGeometry);

let lineJson: Js.Json.t = [%raw
  {|
  {
    "type": "LineString",
    "coordinates": [
      [100.0, 0.0],
      [101.0, 1.0]
    ]
  }
  |}
];

let line =
  GeoJSON.Geometry.(
    Line.twoPoints(
      Position.makeLabels(~longitude=100.0, ~latitude=0.0, ()),
      Position.makeLabels(~longitude=101.0, ~latitude=1.0, ()),
    )
  );

let lineGeometry = GeoJSON.Geometry.(LineString(line));

let polygonJson: Js.Json.t = [%raw
  {|
  {
    "type": "Polygon",
    "coordinates": [
      [
        [100.0, 0.0],
        [101.0, 0.0],
        [101.0, 1.0],
        [100.0, 1.0],
        [100.0, 0.0]
      ]
    ]
  }
  |}
];

let shape =
  GeoJSON.Geometry.(
    Polygon.Shape.makeLabels(
      ~startEnd=Position.makeLabels(~longitude=100.0, ~latitude=0.0, ()),
      ~second=Position.makeLabels(~longitude=101.0, ~latitude=0.0, ()),
      ~third=Position.makeLabels(~longitude=101.0, ~latitude=1.0, ()),
      ~rest=[Position.makeLabels(~longitude=100.0, ~latitude=1.0, ())],
      (),
    )
  );

let polygon = GeoJSON.Geometry.(Polygon(Polygon.Shape(shape)));

let polygonHolesJson: Js.Json.t = [%raw
  {|
  {
    "type": "Polygon",
    "coordinates": [
      [
        [100.0, 0.0],
        [101.0, 0.0],
        [101.0, 1.0],
        [100.0, 1.0],
        [100.0, 0.0]
      ],
      [
        [100.8, 0.8],
        [100.8, 0.2],
        [100.2, 0.2],
        [100.2, 0.8],
        [100.8, 0.8]
      ]
    ]
  }
  |}
];

let polygonHoles =
  GeoJSON.Geometry.(
    Polygon.(
      LinearRing(
        Shape.make(
          Position.makeLabels(~longitude=100.0, ~latitude=0.0, ()),
          Position.makeLabels(~longitude=101.0, ~latitude=0.0, ()),
          Position.makeLabels(~longitude=101.0, ~latitude=1.0, ()),
          [Position.makeLabels(~longitude=100.0, ~latitude=1.0, ())],
        ),
        Shape.make(
          Position.makeLabels(~longitude=100.8, ~latitude=0.8, ()),
          Position.makeLabels(~longitude=100.8, ~latitude=0.2, ()),
          Position.makeLabels(~longitude=100.2, ~latitude=0.2, ()),
          [Position.makeLabels(~longitude=100.2, ~latitude=0.8, ())],
        ),
        [],
      )
    )
  );

let polygonHolesGeometry = GeoJSON.Geometry.Polygon(polygonHoles);

let multiPointJson = [%raw
  {|
  {
    "type": "MultiPoint",
    "coordinates": [
      [100.0, 0.0],
      [101.0, 1.0]
    ]
  }
  |}
];

let multiPointFirst =
  GeoJSON.Geometry.Position.makeLabels(~longitude=100.0, ~latitude=0.0, ());
let multiPointSecond =
  GeoJSON.Geometry.Position.makeLabels(~longitude=101.0, ~latitude=1.0, ());

let multiPoint =
  GeoJSON.Geometry.(MultiPoint([multiPointFirst, multiPointSecond]));

let multiLineJson = [%raw
  {|
  {
    "type": "MultiLineString",
    "coordinates": [
      [
        [100.0, 0.0],
        [101.0, 1.0]
      ],
      [
        [102.0, 2.0],
        [103.0, 3.0]
      ]
    ]
  }
  |}
];

let multiLine =
  GeoJSON.Geometry.(
    MultiLineString([
      Line.twoPoints(
        Position.makeLabels(~longitude=100.0, ~latitude=0.0, ()),
        Position.makeLabels(~longitude=101.0, ~latitude=1.0, ()),
      ),
      Line.twoPoints(
        Position.makeLabels(~longitude=102.0, ~latitude=2.0, ()),
        Position.makeLabels(~longitude=103.0, ~latitude=3.0, ()),
      ),
    ])
  );

let multiPolygonJson: Js.Json.t = [%raw
  {|
  {
    "type": "MultiPolygon",
    "coordinates": [
      [
        [
          [102.0, 2.0],
          [103.0, 2.0],
          [103.0, 3.0],
          [102.0, 3.0],
          [102.0, 2.0]
        ]
      ],
      [
        [
          [100.0, 0.0],
          [101.0, 0.0],
          [101.0, 1.0],
          [100.0, 1.0],
          [100.0, 0.0]
        ],
        [
          [100.2, 0.2],
          [100.2, 0.8],
          [100.8, 0.8],
          [100.8, 0.2],
          [100.2, 0.2]
        ]
      ]
    ]
  }
  |}
];

let polygons =
  GeoJSON.Geometry.[
    Polygon.makeShape(
      ~startEnd=Position.makeLabels(~longitude=102.0, ~latitude=2.0, ()),
      ~second=Position.makeLabels(~longitude=103.0, ~latitude=2.0, ()),
      ~third=Position.makeLabels(~longitude=103.0, ~latitude=3.0, ()),
      ~rest=[Position.makeLabels(~longitude=102.0, ~latitude=3.0, ())],
      (),
    ),
    Polygon.(
      LinearRing(
        Shape.makeLabels(
          ~startEnd=Position.makeLabels(~longitude=100.0, ~latitude=0.0, ()),
          ~second=Position.makeLabels(~longitude=101.0, ~latitude=0.0, ()),
          ~third=Position.makeLabels(~longitude=101.0, ~latitude=1.0, ()),
          ~rest=[Position.makeLabels(~longitude=100.0, ~latitude=1.0, ())],
          (),
        ),
        Shape.makeLabels(
          ~startEnd=Position.makeLabels(~longitude=100.2, ~latitude=0.2, ()),
          ~second=Position.makeLabels(~longitude=100.2, ~latitude=0.8, ()),
          ~third=Position.makeLabels(~longitude=100.8, ~latitude=0.8, ()),
          ~rest=[Position.makeLabels(~longitude=100.8, ~latitude=0.2, ())],
          (),
        ),
        [],
      )
    ),
  ];

let multiPolygonGeometry = GeoJSON.Geometry.MultiPolygon(polygons);

let invalidGeometry: Js.Json.t = [%raw
  {|
    { "type": "foo", "coordinates": []}
  |}
];

let geometryCollectionJson: Js.Json.t = [%raw
  {|
  {
    "type": "GeometryCollection",
    "geometries": [{
      "type": "Point",
      "coordinates": [100.0, 0.0]
    }, {
      "type": "LineString",
      "coordinates": [
        [101.0, 0.0],
        [102.0, 1.0]
      ]
    }]
  }
 |}
];

let geometryCollection =
  GeoJSON.{
    data:
      Data.GeometryCollection(
        GeoJSON.Geometry.[
          Point(Position.makeLabels(~longitude=100.0, ~latitude=0.0, ())),
          LineString(
            Line.twoPoints(
              Position.makeLabels(~longitude=101.0, ~latitude=0.0, ()),
              Position.makeLabels(~longitude=102.0, ~latitude=1.0, ()),
            ),
          ),
        ],
      ),
    boundingBox: None,
  };

let polygonGeometryCollection =
  GeoJSON.(fromGeometries([polygonHolesGeometry]));

let geoBoundingBoxJson: Js.Json.t = [%raw
  {|
  {
    "type": "Point",
    "bbox": [-124.9, 24.4, -66.8, 49.4],
    "coordinates": [100.0, 0.0]
  }
  |}
];

let geoBoundingBox =
  GeoJSON.{
    data: Data.Geometry(pointGeometry),
    boundingBox:
      Some(
        BoundingBox.makeLabels(~w=-124.9, ~s=24.4, ~e=-66.8, ~n=49.4, ()),
      ),
  };

let featureEmptySparseJson = [%raw {| { "type": "Feature" }|}];
let featureEmptyJson = [%raw
  {| { "type": "Feature", "properties": null, "geometry": null }|}
];

let featureEmpty = GeoJSON.Feature.makeLabels();

let featureNoPropertiesJson = [%raw
  {|
  {
    "type": "Feature",
    "geometry": {
      "type": "Point",
      "coordinates": [102.0, 0.5]
    },
    "id": "f2",
    "properties": null
  }
  |}
];

let featureNoProperties =
  GeoJSON.(
    Feature.make(
      Some(Feature.ID.StringID("f2")),
      Some(
        Geometry.point(
          Geometry.Position.makeLabels(~latitude=0.5, ~longitude=102.0, ()),
        ),
      ),
      None,
    )
  );

let featureCompleteJson = [%raw
  {|
  {
    "type": "Feature",
    "id": 1,
    "geometry": {
      "type": "Point",
      "coordinates": [100.0, 0.0]
    },
    "properties": { "foo": "bar" }
  }
|}
];

let featureComplete =
  GeoJSON.(
    Feature.make(
      Some(Feature.ID.NumberID(1.0)),
      Some(pointGeometry),
      Some(Js.Dict.fromList([("foo", Js.Json.string("bar"))])),
    )
  );

let lineFeatureGeoJSON =
  GeoJSON.(fromFeature(Feature.fromGeometry(lineGeometry)));

let pointFeatureCollection =
  GeoJSON.(fromFeatures([Feature.fromGeometry(pointGeometry)]));

// This is the exact example used in section 1.5 of the specification
let featureCollectionJson = [%raw
  {|
  {
    "type": "FeatureCollection",
    "features": [{
      "type": "Feature",
      "geometry": {
        "type": "Point",
        "coordinates": [102.0, 0.5]
      },
      "properties": {
        "prop0": "value0"
      }
    }, {
      "type": "Feature",
      "geometry": {
        "type": "LineString",
        "coordinates": [
          [102.0, 0.0],
          [103.0, 1.0],
          [104.0, 0.0],
          [105.0, 1.0]
        ]
      },
      "properties": {
        "prop0": "value0",
        "prop1": 0.0
      }
    }, {
      "type": "Feature",
      "geometry": {
        "type": "Polygon",
        "coordinates": [
          [
            [100.0, 0.0],
            [101.0, 0.0],
            [101.0, 1.0],
            [100.0, 1.0],
            [100.0, 0.0]
          ]
        ]
      },
      "properties": {
        "prop0": "value0",
        "prop1": {
          "this": "that"
        }
      }
    }]
  }
 |}
];

let featureCollection =
  GeoJSON.{
    boundingBox: None,
    data:
      Data.FeatureCollection([
        Feature.makeLabels(
          ~geometry=
            Geometry.(
              Point(Position.makeLabels(~latitude=0.5, ~longitude=102.0, ()))
            ),
          ~properties=
            Js.Dict.fromList([("prop0", Js.Json.string("value0"))]),
          (),
        ),
        Feature.makeLabels(
          ~geometry=
            Geometry.(
              LineString(
                Line.make(
                  Position.makeLabels(~latitude=0.0, ~longitude=102.0, ()),
                  Position.makeLabels(~latitude=1.0, ~longitude=103.0, ()),
                  [
                    Position.makeLabels(~latitude=0.0, ~longitude=104.0, ()),
                    Position.makeLabels(~latitude=1.0, ~longitude=105.0, ()),
                  ],
                ),
              )
            ),
          ~properties=
            Js.Dict.fromList([
              ("prop0", Js.Json.string("value0")),
              ("prop1", Js.Json.number(0.0)),
            ]),
          (),
        ),
        Feature.makeLabels(
          ~geometry=
            Geometry.(
              Polygon(
                Polygon.makeShape(
                  ~startEnd=
                    Position.makeLabels(~latitude=0.0, ~longitude=100.0, ()),
                  ~second=
                    Position.makeLabels(~latitude=0.0, ~longitude=101.0, ()),
                  ~third=
                    Position.makeLabels(~latitude=1.0, ~longitude=101.0, ()),
                  ~rest=[
                    Position.makeLabels(~latitude=1.0, ~longitude=100.0, ()),
                  ],
                  (),
                ),
              )
            ),
          ~properties=
            Js.Dict.fromList([
              ("prop0", Js.Json.string("value0")),
              (
                "prop1",
                Js.Json.object_(
                  Js.Dict.fromList([("this", Js.Json.string("that"))]),
                ),
              ),
            ]),
          (),
        ),
      ]),
  };

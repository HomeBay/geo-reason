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

let point =
  GeoJSON.Geometry.(
    Point(Position.makeLabels(~longitude=100.0, ~latitude=0.0, ()))
  );

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
    LineString(
      Line.twoPoints(
        Position.makeLabels(~longitude=100.0, ~latitude=0.0, ()),
        Position.makeLabels(~longitude=101.0, ~latitude=1.0, ()),
      ),
    )
  );

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
    Polygon(
      Polygon.(
        LinearRing([
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
        ])
      ),
    )
  );

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

let multiPoint =
  GeoJSON.Geometry.(
    MultiPoint([
      Position.makeLabels(~longitude=100.0, ~latitude=0.0, ()),
      Position.makeLabels(~longitude=101.0, ~latitude=1.0, ()),
    ])
  );

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

let multiPolygon =
  GeoJSON.Geometry.(
    MultiPolygon([
      Polygon.makeShape(
        ~startEnd=Position.makeLabels(~longitude=102.0, ~latitude=2.0, ()),
        ~second=Position.makeLabels(~longitude=103.0, ~latitude=2.0, ()),
        ~third=Position.makeLabels(~longitude=103.0, ~latitude=3.0, ()),
        ~rest=[Position.makeLabels(~longitude=102.0, ~latitude=3.0, ())],
        (),
      ),
      Polygon.(
        LinearRing([
          Shape.makeLabels(
            ~startEnd=
              Position.makeLabels(~longitude=100.0, ~latitude=0.0, ()),
            ~second=Position.makeLabels(~longitude=101.0, ~latitude=0.0, ()),
            ~third=Position.makeLabels(~longitude=101.0, ~latitude=1.0, ()),
            ~rest=[Position.makeLabels(~longitude=100.0, ~latitude=1.0, ())],
            (),
          ),
          Shape.makeLabels(
            ~startEnd=
              Position.makeLabels(~longitude=100.2, ~latitude=0.2, ()),
            ~second=Position.makeLabels(~longitude=100.2, ~latitude=0.8, ()),
            ~third=Position.makeLabels(~longitude=100.8, ~latitude=0.8, ()),
            ~rest=[Position.makeLabels(~longitude=100.8, ~latitude=0.2, ())],
            (),
          ),
        ])
      ),
    ])
  );

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

let geoBoundingBoxJson: Js.Json.t = [%raw
  {|
  {
    "type": "Point",
    "bbox": [-124.9, 24.4, -66.8, 49.4],
    "coordinates": [100.0, 0.0]
  }
  |}
];

let goeBoundingBox =
  GeoJSON.{
    data: Data.Geometry(point),
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
      Some(point),
      Some(Js.Dict.fromList([("foo", Js.Json.string("bar"))])),
    )
  );

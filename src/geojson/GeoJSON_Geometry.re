open Relude.Globals;
module Line = GeoJSON_Line;
module Polygon = GeoJSON_Polygon;
module Position = GeoJSON_Position;

type t =
  | Point(Position.t)
  | LineString(Line.t)
  | Polygon(Polygon.t)
  | MultiPoint(list(Position.t))
  | MultiLineString(list(Line.t))
  | MultiPolygon(list(Polygon.t));

let point = position => Point(position);
let lineString = line => LineString(line);
let polygon = poly => Polygon(poly);
let multiPoint = points => MultiPoint(points);
let multiLineString = lines => MultiLineString(lines);
let multiPolygon = polygons => MultiPolygon(polygons);

let pointFromLatLong = latLong => Point(Position.fromLatLong(latLong));
let polygonFromShape = shape => Polygon(Polygon.Shape(shape));

let getPoints =
  fun
  | Point(point) => [point]
  | MultiPoint(points) => points
  | LineString(_)
  | Polygon(_)
  | MultiLineString(_)
  | MultiPolygon(_) => [];

let getLines =
  fun
  | LineString(line) => [line]
  | MultiLineString(lines) => lines
  | Point(_)
  | Polygon(_)
  | MultiPoint(_)
  | MultiPolygon(_) => [];

let getPolygons =
  fun
  | Polygon(poly) => [poly]
  | MultiPolygon(polygons) => polygons
  | Point(_)
  | LineString(_)
  | MultiPoint(_)
  | MultiLineString(_) => [];

let eq = (a, b) =>
  switch (a, b) {
  | (Point(a), Point(b)) => Position.eq(a, b)
  | (LineString(a), LineString(b)) => Line.eq(a, b)
  | (Polygon(a), Polygon(b)) => Polygon.eq(a, b)
  | (MultiPoint(a), MultiPoint(b)) => List.eqBy(Position.eq, a, b)
  | (MultiLineString(a), MultiLineString(b)) => List.eqBy(Line.eq, a, b)
  | (MultiPolygon(a), MultiPolygon(b)) => List.eqBy(Polygon.eq, a, b)
  | (Point(_), _)
  | (LineString(_), _)
  | (Polygon(_), _)
  | (MultiPoint(_), _)
  | (MultiLineString(_), _)
  | (MultiPolygon(_), _) => false
  };

let decode = {
  let decodeCoords = (innerDecode, constructor) =>
    Decode.AsResult.OfParseError.(
      field("coordinates", map(constructor, innerDecode))
    );

  let decodeCoordsList = innerDecode =>
    Decode.AsResult.OfParseError.list(innerDecode) |> decodeCoords;

  let decodeType =
    fun
    | "Point" => decodeCoords(Position.decode, point)
    | "LineString" => decodeCoords(Line.decode, lineString)
    | "Polygon" => decodeCoords(Polygon.decode, polygon)
    | "MultiPoint" => decodeCoordsList(Position.decode, multiPoint)
    | "MultiLineString" => decodeCoordsList(Line.decode, multiLineString)
    | "MultiPolygon" => decodeCoordsList(Polygon.decode, multiPolygon)
    | _ => (
        json =>
          Result.error(Decode.ParseError.Val(`ExpectedValidOption, json))
      );

  Decode.AsResult.OfParseError.(
    field("type", string) |> flatMap(decodeType)
  );
};

let encodeFields = data => {
  let pairCoords = (name, coords) => [
    ("type", Js.Json.string(name)),
    ("coordinates", coords),
  ];

  let pairList = (name, encode, xs) =>
    Array.(fromList(xs) |> map(encode) |> Js.Json.array) |> pairCoords(name);

  switch (data) {
  | Point(pos) => pairCoords("Point", Position.encode(pos))
  | LineString(line) => pairCoords("LineString", Line.encode(line))
  | Polygon(poly) => pairCoords("Polygon", Polygon.encode(poly))
  | MultiPoint(points) => pairList("MultiPoint", Position.encode, points)
  | MultiLineString(lines) => pairList("MultiLineString", Line.encode, lines)
  | MultiPolygon(polygons) =>
    pairList("MultiPolygon", Polygon.encode, polygons)
  };
};

let encode = data => Js.(Dict.fromList(encodeFields(data)) |> Json.object_);

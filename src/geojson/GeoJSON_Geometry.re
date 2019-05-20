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

let decode = json => {
  let decodeCoords = (innerDecode, constructor) =>
    Decode.AsOption.(field("coordinates", innerDecode |> map(constructor)));

  let decodeCoordsList = innerDecode =>
    Decode.AsOption.list(innerDecode) |> decodeCoords;

  let decodeType =
    fun
    | "Point" => decodeCoords(Position.decode, point)
    | "LineString" => decodeCoords(Line.decode, lineString)
    | "Polygon" => decodeCoords(Polygon.decode, polygon)
    | "MultiPoint" => decodeCoordsList(Position.decode, multiPoint)
    | "MultiLineString" => decodeCoordsList(Line.decode, multiLineString)
    | "MultiPolygon" => decodeCoordsList(Polygon.decode, multiPolygon)
    | _ => const(None);

  json |> Decode.AsOption.(field("type", string) |> flatMap(decodeType));
};

let encode = data => {
  let pairCoords = (name, coords) => [
    ("type", Js.Json.string(name)),
    ("coordinates", coords),
  ];

  let pairList = (name, encode, xs) =>
    Array.(fromList(xs) |> map(encode) |> Js.Json.array) |> pairCoords(name);

  let tuples =
    switch (data) {
    | Point(pos) => pairCoords("Point", Position.encode(pos))
    | LineString(line) => pairCoords("LineString", Line.encode(line))
    | Polygon(poly) => pairCoords("Polygon", Polygon.encode(poly))
    | MultiPoint(points) => pairList("MultiPoint", Position.encode, points)
    | MultiLineString(lines) =>
      pairList("MultiLineString", Line.encode, lines)
    | MultiPolygon(polygons) =>
      pairList("MultiPolygon", Polygon.encode, polygons)
    };

  Js.(Dict.fromList(tuples) |> Json.object_);
};

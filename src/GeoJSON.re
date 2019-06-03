open Relude.Globals;
module BoundingBox = GeoJSON_BoundingBox;
module Geometry = GeoJSON_Geometry;
module Feature = GeoJSON_Feature;

module Data = {
  type t =
    | Geometry(Geometry.t)
    | Feature(Feature.t)
    | GeometryCollection(list(Geometry.t))
    | FeatureCollection(list(Feature.t));

  let geometry = v => Geometry(v);
  let feature = v => Feature(v);
  let geometryCollection = v => GeometryCollection(v);
  let featureCollection = v => FeatureCollection(v);

  let getPoints =
    fun
    | Geometry(geo) => Geometry.getPoints(geo)
    | Feature(feature) => Feature.getPoints(feature)
    | GeometryCollection(geos) => List.flatMap(Geometry.getPoints, geos)
    | FeatureCollection(features) =>
      List.flatMap(Feature.getPoints, features);

  let getLines =
    fun
    | Geometry(geo) => Geometry.getLines(geo)
    | Feature(feature) => Feature.getLines(feature)
    | GeometryCollection(geos) => List.flatMap(Geometry.getLines, geos)
    | FeatureCollection(features) =>
      List.flatMap(Feature.getLines, features);

  let getPolygons =
    fun
    | Geometry(geo) => Geometry.getPolygons(geo)
    | Feature(feature) => Feature.getPolygons(feature)
    | GeometryCollection(geos) => List.flatMap(Geometry.getPolygons, geos)
    | FeatureCollection(features) =>
      List.flatMap(Feature.getPolygons, features);

  let decode = {
    let decodeFromField =
      fun
      | "Feature" =>
        Feature.decode |> Decode.AsResult.OfParseError.map(feature)
      | "GeometryCollection" =>
        Decode.AsResult.OfParseError.(
          field(
            "geometries",
            list(Geometry.decode) |> map(geometryCollection),
          )
        )
      | "FeatureCollection" =>
        Decode.AsResult.OfParseError.(
          field("features", list(Feature.decode) |> map(featureCollection))
        )
      | _ => Geometry.decode |> Decode.AsResult.OfParseError.map(geometry);

    Decode.AsResult.OfParseError.(
      field("type", string) |> flatMap(decodeFromField)
    );
  };

  let encodeFields =
    fun
    | Geometry(geo) => Geometry.encodeFields(geo)
    | Feature(feature) => Feature.encodeFields(feature)
    | GeometryCollection(geos) => [
        ("type", Js.Json.string("GeometryCollection")),
        (
          "geometries",
          Js.Json.array(Array.(fromList(geos) |> map(Geometry.encode))),
        ),
      ]
    | FeatureCollection(features) => [
        ("type", Js.Json.string("FeatureCollection")),
        (
          "features",
          Js.Json.array(Array.(fromList(features) |> map(Feature.encode))),
        ),
      ];
};

type t = {
  data: Data.t,
  boundingBox: option(BoundingBox.t),
};

let make = (data, boundingBox) => {data, boundingBox};
let makeLabels = (~data, ~boundingBox=?, ()) => make(data, boundingBox);

let fromGeometry = data => make(Data.Geometry(data), None);
let fromGeometries = data => make(Data.GeometryCollection(data), None);
let fromFeature = data => make(Data.Feature(data), None);
let fromFeatures = data => make(Data.FeatureCollection(data), None);

let getPoints = ({data}) => Data.getPoints(data);
let getLines = ({data}) => Data.getLines(data);
let getPolygons = ({data}) => Data.getPolygons(data);

let decode =
  Decode.AsResult.OfParseError.(
    map2(make, Data.decode, optionalField("bbox", BoundingBox.decode))
  );

let encode = ({data, boundingBox}) => {
  let dataFields = Data.encodeFields(data);
  let fields =
    boundingBox
    |> Option.map(bbox => ("bbox", BoundingBox.encode(bbox)))
    |> Option.fold(dataFields, bbox => [bbox, ...dataFields]);

  Js.(Dict.fromList(fields) |> Json.object_);
};

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

  let getGeometryObjects = (getInner, data) => {
    let fromFeature = feature =>
      Option.fold([], getInner, feature.Feature.geometry);
    switch (data) {
    | Geometry(geo) => getInner(geo)
    | Feature(feature) => fromFeature(feature)
    | GeometryCollection(geos) => List.flatMap(getInner, geos)
    | FeatureCollection(features) => List.flatMap(fromFeature, features)
    };
  };

  let getPoints = getGeometryObjects(Geometry.getPoints);
  let getLines = getGeometryObjects(Geometry.getLines);
  let getPolygons = getGeometryObjects(Geometry.getPolygons);

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
          Js.Json.array(List.(map(Geometry.encode, geos) |> toArray)),
        ),
      ]
    | FeatureCollection(features) => [
        ("type", Js.Json.string("FeatureCollection")),
        (
          "features",
          Js.Json.array(List.(map(Feature.encode, features) |> toArray)),
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

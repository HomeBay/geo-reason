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

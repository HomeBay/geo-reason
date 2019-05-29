open Relude.Globals;
module BoundingBox = GeoJSON_BoundingBox;
module Geometry = GeoJSON_Geometry;
module Feature = GeoJSON_Feature;

module Data = {
  type t =
    | Geometry(Geometry.t)
    | Feature(Feature.t)
    | GeometryCollection(list(Geometry.t));

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
      ];
};

type t = {
  data: Data.t,
  boundingBox: option(BoundingBox.t),
};

let make = (data, boundingBox) => {data, boundingBox};
let makeLabels = (~data, ~boundingBox=?, ()) => make(data, boundingBox);
let makeGeometry = geo => make(Data.Geometry(geo), None);

let encode = ({data, boundingBox}) => {
  let dataFields = Data.encodeFields(data);
  let fields =
    boundingBox
    |> Option.map(bbox => ("bbox", BoundingBox.encode(bbox)))
    |> Option.fold(dataFields, bbox => [bbox, ...dataFields]);

  Js.(Dict.fromList(fields) |> Json.object_);
};

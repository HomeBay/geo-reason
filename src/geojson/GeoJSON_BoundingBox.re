open Relude.Globals;

module BoundingBox2D = {
  type t = {
    n: float,
    e: float,
    s: float,
    w: float,
  };

  let make = (n, e, s, w) => {n, e, s, w};
  let makeLabels = (~n, ~e, ~s, ~w) => make(n, e, s, w);

  let ne = ({n, e}) =>
    GeoJSON_Position.LatLong.makeLabels(~latitude=n, ~longitude=e);

  let se = ({s, e}) =>
    GeoJSON_Position.LatLong.makeLabels(~latitude=s, ~longitude=e);

  let nw = ({n, w}) =>
    GeoJSON_Position.LatLong.makeLabels(~latitude=n, ~longitude=w);

  let sw = ({s, w}) =>
    GeoJSON_Position.LatLong.makeLabels(~latitude=s, ~longitude=w);
};

module AltitudeRange = {
  type t = {
    lowest: float,
    highest: float,
  };

  /**
   * `make` sets the lesser value to `lowest` and the greater value to
   * `highest`, so the order of arguments doesn't matter.
   */
  let make = (a, b) =>
    a > b ? {lowest: b, highest: a} : {lowest: a, highest: b};

  let eq = (a, b) => a.lowest == b.lowest && a.highest == b.highest;
};

type t = {
  bounds: BoundingBox2D.t,
  altitude: option(AltitudeRange.t),
};

let make = (bounds, altitude) => {bounds, altitude};
let makeLabels = (~n, ~e, ~s, ~w, ~altitude=?, ()) =>
  make(BoundingBox2D.{n, e, s, w}, altitude);

let from2D = make(_, None);

let fromArray = xs =>
  switch (Array.toList(xs)) {
  | [w, s, e, n] => Some(makeLabels(~n, ~e, ~s, ~w, ()))
  | [w, s, high, e, n, low] =>
    let bounds = BoundingBox2D.{n, e, s, w};
    let altitude = Some(AltitudeRange.make(high, low));
    Some(make(bounds, altitude));
  | _ => None
  };

let decode =
  Decode.AsOption.(array(floatFromNumber) |> flatMap(fromArray >> const));

let toArray = ({bounds, altitude}) => {
  let high = Option.map(v => v.AltitudeRange.highest, altitude);
  let low = Option.map(v => v.AltitudeRange.lowest, altitude);

  let (swPoint, nePoint) = BoundingBox2D.(sw(bounds), ne(bounds));
  let sw = GeoJSON_Position.make(swPoint, high) |> GeoJSON_Position.toArray;
  let ne = GeoJSON_Position.make(nePoint, low) |> GeoJSON_Position.toArray;

  Array.concat(sw, ne);
};

let encode = box => toArray(box) |> Js.Json.(Array.map(number) >> array);

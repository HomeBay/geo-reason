open Relude.Globals;
/**
 * Position is the most basic construct in GeoJSON. It represents a specific
 * point in longitude/latitude, optionally including altitude.
 *
 * https://tools.ietf.org/html/rfc7946#section-3.1.1
 */
module LatLong = {
  /**
  * A two-dimensional Position includes only latitude and longitude, but no
  * altitude (or elevation). This is not a "real" type in GeoJSON, but in many
  * cases it's useful to work with positions that are known to be 2D.
  */
  type t = {
    latitude: float,
    longitude: float,
  };

  let make = (latitude, longitude) => {latitude, longitude};
  let makeLabels = (~latitude, ~longitude) => make(latitude, longitude);
  let eq = (a, b) => a.latitude == b.latitude && a.longitude == b.longitude;
  let toArray = ({latitude, longitude}) => [|longitude, latitude|];
};

type t = {
  latlong: LatLong.t,
  altitude: option(float),
};

let make = (latlong, altitude) => {latlong, altitude};
let makeLabels = (~latitude, ~longitude, ~altitude=?, ()) =>
  make(LatLong.make(latitude, longitude), altitude);
let fromLatLong = make(_, None);

let fromList =
  fun
  | [longitude, latitude] => Some(makeLabels(~latitude, ~longitude, ()))
  | [longitude, latitude, altitude] =>
    Some(makeLabels(~latitude, ~longitude, ~altitude, ()))
  | _ => None;

let fromArray = xs => Array.toList(xs) |> fromList;

let decode =
  Decode.AsResult.OfParseError.(
    array(floatFromNumber)
    |> map(fromArray)
    |> flatMap((v, json) =>
         Result.fromOption(ParseError.Val(`ExpectedTuple(2), json), v)
       )
  );

let toArray = ({latlong, altitude}) => {
  let base = LatLong.toArray(latlong);
  Option.fold(base, Array.append(_, base), altitude);
};

let encode = data => toArray(data) |> Js.Json.(Array.map(number) >> array);

let eq = (a, b) =>
  LatLong.eq(a.latlong, b.latlong)
  && Option.eqBy((x, y) => x == y, a.altitude, b.altitude);

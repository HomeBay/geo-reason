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

  let fromLatLong = point =>
    GeoJSON_Position.LatLong.{
      n: point.latitude,
      e: point.longitude,
      s: point.latitude,
      w: point.longitude,
    };

  let ne = ({n, e}) =>
    GeoJSON_Position.LatLong.makeLabels(~latitude=n, ~longitude=e);

  let se = ({s, e}) =>
    GeoJSON_Position.LatLong.makeLabels(~latitude=s, ~longitude=e);

  let nw = ({n, w}) =>
    GeoJSON_Position.LatLong.makeLabels(~latitude=n, ~longitude=w);

  let sw = ({s, w}) =>
    GeoJSON_Position.LatLong.makeLabels(~latitude=s, ~longitude=w);

  /**
   * Given two 2D bounding boxes, return a new bounding box that contains both
   */
  let expand = (a, b) => {
    n: Js.Math.max_float(a.n, b.n),
    e: Js.Math.max_float(a.e, b.e),
    s: Js.Math.min_float(a.s, b.s),
    w: Js.Math.min_float(a.w, b.w),
  };

  /**
   * Given a 2D bounding box and a LatLong point, ensure that the bounding box
   * contains the point, expanding if necessary.
   */
  let expandFromPoint = (bounds, point) =>
    expand(bounds, fromLatLong(point));

  /**
   * Given at least one lat long point, and possibly more, return a 2D Bounding
   * Box containing all the points.
   */
  let fromManyPoints = (first, rest) =>
    List.foldLeft(expandFromPoint, fromLatLong(first), rest);

  /**
   * Given a 2D bounding box, return the LatLong point representing its center.
   */
  let center = ({n, e, s, w}) =>
    GeoJSON_Position.LatLong.makeLabels(
      ~latitude=(n +. s) /. 2.,
      ~longitude=(e +. w) /. 2.,
    );

  let centerLat = bounds => center(bounds).GeoJSON_Position.LatLong.latitude;
  let centerLon = bounds => center(bounds).GeoJSON_Position.LatLong.longitude;
  let vsize = ({n, s}) => abs_float(n -. s);
  let hsize = ({e, w}) => abs_float(e -. w);

  /**
 * Get the size in degrees (float) of the longest side of the bounding box
 */
  let maxSideSize = bounds =>
    Js.Math.max_float(vsize(bounds), hsize(bounds));

  /**
 * Get the size in degrees (float) of the shortest side of the bounding box
 */
  let minSideSize = bounds =>
    Js.Math.min_float(vsize(bounds), hsize(bounds));

  /**
   * Given degrees (as a float), ensure that each side of the box is at least that
   * long, while remaining centered around the same point.
   */
  let ensureMinSize = (deg, bounds) => {
    let halfSide = deg /. 2.;
    let {n, e, s, w} = fromLatLong(center(bounds));
    let minBox = {
      n: n +. halfSide,
      s: s -. halfSide,
      e: e +. halfSide,
      w: w -. halfSide,
    };

    expand(bounds, minBox);
  };

  /**
   * Grow a bounding box by adding a percent-based padding to all sides. Padding
   * of `0` is 0% and 1 is 100%. Negative padding is allowed.
   */
  let pad = (amt: float, {n, e, s, w} as bounds) => {
    let p = maxSideSize(bounds) *. amt;
    {n: n +. p, e: e +. p, s: s -. p, w: w -. p};
  };

  let wrap = ({n, e, s, w}: t): t =>
    makeLabels(
      ~n,
      ~e=e > 180. ? e -. 360. : e,
      ~s,
      ~w=w < (-180.) ? w +. 360. : w,
    );
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

let fromArray = (xs, json) =>
  switch (Array.toList(xs)) {
  | [w, s, e, n] => Result.ok(makeLabels(~n, ~e, ~s, ~w, ()))
  | [w, s, high, e, n, low] =>
    let bounds = BoundingBox2D.{n, e, s, w};
    let altitude = Some(AltitudeRange.make(high, low));
    Result.ok(make(bounds, altitude));
  | _ => Result.error(Decode.ParseError.Val(`ExpectedTuple(4), json))
  };

let decode =
  Decode.AsResult.OfParseError.(
    array(floatFromNumber) |> flatMap(fromArray)
  );

let toArray = ({bounds, altitude}) => {
  let high = Option.map(v => v.AltitudeRange.highest, altitude);
  let low = Option.map(v => v.AltitudeRange.lowest, altitude);

  let (swPoint, nePoint) = BoundingBox2D.(sw(bounds), ne(bounds));
  let sw = GeoJSON_Position.make(swPoint, high) |> GeoJSON_Position.toArray;
  let ne = GeoJSON_Position.make(nePoint, low) |> GeoJSON_Position.toArray;

  Array.concat(sw, ne);
};

let encode = box => toArray(box) |> Js.Json.(Array.map(number) >> array);

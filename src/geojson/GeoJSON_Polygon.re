open Relude.Globals;

module Shape = {
  type t = {
    startEnd: GeoJSON_Position.t,
    second: GeoJSON_Position.t,
    third: GeoJSON_Position.t,
    rest: list(GeoJSON_Position.t),
  };

  let make = (startEnd, second, third, rest) => {
    startEnd,
    second,
    third,
    rest,
  };

  let makeLabels = (~startEnd, ~second, ~third, ~rest=[], ()) =>
    make(startEnd, second, third, rest);

  let encode = ({startEnd, second, third, rest}) =>
    Array.(
      concat([|startEnd, second, third|], fromList(rest))
      |> append(startEnd)
      |> map(GeoJSON_Position.encode)
      |> Js.Json.array
    );
};

type t =
  | Shape(Shape.t)
  | LinearRing(list(Shape.t));

let makeShape = (~startEnd, ~second, ~third, ~rest=?, ()) =>
  Shape(Shape.makeLabels(~startEnd, ~second, ~third, ~rest?, ()));

let toArray =
  fun
  | Shape(s) => [|s|]
  | LinearRing(l) => Array.fromList(l);

let encode = toArray >> Array.map(Shape.encode) >> Js.Json.array;

open Relude.Globals;

type t = {
  first: GeoJSON_Position.t,
  second: GeoJSON_Position.t,
  rest: list(GeoJSON_Position.t),
};

let make = (first, second, rest) => {first, second, rest};
let makeLabels = (~first, ~second, ~rest=[], ()) =>
  make(first, second, rest);

let twoPoints = (first, second) => make(first, second, []);

let fromArray = xs =>
  switch (Array.toList(xs)) {
  | [first, second, ...rest] => Some(make(first, second, rest))
  | _ => None
  };

let decode =
  Decode.AsOption.(
    array(GeoJSON_Position.decode) |> flatMap(fromArray >> const)
  );

let toList = ({first, second, rest}) => [first, second, ...rest];
let toArray = v => toList(v) |> Array.fromList;

// TODO:
// let eq = (a, b) => List.eqBy(GeoJSON_Position.eq, toList(a), toList(b))

let encode = data =>
  toArray(data) |> Array.map(GeoJSON_Position.encode) |> Js.Json.array;

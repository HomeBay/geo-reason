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

let fromArray = (xs, json) =>
  switch (Array.toList(xs)) {
  | [first, second, ...rest] => Result.ok(make(first, second, rest))
  | _ => Result.error(Decode.ParseError.Val(`ExpectedValidOption, json))
  };

let decode =
  Decode.AsResult.OfParseError.(
    array(GeoJSON_Position.decode) |> flatMap(fromArray)
  );

let toList = ({first, second, rest}) => [first, second, ...rest];
let toArray = v => toList(v) |> Array.fromList;

let encode = data =>
  toArray(data) |> Array.map(GeoJSON_Position.encode) |> Js.Json.array;

let eq = (a, b) => List.eqBy(GeoJSON_Position.eq, toList(a), toList(b));

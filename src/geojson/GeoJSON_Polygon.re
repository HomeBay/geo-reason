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

  let fromArray = (xs, json) => {
    let firstLastMatch =
      Option.eqBy(GeoJSON_Position.eq, Array.head(xs), Array.last(xs));

    switch (Array.init(xs) |> Option.getOrElse([||]) |> Array.toList) {
    | [startEnd, second, third, ...rest] when firstLastMatch =>
      Result.ok(makeLabels(~startEnd, ~second, ~third, ~rest, ()))
    | _ => Result.error(Decode.ParseError.Val(`ExpectedValidOption, json))
    };
  };

  let decode =
    Decode.AsResult.OfParseError.(
      array(GeoJSON_Position.decode) |> flatMap(fromArray)
    );

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

let fromList =
  fun
  | [] =>
    Result.error(
      Decode.ParseError.Val(`ExpectedValidOption, Js.Json.array([||])),
    )
  | [x] => Result.ok(Shape(x))
  | xs => Result.ok(LinearRing(xs));

let fromArray = xs => fromList(Array.toList(xs));

let decode =
  Decode.AsResult.OfParseError.(
    array(Shape.decode) |> flatMap(fromArray >> const)
  );

let toArray =
  fun
  | Shape(s) => [|s|]
  | LinearRing(l) => Array.fromList(l);

let encode = toArray >> Array.map(Shape.encode) >> Js.Json.array;

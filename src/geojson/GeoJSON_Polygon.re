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

  let fromArray = xs => {
    let firstLastMatch =
      Option.eqBy(GeoJSON_Position.eq, Array.head(xs), Array.last(xs));

    switch (Array.init(xs) |> Option.getOrElse([||]) |> Array.toList) {
    | [startEnd, second, third, ...rest] when firstLastMatch =>
      Some(makeLabels(~startEnd, ~second, ~third, ~rest, ()))
    | _ => None
    };
  };

  let fromList = xs => Array.fromList(xs) |> fromArray;

  let decode =
    Decode.AsResult.OfParseError.(
      array(GeoJSON_Position.decode)
      |> map(fromArray)
      |> flatMap((v, json) =>
           Result.fromOption(ParseError.Val(`ExpectedTuple(4), json), v)
         )
    );

  let encode = ({startEnd, second, third, rest}) =>
    Array.concat([|startEnd, second, third|], Array.fromList(rest))
    |> Array.append(startEnd)
    |> Array.map(GeoJSON_Position.encode)
    |> Js.Json.array;
};

type t =
  | Shape(Shape.t)
  | LinearRing(Shape.t, Shape.t, list(Shape.t));

let makeShape = (~startEnd, ~second, ~third, ~rest=?, ()) =>
  Shape(Shape.makeLabels(~startEnd, ~second, ~third, ~rest?, ()));

let makeLinearRing = (~shape, ~hole, ~holes) =>
  LinearRing(shape, hole, holes);

let fromList =
  fun
  | [] => None
  | [x] => Some(Shape(x))
  | [shape, hole, ...holes] => Some(makeLinearRing(~shape, ~hole, ~holes));

let fromArray = xs => fromList(Array.toList(xs));

let decode =
  Decode.AsResult.OfParseError.(
    array(Shape.decode)
    |> map(fromArray)
    |> flatMap((v, json) =>
         Result.fromOption(ParseError.Val(`ExpectedTuple(1), json), v)
       )
  );

let toArray =
  fun
  | Shape(s) => [|s|]
  | LinearRing(shape, hole, holes) =>
    Array.fromList([shape, hole, ...holes]);

let encode = toArray >> Array.map(Shape.encode) >> Js.Json.array;

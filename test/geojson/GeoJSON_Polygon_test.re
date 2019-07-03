open Jest;
open Expect;
open Relude.Globals;

module Polygon = GeoJSON.Geometry.Polygon;
module Position = GeoJSON.Geometry.Position;

describe("Polygon", () => {
  let point1 = Position.LatLong.make(0.0, 100.0) |> Position.fromLatLong;
  let point2 = Position.LatLong.make(0.0, 101.0) |> Position.fromLatLong;
  let point3 = Position.LatLong.make(1.0, 101.0) |> Position.fromLatLong;
  let point4 = Position.LatLong.make(1.0, 100.0) |> Position.fromLatLong;

  let triangle =
    Polygon.makeShape(~startEnd=point1, ~second=point2, ~third=point3, ());

  let square =
    Polygon.Shape.makeLabels(
      ~startEnd=point1,
      ~second=point2,
      ~third=point3,
      ~rest=[point4],
      (),
    );

  let squareFromList =
    Polygon.Shape.fromList([point1, point2, point3, point4, point1]);

  let squarePoly =
    Polygon.makeShape(
      ~startEnd=point1,
      ~second=point2,
      ~third=point3,
      ~rest=[point4],
      (),
    );

  let nestedSquares =
    Polygon.makeLinearRing(
      ~shape=square,
      ~hole=
        Polygon.Shape.makeLabels(
          ~startEnd=Position.makeLabels(~latitude=0.8, ~longitude=100.8, ()),
          ~second=Position.makeLabels(~latitude=0.2, ~longitude=100.8, ()),
          ~third=Position.makeLabels(~latitude=0.2, ~longitude=100.2, ()),
          ~rest=[Position.makeLabels(~latitude=0.8, ~longitude=100.2, ())],
          (),
        ),
      ~holes=[],
    );

  let onePointJson = [%raw {| [[[100.0, 0.0]]]|}];
  let twoPointJson = [%raw {| [[[100.0, 0.0], [100.0, 0.0]]]|}];
  let threePointJson = [%raw
    {| [[[100.0, 0.0], [101.0, 0.0], [100.0, 0.0]]]|}
  ];
  let triangleJson = [%raw
    {| [[[100.0, 0.0], [101.0, 0.0], [101.0, 1.0], [100.0, 0.0]]]|}
  ];

  let nestedSquaresJson = [%raw
    {|
      [
        [[100.0, 0.0], [101.0, 0.0], [101.0, 1.0], [100.0, 1.0], [100.0, 0.0]],
        [[100.8, 0.8], [100.8, 0.2], [100.2, 0.2], [100.2, 0.8], [100.8, 0.8]]
      ]
    |}
  ];

  test("makeShape (three points)", () =>
    expect(triangle)
    |> toEqual(
         Polygon.Shape(
           Polygon.Shape.{
             startEnd: point1,
             second: point2,
             third: point3,
             rest: [],
           },
         ),
       )
  );

  test("makeShape (four points)", () =>
    expect(Polygon.Shape(square)) |> toEqual(squarePoly)
  );

  test("Shape.make (four points)", () =>
    expect(Polygon.Shape(square))
    |> toEqual(
         Polygon.Shape(
           Polygon.Shape.{
             startEnd: point1,
             second: point2,
             third: point3,
             rest: [point4],
           },
         ),
       )
  );

  test("Shape.fromList", () =>
    expect(squareFromList) |> toEqual(Some(square))
  );

  test("getOuterShape (shape)", () =>
    expect(Polygon.getOuterShape(Polygon.Shape(square))) |> toEqual(square)
  );

  test("getOuterShape (linear ring)", () =>
    expect(Polygon.getOuterShape(nestedSquares)) |> toEqual(square)
  );

  test("decode success (triangle)", () =>
    expect(Polygon.decode(triangleJson)) |> toEqual(Result.ok(triangle))
  );

  test("decode success (square)", () =>
    expect(Polygon.decode(nestedSquaresJson))
    |> toEqual(Result.ok(nestedSquares))
  );

  test("decode failure (empty)", () =>
    expect(Polygon.decode(Js.Json.array([||])) |> Result.getOk)
    |> toEqual(None)
  );

  test("decode failure (one point)", () =>
    expect(Polygon.decode(onePointJson) |> Result.getOk) |> toEqual(None)
  );

  test("decode failure (two point)", () =>
    expect(Polygon.decode(twoPointJson) |> Result.getOk) |> toEqual(None)
  );

  test("decode failure (three point)", () =>
    expect(Polygon.decode(threePointJson) |> Result.getOk) |> toEqual(None)
  );

  test("encode (three points)", () =>
    expect(Polygon.encode(triangle)) |> toEqual(triangleJson)
  );

  test("encode (square with inner square)", () =>
    expect(Polygon.encode(nestedSquares)) |> toEqual(nestedSquaresJson)
  );
});

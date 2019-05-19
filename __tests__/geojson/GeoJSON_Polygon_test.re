open Jest;
open Expect;

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

  let squarePoly =
    Polygon.makeShape(
      ~startEnd=point1,
      ~second=point2,
      ~third=point3,
      ~rest=[point4],
      (),
    );

  let nestedSquares =
    Polygon.LinearRing([
      square,
      Polygon.Shape.makeLabels(
        ~startEnd=Position.makeLabels(~latitude=0.8, ~longitude=100.8, ()),
        ~second=Position.makeLabels(~latitude=0.2, ~longitude=100.8, ()),
        ~third=Position.makeLabels(~latitude=0.2, ~longitude=100.2, ()),
        ~rest=[Position.makeLabels(~latitude=0.8, ~longitude=100.2, ())],
        (),
      ),
    ]);

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

  test("encode (three points)", () =>
    expect(Polygon.encode(triangle)) |> toEqual(triangleJson)
  );

  test("encode (square with inner square)", () =>
    expect(Polygon.encode(nestedSquares)) |> toEqual(nestedSquaresJson)
  );
});

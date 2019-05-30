open Jest;
open Expect;
open Relude.Globals;

module Geometry = GeoJSON.Geometry;
module Sample = GeoJSON_SampleData;

describe("Geometry", () => {
  test("pointFromLatLong", () =>
    expect(Geometry.pointFromLatLong(Sample.latLong))
    |> toEqual(Geometry.(Point(Position.fromLatLong(Sample.latLong))))
  );

  test("polygonFromShape", () =>
    expect(Geometry.polygonFromShape(Sample.shape))
    |> toEqual(Geometry.(Polygon(Polygon.Shape(Sample.shape))))
  );

  test("decode (point)", () =>
    expect(Geometry.decode(Sample.pointJson))
    |> toEqual(Result.ok(Sample.point))
  );

  test("decode (line)", () =>
    expect(Geometry.decode(Sample.lineJson))
    |> toEqual(Result.ok(Sample.line))
  );

  test("decode (polygon)", () =>
    expect(Geometry.decode(Sample.polygonJson))
    |> toEqual(Result.ok(Sample.polygon))
  );

  test("decode (polygon holes)", () =>
    expect(Geometry.decode(Sample.polygonHolesJson))
    |> toEqual(Result.ok(Sample.polygonHoles))
  );

  test("decode (multi-point)", () =>
    expect(Geometry.decode(Sample.multiPointJson))
    |> toEqual(Result.ok(Sample.multiPoint))
  );

  test("decode (multi-line)", () =>
    expect(Geometry.decode(Sample.multiLineJson))
    |> toEqual(Result.ok(Sample.multiLine))
  );

  test("decode (multi-polygon)", () =>
    expect(Geometry.decode(Sample.multiPolygonJson))
    |> toEqual(Result.ok(Sample.multiPolygon))
  );

  test("decode (invalid type)", () =>
    expect(Geometry.decode(Sample.invalidGeometry))
    |> toEqual(
         Result.error(
           Decode.ParseError.Val(
             `ExpectedValidOption,
             Sample.invalidGeometry,
           ),
         ),
       )
  );

  test("encode (point)", () =>
    expect(Geometry.encode(Sample.point))
    |> toEqual(Sample.pointJson)
  );

  test("encode (line)", () =>
    expect(Geometry.encode(Sample.line)) |> toEqual(Sample.lineJson)
  );

  test("encode (polygon)", () =>
    expect(Geometry.encode(Sample.polygon))
    |> toEqual(Sample.polygonJson)
  );

  test("encode (polygon holes)", () =>
    expect(Geometry.encode(Sample.polygonHoles))
    |> toEqual(Sample.polygonHolesJson)
  );

  test("encode (multi-point)", () =>
    expect(Geometry.encode(Sample.multiPoint))
    |> toEqual(Sample.multiPointJson)
  );

  test("encode (multi-line)", () =>
    expect(Geometry.encode(Sample.multiLine))
    |> toEqual(Sample.multiLineJson)
  );

  test("encode (multi-polygon)", () =>
    expect(Geometry.encode(Sample.multiPolygon))
    |> toEqual(Sample.multiPolygonJson)
  );
});

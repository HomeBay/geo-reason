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

  test("eq (two matching points)", () =>
    expect(Geometry.eq(Sample.pointGeometry, Sample.pointGeometry))
    |> toEqual(true)
  );

  test("eq (two matching lines)", () =>
    expect(Geometry.eq(Sample.lineGeometry, Sample.lineGeometry))
    |> toEqual(true)
  );

  test("eq (two matching polygons)", () =>
    expect(
      Geometry.eq(Sample.polygonHolesGeometry, Sample.polygonHolesGeometry),
    )
    |> toEqual(true)
  );

  test("eq (two matching multi-points)", () =>
    expect(Geometry.eq(Sample.multiPoint, Sample.multiPoint))
    |> toEqual(true)
  );

  test("eq (two matching multi-lines)", () =>
    expect(Geometry.eq(Sample.multiLine, Sample.multiLine)) |> toEqual(true)
  );

  test("eq (two matching multi-polygons)", () =>
    expect(
      Geometry.eq(Sample.multiPolygonGeometry, Sample.multiPolygonGeometry),
    )
    |> toEqual(true)
  );

  test("eq (point, multi-point)", () =>
    expect(Geometry.eq(Sample.pointGeometry, Sample.multiPoint))
    |> toEqual(false)
  );

  test("eq (line, multi-line)", () =>
    expect(Geometry.eq(Sample.lineGeometry, Sample.multiLine))
    |> toEqual(false)
  );

  test("eq (polygon, multi-polygon)", () =>
    expect(
      Geometry.eq(Sample.polygonHolesGeometry, Sample.multiPolygonGeometry),
    )
    |> toEqual(false)
  );

  test("eq (multi-point, point)", () =>
    expect(Geometry.eq(Sample.multiPoint, Sample.pointGeometry))
    |> toEqual(false)
  );

  test("eq (multi-line, line)", () =>
    expect(Geometry.eq(Sample.multiLine, Sample.lineGeometry))
    |> toEqual(false)
  );

  test("eq (multi-polygon, polygon)", () =>
    expect(
      Geometry.eq(Sample.multiPolygonGeometry, Sample.polygonHolesGeometry),
    )
    |> toEqual(false)
  );

  test("decode (point)", () =>
    expect(Geometry.decode(Sample.pointJson))
    |> toEqual(Result.ok(Sample.pointGeometry))
  );

  test("decode (line)", () =>
    expect(Geometry.decode(Sample.lineJson))
    |> toEqual(Result.ok(Sample.lineGeometry))
  );

  test("decode (polygon)", () =>
    expect(Geometry.decode(Sample.polygonJson))
    |> toEqual(Result.ok(Sample.polygon))
  );

  test("decode (polygon holes)", () =>
    expect(Geometry.decode(Sample.polygonHolesJson))
    |> toEqual(Result.ok(Sample.polygonHolesGeometry))
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
    |> toEqual(Result.ok(Sample.multiPolygonGeometry))
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
    expect(Geometry.encode(Sample.pointGeometry))
    |> toEqual(Sample.pointJson)
  );

  test("encode (line)", () =>
    expect(Geometry.encode(Sample.lineGeometry)) |> toEqual(Sample.lineJson)
  );

  test("encode (polygon)", () =>
    expect(Geometry.encode(Sample.polygon)) |> toEqual(Sample.polygonJson)
  );

  test("encode (polygon holes)", () =>
    expect(Geometry.encode(Sample.polygonHolesGeometry))
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
    expect(Geometry.encode(Sample.multiPolygonGeometry))
    |> toEqual(Sample.multiPolygonJson)
  );
});

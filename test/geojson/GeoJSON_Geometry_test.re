open Jest;
open Expect;
open Relude.Globals;

module Geometry = GeoJSON.Geometry;
module SampleJson = GeoJSON_SampleJson;

describe("Geometry", () => {
  test("decode (point)", () =>
    expect(Geometry.decode(SampleJson.pointJson))
    |> toEqual(Result.ok(SampleJson.point))
  );

  test("decode (line)", () =>
    expect(Geometry.decode(SampleJson.lineJson))
    |> toEqual(Result.ok(SampleJson.line))
  );

  test("decode (polygon)", () =>
    expect(Geometry.decode(SampleJson.polygonJson))
    |> toEqual(Result.ok(SampleJson.polygon))
  );

  test("decode (polygon holes)", () =>
    expect(Geometry.decode(SampleJson.polygonHolesJson))
    |> toEqual(Result.ok(SampleJson.polygonHoles))
  );

  test("decode (multi-point)", () =>
    expect(Geometry.decode(SampleJson.multiPointJson))
    |> toEqual(Result.ok(SampleJson.multiPoint))
  );

  test("decode (multi-line)", () =>
    expect(Geometry.decode(SampleJson.multiLineJson))
    |> toEqual(Result.ok(SampleJson.multiLine))
  );

  test("decode (multi-polygon)", () =>
    expect(Geometry.decode(SampleJson.multiPolygonJson))
    |> toEqual(Result.ok(SampleJson.multiPolygon))
  );

  test("decode (invalid type)", () =>
    expect(Geometry.decode(SampleJson.invalidGeometry))
    |> toEqual(
         Result.error(
           Decode.ParseError.Val(
             `ExpectedValidOption,
             SampleJson.invalidGeometry,
           ),
         ),
       )
  );

  test("encode (point)", () =>
    expect(Geometry.encode(SampleJson.point))
    |> toEqual(SampleJson.pointJson)
  );

  test("encode (line)", () =>
    expect(Geometry.encode(SampleJson.line)) |> toEqual(SampleJson.lineJson)
  );

  test("encode (polygon)", () =>
    expect(Geometry.encode(SampleJson.polygon))
    |> toEqual(SampleJson.polygonJson)
  );

  test("encode (polygon holes)", () =>
    expect(Geometry.encode(SampleJson.polygonHoles))
    |> toEqual(SampleJson.polygonHolesJson)
  );

  test("encode (multi-point)", () =>
    expect(Geometry.encode(SampleJson.multiPoint))
    |> toEqual(SampleJson.multiPointJson)
  );

  test("encode (multi-line)", () =>
    expect(Geometry.encode(SampleJson.multiLine))
    |> toEqual(SampleJson.multiLineJson)
  );

  test("encode (multi-polygon)", () =>
    expect(Geometry.encode(SampleJson.multiPolygon))
    |> toEqual(SampleJson.multiPolygonJson)
  );
});

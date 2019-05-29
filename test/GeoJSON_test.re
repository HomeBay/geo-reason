open Jest;
open Expect;

module SampleJson = GeoJSON_SampleJson;

describe("GeoJSON", () => {
  test("encode (Point Geometry, no bbox)", () =>
    expect(GeoJSON.(encode(makeGeometry(SampleJson.point))))
    |> toEqual(SampleJson.pointJson)
  );

  test("encode (GeometryCollection, no bbox)", () =>
    expect(GeoJSON.encode(SampleJson.geometryCollection))
    |> toEqual(SampleJson.geometryCollectionJson)
  );

  test("encode (Point Geometry, with bbox)", () =>
    expect(GeoJSON.encode(SampleJson.goeBoundingBox))
    |> toEqual(SampleJson.geoBoundingBoxJson)
  );
});

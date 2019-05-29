open Jest;
open Expect;
open Relude.Globals;

module SampleJson = GeoJSON_SampleJson;

describe("GeoJSON", () => {
  test("decode (Point Geometry, with bbox)", () =>
    expect(GeoJSON.decode(SampleJson.geoBoundingBoxJson))
    |> toEqual(Result.ok(SampleJson.goeBoundingBox))
  );

  test("decode (Feature, no bbox)", () =>
    expect(GeoJSON.decode(SampleJson.featureCompleteJson))
    |> toEqual(
         Result.ok(
           GeoJSON.(
             makeLabels(~data=Data.Feature(SampleJson.featureComplete), ())
           ),
         ),
       )
  );

  test("decode (GeometryCollection, no bbox)", () =>
    expect(GeoJSON.Data.decode(SampleJson.geometryCollectionJson))
    |> toEqual(Result.ok(SampleJson.geometryCollection.GeoJSON.data))
  );

  test("decode (FeatureCollection, no bbox)", () =>
    expect(GeoJSON.Data.decode(SampleJson.featureCollectionJson))
    |> toEqual(Result.ok(SampleJson.featureCollection.GeoJSON.data))
  );

  test("encode (Point Geometry, with bbox)", () =>
    expect(GeoJSON.encode(SampleJson.goeBoundingBox))
    |> toEqual(SampleJson.geoBoundingBoxJson)
  );

  test("encode (Feature, no bbox)", () =>
    expect(
      GeoJSON.(
        encode(make(Data.Feature(SampleJson.featureComplete), None))
      ),
    )
    |> toEqual(SampleJson.featureCompleteJson)
  );

  test("encode (Point Geometry, no bbox)", () =>
    expect(GeoJSON.(encode(make(Data.Geometry(SampleJson.point), None))))
    |> toEqual(SampleJson.pointJson)
  );

  test("encode (GeometryCollection, no bbox)", () =>
    expect(GeoJSON.encode(SampleJson.geometryCollection))
    |> toEqual(SampleJson.geometryCollectionJson)
  );

  test("encode (Feature Collection, no bbox)", () =>
    expect(GeoJSON.encode(SampleJson.featureCollection))
    |> toEqual(SampleJson.featureCollectionJson)
  );
});

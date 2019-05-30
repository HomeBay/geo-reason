open Jest;
open Expect;
open Relude.Globals;

module SampleJson = GeoJSON_SampleJson;

describe("GeoJSON", () => {
  test("fromGeometry", () =>
    expect(GeoJSON.fromGeometry(SampleJson.point))
    |> toEqual(
         GeoJSON.{data: Data.Geometry(SampleJson.point), boundingBox: None},
       )
  );

  test("fromGeometries", () =>
    expect(GeoJSON.fromGeometries([SampleJson.point]))
    |> toEqual(
         GeoJSON.{
           data: Data.GeometryCollection([SampleJson.point]),
           boundingBox: None,
         },
       )
  );

  test("fromFeature", () =>
    expect(GeoJSON.fromFeature(SampleJson.featureComplete))
    |> toEqual(
         GeoJSON.{
           data: Data.Feature(SampleJson.featureComplete),
           boundingBox: None,
         },
       )
  );

  test("fromFeatureCollection", () =>
    expect(
      GeoJSON.fromFeatures([
        SampleJson.featureEmpty,
        SampleJson.featureComplete,
      ]),
    )
    |> toEqual(
         GeoJSON.{
           data:
             Data.FeatureCollection([
               SampleJson.featureEmpty,
               SampleJson.featureComplete,
             ]),
           boundingBox: None,
         },
       )
  );

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

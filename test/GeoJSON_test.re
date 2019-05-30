open Jest;
open Expect;
open Relude.Globals;

module Sample = GeoJSON_SampleData;

describe("GeoJSON", () => {
  test("fromGeometry", () =>
    expect(GeoJSON.fromGeometry(Sample.point))
    |> toEqual(
         GeoJSON.{data: Data.Geometry(Sample.point), boundingBox: None},
       )
  );

  test("fromGeometries", () =>
    expect(GeoJSON.fromGeometries([Sample.point]))
    |> toEqual(
         GeoJSON.{
           data: Data.GeometryCollection([Sample.point]),
           boundingBox: None,
         },
       )
  );

  test("fromFeature", () =>
    expect(GeoJSON.fromFeature(Sample.featureComplete))
    |> toEqual(
         GeoJSON.{
           data: Data.Feature(Sample.featureComplete),
           boundingBox: None,
         },
       )
  );

  test("fromFeatureCollection", () =>
    expect(
      GeoJSON.fromFeatures([Sample.featureEmpty, Sample.featureComplete]),
    )
    |> toEqual(
         GeoJSON.{
           data:
             Data.FeatureCollection([
               Sample.featureEmpty,
               Sample.featureComplete,
             ]),
           boundingBox: None,
         },
       )
  );

  test("decode (Point Geometry, with bbox)", () =>
    expect(GeoJSON.decode(Sample.geoBoundingBoxJson))
    |> toEqual(Result.ok(Sample.goeBoundingBox))
  );

  test("decode (Feature, no bbox)", () =>
    expect(GeoJSON.decode(Sample.featureCompleteJson))
    |> toEqual(
         Result.ok(
           GeoJSON.(
             makeLabels(~data=Data.Feature(Sample.featureComplete), ())
           ),
         ),
       )
  );

  test("decode (GeometryCollection, no bbox)", () =>
    expect(GeoJSON.Data.decode(Sample.geometryCollectionJson))
    |> toEqual(Result.ok(Sample.geometryCollection.GeoJSON.data))
  );

  test("decode (FeatureCollection, no bbox)", () =>
    expect(GeoJSON.Data.decode(Sample.featureCollectionJson))
    |> toEqual(Result.ok(Sample.featureCollection.GeoJSON.data))
  );

  test("encode (Point Geometry, with bbox)", () =>
    expect(GeoJSON.encode(Sample.goeBoundingBox))
    |> toEqual(Sample.geoBoundingBoxJson)
  );

  test("encode (Feature, no bbox)", () =>
    expect(
      GeoJSON.(encode(make(Data.Feature(Sample.featureComplete), None))),
    )
    |> toEqual(Sample.featureCompleteJson)
  );

  test("encode (Point Geometry, no bbox)", () =>
    expect(GeoJSON.(encode(make(Data.Geometry(Sample.point), None))))
    |> toEqual(Sample.pointJson)
  );

  test("encode (GeometryCollection, no bbox)", () =>
    expect(GeoJSON.encode(Sample.geometryCollection))
    |> toEqual(Sample.geometryCollectionJson)
  );

  test("encode (Feature Collection, no bbox)", () =>
    expect(GeoJSON.encode(Sample.featureCollection))
    |> toEqual(Sample.featureCollectionJson)
  );
});

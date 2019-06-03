open Jest;
open Expect;
open Relude.Globals;

module Sample = GeoJSON_SampleData;

describe("GeoJSON", () => {
  test("fromGeometry", () =>
    expect(GeoJSON.fromGeometry(Sample.pointGeometry))
    |> toEqual(
         GeoJSON.{
           data: Data.Geometry(Sample.pointGeometry),
           boundingBox: None,
         },
       )
  );

  test("fromGeometries", () =>
    expect(GeoJSON.fromGeometries([Sample.pointGeometry]))
    |> toEqual(
         GeoJSON.{
           data: Data.GeometryCollection([Sample.pointGeometry]),
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

  test("getPoints (from MultiPoint Geometry)", () =>
    expect(GeoJSON.(fromGeometry(Sample.multiPoint) |> getPoints))
    |> toEqual([Sample.multiPointFirst, Sample.multiPointSecond])
  );

  test("getPoints (from Point Geometry)", () =>
    expect(GeoJSON.(Data.getPoints(Sample.pointGeoJson.data)))
    |> toEqual([Sample.pointPosition])
  );

  test("getPoints (from Line Feature)", () =>
    expect(GeoJSON.getPoints(Sample.lineFeatureGeoJSON)) |> toEqual([])
  );

  test("getLines (from Line Feature)", () =>
    expect(GeoJSON.getLines(Sample.lineFeatureGeoJSON))
    |> toEqual([Sample.line])
  );

  test("getLines (from MultiLine Geometry)", () =>
    expect(
      GeoJSON.(
        Geometry.multiLineString([Sample.line]) |> fromGeometry |> getLines
      ),
    )
    |> toEqual([Sample.line])
  );

  test("getLines (from Point FeatureCollection)", () =>
    expect(GeoJSON.(Data.getLines(Sample.pointFeatureCollection.data)))
    |> toEqual([])
  );

  test("getPolygons (from Polygon GeometryCollection)", () =>
    expect(GeoJSON.getPolygons(Sample.polygonGeometryCollection))
    |> toEqual([Sample.polygonHoles])
  );

  test("getPolygons (from MultiPolygon Geometry)", () =>
    expect(GeoJSON.(getPolygons(fromGeometry(Sample.multiPolygonGeometry))))
    |> toEqual(Sample.polygons)
  );

  test("getPolygons (from Point Geometry)", () =>
    expect(GeoJSON.(Data.getPolygons(Sample.pointGeoJson.data)))
    |> toEqual([])
  );

  test("decode (Point Geometry, with bbox)", () =>
    expect(GeoJSON.decode(Sample.geoBoundingBoxJson))
    |> toEqual(Result.ok(Sample.geoBoundingBox))
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
    expect(GeoJSON.encode(Sample.geoBoundingBox))
    |> toEqual(Sample.geoBoundingBoxJson)
  );

  test("encode (Feature, no bbox)", () =>
    expect(
      GeoJSON.(encode(make(Data.Feature(Sample.featureComplete), None))),
    )
    |> toEqual(Sample.featureCompleteJson)
  );

  test("encode (Point Geometry, no bbox)", () =>
    expect(GeoJSON.encode(Sample.pointGeoJson)) |> toEqual(Sample.pointJson)
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

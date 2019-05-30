open Jest;
open Expect;
open Relude.Globals;

module Feature = GeoJSON.Feature;
module Sample = GeoJSON_SampleData;

describe("Feature", () => {
  test("fromGeometry", () =>
    expect(Feature.fromGeometry(Sample.point))
    |> toEqual(
         Feature.{id: None, geometry: Some(Sample.point), properties: None},
       )
  );

  test("decode (empty)", () =>
    expect(Feature.decode(Sample.featureEmptySparseJson))
    |> toEqual(Result.ok(Sample.featureEmpty))
  );

  test("decode (no properties)", () =>
    expect(Feature.decode(Sample.featureNoPropertiesJson))
    |> toEqual(Result.ok(Sample.featureNoProperties))
  );

  test("decode (with properties, numeric id)", () =>
    expect(Feature.decode(Sample.featureCompleteJson))
    |> toEqual(Result.ok(Sample.featureComplete))
  );

  test("encode (empty)", () =>
    expect(Feature.encode(Sample.featureEmpty))
    |> toEqual(Sample.featureEmptyJson)
  );

  test("encode (complete)", () =>
    expect(Feature.encode(Sample.featureComplete))
    |> toEqual(Sample.featureCompleteJson)
  );

  // this functionality gets inlined by the compiler, so Jest thinks we're not
  // testing it... this is just to make the coverage happy.
  test("encode ID", () =>
    expect(Feature.ID.(encode(StringID("a"))))
    |> toEqual(Js.Json.string("a"))
  );
});

open Jest;
open Expect;

module Feature = GeoJSON.Feature;
module SampleJson = GeoJSON_SampleJson;

describe("Feature", () => {
  test("decode (empty)", () =>
    expect(Feature.decode(SampleJson.featureEmptySparseJson))
    |> toEqual(Some(SampleJson.featureEmpty))
  );

  test("decode (no properties)", () =>
    expect(Feature.decode(SampleJson.featureNoPropertiesJson))
    |> toEqual(Some(SampleJson.featureNoProperties))
  );

  test("decode (with properties, numeric id)", () =>
    expect(Feature.decode(SampleJson.featureCompleteJson))
    |> toEqual(Some(SampleJson.featureComplete))
  );

  test("encode (empty)", () =>
    expect(Feature.encode(SampleJson.featureEmpty))
    |> toEqual(SampleJson.featureEmptyJson)
  );

  test("encode (complete)", () =>
    expect(Feature.encode(SampleJson.featureComplete))
    |> toEqual(SampleJson.featureCompleteJson)
  );

  // this functionality gets inlined by the compiler, so Jest thinks we're not
  // testing it... this is just to make the coverage happy.
  test("encode ID", () =>
    expect(Feature.ID.(encode(StringID("a"))))
    |> toEqual(Js.Json.string("a"))
  );
});

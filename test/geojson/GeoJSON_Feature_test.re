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

  // TODO: decode with properties
  // TODO: decode with numeric id

  test("encode (empty)", () =>
    expect(Feature.encode(SampleJson.featureEmpty))
    |> toEqual(SampleJson.featureEmptyJson)
  );
});

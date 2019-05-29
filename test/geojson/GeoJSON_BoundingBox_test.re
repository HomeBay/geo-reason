open Jest;
open Expect;

module LatLong = GeoJSON.Geometry.Position.LatLong;

module BoundingBox = GeoJSON.BoundingBox;
module BoundingBox2D = BoundingBox.BoundingBox2D;
module AltitudeRange = BoundingBox.AltitudeRange;

describe("BoundingBox2D", () => {
  let (n, e, s, w) = (49.4, (-66.8), 24.4, (-124.9));
  let box = BoundingBox2D.make(n, e, s, w);
  let box' = BoundingBox2D.makeLabels(~n, ~e, ~s, ~w);

  test("make", () =>
    expect(box) |> toEqual(BoundingBox2D.{n, e, s, w})
  );

  test("makeLabels", () =>
    expect(box) |> toEqual(box')
  );

  test("ne", () =>
    expect(BoundingBox2D.ne(box)) |> toEqual(LatLong.make(n, e))
  );

  test("se", () =>
    expect(BoundingBox2D.se(box)) |> toEqual(LatLong.make(s, e))
  );

  test("nw", () =>
    expect(BoundingBox2D.nw(box)) |> toEqual(LatLong.make(n, w))
  );

  test("sw", () =>
    expect(BoundingBox2D.sw(box)) |> toEqual(LatLong.make(s, w))
  );
});

describe("AltitudeRange", () => {
  let lowest = 0.0;
  let highest = 100.0;
  let a = AltitudeRange.make(lowest, highest);
  let b = AltitudeRange.make(highest, lowest);

  test("make (lowest first)", () =>
    expect(a) |> toEqual(AltitudeRange.{lowest, highest})
  );

  test("make (highest first)", () =>
    expect(b) |> toEqual(AltitudeRange.{lowest, highest})
  );

  test("eq", () =>
    expect(AltitudeRange.eq(a, b)) |> toBe(true)
  );

  test("not eq", () =>
    expect(AltitudeRange.(eq(a, make(0.0, -100.0)))) |> toBe(false)
  );
});

describe("BoundingBox", () => {
  let (n, e, s, w) = (49.4, (-66.8), 24.4, (-124.9));
  let bounds = BoundingBox2D.makeLabels(~n, ~e, ~s, ~w);
  let altitude = AltitudeRange.make(0.0, 100.0);
  let box = BoundingBox.make(bounds, None);
  let boxWithAltitude = BoundingBox.make(bounds, Some(altitude));

  let boxJson: Js.Json.t = [%raw {| [-124.9, 24.4, -66.8, 49.4] |}];
  let boxWithAltitudeJson: Js.Json.t = [%raw
    {| [-124.9, 24.4, 100.0, -66.8, 49.4, 0.0] |}
  ];

  // So many invalid tests to make the coverage happy...
  let invalidOne: Js.Json.t = [%raw {| [1.0] |}];
  let invalidTwo: Js.Json.t = [%raw {| [1.0, 2.0] |}];
  let invalidThree: Js.Json.t = [%raw {| [1.0, 2.0, 3.0] |}];
  let invalidFive: Js.Json.t = [%raw {| [1.0, 2.0, 3.0, 4.0, 5.0] |}];

  test("make (no altitude)", () =>
    expect(box) |> toEqual(BoundingBox.{bounds, altitude: None})
  );

  test("make (with altitude)", () =>
    expect(boxWithAltitude)
    |> toEqual(BoundingBox.{bounds, altitude: Some(altitude)})
  );

  test("makeLabels", () =>
    expect(BoundingBox.makeLabels(~n, ~e, ~s, ~w, ())) |> toEqual(box)
  );

  test("from2D", () =>
    expect(BoundingBox.from2D(bounds)) |> toEqual(box)
  );

  test("encode (no altitude)", () =>
    expect(BoundingBox.encode(box)) |> toEqual(boxJson)
  );

  test("encode (with altitude)", () =>
    expect(BoundingBox.encode(boxWithAltitude))
    |> toEqual(boxWithAltitudeJson)
  );

  test("decode success (no altitude)", () =>
    expect(BoundingBox.decode(boxJson)) |> toEqual(Some(box))
  );

  test("decode success (with altitude)", () =>
    expect(BoundingBox.decode(boxWithAltitudeJson))
    |> toEqual(Some(boxWithAltitude))
  );

  test("decode failure (empty array)", () =>
    expect(BoundingBox.decode(Js.Json.array([||]))) |> toEqual(None)
  );

  test("decode failure (singleton array)", () =>
    expect(BoundingBox.decode(invalidOne)) |> toEqual(None)
  );

  test("decode failure (two-value array)", () =>
    expect(BoundingBox.decode(invalidTwo)) |> toEqual(None)
  );

  test("decode failure (three-value array)", () =>
    expect(BoundingBox.decode(invalidThree)) |> toEqual(None)
  );

  test("decode failure (five-value array)", () =>
    expect(BoundingBox.decode(invalidFive)) |> toEqual(None)
  );
});

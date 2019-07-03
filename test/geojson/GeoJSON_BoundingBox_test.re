open Jest;
open Expect;
open Relude.Globals;

module LatLong = GeoJSON.Geometry.Position.LatLong;

module BoundingBox = GeoJSON.BoundingBox;
module BoundingBox2D = BoundingBox.BoundingBox2D;
module AltitudeRange = BoundingBox.AltitudeRange;

let decodeFailure = json =>
  Result.error(Decode.ParseError.Val(`ExpectedTuple(4), json));

describe("BoundingBox2D", () => {
  let (n, e, s, w) = (49.4, (-66.8), 24.4, (-124.9));
  let box = BoundingBox2D.make(n, e, s, w);
  let box' = BoundingBox2D.makeLabels(~n, ~e, ~s, ~w);
  let boxb = BoundingBox2D.make(1.0, 2.0, -1.0, -2.0);

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

  test("centerLat", () =>
    expect(BoundingBox2D.centerLat(boxb)) |> toEqual(0.0)
  );

  test("centerLon", () =>
    expect(BoundingBox2D.centerLon(boxb)) |> toEqual(0.0)
  );

  test("fromManyPoints", () =>
    expect(
      BoundingBox2D.fromManyPoints(
        LatLong.makeLabels(~latitude=0.0, ~longitude=0.0),
        [
          LatLong.makeLabels(~latitude=1.0, ~longitude=20.0),
          LatLong.makeLabels(~latitude=-3.0, ~longitude=-40.0),
        ],
      ),
    )
    |> toEqual(BoundingBox2D.{n: 1.0, e: 20.0, s: (-3.0), w: (-40.0)})
  );

  test("minSideSize", () =>
    expect(BoundingBox2D.minSideSize(boxb)) |> toEqual(2.)
  );

  test("ensureMinSize", () =>
    expect(BoundingBox2D.ensureMinSize(10.0, boxb))
    |> toEqual(BoundingBox2D.make(5.0, 5.0, -5.0, -5.0))
  );

  test("pad", () =>
    expect(BoundingBox2D.pad(0.5, boxb))
    |> toEqual(BoundingBox2D.make(3.0, 4.0, -3.0, -4.0))
  );

  test("wrap", () =>
    expect(BoundingBox2D.(wrap({n: 0.0, e: 185.0, s: 0.0, w: (-185.0)})))
    |> toEqual(BoundingBox2D.{n: 0.0, e: (-175.0), s: 0.0, w: 175.0})
  );

  test("wrap (unnecessary)", () =>
    expect(BoundingBox2D.wrap(boxb)) |> toEqual(boxb)
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
  let boxFromList = BoundingBox.fromList([w, s, e, n]);
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

  test("fromList", () =>
    expect(boxFromList) |> toEqual(Some(box))
  );

  test("encode (no altitude)", () =>
    expect(BoundingBox.encode(box)) |> toEqual(boxJson)
  );

  test("encode (with altitude)", () =>
    expect(BoundingBox.encode(boxWithAltitude))
    |> toEqual(boxWithAltitudeJson)
  );

  test("decode success (no altitude)", () =>
    expect(BoundingBox.decode(boxJson)) |> toEqual(Result.ok(box))
  );

  test("decode success (with altitude)", () =>
    expect(BoundingBox.decode(boxWithAltitudeJson))
    |> toEqual(Result.ok(boxWithAltitude))
  );

  test("decode failure (empty array)", () =>
    expect(BoundingBox.decode(Js.Json.array([||])))
    |> toEqual(decodeFailure(Js.Json.array([||])))
  );

  test("decode failure (singleton array)", () =>
    expect(BoundingBox.decode(invalidOne))
    |> toEqual(decodeFailure(invalidOne))
  );

  test("decode failure (two-value array)", () =>
    expect(BoundingBox.decode(invalidTwo))
    |> toEqual(decodeFailure(invalidTwo))
  );

  test("decode failure (three-value array)", () =>
    expect(BoundingBox.decode(invalidThree))
    |> toEqual(decodeFailure(invalidThree))
  );

  test("decode failure (five-value array)", () =>
    expect(BoundingBox.decode(invalidFive))
    |> toEqual(decodeFailure(invalidFive))
  );
});

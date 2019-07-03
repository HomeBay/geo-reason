open Jest;
open Expect;
open Relude.Globals;

module Position = GeoJSON.Geometry.Position;
module LatLong = Position.LatLong;

let decodeFailure = json =>
  Result.error(Decode.ParseError.Val(`ExpectedTuple(2), json));

describe("LatLong", () => {
  let a = LatLong.makeLabels(~latitude=40.0, ~longitude=-105.1);
  let a' = LatLong.makeLabels(~latitude=40.0, ~longitude=-105.1);
  let b = LatLong.makeLabels(~latitude=-105.1, ~longitude=40.0);

  test("make", () =>
    expect(LatLong.make(123.456, 0.0))
    |> toEqual(LatLong.{latitude: 123.456, longitude: 0.0})
  );

  test("makeLabels", () =>
    expect(a.LatLong.latitude) |> toEqual(40.0)
  );

  test("eq", () =>
    expect(LatLong.eq(a, a')) |> toBe(true)
  );

  test("not eq", () =>
    expect(LatLong.eq(a, b)) |> toBe(false)
  );
});

describe("Position", () => {
  let latitude = 40.0;
  let longitude = (-105.1);
  let latlong = LatLong.makeLabels(~latitude, ~longitude);
  let pos = Position.fromLatLong(latlong);
  let pos' = Position.make(latlong, None);
  let posWithAltitude =
    Position.makeLabels(~latitude, ~longitude, ~altitude=100.0, ());
  let posWithAltitude' = Position.make(latlong, Some(100.0));

  let posJson: Js.Json.t = [%raw {| [ -105.1, 40.0 ] |}];
  let posWithAltitudeJson: Js.Json.t = [%raw {| [ -105.1, 40.0, 100.0] |}];

  test("make", () =>
    expect(Position.make(latlong, Some(1.0)))
    |> toEqual({
         Position.altitude: Some(1.0),
         latlong: LatLong.{latitude, longitude},
       })
  );

  test("makeLabels", () =>
    expect(posWithAltitude.Position.altitude) |> toEqual(Some(100.0))
  );

  test("fromLatLong", () =>
    expect(pos.Position.latlong) |> toEqual(latlong)
  );

  test("fromLatLong has no altitude", () =>
    expect(pos.Position.altitude) |> toEqual(None)
  );

  test("decode success (no altitude)", () =>
    expect(Position.decode(posJson)) |> toEqual(Result.ok(pos))
  );

  test("decode success (with altitude)", () =>
    expect(Position.decode(posWithAltitudeJson))
    |> toEqual(Result.ok(posWithAltitude))
  );

  test("decode empty failure", () =>
    expect(Position.decode(Js.Json.array([||])))
    |> toEqual(decodeFailure(Js.Json.array([||])))
  );

  test("decode singleton failure", () => {
    let json = Js.Json.(array([|number(100.0)|]));
    expect(Position.decode(json)) |> toEqual(decodeFailure(json));
  });

  test("encode (no altitude)", () =>
    expect(Position.encode(pos)) |> toEqual(posJson)
  );

  test("encode (with altitude)", () =>
    expect(Position.encode(posWithAltitude)) |> toEqual(posWithAltitudeJson)
  );

  test("eq (no altitude)", () =>
    expect(Position.eq(pos, pos')) |> toBe(true)
  );

  test("eq (with altitude)", () =>
    expect(Position.eq(posWithAltitude, posWithAltitude')) |> toBe(true)
  );

  test("not eq (altitude-vs-none)", () =>
    expect(Position.eq(pos, posWithAltitude)) |> toBe(false)
  );

  test("not eq (different altitude)", () =>
    expect(
      Position.eq(Position.make(latlong, Some(2.22)), posWithAltitude),
    )
    |> toBe(false)
  );

  test("not eq (different latlong)", () =>
    expect(
      Position.eq(Position.makeLabels(~latitude, ~longitude=0.0, ()), pos),
    )
    |> toBe(false)
  );
});

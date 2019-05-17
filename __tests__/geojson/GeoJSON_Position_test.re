open Jest;
open Expect;

module Position = GeoJSON.Position;
module LatLong = GeoJSON.Position.LatLong;

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
  let posWithAltitude =
    Position.makeLabels(~latitude, ~longitude, ~altitude=100.0, ());

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
    expect(Position.decode(posJson)) |> toEqual(Some(pos))
  );

  test("decode success (with altitude)", () =>
    expect(Position.decode(posWithAltitudeJson))
    |> toEqual(Some(posWithAltitude))
  );

  test("decode empty failure", () =>
    expect(Position.decode(Js.Json.array([||]))) |> toEqual(None)
  );

  test("decode singleton failure", () =>
    expect(Position.decode(Js.Json.(array([|number(100.0)|]))))
    |> toEqual(None)
  );

  test("encode (no altitude)", () =>
    expect(Position.encode(pos)) |> toEqual(posJson)
  );

  test("encode (with altitude)", () =>
    expect(Position.encode(posWithAltitude)) |> toEqual(posWithAltitudeJson)
  );
});

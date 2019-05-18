open Jest;
open Expect;

module Position = GeoJSON.Position;
module Line = GeoJSON.Line;

describe("Line", () => {
  let point1 = Position.makeLabels(~latitude=40.0, ~longitude=-123.45, ());
  let point2 = Position.makeLabels(~latitude=10.0, ~longitude=0.0, ());
  let point3 = Position.makeLabels(~latitude=-10.0, ~longitude=0.0, ());
  let point4 = Position.makeLabels(~latitude=-40.0, ~longitude=123.0, ());

  let line1 = Line.make(point1, point2, [point3, point4]);
  let line1' =
    Line.makeLabels(
      ~first=point1,
      ~second=point2,
      ~rest=[point3, point4],
      (),
    );
  let line2 = Line.makeLabels(~first=point1, ~second=point2, ());
  let line3 = Line.twoPoints(point1, point2);

  let line2Json: Js.Json.t = [%raw {|
    [[-123.45, 40.0], [0.0, 10.0]]
  |}];

  let invalidJson: Js.Json.t = [%raw {| [[-123.45, 40.0]] |}];

  test("make", () =>
    expect(line1)
    |> toEqual(Line.{first: point1, second: point2, rest: [point3, point4]})
  );

  test("makeLabels (with rest)", () =>
    expect(line1')
    |> toEqual(Line.{first: point1, second: point2, rest: [point3, point4]})
  );

  test("makeLabels (no rest)", () =>
    expect(line2) |> toEqual(Line.{first: point1, second: point2, rest: []})
  );

  test("twoPoints", () =>
    expect(line3) |> toEqual(Line.{first: point1, second: point2, rest: []})
  );

  test("decode success", () =>
    expect(Line.decode(line2Json)) |> toEqual(Some(line2))
  );

  test("decode failure (empty)", () =>
    expect(Line.decode(Js.Json.array([||]))) |> toEqual(None)
  );

  test("decode failure (singleton)", () =>
    expect(Line.decode(invalidJson)) |> toEqual(None)
  );

  test("toArray", () =>
    expect(Line.toArray(line1))
    |> toEqual([|point1, point2, point3, point4|])
  );

  test("encode", () =>
    expect(Line.encode(line2)) |> toEqual(line2Json)
  );

  test("eq", () =>
    expect(Line.eq(line1, line1')) |> toBe(true)
  );

  test("not eq", () =>
    expect(Line.eq(line1, line2)) |> toBe(false)
  );
});

open Relude.Globals;

module ID = {
  type t =
    | StringID(string)
    | NumberID(float);

  let decode =
    Decode.AsResult.OfParseError.(
      alt(
        string |> map(v => StringID(v)),
        floatFromNumber |> map(v => NumberID(v)),
      )
    );

  let encode =
    fun
    | StringID(s) => Js.Json.string(s)
    | NumberID(n) => Js.Json.number(n);
};

type t = {
  id: option(ID.t),
  geometry: option(GeoJSON_Geometry.t),
  properties: option(Js.Dict.t(Js.Json.t)),
};

let make = (id, geometry, properties) => {id, geometry, properties};
let makeLabels = (~id=?, ~geometry=?, ~properties=?, ()) =>
  make(id, geometry, properties);

/**
 * Decode JSON into a GeoJSON Feature. According to [section 3.2 of the spec][1]
 * a "Feature" is supposed to have "geometry" and "properties" fields, which may
 * have nullable fields. Here, our decoder is a little more flexible than that,
 * and it allows the fields to be omitted entirely.
 *
 * [1] https://tools.ietf.org/html/rfc7946#section-3.2
 */
let decode =
  Decode.AsResult.OfParseError.Pipeline.(
    pure(make)
    |> optionalField("id", ID.decode)
    |> optionalField("geometry", GeoJSON_Geometry.decode)
    |> optionalField("properties", dict(Result.pure))
  );

/**
 * As mentioned in the `decode` section above, the GeoJSON spec indicates that
 * a valid Feature should includ "geometry" and "properties" fields, even if
 * those values are null. The "id" field can be omitted.
 */
let encodeFields = ({id, geometry, properties}) =>
  List.catOptions([
    Some(("type", Js.Json.string("Feature"))),
    Option.map(v => ("id", ID.encode(v)), id),
    Some((
      "geometry",
      Option.fold(Js.Json.null, GeoJSON_Geometry.encode, geometry),
    )),
    Some((
      "properties",
      Option.fold(Js.Json.null, Js.Json.object_, properties),
    )),
  ]);

let encode = v => encodeFields(v) |> Js.Dict.fromList |> Js.Json.object_;

open Fest;

// update melange-fest dependency and then you can remove this:
[@mel.send.pipe: assertion]
external deepStrictEqual: ('a, 'a) => unit = "deepStrictEqual";

test("some test", () =>
  expect |> strict_equal(1, 1)
);

type foo =
  | Foo
  | Bar(int)
  | Baz(string);

test("foo", () =>
  expect |> equal(Foo, Foo)
);

test("bar", () =>
  expect |> deepStrictEqual(Bar(42), Bar(42))
);

test("baz", () =>
  expect |> deepStrictEqual(Baz("wunderbar"), Baz("wunderbar"))
);

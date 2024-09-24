open Fest;

test("Zero, one, and two elements", () => {
  expect |> deepEqual(ListSafe.humanize([]), "");

  expect |> deepEqual(ListSafe.humanize(["foo"]), "foo");

  expect |> deepEqual(ListSafe.humanize(["foo", "bar"]), "foo and bar");
});

test("Three, four, and five elements", () => {
  expect |> deepEqual(ListSafe.humanize(["A", "B", "C"]), "A, B, and C");

  expect
  |> deepEqual(ListSafe.humanize(["A", "B", "C", "D"]), "A, B, C, and D");

  expect
  |> deepEqual(
       ListSafe.humanize(["A", "B", "C", "D", "E"]),
       "A, B, C, D, and E",
     );
});

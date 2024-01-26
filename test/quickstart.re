open Fest;

let () = test("some test", () =>
           expect |> strict_equal(1, 1)
         );

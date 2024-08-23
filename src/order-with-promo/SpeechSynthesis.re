/** Bindings for SpeechSynthesis API */

type t;

module Voice = {
  type t;

  [@mel.get] external getLang: t => string = "lang";
  [@mel.get] external getName: t => string = "name";
};

module Utterance = {
  type t;
  [@mel.new] external make: string => t = "SpeechSynthesisUtterance";

  [@mel.set] external setVoice: (t, Voice.t) => unit = "voice";
};

// could also use @mel.scope
external getVoices: unit => array(Voice.t) = "speechSynthesis.getVoices";

external addVoicesChangedListener:
  (
    [@mel.as "voiceschanged"] _,
    unit => unit,
    [@mel.as {json|{"once": true}|json}] _
  ) =>
  unit =
  "speechSynthesis.addEventListener";

external speak: Utterance.t => unit = "speechSynthesis.speak";

[@mel.scope "speechSynthesis"] external cancel: unit => unit = "cancel";

let getVoices = () => {
  Js.Promise.make((~resolve, ~reject as _) =>
    switch (getVoices()) {
    | [||] => addVoicesChangedListener(() => resolve(. getVoices()))
    | voices => resolve(. voices)
    }
  );
};

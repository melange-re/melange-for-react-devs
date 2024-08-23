module Voice = SpeechSynthesis.Voice;

let getVoices = () => {
  SpeechSynthesis.getVoices()
  |> Promise.map(voices =>
       voices
       |> Array.toList
       |> List.filter(v =>
            v |> Voice.getLang |> Js.String.startsWith(~prefix="en-")
          )
     );
};

let speak = (voice, text) => {
  module Utterance = SpeechSynthesis.Utterance;
  let utterance = Utterance.make(text);
  Utterance.setVoice(utterance, voice);
  SpeechSynthesis.speak(utterance);
};

module Inner = {
  type state = {
    counter: int,
    voice: Voice.t,
    intervalId: option(Js.Global.intervalId),
  };

  [@react.component]
  let make = (~voice as initialVoice: Voice.t, ~voices: list(Voice.t)) => {
    let (state, setState) =
      React.useState(() =>
        {counter: 30, voice: initialVoice, intervalId: None}
      );

    let speakAndDecrement = () => {
      setState(({counter, voice, intervalId} as oldState) => {
        let newCounter = counter - 1;
        let newIntervalId =
          if (newCounter > 0) {
            speak(voice, string_of_int(newCounter));
            intervalId;
          } else {
            speak(voice, "Time's up!");
            intervalId |> Option.iter(Js.Global.clearInterval);
            None;
          };
        {...oldState, counter: newCounter, intervalId: newIntervalId};
      });
    };

    React.useEffect0(() =>
      Some(() => state.intervalId |> Option.iter(Js.Global.clearInterval))
    );

    <div>
      <NumberInput
        disabled={state.intervalId |> Option.is_some}
        value={state.counter}
        onChange={counter => setState(_ => {...state, counter})}
      />
      <select
        value={Voice.getName(state.voice)}
        onChange={evt => {
          let name = RR.getValueFromEvent(evt);
          voices
          |> List.find_opt(v => Voice.getName(v) == name)
          |> Option.iter(voice => setState(_ => {...state, voice}));
        }}>
        {voices
         |> List.map(v => {
              let name = v |> Voice.getName;
              <option key=name value=name> {name |> RR.s} </option>;
            })
         |> RR.list}
      </select>
      <button
        onClick={_evt =>
          switch (state.intervalId) {
          | Some(intervalId) =>
            Js.Global.clearInterval(intervalId);
            setState(_ => {...state, intervalId: None});
          | None =>
            let intervalId =
              Js.Global.setInterval(1000, ~f=speakAndDecrement) |> Option.some;
            setState(_ => {...state, intervalId});
          }
        }>
        {(state.intervalId == None ? "Start" : "Stop") |> RR.s}
      </button>
    </div>;
  };
};

[@react.component]
let make = () => {
  let (voices, setVoices) = RR.useStateValue([]);

  React.useEffect0(() => {
    getVoices() |> Promise.iter(setVoices);
    None;
  });

  switch (voices) {
  | [] => React.null
  | [voice, ..._] => <Inner voice voices />
  };
};
